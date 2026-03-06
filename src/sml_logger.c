#include <stdarg.h>
#include <stdio.h>

#include "SML/sml_math.h"
#include "SML/sml_string.h"
#include "SML/sml_logger.h"

/*
 * Maximum length of a log message
 */
#define SML_LOG_MSG_LEN 4096

/*
 * X macro for logger attributes
 */
#define FOR_LIST_OF_ATTRIBS(DO) \
    DO(message) \
    DO(level) \
    DO(func) \
    DO(line) 

/*
 * Enumeration for logger attributes
 */
#define DEFINE_ATTRIB_ENUM(attrib_) SML_LOGGER_ATTRIB_ ## attrib_,
typedef enum SML_LoggerAttributeID {
    FOR_LIST_OF_ATTRIBS(DEFINE_ATTRIB_ENUM)
    SML_LOGGER_ATTRIB_MAX_COUNT
} SML_LoggerAttributeID;

/**
 * @brief A single logger attribute
 * 
 */
typedef struct SML_LoggerAttribute {
    uint8_t id;             /**< attribute identifier */
    uint8_t formatIdx;      /**< index in format string */
} SML_LoggerAttribute;

/**
 * @brief Complete logger format specification
 * 
 */
typedef struct SML_LoggerFormat {
    SML_LoggerAttribute attribs[SML_LOGGER_ATTRIB_MAX_COUNT];  /**< array of format attributes*/
    unsigned int attribCount;                                  /**< number of used attributes */
    char formatStr[64];                                        /**< format string */
} SML_LoggerFormat;

/**
 * @brief The main logger struct
 */
typedef struct SML_Logger {
    SML_LoggerFormat format;                /**< format specification */
    SML_Logger_consoleWriteFn writeConsole; /**< console write function */
    SML_LogLevel logLvlConsole;             /**< lowest log level that is printed to console */
    SML_LogLevel logLvlFile;                /**< lowest log level that is printed to file */
    bool isInit;                            /**< is logger initialized? */
} SML_Logger;

/*
 * Static functions
 */
static void               SML_Logger_writeConsole(const char *message, unsigned int color);
static inline SML_Logger* SML_Logger_get(void);
static void               SML_Logger_init(SML_Logger *logger);

static bool               SML_LoggerFormat_set(SML_LoggerFormat *logFmt, const char *fmt);
static uint8_t            SML_Logger_AttribName2Enum(const char *name);

/* provide storage for the singleton logger structure */
static SML_Logger g_logger = {
    .format = { 
        .attribCount = 1,
        .formatStr = "%", 
        .attribs[0] = {
            .id = SML_LOGGER_ATTRIB_message, 
            .formatIdx = 0
        }
    },
    .writeConsole = SML_Logger_writeConsole,
    .logLvlConsole = SML_LOG_LVL_WARN,
    .logLvlFile = SML_LOG_LVL_WARN,
    .isInit = false
};

#if defined(WIN32)
static void SML_Logger_writeConsole(const char *message, unsigned int color)
{
    // TODO: make proper function for windows
    (void)color;
    printf("%s\n", message);
}
#else
static void SML_Logger_writeConsole(const char *message, unsigned int color)
{
    static const char *colorStrings[] = { "1;30", "1;34", "1;32", "1;33", "1;31", "0;41" };
    printf("\033[%sm%s\033[0m\n", colorStrings[color], message);
}
#endif

static inline SML_Logger* SML_Logger_get(void)
{
    /* lazy init */
    if (!g_logger.isInit) {
        SML_Logger_init(&g_logger);
    }
    return &g_logger;
}

static void SML_Logger_init(SML_Logger *logger)
{
    SML_LoggerFormat_set(&logger->format, "[%(level)] - %(message)");
    logger->writeConsole  = SML_Logger_writeConsole;
    logger->logLvlConsole = SML_LOG_LVL_WARN;
    logger->logLvlFile    = SML_LOG_LVL_WARN;
    logger->isInit        = true;
}

static uint8_t SML_Logger_AttribName2Enum(const char *name)
{
#define ATTRIBNAME_2_ENUM(attrib_, ...) if (!strcmp(name, #attrib_)) return SML_LOGGER_ATTRIB_ ## attrib_;
    FOR_LIST_OF_ATTRIBS(ATTRIBNAME_2_ENUM)
    return SML_LOGGER_ATTRIB_MAX_COUNT;
}

