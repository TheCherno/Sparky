#include "gc_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* System Functions */
gc_SystemOps* gcX_ops = 0;

static void* gcX_defaultAllocFunc(gc_uint32 in_size)
{
  return malloc(in_size);
}
static void* gcX_defaultReallocFunc(void* in_ptr, gc_uint32 in_size)
{
  return realloc(in_ptr, in_size);
}
static void gcX_defaultFreeFunc(void* in_ptr)
{
  free(in_ptr);
}
static gc_SystemOps s_defaultCallbacks;
gc_result gc_initialize(gc_SystemOps* in_callbacks)
{
  s_defaultCallbacks.allocFunc = &gcX_defaultAllocFunc;
  s_defaultCallbacks.reallocFunc = &gcX_defaultReallocFunc;
  s_defaultCallbacks.freeFunc = &gcX_defaultFreeFunc;
  gcX_ops = in_callbacks ? in_callbacks : &s_defaultCallbacks;
  return GC_SUCCESS;
}
gc_result gc_shutdown()
{
  gcX_ops = 0;
  return GC_SUCCESS;
}

/* Circular Buffer Functions */
gc_CircBuffer* gc_buffer_create(gc_uint32 in_size)
{
  gc_CircBuffer* ret;
  if(!in_size || (in_size & (in_size - 1))) /* Must be power-of-two*/
    return 0;
  ret = gcX_ops->allocFunc(sizeof(gc_CircBuffer));
  ret->data = gcX_ops->allocFunc(in_size);
  ret->dataSize = in_size;
  ret->nextAvail = 0;
  ret->nextFree = 0;
  return ret;
}
gc_result gc_buffer_destroy(gc_CircBuffer* in_buffer)
{
  gcX_ops->freeFunc(in_buffer->data);
  gcX_ops->freeFunc(in_buffer);
  return GC_SUCCESS;
}
gc_uint32 gc_buffer_bytesAvail(gc_CircBuffer* in_buffer)
{
  /* producer/consumer call (all race-induced errors should be tolerable) */
  return in_buffer->nextFree - in_buffer->nextAvail;
}
gc_uint32 gc_buffer_bytesFree(gc_CircBuffer* in_buffer)
{
  /* producer/consumer call (all race-induced errors should be tolerable) */
  return in_buffer->dataSize - gc_buffer_bytesAvail(in_buffer);
}
gc_int32 gc_buffer_getFree(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes,
                           void** out_dataA, gc_uint32* out_sizeA,
                           void** out_dataB, gc_uint32* out_sizeB)
{
  /* producer-only call */
  gc_CircBuffer* b = in_buffer;
  gc_uint32 size = b->dataSize;
  gc_uint32 nextFree = b->nextFree % size;
  gc_uint32 nextAvail = b->nextAvail % size;
  gc_uint32 maxBytes = size - nextFree;
  if(in_numBytes > gc_buffer_bytesFree(b))
    return -1;
  if(maxBytes >= in_numBytes)
  {
    *out_dataA = &b->data[nextFree];
    *out_sizeA = in_numBytes;
    return 1;
  }
  else
  {
    *out_dataA = &b->data[nextFree];
    *out_sizeA = maxBytes;
    *out_dataB = &b->data[0];
    *out_sizeB = in_numBytes - maxBytes;
    return 2;
  }
}
gc_result gc_buffer_write(gc_CircBuffer* in_buffer, void* in_data,
                          gc_uint32 in_numBytes)
{
  /* TODO: Make this call gc_buffer_getFree() instead of duping code */

  /* producer-only call */
  gc_CircBuffer* b = in_buffer;
  gc_uint32 size = b->dataSize;
  gc_uint32 nextFree = b->nextFree % size;
  gc_uint32 nextAvail = b->nextAvail % size;
  gc_uint32 maxBytes = size - nextFree;
  if(in_numBytes > gc_buffer_bytesFree(b))
    return GC_ERROR_GENERIC;
  if(maxBytes >= in_numBytes)
    memcpy(&b->data[nextFree], in_data, in_numBytes);
  else
  {
    memcpy(&b->data[nextFree], in_data, maxBytes);
    memcpy(&b->data[0], (char*)in_data + maxBytes, in_numBytes - maxBytes);
  }
  b->nextFree += in_numBytes;
  return GC_SUCCESS;
}
gc_int32 gc_buffer_getAvail(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes,
                            void** out_dataA, gc_uint32* out_sizeA,
                            void** out_dataB, gc_uint32* out_sizeB)
{
  /* consumer-only call */
  gc_CircBuffer* b = in_buffer;
  gc_uint32 bytesAvailable = gc_buffer_bytesAvail(in_buffer);
  gc_uint32 size = b->dataSize;
  gc_uint32 nextFree = b->nextFree % size;
  gc_uint32 nextAvail = b->nextAvail % size;
  gc_uint32 maxBytes = size - nextAvail;
  if(bytesAvailable < in_numBytes)
    return -1;
  if(maxBytes >= in_numBytes)
  {
    *out_dataA = &b->data[nextAvail];
    *out_sizeA = in_numBytes;
    *out_dataB = 0;
    *out_sizeB = 0;
    return 1;
  }
  else
  {
    *out_dataA = &b->data[nextAvail];
    *out_sizeA = maxBytes;
    *out_dataB = &b->data[0];
    *out_sizeB = in_numBytes - maxBytes;
    return 2;
  }
}
void gc_buffer_read(gc_CircBuffer* in_buffer, void* in_data,
                    gc_uint32 in_numBytes)
{
  /* consumer-only call */
  void* data[2];
  gc_uint32 size[2];
  gc_int32 ret = gc_buffer_getAvail(in_buffer, in_numBytes,
                                   &data[0], &size[0],
                                   &data[1], &size[1]);
  if(ret >= 1)
  {
    memcpy(in_data, data[0], size[0]);
    if(ret == 2)
      memcpy((char*)in_data + size[0], data[1], size[1]);
  }
}
void gc_buffer_produce(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes)
{
  /* producer-only call */
  in_buffer->nextFree += in_numBytes;
}

void gc_buffer_consume(gc_CircBuffer* in_buffer, gc_uint32 in_numBytes)
{
  /* consumer-only call */
  in_buffer->nextAvail += in_numBytes;
}

/* List Functions */
void gc_list_head(gc_Link* in_head)
{
  in_head->next = in_head;
  in_head->prev = in_head;
}
void gc_list_link(gc_Link* in_head, gc_Link* in_link, void* in_data)
{
  in_link->data = in_data;
  in_link->prev = in_head;
  in_link->next = in_head->next;
  in_head->next->prev = in_link;
  in_head->next = in_link;
}
void gc_list_unlink(gc_Link* in_link)
{
  in_link->prev->next = in_link->next;
  in_link->next->prev = in_link->prev;
  in_link->prev = 0;
  in_link->next = 0;
  in_link->data = 0;
}
