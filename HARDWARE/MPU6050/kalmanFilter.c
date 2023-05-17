#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "kalmanFilter.h"

#include <string.h>





float fc = 0.2f;     //截止频率
float Ts = 0.1f;    //采样周期
float pi = 3.14159f; //π
float alpha = 0;     //滤波系数

/************************ 滤波器初始化 alpha *****************************/
void low_pass_filter_init(void)
{
  float b = 2.0 * pi * fc * Ts;
  alpha = b / (b + 1);
}

float low_pass_filter(float value)
{
  static float out_last = 0; //上一次滤波值
  float out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }

  /*************************** 一阶滤波 *********************************/
  out = out_last + alpha * (value - out_last);
  out_last = out;

  return out;
}

//const int NL = 128;
#define NL  318
const float b[318] = {
  -9.100619354e-05,-4.514323155e-05,-5.597315248e-05,-6.817954272e-05,-8.182807505e-05,
  -9.693927859e-05,-0.0001136004503,-0.0001318188588,-0.0001516122575,-0.0001728952629,
  -0.0001956833294,-0.0002199251612,-0.0002455748036,-0.0002723759098,-0.0003003923921,
  -0.0003294139751,-0.0003591687419,-0.0003895990085,-0.0004203417629,-0.0004512347223,
  -0.0004819184833,-0.0005121271824,-0.0005415011547,-0.000569719472,-0.0005963586736,
  -0.0006210439606,-0.0006433582748,-0.000662878505,-0.0006791171036,-0.000691696885,
  -0.0007001026534,-0.0007039319025,-0.000702713558,-0.0006960250903,-0.0006834501983,
  -0.0006645845133,-0.0006390465423,-0.0006064993213,-0.0005666359211,-0.0005191821838,
  -0.0004639176186,-0.0004006906238,-0.0003293740156,-0.0002499265538,-0.0001623672288,
  -6.676976773e-05,3.668778663e-05,0.0001477908663, 0.000266209885,0.0003915518173,
  0.0005233312841,0.0006609908887,0.0008038730593,0.0009512467659, 0.001102302223,
   0.001256146235, 0.001411806908, 0.001568253851, 0.001724369824,  0.00187900092,
   0.002030918142, 0.002178854309,  0.00232151011, 0.002457547234, 0.002585614799,
   0.002704349346, 0.002812398365, 0.002908412134, 0.002991077024,  0.00305911433,
    0.00311129773, 0.003146467963, 0.003163542133, 0.003161523491, 0.003139533103,
    0.00309678656, 0.003032646142, 0.002946600318, 0.002838297514, 0.002707537031,
   0.002554296516, 0.002378727077,  0.00218116655, 0.001962144626, 0.001722385176,
   0.001462810207, 0.001184547902,0.0008889156743,0.0005774410092,0.0002518425172,
  -8.597047417e-05,-0.000433900248,-0.0007896752213,-0.001150851604,-0.001514835516,
  -0.001878884039,-0.002240131143, -0.00259559392,-0.002942197025,-0.003276790027,
  -0.003596163588,-0.003897073679,-0.004176268354,-0.004430497531,-0.004656551871,
  -0.004851273261,-0.005011589732,-0.005134531297,-0.005217259284,-0.005257087294,
  -0.005251505878,-0.005198207684,-0.005095101427,-0.004940344021, -0.00473235175,
  -0.004469820764,-0.004151745234,-0.003777431091, -0.00334650767,-0.002858942142,
  -0.002315041842,-0.001715470455, -0.00106124091,-0.0003537284501,0.0004053420271,
   0.001213887241, 0.002069481416, 0.002969360445, 0.003910431638, 0.004889289849,
   0.005902233068, 0.006945277099,  0.00801418256,  0.00910446886,  0.01021144632,
    0.01133023947,  0.01245581359,  0.01358300634,  0.01470655948,  0.01582114771,
    0.01692141406,  0.01800199971,   0.0190575812,  0.02008290216,  0.02107280679,
    0.02202227339,  0.02292644791,  0.02378067002,  0.02458051406,  0.02532180585,
    0.02600065619,  0.02661349252,  0.02715706825,  0.02762850188,  0.02802527696,
     0.0283452794,  0.02858679369,  0.02874852717,  0.02882961184,  0.02882961184,
    0.02874852717,  0.02858679369,   0.0283452794,  0.02802527696,  0.02762850188,
    0.02715706825,  0.02661349252,  0.02600065619,  0.02532180585,  0.02458051406,
    0.02378067002,  0.02292644791,  0.02202227339,  0.02107280679,  0.02008290216,
     0.0190575812,  0.01800199971,  0.01692141406,  0.01582114771,  0.01470655948,
    0.01358300634,  0.01245581359,  0.01133023947,  0.01021144632,  0.00910446886,
    0.00801418256, 0.006945277099, 0.005902233068, 0.004889289849, 0.003910431638,
   0.002969360445, 0.002069481416, 0.001213887241,0.0004053420271,-0.0003537284501,
   -0.00106124091,-0.001715470455,-0.002315041842,-0.002858942142, -0.00334650767,
  -0.003777431091,-0.004151745234,-0.004469820764, -0.00473235175,-0.004940344021,
  -0.005095101427,-0.005198207684,-0.005251505878,-0.005257087294,-0.005217259284,
  -0.005134531297,-0.005011589732,-0.004851273261,-0.004656551871,-0.004430497531,
  -0.004176268354,-0.003897073679,-0.003596163588,-0.003276790027,-0.002942197025,
   -0.00259559392,-0.002240131143,-0.001878884039,-0.001514835516,-0.001150851604,
  -0.0007896752213,-0.000433900248,-8.597047417e-05,0.0002518425172,0.0005774410092,
  0.0008889156743, 0.001184547902, 0.001462810207, 0.001722385176, 0.001962144626,
    0.00218116655, 0.002378727077, 0.002554296516, 0.002707537031, 0.002838297514,
   0.002946600318, 0.003032646142,  0.00309678656, 0.003139533103, 0.003161523491,
   0.003163542133, 0.003146467963,  0.00311129773,  0.00305911433, 0.002991077024,
   0.002908412134, 0.002812398365, 0.002704349346, 0.002585614799, 0.002457547234,
    0.00232151011, 0.002178854309, 0.002030918142,  0.00187900092, 0.001724369824,
   0.001568253851, 0.001411806908, 0.001256146235, 0.001102302223,0.0009512467659,
  0.0008038730593,0.0006609908887,0.0005233312841,0.0003915518173, 0.000266209885,
  0.0001477908663,3.668778663e-05,-6.676976773e-05,-0.0001623672288,-0.0002499265538,
  -0.0003293740156,-0.0004006906238,-0.0004639176186,-0.0005191821838,-0.0005666359211,
  -0.0006064993213,-0.0006390465423,-0.0006645845133,-0.0006834501983,-0.0006960250903,
  -0.000702713558,-0.0007039319025,-0.0007001026534,-0.000691696885,-0.0006791171036,
  -0.000662878505,-0.0006433582748,-0.0006210439606,-0.0005963586736,-0.000569719472,
  -0.0005415011547,-0.0005121271824,-0.0004819184833,-0.0004512347223,-0.0004203417629,
  -0.0003895990085,-0.0003591687419,-0.0003294139751,-0.0003003923921,-0.0002723759098,
  -0.0002455748036,-0.0002199251612,-0.0001956833294,-0.0001728952629,-0.0001516122575,
  -0.0001318188588,-0.0001136004503,-9.693927859e-05,-8.182807505e-05,-6.817954272e-05,
  -5.597315248e-05,-4.514323155e-05,-9.100619354e-05
};






