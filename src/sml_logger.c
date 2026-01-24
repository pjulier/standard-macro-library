#include <stdarg.h>
#include <stdio.h>

#include "SML/sml_string.h"
#include "SML/sml_logger.h"

/**
 * @brief Maximum length of a log message
 */
#define SML_LOG_MSG_LEN 4096

/*
 * Static functions
 */
static void SML_Logger_writeConsole(const char *msg, unsigned int color);
static inline SML_Logger* SML_Logger_get(void);
static void SML_Logger_initDflt(SML_Logger *logger);

/* provide storage for the singleton logger structure */
static SML_Logger g_logger = {
    .writeConsole = SML_Logger_writeConsole,
    .logLvlConsole = SML_LOG_LVL_WARN,
    .logLvlFile = SML_LOG_LVL_WARN,
    .isInit = false
};

#if defined(WIN32)
static void SML_Logger_writeConsole(const char *msg, unsigned int color)
{
    // TODO: make proper function for windows
    (void)color;
    printf("%s\n", msg);
}
#else
static void SML_Logger_writeConsole(const char *msg, unsigned int color)
{
    static const char *colorStrings[] = { "1;30", "1;34", "1;32", "1;33", "1;31", "0;41" };
    printf("\033[%sm%s\033[0m\n", colorStrings[color], msg);
}
#endif

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
    logger->writeConsole  = SML_Logger_writeConsole;
    logger->logLvlConsole = SML_LOG_LVL_WARN;
    logger->logLvlFile    = SML_LOG_LVL_WARN;
    logger->isInit        = true;
}



/*
 * Public functions
 */
void SML_Logger_setConsoleWriteFn(SML_Logger_consoleWriteFn fn)
{
    SML_Logger *logger = SML_Logger_get();
    if (!fn) {
        /* set to default */
        logger->writeConsole = SML_Logger_writeConsole;
        return;
    }
    logger->writeConsole = fn;
}

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
        vsnprintf(to, SML_LOG_MSG_LEN - 8, msg, ap);
        va_end(ap);

        /* write to console */
        (*logger->writeConsole)(out_msg, level);
    }
    // TODO: output to file
}