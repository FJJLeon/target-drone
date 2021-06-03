
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
#define _USE_MATH_DEFINES

#include <drone_util.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 13

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Start function
 *
 */
void drone_trackgen_Start_wrapper(void **pW,
			const int32_T *para_role_type, const int_T p_width0,
			const int32_T *para_role_tag, const int_T p_width1,
			const int32_T *para_role_id, const int_T p_width2)
{
/* %%%-SFUNWIZ_wrapper_Start_Changes_BEGIN --- EDIT HERE TO _END */
/*
 * Custom Start code goes here.
 */
    // malloc pWork memory for filename      
    pW[0] = getLogName(para_role_id[0], para_role_tag[0]);
    const char *fn = (const char*) pW[0];
    LOG(DEBUG, 1, fn, "LOG FILE: %s\n", fn);
/* %%%-SFUNWIZ_wrapper_Start_Changes_END --- EDIT HERE TO _BEGIN */
}
/*
 * Output function
 *
 */
void drone_trackgen_Outputs_wrapper(const int32_T *in,
			int32_T *out,
			void **pW,
			const int32_T *para_role_type, const int_T p_width0,
			const int32_T *para_role_tag, const int_T p_width1,
			const int32_T *para_role_id, const int_T p_width2)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
// retrieve LOG filename from pWork
    const char *fn = (const char *)pW[0];
    LOG(DEBUG, 1, fn, "input x = %d y = %d z = %d\n", in[0], in[1], in[2]);
    
     memcpy(out, in, POSTURE_SIZE);

    /*
    // transmission use integer (double x1000 truncation) 
    double x_in = (double)in[0] / 1000, y_in = (double)in[1] / 1000;
    double theta = atan2(y_in, x_in) - M_PI / 120 * in[17];
    LOG(DEBUG, "new theta = %f radian = %f angle", theta, theta / M_PI * 180);

    // 虽然这里的输入有 17 个字段，后 5 个字段为操作，在靶机中用不到
    // 好吧现在多了一个 steps，表示走过的单位步数，要用
    memcpy(out, in, POSTURE_SIZE);
    out[0] = (int32_T) (200 * cos(theta) * 1000);
    out[1] = (int32_T) (200 * sin(theta) * 1000);
    //out[2] = fabs(theta) < 0.001 ? in[2] + 1000: in[2];
    out[2] = in[2] + 100;
    out[5] = (int)((theta - M_PI) * 1000);
    if (fabs(theta) < 0.001) {
        LOG(DEBUG, " z climb, theta = %f, fabs(theta) = %f, z chaneg from %d to %d", theta, fabs(theta), in[2], out[2]);
    }
    LOG(DEBUG, "output x = %d y = %d z = %d", out[0], out[1], out[2]);
    */
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}