// int main()
// {
// 	FILE *fp=NULL;
// 	int i;
	
// 	float aa[1024]={0},bb[1024]={0};
	
// 	fp = fopen("data.txt","rb");
// 	for(i=0;i<133;i++)
// 	{
// 		fscanf(fp,"%f",&aa[i]);
// 	}
// 	fclose(fp);
	
// 	filter_iir(aa,bb,133);
	
// 	fp = fopen("out.txt","wb");
// 	for(i=0;i<133;i++)
// 	{
// 		fprintf(fp,"%llf\n",bb[i]);
// 	}
// 	fclose(fp);
	
// 	return 0;
// } 

void filter_fir(float *in,float *out)
{
	int i = 0, j = 0;
	// for(i=NL;i<in_length;i++)
	// {
		for(j=0;j<NL;j++)
		{
			*out += b[j] * in[NL - j];
		}
		// for(j=1;j<NL;j++)
		// {
		// 	out[i] -= a[j]*out[i-j];
		// }
//	}
}
void filter_FIR(float *in, float *out)
{
  static float a_tmp[3][NL] = {0}; //上一次滤波值
  float out_tmp[3] = {0};
  static float out_tmp_old[3] = {0};

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static int fisrt_flag = 0;
  if (fisrt_flag < NL)
  {
    for(int i = 0; i < 3; i++)
    {
      a_tmp[i][fisrt_flag] = in[i];
    }
    fisrt_flag ++;
    return;
  }
  for(int i = 0; i < 3; i++)
  {
    a_tmp[i][NL - 1] = in[i];
  }
  /*************************** 滤波 *********************************/
  for(int i = 0; i < 3; i++)
  {
    filter_fir((float *)&(a_tmp[i]), &out_tmp[i]);
    if(fabs(out_tmp[i] - out_tmp_old[i]) > 800)
      out_tmp[i] = out_tmp_old[i];
    if(i != 2)
    {
      if(fabs(out_tmp[i]) < 100)
        out_tmp[i] = 0;
    }else
    {
      out_tmp[i] -= 80;
      if(fabs(out_tmp[i]) < 100)
        out_tmp[i] = 0;     
    }
    out[i] = out_tmp[i];
    out_tmp_old[i] = out[i];
  }
  /*************************** 更新 *********************************/
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < (NL - 1); j++)
    {
      a_tmp[i][j] = a_tmp[i][j+1];
    }
  }
}

