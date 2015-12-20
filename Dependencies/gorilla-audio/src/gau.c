#include "ga.h"
#include "gau.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#ifdef _WIN32
#pragma warning(disable:4996)
#endif /* _WIN32 */

#ifdef __linux__
#include <strings.h>
#define stricmp strcasecmp
#endif /* __linux__ */

/* High-Level Manager */
typedef struct gau_Manager {
  gc_int32 threadPolicy;
  gc_Thread* mixThread;
  gc_Thread* streamThread;
  ga_Device* device;
  ga_Mixer* mixer;
  ga_StreamManager* streamMgr;
  gc_int32 sampleSize;
  gc_int16* mixBuffer;
  ga_Format format;
  gc_int32 killThreads;
} gau_Manager;

static gc_int32 gauX_mixThreadFunc(void* in_context)
{
  gau_Manager* ctx = (gau_Manager*)in_context;
  ga_Mixer* m = ctx->mixer;
  gc_int32 sampleSize = ga_format_sampleSize(&ctx->format);
  while(!ctx->killThreads)
  {
    gc_int32 numToQueue = ga_device_check(ctx->device);
    while(numToQueue--)
    {
      ga_mixer_mix(m, ctx->mixBuffer);
      ga_device_queue(ctx->device, ctx->mixBuffer);
    }
    gc_thread_sleep(5);
  }
  return 0;
}
static gc_int32 gauX_streamThreadFunc(void* in_context)
{
  gau_Manager* ctx = (gau_Manager*)in_context;
  ga_StreamManager* mgr = ctx->streamMgr;
  while(!ctx->killThreads)
  {
    ga_stream_manager_buffer(mgr);
    gc_thread_sleep(50);
  }
  return 0;
}
gau_Manager* gau_manager_create()
{
  gau_Manager* ret;
  ret = gau_manager_create_custom(GA_DEVICE_TYPE_DEFAULT, GAU_THREAD_POLICY_SINGLE, 4, 512);
  return ret;
}
gau_Manager* gau_manager_create_custom(gc_int32 in_devType,
                                       gc_int32 in_threadPolicy,
                                       gc_int32 in_numBuffers,
                                       gc_int32 in_bufferSamples)
{
  gau_Manager* ret = gcX_ops->allocFunc(sizeof(gau_Manager));

  assert(in_threadPolicy == GAU_THREAD_POLICY_SINGLE ||
         in_threadPolicy == GAU_THREAD_POLICY_MULTI);
  assert(in_bufferSamples > 128);
  assert(in_numBuffers >= 2);

  /* Open device */
  memset(&ret->format, 0, sizeof(ga_Format));
  ret->format.bitsPerSample = 16;
  ret->format.numChannels = 2;
  ret->format.sampleRate = 44100;
  ret->device = ga_device_open(in_devType, in_numBuffers, in_bufferSamples, &ret->format);
  assert(ret->device);

  /* Initialize mixer */
  ret->mixer = ga_mixer_create(&ret->format, in_bufferSamples);
  ret->streamMgr = ga_stream_manager_create();
  ret->sampleSize = ga_format_sampleSize(&ret->format);
  ret->mixBuffer = (gc_int16*)gcX_ops->allocFunc(ret->mixer->numSamples * ret->sampleSize);

  /* Create and run mixer and stream threads */
  ret->threadPolicy = in_threadPolicy;
  ret->killThreads = 0;
  if(ret->threadPolicy == GAU_THREAD_POLICY_MULTI)
  {
    ret->mixThread = gc_thread_create(gauX_mixThreadFunc, ret, GC_THREAD_PRIORITY_HIGH, 64 * 1024);
    ret->streamThread = gc_thread_create(gauX_streamThreadFunc, ret, GC_THREAD_PRIORITY_HIGH, 64 * 1024);
    gc_thread_run(ret->mixThread);
    gc_thread_run(ret->streamThread);
  }
  else
  {
    ret->mixThread = 0;
    ret->streamThread = 0;
  }

  return ret;
}
void gau_manager_update(gau_Manager* in_mgr)
{
  if(in_mgr->threadPolicy == GAU_THREAD_POLICY_SINGLE)
  {
    gc_int16* buf = in_mgr->mixBuffer;
    ga_Mixer* mixer = in_mgr->mixer;
    ga_Device* dev = in_mgr->device;
    ga_Format* fmt = &in_mgr->format;
    gc_int32 numToQueue = ga_device_check(dev);
    while(numToQueue--)
    {
      ga_mixer_mix(mixer, buf);
      ga_device_queue(dev, buf);
    }
    ga_stream_manager_buffer(in_mgr->streamMgr);
  }
  ga_mixer_dispatch(in_mgr->mixer);
}
ga_Mixer* gau_manager_mixer(gau_Manager* in_mgr)
{
  return in_mgr->mixer;
}
ga_StreamManager* gau_manager_streamManager(gau_Manager* in_mgr)
{
  return in_mgr->streamMgr;
}
ga_Device* gau_manager_device(gau_Manager* in_mgr)
{
  return in_mgr->device;
}
void gau_manager_destroy(gau_Manager* in_mgr)
{
  if(in_mgr->threadPolicy == GAU_THREAD_POLICY_MULTI)
  {
    in_mgr->killThreads = 1;
    gc_thread_join(in_mgr->streamThread);
    gc_thread_join(in_mgr->mixThread);
  }

  /* Clean up mixer and stream manager */
  ga_stream_manager_destroy(in_mgr->streamMgr);
  ga_mixer_destroy(in_mgr->mixer);
  gcX_ops->freeFunc(in_mgr->mixBuffer);
  ga_device_close(in_mgr->device);
  gcX_ops->freeFunc(in_mgr);
}

/* On-Finish Callbacks */
void gau_on_finish_destroy(ga_Handle* in_finishedHandle, void* in_context)
{
  ga_handle_destroy(in_finishedHandle);
}

/* File-Based Data Source */
typedef struct gau_DataSourceFileContext {
  FILE* f;
  gc_Mutex* fileMutex;
} gau_DataSourceFileContext;

typedef struct gau_DataSourceFile {
  ga_DataSource dataSrc;
  gau_DataSourceFileContext context;
} gau_DataSourceFile;

