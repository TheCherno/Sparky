#pragma once

#include <stdio.h>

#define SPARKY_ASSERT(x, m) \
	do { \
	if (!(x)) \
		{ \
		char* file = __FILE__; \
		unsigned int last = 0; \
		char* c; \
		for (c = file; *c != '\0'; c++) \
				{ \
			if (*c == '\\' || *c == '/') \
				last = c - file; \
				} \
		printf("\n"); \
		printf("*************************\n"); \
		printf("    ASSERTION FAILED!    \n"); \
		printf("*************************\n"); \
		printf("%s\n", #x); \
		char* message = m; \
		if (message[0] != '\0') \
			printf("%s\n", m); \
		printf("-> "); \
		for (int i = last + 1; i < c - file; i++) \
			printf("%c", file[i]); \
		printf(":%d\n", __LINE__); \
		*(int*)NULL = 8; \
	} \
	} while(0)

#define SPARKY_LOG_LEVEL_FATAL 0
#define SPARKY_LOG_LEVEL_ERROR 1
#define SPARKY_LOG_LEVEL_WARN  2
#define SPARKY_LOG_LEVEL_INFO  3

#ifndef SPARKY_LOG_LEVEL
	#define SPARKY_LOG_LEVEL SPARKY_LOG_LEVEL_INFO
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_FATAL
	#define SPARKY_FATAL(x, ...) do { printf("[SPARKY][FATAL]: "); printf(x, __VA_ARGS__); printf("\n"); SPARKY_ASSERT(false, ""); } while(0)
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_ERROR
	#define SPARKY_ERROR(x, ...) do { printf("[SPARKY][ERROR]: "); printf(x, __VA_ARGS__); printf("\n"); } while(0)
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_WARN
	#define SPARKY_WARN(x, ...) do { printf("[SPARKY][WARN]: "); printf(x, __VA_ARGS__); printf("\n"); } while(0)
#endif

#if SPARKY_LOG_LEVEL >= SPARKY_LOG_LEVEL_INFO
	#define SPARKY_INFO(x, ...) do { printf("[SPARKY][INFO]: "); printf(x, __VA_ARGS__); printf("\n"); } while(0)
#endif