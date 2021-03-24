
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

#include <simstruc.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 12

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output function
 *
 */
void drone_trackgen_Outputs_wrapper(const int32_T *in,
			int32_T *out)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
ssPrintf("\n[trackgen][input] x = %d y = %d z = %d\n", in[0], in[1], in[2]);
    double x_in = (double)in[0] / 1000, y_in = (double)in[1] / 1000;
    double theta = atan2(y_in, x_in) - M_PI / 12;
    ssPrintf("[trackgen] theta = %f (radian) = %f (angle)\n", theta, theta / M_PI * 180);

    memcpy(out, in, sizeof(in[0]) * 12);
    out[0] = (int32_T) (100 * cos(theta) * 1000);
    out[1] = (int32_T) (100 * sin(theta) * 1000);
    out[2] = fabs(theta) < 0.001 ? in[2]: in[2];
    if (abs(theta) < 0.001) {
        ssPrintf("[trackgen] z [change], theta = %f, fabs(theta) = %f, old z = %d, new z = %d\n", theta, fabs(theta), in[2], out[2]);
    }
    ssPrintf("[trackgen][output] x = %d y = %d z = %d\n", out[0], out[1], out[2]);

    /*
    unsigned char *p;
    p = (unsigned char *)&out[0];
    ssPrintf("x = %f, (HEX in BE) %02X %02X %02X %02X %02X %02X %02X %02X\n",out[0],p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);
    p = (unsigned char *)&out[1];
    ssPrintf("y = %f, (HEX in BE) %02X %02X %02X %02X %02X %02X %02X %02X\n",out[1],p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);
    p = (unsigned char *)&out[2];
    ssPrintf("z = %f, (HEX in BE) %02X %02X %02X %02X %02X %02X %02X %02X\n",out[2],p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);
    */
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}