gc_int32 gauX_data_source_file_read(void* in_context, void* in_dst, gc_int32 in_size, gc_int32 in_count)
{
  gau_DataSourceFileContext* ctx = (gau_DataSourceFileContext*)in_context;
  gc_int32 ret;
  gc_mutex_lock(ctx->fileMutex);
  ret = (gc_int32)fread(in_dst, in_size, in_count, ctx->f);
  gc_mutex_unlock(ctx->fileMutex);
  return ret;
}
gc_int32 gauX_data_source_file_seek(void* in_context, gc_int32 in_offset, gc_int32 in_origin)
{
  gau_DataSourceFileContext* ctx = (gau_DataSourceFileContext*)in_context;
  gc_mutex_lock(ctx->fileMutex);
  switch(in_origin)
  {
  case GA_SEEK_ORIGIN_SET: fseek(ctx->f, in_offset, SEEK_SET); break;
  case GA_SEEK_ORIGIN_CUR: fseek(ctx->f, in_offset, SEEK_CUR); break;
  case GA_SEEK_ORIGIN_END: fseek(ctx->f, in_offset, SEEK_END); break;
  }
  gc_mutex_unlock(ctx->fileMutex);
  return 0;
}
gc_int32 gauX_data_source_file_tell(void* in_context)
{
  gau_DataSourceFileContext* ctx = (gau_DataSourceFileContext*)in_context;
  gc_int32 ret;
  gc_mutex_lock(ctx->fileMutex);
  ret = ftell(ctx->f);
  gc_mutex_unlock(ctx->fileMutex);
  return ret;
}
void gauX_data_source_file_close(void* in_context)
{
  gau_DataSourceFileContext* ctx = (gau_DataSourceFileContext*)in_context;
  fclose(ctx->f);
  gc_mutex_destroy(ctx->fileMutex);
}
ga_DataSource* gau_data_source_create_file(const char* in_filename)
{
  gau_DataSourceFile* ret = gcX_ops->allocFunc(sizeof(gau_DataSourceFile));
  ga_data_source_init(&ret->dataSrc);
  ret->dataSrc.flags = GA_FLAG_SEEKABLE | GA_FLAG_THREADSAFE;
  ret->dataSrc.readFunc = &gauX_data_source_file_read;
  ret->dataSrc.seekFunc = &gauX_data_source_file_seek;
  ret->dataSrc.tellFunc = &gauX_data_source_file_tell;
  ret->dataSrc.closeFunc = &gauX_data_source_file_close;
  ret->context.f = fopen(in_filename, "rb");
  if(ret->context.f)
    ret->context.fileMutex = gc_mutex_create();
  else
  {
    gcX_ops->freeFunc(ret);
    ret = 0;
  }
  return (ga_DataSource*)ret;
}

/* File-Based Archived Data Source */
typedef struct gau_DataSourceFileArcContext {
  gc_int32 offset;
  gc_int32 size;
  FILE* f;
  gc_Mutex* fileMutex;
} gau_DataSourceFileArcContext;

typedef struct gau_DataSourceFileArc {
  ga_DataSource dataSrc;
  gau_DataSourceFileArcContext context;
} gau_DataSourceFileArc;

gc_int32 gauX_data_source_file_arc_read(void* in_context, void* in_dst, gc_int32 in_size, gc_int32 in_count)
{
  gau_DataSourceFileArcContext* ctx = (gau_DataSourceFileArcContext*)in_context;
  gc_int32 ret;
  /* TODO: Implement in-archive EOF detection? */
  gc_mutex_lock(ctx->fileMutex);
  ret = (gc_int32)fread(in_dst, in_size, in_count, ctx->f);
  gc_mutex_unlock(ctx->fileMutex);
  return ret;
}
gc_int32 gauX_data_source_file_arc_seek(void* in_context, gc_int32 in_offset, gc_int32 in_origin)
{
  /* TODO: What is the best way to resolve the seeking-OOB cases? */
  gau_DataSourceFileArcContext* ctx = (gau_DataSourceFileArcContext*)in_context;
  gc_mutex_lock(ctx->fileMutex);
  switch(in_origin)
  {
  case GA_SEEK_ORIGIN_SET:
    if(ctx->size > 0 && in_offset > ctx->size)
    {
      gc_mutex_unlock(ctx->fileMutex);
      return -1;
    }
    fseek(ctx->f, ctx->offset + in_offset, SEEK_SET);
    break;
  case GA_SEEK_ORIGIN_CUR:
    {
      gc_int32 curPos = ftell(ctx->f) - ctx->offset;
      gc_int32 newPos = curPos + in_offset;
      if(newPos < 0 || (ctx->size > 0 && newPos > ctx->size))
      {
        gc_mutex_unlock(ctx->fileMutex);
        return -1;
      }
      fseek(ctx->f, in_offset, SEEK_CUR);
    }
    break;
  case GA_SEEK_ORIGIN_END:
    if(ctx->size <= 0)
    {
      gc_mutex_unlock(ctx->fileMutex);
      return -1;
    }
    fseek(ctx->f, ctx->offset + ctx->size + in_offset, SEEK_SET);
    break;
  }
  gc_mutex_unlock(ctx->fileMutex);
  return 0;
}
gc_int32 gauX_data_source_file_arc_tell(void* in_context)
{
  gau_DataSourceFileArcContext* ctx = (gau_DataSourceFileArcContext*)in_context;
  gc_int32 ret;
  gc_mutex_lock(ctx->fileMutex);
  ret = ftell(ctx->f) - ctx->offset;
  gc_mutex_unlock(ctx->fileMutex);
  return ret;
}
void gauX_data_source_file_arc_close(void* in_context)
{
  gau_DataSourceFileArcContext* ctx = (gau_DataSourceFileArcContext*)in_context;
  fclose(ctx->f);
  gc_mutex_destroy(ctx->fileMutex);
}
ga_DataSource* gau_data_source_create_file_arc(const char* in_filename, gc_int32 in_offset, gc_int32 in_size)
{
  gau_DataSourceFileArc* ret = gcX_ops->allocFunc(sizeof(gau_DataSourceFileArc));
  ga_data_source_init(&ret->dataSrc);
  ret->dataSrc.flags = GA_FLAG_SEEKABLE | GA_FLAG_THREADSAFE;
  ret->dataSrc.readFunc = &gauX_data_source_file_arc_read;
  ret->dataSrc.seekFunc = &gauX_data_source_file_arc_seek;
  ret->dataSrc.tellFunc = &gauX_data_source_file_arc_tell;
  ret->dataSrc.closeFunc = &gauX_data_source_file_arc_close;
  ret->context.offset = in_offset;
  ret->context.size = in_size;
  ret->context.f = fopen(in_filename, "rb");
  if(ret->context.f && in_size >= 0)
  {
    ret->context.fileMutex = gc_mutex_create();
    fseek(ret->context.f, in_offset, SEEK_SET);
  }
  else
  {
    gcX_ops->freeFunc(ret);
    ret = 0;
  }
  return (ga_DataSource*)ret;
}


/* Memory-Based Data Source */
typedef struct gau_DataSourceMemoryContext {
  ga_Memory* memory;
  gc_int32 pos;
  gc_Mutex* memMutex;
} gau_DataSourceMemoryContext;

typedef struct gau_DataSourceMemory {
  ga_DataSource dataSrc;
  gau_DataSourceMemoryContext context;
} gau_DataSourceMemory;

