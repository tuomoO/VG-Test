#pragma once

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#define LOG(...) ((void)__android_log_print(ANDROID_LOG_WARN, "VGTEST", __VA_ARGS__))

#define LOG_ACCELEROMETER false