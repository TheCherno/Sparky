#include "ga.h"
#include "ga_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

/* Tell Jumps */
typedef struct gau_TellJumpData {
  gc_int32 pos;
  gc_int32 delta;
} gau_TellJumpData;

typedef struct gau_TellJumpLink {
  gc_Link link;
  gau_TellJumpData data;
} gau_TellJumpLink;

void gauX_tell_jump_push(gc_Link* in_head, gc_int32 in_pos, gc_int32 in_delta)
{
  gau_TellJumpLink* link = gcX_ops->allocFunc(sizeof(gau_TellJumpLink));
  link->link.data = &link->data;
  link->data.pos = in_pos;
  link->data.delta = in_delta;
  gc_list_link(in_head->prev, (gc_Link*)link, &link->data);
}
gc_int32 gauX_tell_jump_peek(gc_Link* in_head, gc_int32* out_pos, gc_int32* out_delta)
{
  gau_TellJumpLink* link;
  if(in_head->next == in_head)
    return 0;
  link = (gau_TellJumpLink*)in_head->next;
  *out_pos = link->data.pos;
  *out_delta = link->data.delta;
  return 1;
}
gc_int32 gauX_tell_jump_pop(gc_Link* in_head)
{
  gau_TellJumpLink* link;
  if(in_head->next == in_head)
    return 0;
  link = (gau_TellJumpLink*)in_head->next;
  gc_list_unlink((gc_Link*)link);
  gcX_ops->freeFunc(link);
  return 1;
}
gc_int32 gauX_tell_jump_process(gc_Link* in_head, gc_int32 in_advance)
{
  gc_int32 ret = 0;
  gau_TellJumpLink* link = (gau_TellJumpLink*)in_head->next;
  while((gc_Link*)link != in_head)
  {
    gau_TellJumpLink* oldLink = link;
    link = (gau_TellJumpLink*)link->link.next;
    oldLink->data.pos -= in_advance;
    if(oldLink->data.pos <= 0)
    {
      ret += oldLink->data.delta;
      gc_list_unlink((gc_Link*)oldLink);
    }
  }
  return ret;
}
void gauX_tell_jump_clear(gc_Link* in_head)
{
  while(gauX_tell_jump_pop(in_head));
}


/* Stream Link */
typedef struct gaX_StreamLink {
  gc_Link link;
  gc_int32 refCount;
  gc_Mutex* produceMutex;
  gc_Mutex* refMutex;
  ga_BufferedStream* stream;
} gaX_StreamLink;

gaX_StreamLink* gaX_stream_link_create()
{
  gaX_StreamLink* ret = (gaX_StreamLink*)gcX_ops->allocFunc(sizeof(gaX_StreamLink));
  ret->refCount = 1;
  ret->refMutex = gc_mutex_create();
  ret->produceMutex = gc_mutex_create();
  ret->stream = 0;
  return ret;
}
gc_int32 gaX_stream_link_produce(gaX_StreamLink* in_streamLink)
{
  gc_int32 ret = 1;
  gc_mutex_lock(in_streamLink->produceMutex);
  if(in_streamLink->stream)
  {
    /* Mutexing this entire section guarantees that ga_stream_destroy()
       cannot occur during production */
    ga_stream_produce(in_streamLink->stream);
    ret = 0;
  }
  gc_mutex_unlock(in_streamLink->produceMutex);
  return ret;
}
void gaX_stream_link_kill(gaX_StreamLink* in_streamLink)
{
  gc_mutex_lock(in_streamLink->produceMutex);
  in_streamLink->stream = 0;
  gc_mutex_unlock(in_streamLink->produceMutex);
}
void gaX_stream_link_destroy(gaX_StreamLink* in_streamLink)
{
  gc_mutex_destroy(in_streamLink->produceMutex);
  gc_mutex_destroy(in_streamLink->refMutex);
  gcX_ops->freeFunc(in_streamLink);
}
void gaX_stream_link_acquire(gaX_StreamLink* in_streamLink)
{
  gc_mutex_lock(in_streamLink->refMutex);
  ++in_streamLink->refCount;
  gc_mutex_unlock(in_streamLink->refMutex);
}
void gaX_stream_link_release(gaX_StreamLink* in_streamLink)
{
  gc_int32 refCount;
  assert(in_streamLink->refCount > 0);
  gc_mutex_lock(in_streamLink->refMutex);
  --in_streamLink->refCount;
  refCount = in_streamLink->refCount;
  gc_mutex_unlock(in_streamLink->refMutex);
  if(refCount == 0)
    gaX_stream_link_destroy(in_streamLink);
}