gc_int32 gauX_data_source_memory_read(void* in_context, void* in_dst, gc_int32 in_size, gc_int32 in_count)
{
  gau_DataSourceMemoryContext* ctx = (gau_DataSourceMemoryContext*)in_context;
  gc_int32 ret = 0;
  gc_int32 dataSize = ga_memory_size(ctx->memory);
  gc_int32 toRead = in_size * in_count;
  gc_int32 remaining;

  gc_mutex_lock(ctx->memMutex);
  remaining = dataSize - ctx->pos;
  toRead = toRead < remaining ? toRead : remaining;
  toRead = toRead - (toRead % in_size);
  if(toRead)
  {
    memcpy(in_dst, (char*)ga_memory_data(ctx->memory) + ctx->pos, toRead);
    ctx->pos += toRead;
    ret = toRead / in_size;
  }
  gc_mutex_unlock(ctx->memMutex);
  return ret;
}
gc_int32 gauX_data_source_memory_seek(void* in_context, gc_int32 in_offset, gc_int32 in_origin)
{
  gau_DataSourceMemoryContext* ctx = (gau_DataSourceMemoryContext*)in_context;
  gc_int32 dataSize = ga_memory_size(ctx->memory);
  gc_mutex_lock(ctx->memMutex);
  switch(in_origin)
  {
  case GA_SEEK_ORIGIN_SET: ctx->pos = in_offset; break;
  case GA_SEEK_ORIGIN_CUR: ctx->pos += in_offset; break;
  case GA_SEEK_ORIGIN_END: ctx->pos = dataSize - in_offset; break;
  }
  ctx->pos = ctx->pos < 0 ? 0 : ctx->pos > dataSize ? dataSize : ctx->pos;
  gc_mutex_unlock(ctx->memMutex);
  return 0;
}
gc_int32 gauX_data_source_memory_tell(void* in_context)
{
  gau_DataSourceMemoryContext* ctx = (gau_DataSourceMemoryContext*)in_context;
  gc_int32 ret;
  gc_mutex_lock(ctx->memMutex);
  ret = ctx->pos;
  gc_mutex_unlock(ctx->memMutex);
  return ret;
}
void gauX_data_source_memory_close(void* in_context)
{
  gau_DataSourceMemoryContext* ctx = (gau_DataSourceMemoryContext*)in_context;
  ga_memory_release(ctx->memory);
  gc_mutex_destroy(ctx->memMutex);
}
ga_DataSource* gau_data_source_create_memory(ga_Memory* in_memory)
{
  gau_DataSourceMemory* ret = gcX_ops->allocFunc(sizeof(gau_DataSourceMemory));
  ga_data_source_init(&ret->dataSrc);
  ret->dataSrc.flags = GA_FLAG_SEEKABLE | GA_FLAG_THREADSAFE;
  ret->dataSrc.readFunc = &gauX_data_source_memory_read;
  ret->dataSrc.seekFunc = &gauX_data_source_memory_seek;
  ret->dataSrc.tellFunc = &gauX_data_source_memory_tell;
  ret->dataSrc.closeFunc = &gauX_data_source_memory_close;
  ga_memory_acquire(in_memory);
  ret->context.memory = in_memory;
  ret->context.pos = 0;
  ret->context.memMutex = gc_mutex_create();
  return (ga_DataSource*)ret;
}

/* WAV Sample Source */
typedef struct ga_WavData
{
  gc_int32 fileSize;
  gc_int16 fmtTag, channels, blockAlign, bitsPerSample;
  gc_int32 fmtSize, sampleRate, bytesPerSec;
  gc_int32 dataOffset, dataSize;
} ga_WavData;

void gauX_data_source_advance(ga_DataSource* in_dataSrc, gc_int32 in_delta)
{
  if(ga_data_source_flags(in_dataSrc) & GA_FLAG_SEEKABLE)
    ga_data_source_seek(in_dataSrc, in_delta, GA_SEEK_ORIGIN_CUR);
  else
  {
    char buffer[256];
    while(in_delta > 0)
    {
      gc_int32 advance = in_delta > 256 ? 256 : in_delta;
      gc_int32 bytesAdvanced = ga_data_source_read(in_dataSrc, &buffer[0], 1, advance);
      in_delta -= bytesAdvanced;
    }
  }
}
gc_result gauX_sample_source_wav_load_header(ga_DataSource* in_dataSrc, ga_WavData* out_wavData)
{
  /* TODO: Make this work with non-blocking reads? Need to get this data... */
  ga_WavData* wavData = out_wavData;
  gc_int32 seekable = ga_data_source_flags(in_dataSrc) & GA_FLAG_SEEKABLE ? 1 : 0;
  gc_int32 dataOffset = 0;
  char id[5];
  id[4] = 0;
  if(!in_dataSrc)
    return GC_ERROR_GENERIC;
  ga_data_source_read(in_dataSrc, &id[0], sizeof(char), 4); /* 'RIFF' */
  dataOffset += 4;
  if(!strcmp(id, "RIFF"))
  {
    ga_data_source_read(in_dataSrc, &wavData->fileSize, sizeof(gc_int32), 1);
    ga_data_source_read(in_dataSrc, &id[0], sizeof(char), 4); /* 'WAVE' */
    dataOffset += 8;
    if(!strcmp(id, "WAVE"))
    {
      gc_int32 dataFound = 0;
      gc_int32 hdrFound = 0;
      do
      {
        gc_int32 chunkSize = 0;
        ga_data_source_read(in_dataSrc, &id[0], sizeof(char), 4);
        ga_data_source_read(in_dataSrc, &chunkSize, sizeof(gc_int32), 1);
        dataOffset += 8;
        if(!hdrFound && !strcmp(id, "fmt ")) /* 'fmt ' */
        {
          wavData->fmtSize = chunkSize;
          ga_data_source_read(in_dataSrc, &wavData->fmtTag, sizeof(gc_int16), 1);
          ga_data_source_read(in_dataSrc, &wavData->channels, sizeof(gc_int16), 1);
          ga_data_source_read(in_dataSrc, &wavData->sampleRate, sizeof(gc_int32), 1);
          ga_data_source_read(in_dataSrc, &wavData->bytesPerSec, sizeof(gc_int32), 1);
          ga_data_source_read(in_dataSrc, &wavData->blockAlign, sizeof(gc_int16), 1);
          ga_data_source_read(in_dataSrc, &wavData->bitsPerSample, sizeof(gc_int16), 1);
          gauX_data_source_advance(in_dataSrc, chunkSize - 16);
          hdrFound = 1;
        }
        else if(!dataFound && !strcmp(id, "data")) /* 'data' */
        {
          wavData->dataSize = chunkSize;
          wavData->dataOffset = dataOffset;
          dataFound = 1;
        }
        else
        {
          gauX_data_source_advance(in_dataSrc, chunkSize);
        }
        dataOffset += chunkSize;
      } while(!(hdrFound && dataFound)); /* TODO: Need End-Of-Data support in Data Sources */
      if(hdrFound && dataFound)
        return GC_SUCCESS;
    }
  }
  return GC_ERROR_GENERIC;
}

typedef struct gau_SampleSourceWavContext {
  ga_DataSource* dataSrc;
  ga_WavData wavHeader;
  gc_int32 sampleSize;
  gc_int32 pos;
  gc_Mutex* posMutex;
} gau_SampleSourceWavContext;

typedef struct gau_SampleSourceWav {
  ga_SampleSource sampleSrc;
  gau_SampleSourceWavContext context;
} gau_SampleSourceWav;

