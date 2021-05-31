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
    int32_T tic; // 帧
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

// 模型实例与角色（用于load balance）
/*
 * Role 会在该模块初始化时发给通讯告知这是 某个具体类型靶机\运动学模型的某个实例的收\发模块
 *
 * 靶机的 type 是 0
 * tag 表示靶机的具体类型，静止的靶机 static - 01
 * id 是该具体类型靶机的 id，可供多实例使用
 *
 * 运动学模型机的 type 是 1
 * 暂时没有 tag，都是 0
 * id 是该运动学模型机的 id，可供多实例使用
 * 
 * direction 与通讯服务器的socket相关，0该模块表示从通讯服务器接收数据（位姿+操作），1表示向通讯服务器发送数据（位姿）
 * ip_addr = [202,120,40,8];
 * ip_addr = [10,119,6,245];
 * 
 * 静止靶机 1
role_type = 0;
role_tag = 01;
role_id = 0001;
 * 静止靶机 2
role_type = 0;
role_tag = 01;
role_id = 0002;
 * 运动学模型 1
role_type = 1;
role_tag = 00;
role_id = 1001;
 * 运动学模型 2
role_type = 1;
role_tag = 00;
role_id = 1002;

 */
typedef struct {
    int32_T type; // 0 for target, 1 for kine
    int32_T tag;  // save for different kind target
    int32_T id;   // instance id
    int32_T direction; // 0 for commu2model, 1 for model2commu
} Role;
#define ROLE_TYPE_DRONE 0
#define ROLE_TYPE_KINE 1
#define ROLE_TAG_DRONE_STATIC 00
#define ROLE_TAG_DRONE_CYCLE 01
#define ROLE_TAG_KINE 10
#define ROLE_DIR_RECV 0
#define ROLE_DIR_SEND 1
extern const int ROLE_SIZE;

// 统一日志函数
#define LOG_LEVEL 0

#define DEBUG 0
#define WARNING 1
#define ERROR 2

extern void mylog(const char *func, const char *file, const int line,
          const int level, const char *format, ...);

#define LOG(level, format, ...) mylog(__func__, __FILE__, __LINE__, level, format, ##__VA_ARGS__)

#endif