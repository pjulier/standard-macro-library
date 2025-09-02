#ifndef INCLUDE_SML_LOGGER_H
#define INCLUDE_SML_LOGGER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
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

/**
 * @brief The main logger struct
 */
typedef struct SML_Logger {
    SML_Logger_consoleWriteFn writeConsole; /**< console write function */
    SML_LogLevel logLvlConsole;             /**< lowest log level that is printed to console */
    SML_LogLevel logLvlFile;                /**< lowest log level that is printed to file */
    bool isInit;                            /**< is logger initialized? */
} SML_Logger;

void SML_Logger_setConsoleWriteFn(SML_Logger_consoleWriteFn fn);
bool SML_Logger_setLogLevelConsole(SML_LogLevel level);
void SML_Logger_output(SML_LogLevel level, const char *msg, ...);

/*
 * Logging macro definitions
 * NOTE: these should be preferred over
 * calling SML_Logger_output() directly
 */
#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_FATAL
#define LOGFATAL(message, ...) SML_Logger_output(SML_LOG_LVL_FATAL, message, ##__VA_ARGS__)
#else
#define LOGFATAL(message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_ERROR
#define LOGERROR(message, ...) SML_Logger_output(SML_LOG_LVL_ERROR, message, ##__VA_ARGS__)
#else
#define LOGERROR(message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_WARN
#define LOGWARN(message, ...) SML_Logger_output(SML_LOG_LVL_WARN, message, ##__VA_ARGS__)
#else
#define LOGWARN(message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_INFO
#define LOGINFO(message, ...) SML_Logger_output(SML_LOG_LVL_INFO, message, ##__VA_ARGS__)
#else
#define LOGINFO(message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_DEBUG
#define LOGDEBUG(message, ...) SML_Logger_output(SML_LOG_LVL_DEBUG, message, ##__VA_ARGS__)
#else
#define LOGDEBUG(message, ...)
#endif

#if SML_LOG_SUPPORT_LEVEL <= SML_LOG_LVL_TRACE
#define LOGTRACE(message, ...) SML_Logger_output(SML_LOG_LVL_TRACE, message, ##__VA_ARGS__)
#else
#define LOGTRACE(message, ...)
#endif


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_LOGGER_H */