gc_int32 gauX_sample_source_wav_read(void* in_context, void* in_dst, gc_int32 in_numSamples,
                                     tOnSeekFunc in_onSeekFunc, void* in_seekContext)
{
  gau_SampleSourceWavContext* ctx = &((gau_SampleSourceWav*)in_context)->context;
  gc_int32 numRead = 0;
  gc_int32 totalSamples = ctx->wavHeader.dataSize / ctx->sampleSize;
  gc_mutex_lock(ctx->posMutex);
  if(ctx->pos + in_numSamples > totalSamples)
    in_numSamples = totalSamples - ctx->pos;
  if(in_numSamples > 0)
  {
    numRead = ga_data_source_read(ctx->dataSrc, in_dst, ctx->sampleSize, in_numSamples);
    ctx->pos += numRead;
  }
  gc_mutex_unlock(ctx->posMutex);
  return numRead;
}
gc_int32 gauX_sample_source_wav_end(void* in_context)
{
  gau_SampleSourceWavContext* ctx = &((gau_SampleSourceWav*)in_context)->context;
  gc_int32 totalSamples = ctx->wavHeader.dataSize / ctx->sampleSize;
  return ctx->pos == totalSamples; /* No need to mutex this use */
}
gc_int32 gauX_sample_source_wav_seek(void* in_context, gc_int32 in_sampleOffset)
{
  gau_SampleSourceWavContext* ctx = &((gau_SampleSourceWav*)in_context)->context;
  gc_int32 ret;
  gc_mutex_lock(ctx->posMutex);
  ret = ga_data_source_seek(ctx->dataSrc, ctx->wavHeader.dataOffset + in_sampleOffset * ctx->sampleSize, GA_SEEK_ORIGIN_SET);
  if(ret >= 0)
    ctx->pos = in_sampleOffset;
  gc_mutex_unlock(ctx->posMutex);
  return ret;
}
gc_int32 gauX_sample_source_wav_tell(void* in_context, gc_int32* out_totalSamples)
{
  gau_SampleSourceWavContext* ctx = &((gau_SampleSourceWav*)in_context)->context;
  if(out_totalSamples)
    *out_totalSamples = ctx->wavHeader.dataSize / ctx->sampleSize;
  return ctx->pos; /* No need to mutex this use */
}
void gauX_sample_source_wav_close(void* in_context)
{
  gau_SampleSourceWavContext* ctx = &((gau_SampleSourceWav*)in_context)->context;
  ga_data_source_release(ctx->dataSrc);
  gc_mutex_destroy(ctx->posMutex);
}
ga_SampleSource* gau_sample_source_create_wav(ga_DataSource* in_dataSrc)
{
  gc_result validHeader;
  gau_SampleSourceWav* ret = gcX_ops->allocFunc(sizeof(gau_SampleSourceWav));
  gau_SampleSourceWavContext* ctx = &ret->context;
  gc_int32 seekable = ga_data_source_flags(in_dataSrc) & GA_FLAG_SEEKABLE ? 1 : 0;
  ga_sample_source_init(&ret->sampleSrc);
  ret->sampleSrc.flags = GA_FLAG_THREADSAFE;
  if(seekable)
    ret->sampleSrc.flags |= GA_FLAG_SEEKABLE;
  ret->sampleSrc.readFunc = &gauX_sample_source_wav_read;
  ret->sampleSrc.endFunc = &gauX_sample_source_wav_end;
  if(seekable)
  {
    ret->sampleSrc.seekFunc = &gauX_sample_source_wav_seek;
    ret->sampleSrc.tellFunc = &gauX_sample_source_wav_tell;
  }
  ret->sampleSrc.closeFunc = &gauX_sample_source_wav_close;
  ctx->pos = 0;
  ga_data_source_acquire(in_dataSrc);
  ctx->dataSrc = in_dataSrc;
  validHeader = gauX_sample_source_wav_load_header(in_dataSrc, &ctx->wavHeader);
  if(validHeader == GC_SUCCESS)
  {
    ctx->posMutex = gc_mutex_create();
    ret->sampleSrc.format.numChannels = ctx->wavHeader.channels;
    ret->sampleSrc.format.bitsPerSample = ctx->wavHeader.bitsPerSample;
    ret->sampleSrc.format.sampleRate = ctx->wavHeader.sampleRate;
    ctx->sampleSize = ga_format_sampleSize(&ret->sampleSrc.format);
  }
  else
  {
    ga_data_source_release(in_dataSrc);
    gcX_ops->freeFunc(ret);
    ret = 0;
  }
  return (ga_SampleSource*)ret;
}

/* OGG Sample Source */
#include <vorbis/vorbisfile.h>

typedef struct gau_OggDataSourceCallbackData
{
  ga_DataSource* dataSrc;
} gau_OggDataSourceCallbackData;

size_t gauX_sample_source_ogg_callback_read(void *ptr, size_t size, size_t nmemb, void *datasource)
{
  gau_OggDataSourceCallbackData* stream = (gau_OggDataSourceCallbackData*)datasource;
  ga_DataSource* ds = stream->dataSrc;
  size_t ret = (size_t)ga_data_source_read(ds, ptr, size, nmemb);
  return ret;
}
int gauX_sample_source_ogg_callback_seek(void *datasource, ogg_int64_t offset, int whence)
{
  gau_OggDataSourceCallbackData* stream = (gau_OggDataSourceCallbackData*)datasource;
  ga_DataSource* ds = stream->dataSrc;
  switch(whence)
  {
  case SEEK_SET: return ga_data_source_seek(ds, (gc_int32)offset, GA_SEEK_ORIGIN_SET);
  case SEEK_CUR: return ga_data_source_seek(ds, (gc_int32)offset, GA_SEEK_ORIGIN_CUR);
  case SEEK_END: return ga_data_source_seek(ds, (gc_int32)offset, GA_SEEK_ORIGIN_END);
  }
  return -1;
}
long gauX_sample_source_ogg_callback_tell(void *datasource)
{
  gau_OggDataSourceCallbackData* stream = (gau_OggDataSourceCallbackData*)datasource;
  ga_DataSource* ds = stream->dataSrc;
  return ga_data_source_tell(ds);
}
int gauX_sample_source_ogg_callback_close(void *datasource)
{
  gau_OggDataSourceCallbackData* stream = (gau_OggDataSourceCallbackData*)datasource;
  ga_DataSource* ds = stream->dataSrc;
  return 1;
}

typedef struct gau_SampleSourceOggContext {
  ga_DataSource* dataSrc;
  gc_int32 endOfSamples;
  OggVorbis_File oggFile;
  vorbis_info* oggInfo;
  gau_OggDataSourceCallbackData oggCallbackData;
  gc_Mutex* oggMutex;
} gau_SampleSourceOggContext;

typedef struct gau_SampleSourceOgg {
  ga_SampleSource sampleSrc;
  gau_SampleSourceOggContext context;
} gau_SampleSourceOgg;

FILE* gauX_openWavFile(const char* in_fn, size_t* out_dataSizeOff)
{
  FILE* f = fopen(in_fn, "wb");
  gc_int32 size = 0;
  gc_uint16 val16 = 0;
  gc_uint32 val32 = 0;
  fwrite("RIFF", 1, 4, f);
  fwrite(&size, 4, 1, f); /* file size */
  fwrite("WAVE", 1, 4, f);
  fwrite("fmt ", 1, 4, f);
  size = 16;
  fwrite(&size, 4, 1, f); /* format chunk size */
  val16 = 0x0001;
  fwrite(&val16, 2, 1, f);
  val16 = 2;
  fwrite(&val16, 2, 1, f);
  val32 = 44100;
  fwrite(&val32, 4, 1, f);
  val32 = 4 * 44100;
  fwrite(&val32, 4, 1, f);
  val16 = 2;
  fwrite(&val16, 2, 1, f);
  val16 = 16;
  fwrite(&val16, 2, 1, f);
  fwrite("data", 1, 4, f);
  *out_dataSizeOff = ftell(f);
  size = 0;
  fwrite(&size, 4, 1, f); /* data size */
  return f;
}
void gauX_closeWavFile(FILE* in_f, size_t in_dataSizeOff)
{
  size_t totalSize = ftell(in_f);
  size_t size = totalSize - 8;
  fseek(in_f, 4, SEEK_SET);
  fwrite(&size, 4, 1, in_f);
  fseek(in_f, in_dataSizeOff, SEEK_SET);
  size = size - (in_dataSizeOff + 4);
  fwrite(&size, 4, 1, in_f);
  fclose(in_f);
}

