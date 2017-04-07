#ifndef FPP_DEBUG_HPP
#define FPP_DEBUG_HPP

#include <android/log.h>
#include <cstdlib>

#define FPP_PEDANTIC 1
#define FPP_LOG_DEBUG 1

#if FPP_LOG_DEBUG
#define FPP_LOGD(...) {__android_log_print(ANDROID_LOG_DEBUG, FPP_LOG_TAG, __VA_ARGS__);}
#else
#define FPP_LOGD(...) {}
#endif
#define FPP_LOGI(...) {__android_log_print(ANDROID_LOG_INFO, FPP_LOG_TAG, __VA_ARGS__);}
#define FPP_LOGW(...) {__android_log_print(ANDROID_LOG_WARN, FPP_LOG_TAG, __VA_ARGS__);}
#define FPP_LOGE(...) {__android_log_print(ANDROID_LOG_ERROR, FPP_LOG_TAG, __VA_ARGS__);}

#define FPP_ERR(...) { FPP_LOGE(__VA_ARGS__); std::exit(EXIT_FAILURE); }
#define FPP_ERR_IF(cond, ...) { if (cond) { FPP_ERR(__VA_ARGS__); } }
#define FPP_WARN_IF(cond, ...) { if (cond) { FPP_LOGW(__VA_ARGS__); } }

#if FPP_PEDANTIC
#define FPP_PED_ERR(...) { FPP_ERR(__VA_ARGS__); }
#define FPP_PED_ERR_IF(cond, ...) { FPP_ERR_IF(cond, __VA_ARGS__); }
#define FPP_PED_WARN(...) { FPP_LOGW(__VA_ARGS__); }
#define FPP_PED_WARN_IF(cond, ...) { FPP_WARN_IF(cond, __VA_ARGS__); }
#else
#define FPP_PED_ERR(...) {}
#define FPP_PED_ERR_IF(cond, ...) {}
#define FPP_PED_WARN(...) {}
#define FPP_PED_WARN_IF(cond, ...) {}
#endif

#endif
