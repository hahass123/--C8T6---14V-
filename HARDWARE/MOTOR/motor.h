/*******************************************************************
�������ͷ�ļ�
2018.12.29 19��56
���û��Զ���ͷ�ļ�����
*******************************************************************/

/*Define to prevent recursive inclusion--------------------------------*/
#ifndef __MOTOR_H__
#define __MOTOR_H__

//����L298Nģ�飬ÿ���ĸ��߼�������Ƶ������ת
#define MODULEA_IN1 PBout(0)
#define MODULEA_IN2 PBout(1)
#define MODULEA_IN3 PBout(10)
#define MODULEA_IN4 PBout(11)
#define MODULEB_IN1 PBout(12)
#define MODULEB_IN2 PBout(13)
#define MODULEB_IN3 PBout(14)
#define MODULEB_IN4 PBout(15)

/*Include-----------------------------------------------------------------*/
#include "sys.h"

/*Founctions-----------------------------------------------------------*/
void Motor_Control_Init(void);

#endif