gc_int32 gauX_sample_source_ogg_read(void* in_context, void* in_dst, gc_int32 in_numSamples,
                                     tOnSeekFunc in_onSeekFunc, void* in_seekContext)
{
  gau_SampleSourceOggContext* ctx = &((gau_SampleSourceOgg*)in_context)->context;
  gc_int32 samplesLeft = in_numSamples;
  gc_int32 samplesRead;
  gc_int32 channels = ctx->oggInfo->channels;
  gc_int32 totalSamples = 0;
  size_t dataSizeOff = 0;
  do{
    gc_int32 bitStream;
    gc_float32** samples;
    gc_int32 i;
    gc_int16* dst;
    gc_int32 channel;
    gc_mutex_lock(ctx->oggMutex);
    samplesRead = ov_read_float(&ctx->oggFile, &samples, samplesLeft, &bitStream);
    if(samplesRead == 0)
      ctx->endOfSamples = 1;
    gc_mutex_unlock(ctx->oggMutex);
    if(samplesRead > 0)
    {
      samplesLeft -= samplesRead;
      dst = (gc_int16*)(in_dst) + totalSamples * channels;
      totalSamples += samplesRead;
      for(i = 0; i < samplesRead; ++i)
      {
        for(channel = 0; channel < channels; ++channel, ++dst)
        {
          gc_float32 sample = samples[channel][i] * 32768.0f;
          gc_int32 int32Sample = (gc_int32)sample;
          gc_int16 int16Sample;
          int32Sample = int32Sample > 32767 ? 32767 : int32Sample < -32768 ? -32768 : int32Sample;
          int16Sample = (gc_int16)int32Sample;
          *dst = int16Sample;
        }
      }
    }
  } while (samplesRead > 0 && samplesLeft);
  return totalSamples;
}
gc_int32 gauX_sample_source_ogg_end(void* in_context)
{
  gau_SampleSourceOggContext* ctx = &((gau_SampleSourceOgg*)in_context)->context;
  return ctx->endOfSamples; /* No need for a mutex here */
}
gc_int32 gauX_sample_source_ogg_seek(void* in_context, gc_int32 in_sampleOffset)
{
  gau_SampleSourceOggContext* ctx = &((gau_SampleSourceOgg*)in_context)->context;
  gc_int32 ret;
  gc_mutex_lock(ctx->oggMutex);
  ret = ov_pcm_seek(&ctx->oggFile, in_sampleOffset);
  ctx->endOfSamples = 0;
  gc_mutex_unlock(ctx->oggMutex);
  return ret;
}
gc_int32 gauX_sample_source_ogg_tell(void* in_context, gc_int32* out_totalSamples)
{
  gau_SampleSourceOggContext* ctx = &((gau_SampleSourceOgg*)in_context)->context;
  gc_int32 ret;
  gc_mutex_lock(ctx->oggMutex);
  /* TODO: Decide whether to support total samples for OGG files */
  if(out_totalSamples)
    *out_totalSamples = ov_pcm_total(&ctx->oggFile, -1); /* Note: This isn't always valid when the stream is poorly-formatted */
  ret = (gc_int32)ov_pcm_tell(&ctx->oggFile);
  gc_mutex_unlock(ctx->oggMutex);
  return ret;
}
void gauX_sample_source_ogg_close(void* in_context)
{
  gau_SampleSourceOggContext* ctx = &((gau_SampleSourceOgg*)in_context)->context;
  ov_clear(&ctx->oggFile);
  ga_data_source_release(ctx->dataSrc);
  gc_mutex_destroy(ctx->oggMutex);
}
ga_SampleSource* gau_sample_source_create_ogg(ga_DataSource* in_dataSrc)
{
  gau_SampleSourceOgg* ret = gcX_ops->allocFunc(sizeof(gau_SampleSourceOgg));
  gau_SampleSourceOggContext* ctx = &ret->context;
  gc_int32 endian = 0; /* 0 is little endian (aka x86), 1 is big endian */
  gc_int32 bytesPerSample = 2;
  gc_int32 isValidOgg = 0;
  gc_int32 oggIsOpen;
  ov_callbacks oggCallbacks;
  gc_int32 seekable = ga_data_source_flags(in_dataSrc) & GA_FLAG_SEEKABLE ? 1 : 0;
  ga_sample_source_init(&ret->sampleSrc);
  ret->sampleSrc.flags = GA_FLAG_THREADSAFE;
  if(seekable)
    ret->sampleSrc.flags |= GA_FLAG_SEEKABLE;
  ret->sampleSrc.readFunc = &gauX_sample_source_ogg_read;
  ret->sampleSrc.endFunc = &gauX_sample_source_ogg_end;
  if(seekable)
  {
    ret->sampleSrc.seekFunc = &gauX_sample_source_ogg_seek;
    ret->sampleSrc.tellFunc = &gauX_sample_source_ogg_tell;
  }
  ret->sampleSrc.closeFunc = &gauX_sample_source_ogg_close;
  ga_data_source_acquire(in_dataSrc);
  ctx->dataSrc = in_dataSrc;
  ctx->endOfSamples = 0;

  /* OGG Setup */
  oggCallbacks.read_func = &gauX_sample_source_ogg_callback_read;
  if(seekable)
  {
    oggCallbacks.seek_func = &gauX_sample_source_ogg_callback_seek;
    oggCallbacks.tell_func = &gauX_sample_source_ogg_callback_tell;
  }
  else
  {
    oggCallbacks.seek_func = 0;
    oggCallbacks.tell_func = 0;
  }
  oggCallbacks.close_func = &gauX_sample_source_ogg_callback_close;
  ctx->oggCallbackData.dataSrc = in_dataSrc;
  oggIsOpen = ov_open_callbacks(&ctx->oggCallbackData, &ctx->oggFile, 0, 0, oggCallbacks);
  if(oggIsOpen == 0) /* 0 means "open" */
  {
    ctx->oggInfo = ov_info(&ctx->oggFile, -1);
    ov_pcm_seek(&ctx->oggFile, 0); /* Seek fixes some poorly-formatted OGGs. */
    isValidOgg = ctx->oggInfo->channels <= 2;
    if(isValidOgg)
    {
      ret->sampleSrc.format.bitsPerSample = bytesPerSample * 8;
      ret->sampleSrc.format.numChannels = ctx->oggInfo->channels;
      ret->sampleSrc.format.sampleRate = ctx->oggInfo->rate;
    }
    else
      ov_clear(&ctx->oggFile);
  }
  if(isValidOgg)
    ctx->oggMutex = gc_mutex_create();
  else
  {
    ga_data_source_release(in_dataSrc);
    gcX_ops->freeFunc(ret);
    ret = 0;
  }
  return (ga_SampleSource*)ret;
}

