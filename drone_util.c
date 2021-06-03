#include <drone_util.h>

const int POSTURE_SIZE = sizeof(Posture);

const int OPERATION_SIZE = sizeof(Operation);

const int RECVPACKAGE_SIZE = sizeof(RecvPackage);

const int ROLE_SIZE = sizeof(Role);

void mylog(const char *func, const char *file, const int line,
          const int level, const int tolog, const char *fn,
          const char *format, ...)
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
//     time_t t;
//     struct tm *p;
//     time(&t);
//     p = localtime(&t);
//     char time_str[18];
//     sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", 1900+p->tm_year, p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    time_t timer;
    char time_str[26];
    struct tm* tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    
    // 日志内容格式转换
    va_list ap;
    va_start(ap, format);
    char fmt_str[2048];
    vsnprintf(fmt_str, sizeof(fmt_str), format, ap);
    va_end(ap);
    
    // 打印日志到 Viewer
    // ssPrintf("[%s]%s[%s@%s:%d] %s\n", level_mark, time_str, func, file, line, fmt_str);
    // 忽略 __FILE__ 打印日志
    ssPrintf("[%s][%s][%s:%d] %s", level_mark, time_str, func, line, fmt_str);
    
    // 打印日志到 文件
    //int tolog = 1;
    //const char *filename = "./logs/firstlog.txt";
    int ret = -1;
    FILE *fp = NULL;
    
    if (tolog == 1) {
        fp = fopen(fn, "a+");
        ret = fprintf(fp, "[%s][%s][%s:%d] %s", level_mark, time_str, func, line, fmt_str);
    }
    
    if (ret >= 0) {
        fflush(fp);
        fclose(fp);
    }
    
};



const char *getLogName(const int32_T role_id, const int32_T role_tag)
{
    char role_str[7];
    sprintf(role_str, "%04d_%02d", role_id, role_tag);
    
    const char *dirname = "./logs";
    const char *postfix= "txt";
    
    char *filename = malloc(strlen(dirname) + 1 + strlen(role_str) + 1 + strlen(postfix));
    sprintf(filename, "%s/%s.%s", dirname, role_str, postfix);  // ./logs/1001_01.txt
    
    return filename;
}