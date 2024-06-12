#ifndef _LOGGING_H
#define _LOGGING_H

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_NONE 4

void _log(const unsigned level, const char *file, int line, const char *fmt, ...);

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_D(...) _log(LOG_LEVEL_DEBUG, __func__, __LINE__, __VA_ARGS__)
#else
#define LOG_D(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_I(...) _log(LOG_LEVEL_INFO, __func__, __LINE__, __VA_ARGS__)
#else
#define LOG_I(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_WARNING
#define LOG_W(...) _log(LOG_LEVEL_WARNING, __func__, __LINE__, __VA_ARGS__)
#else
#define LOG_W(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_E(...) _log(LOG_LEVEL_ERROR, __func__, __LINE__, __VA_ARGS__)
#else
#define LOG_E(...)
#endif

#endif // _LOGGING_H