/* Stream Sample Source */
typedef struct gau_SampleSourceStreamContext {
  ga_BufferedStream* stream;
} gau_SampleSourceStreamContext;

typedef struct gau_SampleSourceStream {
  ga_SampleSource sampleSrc;
  gau_SampleSourceStreamContext context;
} gau_SampleSourceStream;

gc_int32 gauX_sample_source_stream_read(void* in_context, void* in_dst, gc_int32 in_numSamples,
                                        tOnSeekFunc in_onSeekFunc, void* in_seekContext)
{
  gau_SampleSourceStreamContext* ctx = &((gau_SampleSourceStream*)in_context)->context;
  gc_int32 numRead = 0;
  numRead = ga_stream_read(ctx->stream, in_dst, in_numSamples);
  return numRead;
}
gc_int32 gauX_sample_source_stream_end(void* in_context)
{
  gau_SampleSourceStreamContext* ctx = &((gau_SampleSourceStream*)in_context)->context;
  return ga_stream_end(ctx->stream);
}
gc_int32 gauX_sample_source_stream_ready(void* in_context, gc_int32 in_numSamples)
{
  gau_SampleSourceStreamContext* ctx = &((gau_SampleSourceStream*)in_context)->context;
  return ga_stream_ready(ctx->stream, in_numSamples);
}
gc_int32 gauX_sample_source_stream_seek(void* in_context, gc_int32 in_sampleOffset)
{
  gau_SampleSourceStreamContext* ctx = &((gau_SampleSourceStream*)in_context)->context;
  return ga_stream_seek(ctx->stream, in_sampleOffset);
}
gc_int32 gauX_sample_source_stream_tell(void* in_context, gc_int32* out_totalSamples)
{
  gau_SampleSourceStreamContext* ctx = &((gau_SampleSourceStream*)in_context)->context;
  return ga_stream_tell(ctx->stream, out_totalSamples);
}
void gauX_sample_source_stream_close(void* in_context)
{
  gau_SampleSourceStreamContext* ctx = &((gau_SampleSourceStream*)in_context)->context;
  ga_stream_release(ctx->stream);
}
ga_SampleSource* gau_sample_source_create_stream(ga_StreamManager* in_mgr, ga_SampleSource* in_sampleSrc, gc_int32 in_bufferSamples)
{
  gau_SampleSourceStream* ret = gcX_ops->allocFunc(sizeof(gau_SampleSourceStream));
  gau_SampleSourceStreamContext* ctx = &ret->context;
  gc_int32 sampleSize;
  ga_BufferedStream* stream;
  ga_sample_source_init(&ret->sampleSrc);
  ga_sample_source_format(in_sampleSrc, &ret->sampleSrc.format);
  sampleSize = ga_format_sampleSize(&ret->sampleSrc.format);
  stream = ga_stream_create(in_mgr, in_sampleSrc, in_bufferSamples * sampleSize);
  if(stream)
  {
    ctx->stream = stream;
    ret->sampleSrc.flags = ga_stream_flags(stream);
    ret->sampleSrc.flags |= GA_FLAG_THREADSAFE;
    ret->sampleSrc.readFunc = &gauX_sample_source_stream_read;
    ret->sampleSrc.endFunc = &gauX_sample_source_stream_end;
    ret->sampleSrc.readyFunc = &gauX_sample_source_stream_ready;
    if(ret->sampleSrc.flags & GA_FLAG_SEEKABLE)
    {
      ret->sampleSrc.seekFunc = &gauX_sample_source_stream_seek;
      ret->sampleSrc.tellFunc = &gauX_sample_source_stream_tell;
    }
    ret->sampleSrc.closeFunc = &gauX_sample_source_stream_close;
  }
  else
  {
    gcX_ops->freeFunc(ret);
    ret = 0;
  }
  return (ga_SampleSource*)ret;
}

/* Loop Sample Source */
typedef struct gau_SampleSourceLoopContext {
  ga_SampleSource* innerSrc;
  gc_int32 triggerSample;
  gc_int32 targetSample;
  gc_Mutex* loopMutex;
  gc_int32 sampleSize;
  volatile gc_int32 loopCount;
} gau_SampleSourceLoopContext;

struct gau_SampleSourceLoop {
  ga_SampleSource sampleSrc;
  gau_SampleSourceLoopContext context;
};

gc_int32 gauX_sample_source_loop_read(void* in_context, void* in_dst, gc_int32 in_numSamples,
                                      tOnSeekFunc in_onSeekFunc, void* in_seekContext)
{
  gau_SampleSourceLoopContext* ctx = &((gau_SampleSourceLoop*)in_context)->context;
  gc_int32 numRead = 0;
  gc_int32 triggerSample, targetSample;
  gc_int32 pos, total;
  gc_int32 sampleSize;
  gc_int32 totalRead = 0;
  ga_SampleSource* ss = ctx->innerSrc;
  gc_mutex_lock(ctx->loopMutex);
  triggerSample = ctx->triggerSample;
  targetSample = ctx->targetSample;
  gc_mutex_unlock(ctx->loopMutex);
  pos = ga_sample_source_tell(ss, &total);
  if((targetSample < 0 && triggerSample <= 0))
    return ga_sample_source_read(ss, in_dst, in_numSamples, 0, 0);
  if(triggerSample <= 0)
    triggerSample = total;
  if(pos > triggerSample)
    return ga_sample_source_read(ss, in_dst, in_numSamples, 0, 0);
  sampleSize = ctx->sampleSize;
  while(in_numSamples)
  {
    gc_int32 avail = triggerSample - pos;
    gc_int32 doSeek = avail <= in_numSamples;
    gc_int32 toRead = doSeek ? avail : in_numSamples;
    numRead = ga_sample_source_read(ss, in_dst,  toRead, 0, 0);
    totalRead += numRead;
    in_numSamples -= numRead;
    in_dst = (char*)in_dst + numRead * sampleSize;
    if(doSeek && toRead == numRead)
    {
      ga_sample_source_seek(ss, targetSample);
      ++ctx->loopCount;
      if(in_onSeekFunc)
        in_onSeekFunc(totalRead, targetSample - triggerSample, in_seekContext);
    }
    pos = ga_sample_source_tell(ss, &total);
  }
  return totalRead;
}
gc_int32 gauX_sample_source_loop_end(void* in_context)
{
  gau_SampleSourceLoopContext* ctx = &((gau_SampleSourceLoop*)in_context)->context;
  return ga_sample_source_end(ctx->innerSrc);
}
gc_int32 gauX_sample_source_loop_ready(void* in_context, gc_int32 in_numSamples)
{
  gau_SampleSourceLoopContext* ctx = &((gau_SampleSourceLoop*)in_context)->context;
  return ga_sample_source_ready(ctx->innerSrc, in_numSamples);
}
gc_int32 gauX_sample_source_loop_seek(void* in_context, gc_int32 in_sampleOffset)
{
  gau_SampleSourceLoopContext* ctx = &((gau_SampleSourceLoop*)in_context)->context;
  return ga_sample_source_seek(ctx->innerSrc, in_sampleOffset);
}
gc_int32 gauX_sample_source_loop_tell(void* in_context, gc_int32* out_totalSamples)
{
  gau_SampleSourceLoopContext* ctx = &((gau_SampleSourceLoop*)in_context)->context;
  return ga_sample_source_tell(ctx->innerSrc, out_totalSamples);
}
void gauX_sample_source_loop_close(void* in_context)
{
  gau_SampleSourceLoopContext* ctx = &((gau_SampleSourceLoop*)in_context)->context;
  ga_sample_source_release(ctx->innerSrc);
  gc_mutex_destroy(ctx->loopMutex);
}
void gau_sample_source_loop_set(gau_SampleSourceLoop* in_sampleSrc, gc_int32 in_triggerSample, gc_int32 in_targetSample)
{
  gau_SampleSourceLoopContext* ctx = &in_sampleSrc->context;
  gc_mutex_lock(ctx->loopMutex);
  ctx->targetSample = in_targetSample;
  ctx->triggerSample = in_triggerSample;
  ctx->loopCount = 0;
  gc_mutex_unlock(ctx->loopMutex);
}
gc_int32 gau_sample_source_loop_count(gau_SampleSourceLoop* in_sampleSrc)
{
  gau_SampleSourceLoopContext* ctx = &in_sampleSrc->context;
  return ctx->loopCount;
}
void gau_sample_source_loop_clear(gau_SampleSourceLoop* in_sampleSrc)
{
  gau_sample_source_loop_set(in_sampleSrc, -1, -1);
}
gau_SampleSourceLoop* gau_sample_source_create_loop(ga_SampleSource* in_sampleSrc)
{
  gau_SampleSourceLoop* ret = gcX_ops->allocFunc(sizeof(gau_SampleSourceLoop));
  gau_SampleSourceLoopContext* ctx = &ret->context;
  gc_int32 sampleSize;
  ga_sample_source_init(&ret->sampleSrc);
  ga_sample_source_acquire(in_sampleSrc);
  ga_sample_source_format(in_sampleSrc, &ret->sampleSrc.format);
  sampleSize = ga_format_sampleSize(&ret->sampleSrc.format);
  ctx->triggerSample = -1;
  ctx->targetSample = -1;
  ctx->loopCount = 0;
  ctx->loopMutex = gc_mutex_create();
  ctx->innerSrc = in_sampleSrc;
  ctx->sampleSize = sampleSize;
  ret->sampleSrc.flags = ga_sample_source_flags(in_sampleSrc);
  ret->sampleSrc.flags |= GA_FLAG_THREADSAFE;
  assert(ret->sampleSrc.flags & GA_FLAG_SEEKABLE);
  ret->sampleSrc.readFunc = &gauX_sample_source_loop_read;
  ret->sampleSrc.endFunc = &gauX_sample_source_loop_end;
  ret->sampleSrc.readyFunc = &gauX_sample_source_loop_ready;
  ret->sampleSrc.seekFunc = &gauX_sample_source_loop_seek;
  ret->sampleSrc.tellFunc = &gauX_sample_source_loop_tell;
  ret->sampleSrc.closeFunc = &gauX_sample_source_loop_close;
  return ret;
}