/* Stream Manager */
ga_StreamManager* ga_stream_manager_create()
{
  ga_StreamManager* ret = (ga_StreamManager*)gcX_ops->allocFunc(sizeof(ga_StreamManager));
  ret->streamListMutex = gc_mutex_create();
  gc_list_head(&ret->streamList);
  return ret;
}
gaX_StreamLink* gaX_stream_manager_add(ga_StreamManager* in_mgr, ga_BufferedStream* in_stream)
{
  gaX_StreamLink* streamLink = gaX_stream_link_create(in_stream);
  gaX_stream_link_acquire(streamLink); /* The new client adds its own refcount */
  /* It's safe to add() while iterating in stream() because of implicit fault tolerance */
  /* That is, all possible outcomes are valid, despite the race condition */
  /* This is true because we are guaranteed to stream() on the same thread that calls remove() */
  gc_mutex_lock(in_mgr->streamListMutex);
  streamLink->stream = in_stream;
  gc_list_link(&in_mgr->streamList, (gc_Link*)streamLink, streamLink);
  gc_mutex_unlock(in_mgr->streamListMutex);
  return streamLink;
}
void ga_stream_manager_buffer(ga_StreamManager* in_mgr)
{
  gc_Link* link = in_mgr->streamList.next;
  while(link != &in_mgr->streamList)
  {
    gc_int32 streamDead;
    gaX_StreamLink* streamLink;
    streamLink = (gaX_StreamLink*)link->data;
    link = link->next;
    streamDead = gaX_stream_link_produce(streamLink);
    if(streamDead)
    {
      gc_mutex_lock(in_mgr->streamListMutex);
      gc_list_unlink((gc_Link*)streamLink);
      gc_mutex_unlock(in_mgr->streamListMutex);
      gaX_stream_link_release(streamLink);
    }
  }
}
void ga_stream_manager_destroy(ga_StreamManager* in_mgr)
{
  gc_Link* link;
  link = in_mgr->streamList.next;
  while(link != &in_mgr->streamList)
  {
    gaX_StreamLink* oldLink = (gaX_StreamLink*)link;
    link = link->next;
    gaX_stream_link_release(oldLink);
  }
  gc_mutex_destroy(in_mgr->streamListMutex);
  gcX_ops->freeFunc(in_mgr);
}

