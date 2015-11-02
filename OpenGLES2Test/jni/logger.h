#pragma once


#ifdef _DEBUG
#if defined (OS_ANDROID)
#include <android/log.h>
#define Log(tag, message, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, "File: %s, Line: %d, Method: %s, Message: " message, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#endif

#if defined (OS_WINDOWS) 
#include <stdio.h>
#define Log(tag,message,...) printf("[%s]File: %s, Line: %d, Method: %s, Message: "message"\n\n",tag, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#endif

#else
#define  Log(tag, message, ...) (void)(0)
#endif