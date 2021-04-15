#include <drone_util.h>

const int POSTURE_SIZE = sizeof(Posture);

const int OPERATION_SIZE = sizeof(Operation);

const int RECVPACKAGE_SIZE = sizeof(RecvPackage);

void mylog(const char *func, const char *file, const int line,
          const int level, const char *format, ...)
{
    // 日志级别选择
    if (level < LOG_LEVEL) {
        return;
    }
    const char *level_mark;
    switch (level) {
        case DEBUG:
            level_mark = "DEBUG"; break;
        case WARNING:
            level_mark = "WARNING"; break;
        case ERROR:
            level_mark = "ERROR"; break;
        default:
            level_mark = "UNKNOWN";
    }

    // 获取本地时间
    time_t loacl_time;
    char time_str[128];
    time(&loacl_time);
    // strftime(time_str, sizeof(time_str), "[%Y.%m.%d %X]", localtime(&loacl_time));
    strftime(time_str, sizeof(time_str), "%X", localtime(&loacl_time));
    
    // 日志内容格式转换
    va_list ap;
    va_start(ap, format);
    char fmt_str[2048];
    vsnprintf(fmt_str, sizeof(fmt_str), format, ap);
    va_end(ap);
    
    // 打印日志
    // ssPrintf("[%s]%s[%s@%s:%d] %s\n", level_mark, time_str, func, file, line, fmt_str);
    // 忽略 __FILE__ 打印日志
    ssPrintf("[%s][%s][%s:%d] %s\n", level_mark, time_str, func, line, fmt_str);
};