static bool SML_LoggerFormat_set(SML_LoggerFormat *logFmt, const char *fmt)
{
    const char *c = fmt;
    
    enum { SML_LOGGER_FORMAT_STATE_STR, SML_LOGGER_FORMAT_STATE_PERC, SML_LOGGER_FORMAT_STATE_ATTRIB };
    unsigned int state = SML_LOGGER_FORMAT_STATE_STR;

    unsigned int formatStrIdx = 0;
    SML_LoggerFormat format;
    format.attribCount = 0;
    memset(format.formatStr, '\0', sizeof(format.formatStr));

    unsigned int attribStrIdx;
    char attrib[32];
    memset(attrib, '\0', sizeof(attrib));

    while (*c != '\0') {
        switch (state) {
            case SML_LOGGER_FORMAT_STATE_STR:
                if (formatStrIdx >= SML_ARRCOUNT(format.formatStr) - 1) {
                    return false;
                }
                format.formatStr[formatStrIdx++] = *c;
                if (*c == '%') {
                    state = SML_LOGGER_FORMAT_STATE_PERC;
                }
                break;
            case SML_LOGGER_FORMAT_STATE_PERC:
                if (*c != '(') {
                    return false;
                }
                state = SML_LOGGER_FORMAT_STATE_ATTRIB;
                attribStrIdx = 0;
                break;
            case SML_LOGGER_FORMAT_STATE_ATTRIB:
                if (attribStrIdx >= SML_ARRCOUNT(attrib) - 1) {
                    return false;
                }
                if (*c == ')') {
                    attrib[attribStrIdx++] = '\0';
                    uint8_t type = SML_Logger_AttribName2Enum(attrib);

                    if (type == SML_LOGGER_ATTRIB_MAX_COUNT) {
                        return false;
                    }
                    if (format.attribCount >= SML_ARRCOUNT(format.attribs)) {
                        return false;
                    }

                    format.attribs[format.attribCount].id = type;
                    format.attribs[format.attribCount].formatIdx = formatStrIdx - 1;
                    ++format.attribCount;

                    state = SML_LOGGER_FORMAT_STATE_STR;
                } else {
                    attrib[attribStrIdx++] = *c;
                }
                break;
        }
        ++c;
    }

    /* final state correct? */
    if (state != SML_LOGGER_FORMAT_STATE_STR) {
        return false;
    }

    /* success, only now copy format */
    *logFmt = format;
    return true;
}

/*
 * Public functions
 */
bool SML_Logger_setFormat(const char *fmt)
{
    return SML_LoggerFormat_set(&SML_Logger_get()->format, fmt);
}

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

void SML_Logger_output(SML_LogLevel level, const char *func, int line, const char *message, ...)
{
    SML_Logger *logger = SML_Logger_get();

    /* output to console */
    if (logger->logLvlConsole < SML_LOG_LVL_NEVER && level >= logger->logLvlConsole) {
        static const char *levelStrings[] = { "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL" };
        char out_msg[SML_LOG_MSG_LEN];
        size_t remaining = SML_ARRCOUNT(out_msg);
        char *to = out_msg;
        char *pFormat = logger->format.formatStr;

        for (unsigned int i = 0; i < logger->format.attribCount; ++i) {
            SML_LoggerAttribute *attrib = &logger->format.attribs[i];

            /* copy ordinary format characters */
            remaining = SML_LOG_MSG_LEN - (to - out_msg);
            const size_t avail = SML_size_min(remaining, logger->format.formatStr + attrib->formatIdx - pFormat + 1);
            SML_strscpy(to, pFormat, avail);
            to += avail - 1;
            pFormat += avail; /* omitting -1 will consume the '%' character as well */

            remaining = SML_LOG_MSG_LEN - (to - out_msg);
            if (remaining < 2) {
                goto done;
            }

            /* insert formatted data */
            switch (attrib->id) {
                case SML_LOGGER_ATTRIB_message: {
                    va_list ap;
                    va_start(ap, message);
                    const size_t count = vsnprintf(to, remaining, message, ap);
                    to = to + SML_size_min(count, remaining - 1);
                    va_end(ap);
                    break;
                }
                case SML_LOGGER_ATTRIB_level: {
                    const size_t written = SML_strscpy(to, levelStrings[level], remaining);
                    to = to + SML_size_min(written, remaining - 1);
                    break;
                }
                case SML_LOGGER_ATTRIB_func: {
                    const size_t written = SML_strscpy(to, func, remaining);
                    to = to + SML_size_min(written, remaining - 1);
                    break;
                }
                case SML_LOGGER_ATTRIB_line: {
                    to = SML_itoap(to, remaining, line, 10);
                    break;
                }
            }
        }

        /* copy rest of format string */
        SML_strscpy(to, pFormat, SML_LOG_MSG_LEN - (to - out_msg));

done:
        /* write to console */
        (*logger->writeConsole)(out_msg, level);
    }
    // TODO: output to file
}