/* Sound Sample Source */
typedef struct gau_SampleSourceSoundContext {
  ga_Sound* sound;
  gc_int32 sampleSize;
  gc_int32 numSamples;
  gc_Mutex* posMutex;
  volatile gc_int32 pos; /* Volatile, but shouldn't need a mutex around use */
} gau_SampleSourceSoundContext;

typedef struct gau_SampleSourceSound {
  ga_SampleSource sampleSrc;
  gau_SampleSourceSoundContext context;
} gau_SampleSourceSound;

gc_int32 gauX_sample_source_sound_read(void* in_context, void* in_dst, gc_int32 in_numSamples,
                                       tOnSeekFunc in_onSeekFunc, void* in_seekContext)
{
  gau_SampleSourceSoundContext* ctx = &((gau_SampleSourceSound*)in_context)->context;
  ga_Sound* snd = ctx->sound;
  char* src;
  gc_int32 pos;
  gc_int32 avail;
  gc_int32 numRead;
  gc_mutex_lock(ctx->posMutex);
  pos = ctx->pos;
  avail = ctx->numSamples - pos;
  numRead = in_numSamples > avail ? avail : in_numSamples;
  ctx->pos += numRead;
  gc_mutex_unlock(ctx->posMutex);
  src = (char*)ga_sound_data(snd) + pos * ctx->sampleSize;
  memcpy(in_dst, src, numRead * ctx->sampleSize);
  return numRead;
}
gc_int32 gauX_sample_source_sound_end(void* in_context)
{
  gau_SampleSourceSoundContext* ctx = &((gau_SampleSourceSound*)in_context)->context;
  return ctx->pos >= ctx->numSamples;
}
gc_int32 gauX_sample_source_sound_seek(void* in_context, gc_int32 in_sampleOffset)
{
  gau_SampleSourceSoundContext* ctx = &((gau_SampleSourceSound*)in_context)->context;
  if(in_sampleOffset > ctx->numSamples)
    return -1;
  gc_mutex_lock(ctx->posMutex);
  ctx->pos = in_sampleOffset;
  gc_mutex_unlock(ctx->posMutex);
  return 0;
}
gc_int32 gauX_sample_source_sound_tell(void* in_context, gc_int32* out_totalSamples)
{
  gau_SampleSourceSoundContext* ctx = &((gau_SampleSourceSound*)in_context)->context;
  *out_totalSamples = ctx->numSamples;
  return ctx->pos;
}
void gauX_sample_source_sound_close(void* in_context)
{
  gau_SampleSourceSoundContext* ctx = &((gau_SampleSourceSound*)in_context)->context;
  ga_sound_release(ctx->sound);
  gc_mutex_destroy(ctx->posMutex);
}
ga_SampleSource* gau_sample_source_create_sound(ga_Sound* in_sound)
{
  gau_SampleSourceSound* ret = gcX_ops->allocFunc(sizeof(gau_SampleSourceSound));
  gau_SampleSourceSoundContext* ctx = &ret->context;
  gc_int32 sampleSize;
  ga_sample_source_init(&ret->sampleSrc);
  ga_sound_acquire(in_sound);
  ga_sound_format(in_sound, &ret->sampleSrc.format);
  sampleSize = ga_format_sampleSize(&ret->sampleSrc.format);
  ctx->posMutex = gc_mutex_create();
  ctx->sound = in_sound;
  ctx->sampleSize = sampleSize;
  ctx->numSamples = ga_sound_numSamples(in_sound);
  ctx->pos = 0;
  ret->sampleSrc.flags = GA_FLAG_THREADSAFE | GA_FLAG_SEEKABLE;
  ret->sampleSrc.readFunc = &gauX_sample_source_sound_read;
  ret->sampleSrc.endFunc = &gauX_sample_source_sound_end;
  ret->sampleSrc.seekFunc = &gauX_sample_source_sound_seek;
  ret->sampleSrc.tellFunc = &gauX_sample_source_sound_tell;
  ret->sampleSrc.closeFunc = &gauX_sample_source_sound_close;
  return (ga_SampleSource*)ret;
}

/* Helper functions */
#ifdef __APPLE__
#define stricmp strcasecmp
#elif defined(_WIN32)
#define stricmp _stricmp
#endif /* __APPLE__ */
ga_Memory* gau_load_memory_file(const char* in_filename)
{
  ga_Memory* ret;
  ga_DataSource* fileDataSrc = gau_data_source_create_file(in_filename);
  ret = ga_memory_create_data_source(fileDataSrc);
  ga_data_source_release(fileDataSrc);
  return ret;
}

