#ifndef DRONE_UTIL_H
#define DRONE_UTIL_H

#include <stdarg.h>
#include <time.h>

#include <simstruc.h>

// 发送与接收的包结构体

// 战机位姿
typedef struct {
    int32_T x; // 位置
    int32_T y;
    int32_T z;
    int32_T u; // 姿态\朝向  
    int32_T v;
    int32_T w;
    int32_T vx; // 速度
    int32_T vy;
    int32_T vz;
    int32_T vu; // 角速度
    int32_T vv;
    int32_T vw;
} Posture;
extern const int POSTURE_SIZE;

// 接受的操作
typedef struct {
    int32_T pitch;      // 俯仰
    int32_T roll;       // 翻滚
    int32_T yaw;        // 偏航
    int32_T accelerate; // 油门，throttle(节流阀
    uint32_T weapon;    // 武器
    int32_T steps;      // 步距
} Operation;
extern const int OPERATION_SIZE;

// 接收包
typedef struct {
    Posture post;
    Operation op;  
} RecvPackage;
extern const int RECVPACKAGE_SIZE;

#define LOG_LEVEL 0

#define DEBUG 0
#define WARNING 1
#define ERROR 2

extern void mylog(const char *func, const char *file, const int line,
          const int level, const char *format, ...);

#define LOG(level, format, ...) mylog(__func__, __FILE__, __LINE__, level, format, ##__VA_ARGS__)

#endif