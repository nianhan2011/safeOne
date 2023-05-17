#ifndef _KALMANFILTER_H
#define _KALMANFILTER_H

typedef struct 
{
    float Last_P;//上次估算协方差 不可以为0 ! ! ! ! ! 
    float Now_P;//当前估算协方差
    float out;//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
    float Q;//过程噪声协方差
    float R;//观测噪声协方差
}Kalman;




extern Kalman kfp;
void low_pass_filter_init(void);
float low_pass_filter(float value);
void Kalman_Init();
float KalmanFilter(Kalman *kfp,float input);
void filter_FIR(float *in, float *out);
void x_deal(float *a, float *x, float *v);
#endif