
/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif



/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>

#include <drone_util.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 13

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
 
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output function
 *
 */
void drone_kinematics_Outputs_wrapper(const int32_T *in,
			int32_T *out)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
RecvPackage pkg;
    memcpy((char*)&pkg, in, RECVPACKAGE_SIZE);
    
    int len_ratio = 1.8; // 物理 1 米对于 unity 的单位 1
    
    Posture post = pkg.post;
    double x = (double)(post.x) / 1000;
    double y = (double)(post.y) / 1000;
    double z = (double)(post.z) / 1000;
    double u = (double)(post.u) / 1000;
    double v = (double)(post.v) / 1000;
    double w = (double)(post.w) / 1000;
    
    double fai   = u;
    double theta = v;
    double psi   = w;
    
    Operation op = pkg.op;
    int32_T roll  = op.roll;  // w_x  Φ \fai    翻滚
    int32_T pitch = op.pitch; // w_y  Θ \theta  俯仰
    int32_T yaw   = op.yaw;   // w_z  Ψ \psi    偏航 
    double accelerate = op.accelerate;
    int32_T steps = op.steps;
        
    double unit_w = 0.00005 / 100;
    double w_x = roll  * unit_w * steps;
    double w_y = pitch * unit_w * steps;
    double w_z = yaw   * unit_w * steps;
    
//     double d_fai   = (w_x * cos(psi) - w_y * sin(psi)) / cos(theta);
//     double d_theta = w_x * sin(psi) + w_y * cos(psi);
//     double d_psi   = w_z + tan(theta) * (w_x * cos(psi) - w_y * sin(psi));
    double d_fai = w_x;
    double d_theta = w_y;
    double d_psi = w_z;
    
    double new_fai = fai + d_fai;
    double new_theta = theta + d_theta;
    if (new_theta > M_PI / 3) {
        new_theta = M_PI / 3;
    }
    if (new_theta < -M_PI /3) {
         new_theta = -M_PI / 3;   
    }
    double new_psi = psi + d_psi;
    
    // 最慢 20 m/s, 最快 100 m/s
    double unit_v = 0.004;
    double base_acc = 0; // acc: 200 - 1200
    double total_v = (base_acc + accelerate) * unit_v;
    double vx = total_v * cos(new_theta) * cos(new_psi);
    double vy = total_v * cos(new_theta) * sin(new_psi);
    double vz = (-1) * total_v * sin(new_theta);
    
    double sim_time = 0.01;
    x += vx * sim_time * steps;
    y += vy * sim_time * steps;
    z += vz * sim_time * steps;
    
    post.x = (int32_T)(x * 1000);
    post.y = (int32_T)(y * 1000);
    post.z = (int32_T)(z * 1000);
    post.u = (int32_T)(new_fai * 1000);
    post.v = (int32_T)(new_theta * 1000);
    post.w = (int32_T)(new_psi * 1000);
    post.vx = (int32_T)(vx * 1000);
    post.vy = (int32_T)(vy * 1000);
    post.vz = (int32_T)(vz * 1000);
    memcpy(out, (char*)&post, POSTURE_SIZE);
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}


