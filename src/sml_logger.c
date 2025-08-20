#include <stdarg.h>
#include <stdio.h>

#include "SML/sml_string.h"
#include "SML/sml_logger.h"

/**
 * @brief Maximum length of a log message
 */
#define SML_LOG_MSG_LEN 1024

/* provide storage for the singleton logger structure */
static SML_Logger g_logger = {
    .logLvlConsole = SML_LOG_LVL_WARN,
    .logLvlFile = SML_LOG_LVL_WARN,
    .isInit = false
};

/*
 * Static functions
 */
static void SML_Logger_writeConsole(const char *msg, unsigned int color);
static inline SML_Logger* SML_Logger_get(void);
static void SML_Logger_initDflt(SML_Logger *logger);

// TODO: support all platforms, for now only unix
static void SML_Logger_writeConsole(const char *msg, unsigned int color)
{
    static const char *colorStrings[] = { "1;30", "1;34", "1;32", "1;33", "1;31", "0;41" };
    printf("\033[%sm%s\033[0m\n", colorStrings[color], msg);
}

static inline SML_Logger* SML_Logger_get(void)
{
    /* lazy init */
    if (!g_logger.isInit) {
        SML_Logger_initDflt(&g_logger);
    }
    return &g_logger;
}

static void SML_Logger_initDflt(SML_Logger *logger)
{
    logger->logLvlConsole = SML_LOG_LVL_WARN;
    logger->logLvlFile    = SML_LOG_LVL_WARN;
    logger->isInit        = true;
}



/*
 * Public functions
 */
bool SML_Logger_setLogLevelConsole(SML_LogLevel level)
{   
    SML_Logger *logger = SML_Logger_get();
    if (level < SML_LOG_LVL_TRACE || level > SML_LOG_LVL_NEVER) {
        return false;
    }
    logger->logLvlConsole = level;
    return true;
}

void SML_Logger_output(SML_LogLevel level, const char *msg, ...)
{
    SML_Logger *logger = SML_Logger_get();

    /* output to console */
    if (logger->logLvlConsole < SML_LOG_LVL_NEVER && level >= logger->logLvlConsole) {
        static const char *levelStrings[] = { "[TRACE] ","[DEBUG] ","[INFO]  ","[WARN]  ","[ERROR] ","[FATAL] " };
        char out_msg[SML_LOG_MSG_LEN];

        /* copy prefix part */
        char *to = out_msg;
        to = SML_strpcpy(to, levelStrings[level]);

        /* copy message part */
        va_list ap;
        va_start(ap, msg);
        vsnprintf(to, SML_LOG_MSG_LEN - SML_ARRCOUNT(levelStrings[0]), msg, ap);
        va_end(ap);

        /* write to console */
        SML_Logger_writeConsole(out_msg, level);
    }
    // TODO: output to file
}