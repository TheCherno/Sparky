#include "gc_common.h"

#include "gc_thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Thread Functions */

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static gc_int32 priorityLut[] = {
  0, -1, 1, 2
};

gc_Thread* gc_thread_create(gc_ThreadFunc in_threadFunc, void* in_context,
                            gc_int32 in_priority, gc_int32 in_stackSize)
{
  gc_Thread* ret = gcX_ops->allocFunc(sizeof(gc_Thread));
  ret->threadObj = gcX_ops->allocFunc(sizeof(HANDLE));
  ret->threadFunc = in_threadFunc;
  ret->context = in_context;
  ret->priority = in_priority;
  ret->stackSize = in_stackSize;
  *(HANDLE*)ret->threadObj = CreateThread(0, in_stackSize, (LPTHREAD_START_ROUTINE)in_threadFunc, in_context, CREATE_SUSPENDED, (LPDWORD)&ret->id);
  SetThreadPriority(*(HANDLE*)ret->threadObj, priorityLut[in_priority]);
  return ret;
}
void gc_thread_run(gc_Thread* in_thread)
{
  ResumeThread(*(HANDLE*)in_thread->threadObj);
}
void gc_thread_join(gc_Thread* in_thread)
{
  WaitForSingleObject(*(HANDLE*)in_thread->threadObj, INFINITE);
}
void gc_thread_sleep(gc_uint32 in_ms)
{
  Sleep(in_ms);
}
void gc_thread_destroy(gc_Thread* in_thread)
{
  CloseHandle(*(HANDLE*)in_thread->threadObj);
  gcX_ops->freeFunc(in_thread->threadObj);
  gcX_ops->freeFunc(in_thread);
}

#elif defined(__linux__) || defined(__APPLE__)
#include <pthread.h>
#include <sched.h>

static gc_int32 priorityLut[] = {
  0, -10, 10, 19
};

typedef struct LinuxThreadData {
  pthread_t thread;
  pthread_attr_t attr;
  pthread_mutex_t suspendMutex;
  gc_ThreadFunc threadFunc;
  void* context;
} LinuxThreadData;

static void* StaticThreadWrapper(void* in_context)
{
  LinuxThreadData* threadData = (LinuxThreadData*)in_context;
  pthread_mutex_lock(&threadData->suspendMutex);
  threadData->threadFunc(threadData->context);
  pthread_mutex_unlock(&threadData->suspendMutex);
  return 0;
}

gc_Thread* gc_thread_create(gc_ThreadFunc in_threadFunc, void* in_context,
                            gc_int32 in_priority, gc_int32 in_stackSize)
{
  int result = 0;
  struct sched_param param;
  gc_Thread* ret = gcX_ops->allocFunc(sizeof(gc_Thread));
  LinuxThreadData* threadData = (LinuxThreadData*)gcX_ops->allocFunc(sizeof(LinuxThreadData));
  threadData->threadFunc = in_threadFunc;
  threadData->context = in_context;
  
  ret->threadObj = threadData;
  ret->threadFunc = in_threadFunc;
  ret->context = in_context;
  ret->priority = in_priority;
  ret->stackSize = in_stackSize;
  
  pthread_mutex_init(&threadData->suspendMutex, NULL);
  pthread_mutex_lock(&threadData->suspendMutex);
  
  result = pthread_attr_init(&threadData->attr);
  #if defined(__APPLE__) || defined(__ANDROID__)
  param.sched_priority = priorityLut[in_priority];
  #elif defined(__linux__)
  param.__sched_priority = priorityLut[in_priority];
  #endif /* __APPLE__ */
  pthread_attr_setschedparam(&threadData->attr, &param);
  pthread_create(&threadData->thread, &threadData->attr, StaticThreadWrapper, threadData);
  
  return ret;
}
void gc_thread_run(gc_Thread* in_thread)
{
  LinuxThreadData* threadData = (LinuxThreadData*)in_thread->threadObj;
  pthread_mutex_unlock(&threadData->suspendMutex);
}
void gc_thread_join(gc_Thread* in_thread)
{
  LinuxThreadData* threadData = (LinuxThreadData*)in_thread->threadObj;
  pthread_join(threadData->thread, 0);
}
void gc_thread_sleep(gc_uint32 in_ms)
{
  usleep(in_ms * 1000);
}
void gc_thread_destroy(gc_Thread* in_thread)
{
  LinuxThreadData* threadData = (LinuxThreadData*)in_thread->threadObj;
  pthread_mutex_destroy(&threadData->suspendMutex);
  pthread_exit(&threadData->thread);
  pthread_attr_destroy(&threadData->attr);
}

#else
#error Thread class not yet defined for this platform
#endif /* _WIN32 */

/* Mutex Functions */

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

gc_Mutex* gc_mutex_create()
{
  gc_Mutex* ret = gcX_ops->allocFunc(sizeof(gc_Mutex));
  ret->mutex = gcX_ops->allocFunc(sizeof(CRITICAL_SECTION));
  InitializeCriticalSection((CRITICAL_SECTION*)ret->mutex);
  return ret;
}
void gc_mutex_destroy(gc_Mutex* in_mutex)
{
  DeleteCriticalSection((CRITICAL_SECTION*)in_mutex->mutex);
  gcX_ops->freeFunc(in_mutex->mutex);
  gcX_ops->freeFunc(in_mutex);
}
void gc_mutex_lock(gc_Mutex* in_mutex)
{
  EnterCriticalSection((CRITICAL_SECTION*)in_mutex->mutex);
}
void gc_mutex_unlock(gc_Mutex* in_mutex)
{
  LeaveCriticalSection((CRITICAL_SECTION*)in_mutex->mutex);
}

#elif defined(__linux__) || defined(__APPLE__)

#include <pthread.h>

gc_Mutex* gc_mutex_create()
{
  gc_Mutex* ret = gcX_ops->allocFunc(sizeof(gc_Mutex));
  ret->mutex = gcX_ops->allocFunc(sizeof(pthread_mutex_t));
  pthread_mutex_init((pthread_mutex_t*)ret->mutex, NULL);
  return ret;
}
void gc_mutex_destroy(gc_Mutex* in_mutex)
{
  pthread_mutex_destroy((pthread_mutex_t*)in_mutex->mutex);
  gcX_ops->freeFunc(in_mutex->mutex);
  gcX_ops->freeFunc(in_mutex);
}
void gc_mutex_lock(gc_Mutex* in_mutex)
{
  pthread_mutex_lock((pthread_mutex_t*)in_mutex->mutex);
}
void gc_mutex_unlock(gc_Mutex* in_mutex)
{
  pthread_mutex_unlock((pthread_mutex_t*)in_mutex->mutex);
}

#else
#error Mutex class not yet defined for this platform
#endif /* _WIN32 */