/* Stream */
ga_BufferedStream* ga_stream_create(ga_StreamManager* in_mgr, ga_SampleSource* in_sampleSrc, gc_int32 in_bufferSize)
{
  ga_BufferedStream* ret = gcX_ops->allocFunc(sizeof(ga_BufferedStream));
  ret->refCount = 1;
  ret->refMutex = gc_mutex_create();
  ga_sample_source_acquire(in_sampleSrc);
  ga_sample_source_format(in_sampleSrc, &ret->format);
  gc_list_head(&ret->tellJumps);
  ret->innerSrc = in_sampleSrc;
  ret->nextSample = 0;
  ret->seek = 0;
  ret->tell = 0;
  ret->end = 0;
  ret->bufferSize = in_bufferSize;
  ret->flags = ga_sample_source_flags(in_sampleSrc);
  assert(ret->flags & GA_FLAG_THREADSAFE);
  ret->produceMutex = gc_mutex_create();
  ret->seekMutex = gc_mutex_create();
  ret->readMutex = gc_mutex_create();
  ret->buffer = gc_buffer_create(in_bufferSize);
  ret->streamLink = (gc_Link*)gaX_stream_manager_add(in_mgr, ret);
  return ret;
}
void gaX_stream_onSeek(gc_int32 in_sample, gc_int32 in_delta, void* in_seekContext)
{
  ga_BufferedStream* s = (ga_BufferedStream*)in_seekContext;
  gc_int32 samplesAvail, sampleSize;
  ga_Format fmt;
  ga_sample_source_format(s->innerSrc, &fmt);
  sampleSize = ga_format_sampleSize(&fmt);
  gc_mutex_lock(s->readMutex);
  gc_mutex_lock(s->seekMutex);
  samplesAvail = gc_buffer_bytesAvail(s->buffer) / sampleSize;
  gauX_tell_jump_push(&s->tellJumps, samplesAvail + in_sample, in_delta);
  gc_mutex_unlock(s->seekMutex);
  gc_mutex_unlock(s->readMutex);
}
gc_int32 gaX_read_samples_into_stream(ga_BufferedStream* in_stream,
                                      gc_CircBuffer* in_buffer,
                                      gc_int32 in_samples,
                                      ga_SampleSource* in_sampleSrc)
{
  void* dataA;
  void* dataB;
  gc_uint32 sizeA = 0;
  gc_uint32 sizeB = 0;
  gc_int32 numBuffers;
  gc_int32 numWritten = 0;
  ga_Format fmt;
  gc_int32 sampleSize;
  gc_CircBuffer* b = in_buffer;
  ga_sample_source_format(in_sampleSrc, &fmt);
  sampleSize = ga_format_sampleSize(&fmt);
  numBuffers = gc_buffer_getFree(b, in_samples * sampleSize, &dataA, &sizeA, &dataB, &sizeB);
  if(numBuffers >= 1)
  {
    numWritten = ga_sample_source_read(in_sampleSrc, dataA, sizeA / sampleSize, &gaX_stream_onSeek, in_stream);
    if(numBuffers == 2 && numWritten == sizeA)
      numWritten += ga_sample_source_read(in_sampleSrc, dataB, sizeB / sampleSize, &gaX_stream_onSeek, in_stream);
  }
  gc_buffer_produce(b, numWritten * sampleSize);
  return numWritten;
}
void ga_stream_produce(ga_BufferedStream* in_stream)
{
  ga_BufferedStream* s = in_stream;
  gc_CircBuffer* b = s->buffer;
  gc_int32 sampleSize = ga_format_sampleSize(&s->format);
  gc_int32 bytesFree = gc_buffer_bytesFree(b);
  if(s->seek >= 0)
  {
    gc_int32 samplePos;
    gc_mutex_lock(s->readMutex);
    gc_mutex_lock(s->seekMutex);
    if(s->seek >= 0) /* Check again now that we're mutexed */
    {
      samplePos = s->seek;
      s->tell = samplePos;
      s->seek = -1;
      s->nextSample = samplePos;
      ga_sample_source_seek(s->innerSrc, samplePos);
      gc_buffer_consume(s->buffer, gc_buffer_bytesAvail(s->buffer)); /* Clear buffer */
      gauX_tell_jump_clear(&s->tellJumps); /* Clear tell-jump list */
    }
    gc_mutex_unlock(s->seekMutex);
    gc_mutex_unlock(s->readMutex);
  }

  while(bytesFree)
  {
    gc_int32 samplesWritten = 0;
    gc_int32 bytesWritten = 0;
    gc_int32 bytesToWrite = bytesFree;
    samplesWritten = gaX_read_samples_into_stream(s, b, bytesToWrite / sampleSize, s->innerSrc);
    bytesWritten = samplesWritten * sampleSize;
    bytesFree -= bytesWritten;
    s->nextSample += samplesWritten;
    if(bytesWritten < bytesToWrite && ga_sample_source_end(s->innerSrc))
    {
      s->end = 1;
      break;
    }
  }
}
gc_int32 ga_stream_read(ga_BufferedStream* in_stream, void* in_dst, gc_int32 in_numSamples)
{
  ga_BufferedStream* s = in_stream;
  gc_CircBuffer* b = s->buffer;
  gc_int32 delta;

  /* Read the samples */
  gc_int32 samplesConsumed = 0;
  gc_mutex_lock(s->readMutex);
  {
    void* dataA;
    void* dataB;
    gc_uint32 sizeA, sizeB;
    gc_int32 totalBytes = 0;
    gc_int32 sampleSize = ga_format_sampleSize(&s->format);
    gc_int32 dstBytes = in_numSamples * sampleSize;
    gc_int32 avail = gc_buffer_bytesAvail(b);
    dstBytes = dstBytes > avail ? avail : dstBytes;
    if(gc_buffer_getAvail(b, dstBytes, &dataA, &sizeA, &dataB, &sizeB) >= 1)
    {
      gc_int32 bytesToRead = dstBytes < (gc_int32)sizeA ? dstBytes : (gc_int32)sizeA;
      memcpy(in_dst, dataA, bytesToRead);
      totalBytes += bytesToRead;
      if(dstBytes > 0 && dataB)
      {
        gc_int32 dstBytesLeft = dstBytes - bytesToRead;
        bytesToRead = dstBytesLeft < (gc_int32)sizeB ? dstBytesLeft : (gc_int32)sizeB;
        memcpy((char*)in_dst + totalBytes, dataB, bytesToRead);
        totalBytes += bytesToRead;
      }
    }
    samplesConsumed = totalBytes / sampleSize;
    gc_buffer_consume(b, totalBytes);
  }

  /* Update the tell pos */
  gc_mutex_lock(s->seekMutex);
  s->tell += samplesConsumed;
  delta = gauX_tell_jump_process(&s->tellJumps, samplesConsumed);
  s->tell += delta;
  gc_mutex_unlock(s->seekMutex);
  gc_mutex_unlock(s->readMutex);
  return samplesConsumed;
}
gc_int32 ga_stream_ready(ga_BufferedStream* in_stream, gc_int32 in_numSamples)
{
  ga_BufferedStream* s = in_stream;
  gc_int32 avail = gc_buffer_bytesAvail(s->buffer);
  return s->end || avail >= in_numSamples * ga_format_sampleSize(&s->format) && avail > s->bufferSize / 2.0f;
}
gc_int32 ga_stream_end(ga_BufferedStream* in_stream)
{
  ga_BufferedStream* s = in_stream;
  gc_CircBuffer* b = s->buffer;
  gc_int32 bytesAvail = gc_buffer_bytesAvail(b);
  return s->end && bytesAvail == 0;
}
gc_int32 ga_stream_seek(ga_BufferedStream* in_stream, gc_int32 in_sampleOffset)
{
  ga_BufferedStream* s = in_stream;
  gc_mutex_lock(s->seekMutex);
  s->seek = in_sampleOffset;
  gc_mutex_unlock(s->seekMutex);
  return 0;
}
gc_int32 ga_stream_tell(ga_BufferedStream* in_stream, gc_int32* out_totalSamples)
{
  ga_BufferedStream* s = in_stream;
  gc_int32 ret = -1;
  ga_sample_source_tell(s->innerSrc, out_totalSamples);
  gc_mutex_lock(s->seekMutex);
  ret = s->seek >= 0 ? s->seek : s->tell;
  gc_mutex_unlock(s->seekMutex);
  return ret;
}
gc_int32 ga_stream_flags(ga_BufferedStream* in_stream)
{
  ga_BufferedStream* s = in_stream;
  return s->flags;
}
void gaX_stream_destroy(ga_BufferedStream* in_stream)
{
  ga_BufferedStream* s = in_stream;
  gaX_stream_link_kill((gaX_StreamLink*)s->streamLink); /* This must be done first, so that the stream remains valid until it killed */
  gaX_stream_link_release((gaX_StreamLink*)s->streamLink);
  gc_mutex_destroy(s->produceMutex);
  gc_mutex_destroy(s->seekMutex);
  gc_mutex_destroy(s->readMutex);
  gc_mutex_destroy(s->refMutex);
  gc_buffer_destroy(s->buffer);
  gauX_tell_jump_clear(&s->tellJumps);
  ga_sample_source_release(s->innerSrc);
  gcX_ops->freeFunc(s);
}
void ga_stream_acquire(ga_BufferedStream* in_stream)
{
  gc_mutex_lock(in_stream->refMutex);
  ++in_stream->refCount;
  gc_mutex_unlock(in_stream->refMutex);
}
void ga_stream_release(ga_BufferedStream* in_stream)
{
  gc_int32 refCount;
  assert(in_stream->refCount > 0);
  gc_mutex_lock(in_stream->refMutex);
  --in_stream->refCount;
  refCount = in_stream->refCount;
  gc_mutex_unlock(in_stream->refMutex);
  if(refCount == 0)
    gaX_stream_destroy(in_stream);
}
