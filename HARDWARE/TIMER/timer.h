#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "include.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//AD��ֵ�˵�ϵ��
#define P_COEFFICIENT 70
#define D_COEFFICIENT 800
//��ѹ���ˣ�������P����Ҫ��С
/*Founctions-----------------------------------------------------------*/
//TIM3�ճ��ж�
void TIM3_Int_Init(u16 arr,u16 psc);
//TIM4��1234ͨ��PWM���
void TIM4_PWM_Init(u32 arr,u16 psc);


extern u8 count;

#endif




