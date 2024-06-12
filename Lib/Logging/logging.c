#include "logging.h"

#include "SEGGER_RTT.h"
#include <stdarg.h>
#include <string.h>

#define LOG_COLOR_DEFAULT RTT_CTRL_RESET RTT_CTRL_TEXT_BRIGHT_WHITE
#define LOG_COLOR_DEBUG RTT_CTRL_TEXT_BRIGHT_BLUE
#define LOG_COLOR_INFO RTT_CTRL_TEXT_BRIGHT_GREEN
#define LOG_COLOR_WARNING RTT_CTRL_TEXT_BRIGHT_YELLOW
#define LOG_COLOR_ERROR RTT_CTRL_TEXT_BRIGHT_RED
#define LOG_COLOR_FILE RTT_CTRL_TEXT_WHITE

const char *basename(const char *path)
{
    char *base = strrchr(path, '/');
    return base ? base + 1 : path;
}

void _log(const unsigned level, const char *file, int line, const char *fmt, ...)
{
    const char *file_name = basename(file);

    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        SEGGER_RTT_printf(0, "%sDEB %s%s:%d\t%s  ", LOG_COLOR_DEBUG, LOG_COLOR_FILE, file_name, line, LOG_COLOR_DEFAULT);
        break;
    case LOG_LEVEL_INFO:
        SEGGER_RTT_printf(0, "%sINF %s%s:%d\t%s  ", LOG_COLOR_INFO, LOG_COLOR_FILE, file_name, line, LOG_COLOR_DEFAULT);
        break;
    case LOG_LEVEL_WARNING:
        SEGGER_RTT_printf(0, "%sWRN %s%s:%d\t%s  ", LOG_COLOR_WARNING, LOG_COLOR_FILE, file_name, line, LOG_COLOR_DEFAULT);
        break;
    case LOG_LEVEL_ERROR:
        SEGGER_RTT_printf(0, "%sERR %s%s:%d\t%s  ", LOG_COLOR_ERROR, LOG_COLOR_FILE, file_name, line, LOG_COLOR_DEFAULT);
        break;
    default:
        SEGGER_RTT_printf(0, "%s??? %s%s:%d\t%s  ", LOG_COLOR_FILE, LOG_COLOR_FILE, file_name, line, LOG_COLOR_DEFAULT);
        break;
    }

    va_list args;
    va_start(args, fmt);
    SEGGER_RTT_vprintf(0, fmt, &args);
    va_end(args);

    SEGGER_RTT_printf(0, "\n");
}