void get_v_x(float *a, float *v, float *x)
{
  float a_tmp = *a, v_tmp = *v, x_tmp = *x;
  if(a_tmp > -0.3 && a_tmp < 0.3)
    a_tmp = 0;
  x_tmp = x_tmp + v_tmp / 250 + 0.5 * a_tmp /250 /250;
  v_tmp = v_tmp + a_tmp / 250;
  *x = x_tmp;
  *v = v_tmp;
}
#define CAL_NUM 120
#define ZERO_VALUE 0.5
float a_zero[3] = {0.00};

/*
Az  -45<PIT<45 -45<ROL<45
-Az  135<PIT<180 || -180<PIT<-135  -45<ROL<45

Ax   -90<ROL<-45
-Ax  135<PIT<180 || -180<PIT<-135  45<ROL<90

Ay  45<PIT<135 -45<ROL<45
-Ay  -135<PIT<-45  -45<ROL<45
*/
void x_deal(float *a, float *x, float *v)
{
  static float a_tmp[3][CAL_NUM] = {0}; //
  static float v_tmp_old[3][CAL_NUM] = {0}; //
  float out_tmp[3] = {0};
  float v_tmp[3] = {0};
  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static int fisrt_flag = 0;
  if (fisrt_flag < CAL_NUM)
  {
    for(int i = 0; i < 1; i++)
    {
      a_tmp[i][fisrt_flag] = a[i];
    }
    fisrt_flag ++;
  }
  for(int i = 0; i < 1; i++)
  {
    a_tmp[i][CAL_NUM - 1] = a[i]/100;
    out_tmp[i] = x[i];
    v_tmp[i] = v[i];
  }
  /*************************** 滤波 *********************************/
  for(int i = 0; i < 1; i++)
  {
    if(a_tmp[i][CAL_NUM - 1] >= -0.5 && a_tmp[i][CAL_NUM - 1] <= 0.5)
    {
      a_tmp[i][CAL_NUM - 1] = 0.0;
    }
    int zeor_flag = 1;
    float a_zero_tmp = 0;
    for(int j = 0; j < (CAL_NUM - 1); j++)
    {
      a_zero_tmp += a_tmp[i][j];
      if(fabs(a_tmp[i][j]- a_tmp[i][j+1]) >= 0.005)
      {
        zeor_flag = 0;
        break;
      }
    }
    if(zeor_flag)
    {
      a_zero[i] = a_zero_tmp/CAL_NUM;
      v_tmp[i] = 0;
    }
    float a_real = a_tmp[i][CAL_NUM - 1] - a_zero[i];
    zeor_flag = 1;
    for(int j = 0; j < (50 - 1); j++)
    {
      if(j < 50/2 && fabs(v_tmp_old[i][j] - v_tmp_old[i][j+1]) <= 0.004 )
      {
        zeor_flag = 0;
        break;
      }
      if(j > 50/2 && fabs(v_tmp_old[i][j]) >= 25 )
      {
        zeor_flag = 0;
        break;
      }
    }
    if(zeor_flag)
    {
      v_tmp[i] = 0;
    }
    get_v_x(&a_real, &v_tmp[i], &out_tmp[i]);
    x[i] = out_tmp[i];
    v[i] = v_tmp[i];
    v_tmp_old[0][CAL_NUM - 1] = v[i];
  }
  /*************************** 更新 *********************************/
  if (fisrt_flag == CAL_NUM)
  {
    for(int i = 0; i < 1; i++)
    {
      for(int j = 0; j < (CAL_NUM - 1); j++)
      {
        a_tmp[i][j] = a_tmp[i][j+1];
      }
      for(int j = 0; j < (CAL_NUM - 1); j++)
      {
        v_tmp_old[i][j] = v_tmp_old[i][j+1];
      }
    }
  }


}