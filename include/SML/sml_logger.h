#ifndef INCLUDE_SML_LOGGER_H
#define INCLUDE_SML_LOGGER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function name macro
 * Use either the predefined identifier from C99
 * or the non-standard __FUNCTION__ for MSVC
 */
#if defined(_MSC_VER)
#define __SML_FUNC__ __FUNCTION__
#else
#define __SML_FUNC__ __func__
#endif

/*
 * Log levels from lowest to highest priority
 * NOTE: Defined as preprocessor macros instead of enum 
 * so that SML_LOG_SUPPORT_LEVEL can be used
 */
#define SML_LOG_LVL_TRACE 0
#define SML_LOG_LVL_DEBUG 1
#define SML_LOG_LVL_INFO  2
#define SML_LOG_LVL_WARN  3
#define SML_LOG_LVL_ERROR 4
#define SML_LOG_LVL_FATAL 5
#define SML_LOG_LVL_NEVER 6

/**
 * @brief Macro to eliminate logger function calls
 * NOTE: LOG_SUPPORT_LEVEL defines the lowest priority
 * level that is still supported (i.e. remains in the code)
 */
#ifndef SML_LOG_SUPPORT_LEVEL
#define SML_LOG_SUPPORT_LEVEL SML_LOG_LVL_TRACE
#endif

/**
 * @brief Console write function prototype for the logger
 */
typedef void (*SML_Logger_consoleWriteFn)(const char *msg, unsigned int level);

/**
 * @brief Datatype for the log level
 */
typedef int SML_LogLevel;

void SML_Logger_setConsoleWriteFn(SML_Logger_consoleWriteFn fn);
bool SML_Logger_setFormat(const char *tag, const char *fmt);
bool SML_Logger_setLogLevelConsole(const char *tag, SML_LogLevel level);
void SML_Logger_output(const char *tag, SML_LogLevel level, const char *func, int line, const char *msg, ...);
void SML_Logger_destroy(void);

/*
 * Logging macro definitions
 * NOTE: these should be preferred over
 * calling SML_Logger_output() directly
 */
#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_FATAL
#define LOGFATAL(tag, message, ...) SML_Logger_output(tag, SML_LOG_LVL_FATAL, __SML_FUNC__, __LINE__, message, ##__VA_ARGS__)
#else
#define LOGFATAL(tag, message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_ERROR
#define LOGERROR(tag, message, ...) SML_Logger_output(tag, SML_LOG_LVL_ERROR, __SML_FUNC__, __LINE__, message, ##__VA_ARGS__)
#else
#define LOGERROR(tag, message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_WARN
#define LOGWARN(tag, message, ...) SML_Logger_output(tag, SML_LOG_LVL_WARN, __SML_FUNC__, __LINE__, message, ##__VA_ARGS__)
#else
#define LOGWARN(tag, message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_INFO
#define LOGINFO(tag, message, ...) SML_Logger_output(tag, SML_LOG_LVL_INFO, __SML_FUNC__, __LINE__, message, ##__VA_ARGS__)
#else
#define LOGINFO(tag, message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_DEBUG
#define LOGDEBUG(tag, message, ...) SML_Logger_output(tag, SML_LOG_LVL_DEBUG, __SML_FUNC__, __LINE__, message, ##__VA_ARGS__)
#else
#define LOGDEBUG(tag, message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_TRACE
#define LOGTRACE(tag, message, ...) SML_Logger_output(tag, SML_LOG_LVL_TRACE,  __SML_FUNC__, __LINE__, message, ##__VA_ARGS__)
#else
#define LOGTRACE(tag, message, ...)
#endif


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_LOGGER_H */
