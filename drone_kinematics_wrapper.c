
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
#define y_width 12

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
    
    Posture post = pkg.post;
    double x = post.x / 1000;
    double y = post.y / 1000;
    double z = post.z / 1000;
    double u = (double)(post.u) / 1000;
    double v = (double)(post.v) / 1000;
    double w = (double)(post.w) / 1000;
    
    double fai   = u;
    double theta = v;
    double psi   = w;
    
    Operation op = pkg.op;
    int32_T roll  = op.roll;  // w_x  ¦µ \fai    ·­¹ö
    int32_T pitch = op.pitch; // w_y  ¦¨ \theta  ¸©Ñö
    int32_T yaw   = op.yaw;   // w_z  ¦· \psi    Æ«º½ 
    int32_T accelerate = op.accelerate;
    
    double unit_w = 0.00005;
    double w_x = roll  * unit_w;
    double w_y = pitch * unit_w;
    double w_z = yaw   * unit_w;
    
//     double d_fai   = (w_x * cos(psi) - w_y * sin(psi)) / cos(theta);
//     double d_theta = w_x * sin(psi) + w_y * cos(psi);
//     double d_psi   = w_z + tan(theta)(w_x * cos(psi) - w_y * sin(psi));
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
    
    double unit_v = 0.005;
    double base_acc = 100;
    double total_v = (base_acc + accelerate) * unit_v;
    double vx = total_v * cos(new_theta) * cos(new_psi);
    double vy = total_v * cos(new_theta) * sin(new_psi);
    double vz = total_v * sin(new_theta);
    
    double sim_time = 1;
    x += vx * sim_time;
    y += vy * sim_time;
    z -= vz * sim_time;
    
    post.x = (int)(x * 1000);
    post.y = (int)(y * 1000);
    post.z = (int)(z * 1000);
    post.u = (int)(new_fai * 1000);
    post.v = (int)(new_theta * 1000);
    post.w = (int)(new_psi * 1000);
    post.vx = (int)(vx * 1000);
    post.vy = (int)(vy * 1000);
    post.vz = (int)(vz * 1000);
    memcpy(out, (char*)&post, POSTURE_SIZE);
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}


