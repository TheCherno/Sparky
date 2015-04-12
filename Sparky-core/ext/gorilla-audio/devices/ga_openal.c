#include "../ga.h"

#include "ga_openal.h"

#include <OpenAL/al.h>"
#include <OpenAL/alc.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

const char* gaX_openAlErrorToString(ALuint error)
{
  const char* errMsg = 0;
  switch(error)
  {
  case AL_NO_ERROR: errMsg = "OpenAL error - None"; break;
  case AL_INVALID_NAME: errMsg = "OpenAL error - Invalid name."; break;
  case AL_INVALID_ENUM: errMsg = "OpenAL error - Invalid enum."; break;
  case AL_INVALID_VALUE: errMsg = "OpenAL error - Invalid value."; break;
  case AL_INVALID_OPERATION: errMsg = "OpenAL error - Invalid op."; break;
  case AL_OUT_OF_MEMORY: errMsg = "OpenAL error - Out of memory."; break;
  default: errMsg = "OpenAL error - Unknown error."; break;
  }
  return errMsg;
}

static gc_int32 AUDIO_ERROR = 0;

#define CHECK_AL_ERROR \
  if((AUDIO_ERROR = alGetError()) != AL_NO_ERROR) \
    printf("%s\n", gaX_openAlErrorToString(AUDIO_ERROR));

ga_DeviceImpl_OpenAl* gaX_device_open_openAl(gc_int32 in_numBuffers,
                                             gc_int32 in_numSamples,
                                             ga_Format* in_format)
{
  ga_DeviceImpl_OpenAl* ret = gcX_ops->allocFunc(sizeof(ga_DeviceImpl_OpenAl));
  ALCboolean ctxRet;

  ret->devType = GA_DEVICE_TYPE_OPENAL;
  ret->numBuffers = in_numBuffers;
  ret->numSamples = in_numSamples;
  memcpy(&ret->format, in_format, sizeof(ga_Format));
  ret->nextBuffer = 0;
  ret->emptyBuffers = ret->numBuffers;
#if 0// _WIN32
  ret->dev = alcOpenDevice("DirectSound");
#else
  ret->dev = alcOpenDevice(0);
#endif /* _WIN32 */
  if(!ret->dev)
    goto cleanup;
  ret->context = alcCreateContext(ret->dev, 0);
  if(!ret->context)
    goto cleanup;
  ctxRet = alcMakeContextCurrent(ret->context);
  if(ctxRet == ALC_FALSE)
    goto cleanup;

  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  CHECK_AL_ERROR;
  if(AUDIO_ERROR != AL_NO_ERROR)
    goto cleanup;
  ret->hwBuffers = gcX_ops->allocFunc(sizeof(gc_uint32) * ret->numBuffers);
  alGenBuffers(ret->numBuffers, ret->hwBuffers);
  CHECK_AL_ERROR;
  if(AUDIO_ERROR != AL_NO_ERROR)
    goto cleanup;
  alGenSources(1, &ret->hwSource);
  CHECK_AL_ERROR;
  if(AUDIO_ERROR != AL_NO_ERROR)
  {
    alDeleteBuffers(ret->numBuffers, ret->hwBuffers);
    goto cleanup;
  }
  return ret;

cleanup:
  if(ret->hwBuffers && ret->hwBuffers != 0xcdcdcdcd)
    gcX_ops->freeFunc(ret->hwBuffers);
  if(ret->context)
    alcDestroyContext(ret->context);
  if(ret->dev)
    alcCloseDevice(ret->dev);
  gcX_ops->freeFunc(ret);
  return 0;
}
gc_result gaX_device_close_openAl(ga_DeviceImpl_OpenAl* in_device)
{
  alDeleteSources(1, &in_device->hwSource);
  alDeleteBuffers(in_device->numBuffers, in_device->hwBuffers);
  alcDestroyContext(in_device->context);
  alcCloseDevice(in_device->dev);
  in_device->devType = GA_DEVICE_TYPE_UNKNOWN;
  gcX_ops->freeFunc(in_device->hwBuffers);
  gcX_ops->freeFunc(in_device);
  return GC_SUCCESS;
}
gc_int32 gaX_device_check_openAl(ga_DeviceImpl_OpenAl* in_device)
{
  gc_int32 whichBuf = 0;
  ga_DeviceImpl_OpenAl* d = in_device;
  gc_int32 numProcessed = 0;
  alGetSourcei(in_device->hwSource, AL_BUFFERS_PROCESSED, &numProcessed);
  CHECK_AL_ERROR;
  while(numProcessed--)
  {
    whichBuf = (d->nextBuffer + d->emptyBuffers++) % d->numBuffers;
    alSourceUnqueueBuffers(d->hwSource, 1, &d->hwBuffers[whichBuf]);
    CHECK_AL_ERROR;
  }
  return d->emptyBuffers;
}
gc_result gaX_device_queue_openAl(ga_DeviceImpl_OpenAl* in_device,
                                  void* in_buffer)
{
  gc_int32 formatOal;
  gc_int32 sampleSize;
  ALint state;
  gc_int32 bps = in_device->format.bitsPerSample;
  ga_DeviceImpl_OpenAl* d = in_device;

  if(in_device->format.numChannels == 1)
    formatOal = (gc_int32)(bps == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8);
  else
    formatOal = (gc_int32)(bps == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8);
  sampleSize = ga_format_sampleSize(&in_device->format);
  alBufferData(d->hwBuffers[d->nextBuffer], formatOal, in_buffer,
               (ALsizei)in_device->numSamples * sampleSize, in_device->format.sampleRate);
  CHECK_AL_ERROR;
  if(AUDIO_ERROR != AL_NO_ERROR)
    return GC_ERROR_GENERIC;
  alSourceQueueBuffers(d->hwSource, 1, &d->hwBuffers[d->nextBuffer]);
  CHECK_AL_ERROR;
  if(AUDIO_ERROR != AL_NO_ERROR)
    return GC_ERROR_GENERIC;
  d->nextBuffer = (d->nextBuffer + 1) % d->numBuffers;
  --d->emptyBuffers;
  alGetSourcei(d->hwSource, AL_SOURCE_STATE, &state);
  CHECK_AL_ERROR;
  if(state != AL_PLAYING)
  {
    /* NOTE: calling this, even as a 'noop', can cause a clicking sound. */
    alSourcePlay(d->hwSource);
  }
  CHECK_AL_ERROR;
  return GC_SUCCESS;
}