ga_Sound* gau_load_sound_file(const char* in_filename, const char* in_format)
{
  ga_Sound* ret = 0;
  ga_DataSource* dataSrc = gau_data_source_create_file(in_filename);
  if(dataSrc)
  {
    ga_SampleSource* sampleSrc = 0;
    if(stricmp(in_format, "ogg") == 0)
      sampleSrc = gau_sample_source_create_ogg(dataSrc);
    else if(stricmp(in_format, "wav") == 0)
      sampleSrc = gau_sample_source_create_wav(dataSrc);
    ga_data_source_release(dataSrc);
    if(sampleSrc)
    {
      ret = ga_sound_create_sample_source(sampleSrc);
      ga_sample_source_release(sampleSrc);
    }
  }
  return ret;
}
ga_Handle* gau_create_handle_sound(ga_Mixer* in_mixer, ga_Sound* in_sound,
                                   ga_FinishCallback in_callback, void* in_context,
                                   gau_SampleSourceLoop** out_loopSrc)
{
  ga_Handle* ret = 0;
  ga_SampleSource* sampleSrc = sampleSrc = gau_sample_source_create_sound(in_sound);
  if(sampleSrc)
  {
    ga_SampleSource* sampleSrc2 = sampleSrc;
    if(out_loopSrc)
    {
      gau_SampleSourceLoop* loopSampleSrc = gau_sample_source_create_loop(sampleSrc);
      gau_sample_source_loop_set(loopSampleSrc, -1, 0);
      ga_sample_source_release(sampleSrc);
      *out_loopSrc = loopSampleSrc;
      sampleSrc2 = (ga_SampleSource*)loopSampleSrc;
    }
    if(sampleSrc2)
    {
      ret = ga_handle_create(in_mixer, sampleSrc2);
      if(sampleSrc == sampleSrc2)
        ga_sample_source_release(sampleSrc2);
      ga_handle_setCallback(ret, in_callback, in_context);
    }
  }
  return ret;
}

ga_Handle* gau_create_handle_memory(ga_Mixer* in_mixer, ga_Memory* in_memory, const char* in_format,
                                    ga_FinishCallback in_callback, void* in_context,
                                    gau_SampleSourceLoop** out_loopSrc)
{
  ga_Handle* ret = 0;
  ga_DataSource* dataSrc = gau_data_source_create_memory(in_memory);
  if(dataSrc)
  {
    ga_SampleSource* sampleSrc = 0;
    if(stricmp(in_format, "ogg") == 0)
      sampleSrc = gau_sample_source_create_ogg(dataSrc);
    else if(stricmp(in_format, "wav") == 0)
      sampleSrc = gau_sample_source_create_wav(dataSrc);
    if(sampleSrc)
    {
      ga_SampleSource* sampleSrc2 = sampleSrc;
      if(out_loopSrc)
      {
        gau_SampleSourceLoop* loopSampleSrc = gau_sample_source_create_loop(sampleSrc);
        gau_sample_source_loop_set(loopSampleSrc, -1, 0);
        ga_sample_source_release(sampleSrc);
        *out_loopSrc = loopSampleSrc;
        sampleSrc2 = (ga_SampleSource*)loopSampleSrc;
      }
      if(sampleSrc2)
      {
        ret = ga_handle_create(in_mixer, sampleSrc2);
        if(sampleSrc == sampleSrc2)
          ga_sample_source_release(sampleSrc2);
        ga_handle_setCallback(ret, in_callback, in_context);
      }
    }
  }
  return ret;
}
ga_Handle* gau_create_handle_buffered_data(ga_Mixer* in_mixer, ga_StreamManager* in_streamMgr,
                                           ga_DataSource* in_dataSrc, const char* in_format,
                                           ga_FinishCallback in_callback, void* in_context,
                                           gau_SampleSourceLoop** out_loopSrc)
{
  ga_Handle* ret = 0;
  ga_DataSource* dataSrc = in_dataSrc;
  if(in_dataSrc)
  {
    ga_SampleSource* sampleSrc = 0;
    if(stricmp(in_format, "ogg") == 0)
      sampleSrc = gau_sample_source_create_ogg(dataSrc);
    else if(stricmp(in_format, "wav") == 0)
      sampleSrc = gau_sample_source_create_wav(dataSrc);
    if(sampleSrc)
    {
      ga_SampleSource* sampleSrc2 = sampleSrc;
      if(out_loopSrc)
      {
        gau_SampleSourceLoop* loopSampleSrc = gau_sample_source_create_loop(sampleSrc);
        gau_sample_source_loop_set(loopSampleSrc, -1, 0);
        ga_sample_source_release(sampleSrc);
        *out_loopSrc = loopSampleSrc;
        sampleSrc2 = (ga_SampleSource*)loopSampleSrc;
      }
      if(sampleSrc2)
      {
        ga_SampleSource* streamSampleSrc = gau_sample_source_create_stream(in_streamMgr,
          sampleSrc2,
          131072);
        if(sampleSrc == sampleSrc2)
          ga_sample_source_release(sampleSrc2);
        if(streamSampleSrc)
        {
          ret = ga_handle_create(in_mixer, streamSampleSrc);
          ga_sample_source_release(streamSampleSrc);
          ga_handle_setCallback(ret, in_callback, in_context);
        }
      }
    }
  }
  return ret;
}
ga_Handle* gau_create_handle_buffered_file(ga_Mixer* in_mixer, ga_StreamManager* in_streamMgr,
                                           const char* in_filename, const char* in_format,
                                           ga_FinishCallback in_callback, void* in_context,
                                           gau_SampleSourceLoop** out_loopSrc)
{
  ga_Handle* ret = 0;
  ga_DataSource* dataSrc = gau_data_source_create_file(in_filename);
  if(dataSrc)
  {
    ga_SampleSource* sampleSrc = 0;
    if(stricmp(in_format, "ogg") == 0)
      sampleSrc = gau_sample_source_create_ogg(dataSrc);
    else if(stricmp(in_format, "wav") == 0)
      sampleSrc = gau_sample_source_create_wav(dataSrc);
    ga_data_source_release(dataSrc);
    if(sampleSrc)
    {
      ga_SampleSource* sampleSrc2 = sampleSrc;
      if(out_loopSrc)
      {
        gau_SampleSourceLoop* loopSampleSrc = gau_sample_source_create_loop(sampleSrc);
        gau_sample_source_loop_set(loopSampleSrc, -1, 0);
        ga_sample_source_release(sampleSrc);
        *out_loopSrc = loopSampleSrc;
        sampleSrc2 = (ga_SampleSource*)loopSampleSrc;
      }
      if(sampleSrc2)
      {
        ga_SampleSource* streamSampleSrc = gau_sample_source_create_stream(in_streamMgr,
          sampleSrc2,
          131072);
        if(sampleSrc == sampleSrc2)
          ga_sample_source_release(sampleSrc2);
        if(streamSampleSrc)
        {
          ret = ga_handle_create(in_mixer, streamSampleSrc);
          ga_sample_source_release(streamSampleSrc);
          ga_handle_setCallback(ret, in_callback, in_context);
        }
      }
    }
  }
  return ret;
}
