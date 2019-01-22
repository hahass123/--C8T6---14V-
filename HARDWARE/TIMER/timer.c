#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTE��ӢSTM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����									  
//////////////////////////////////////////////////////////////////////////////////  

u8 count = 0;
s16 front_lift = 0, frout_right = 0, behind_lift = 0, behind_right = 0;//�����������ĸ����ռ�ձȱ���
float sub_val = 0;//��Ⱥͱ���
float sub_val_last = 0;//��һ�θ�Ӧֵ
float sub_D;//���θ�Ӧֵ��ֵ����Ӧֵ�仯����������ʾ��Ӧֵ�仯��
int i;

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��0x00~
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
        count++;
        //AD�ɼ�����
        adcx1 = Get_Adc1(ADC_Channel_1);
        adcx2 = Get_Adc1(ADC_Channel_2);
        temp1 = (float)adcx1 * (3.3 / 4096);
        temp2 = (float)adcx2 * (3.3 / 4096);
        
        sub_val_last = sub_val;
        //���ҵ�в�Ⱥ�
        sub_val = (float)(adcx1 - adcx2) / (float)(adcx1 + adcx2);
        //��Ӧֵ�仯��
        sub_D = sub_val - sub_val_last;
        
        //�ٶȿ���ռ�ձȣ���Ϊռ�ձ��Ƿ��ŵģ�0%Ϊ��ռ�ձ�
        front_lift = SETTING_SPEED - sub_val * P_COEFFICIENT - sub_D * D_COEFFICIENT;
        frout_right = SETTING_SPEED + sub_val * P_COEFFICIENT + sub_D * D_COEFFICIENT;
        behind_lift = SETTING_SPEED - sub_val * P_COEFFICIENT - sub_D * D_COEFFICIENT;
        behind_right = SETTING_SPEED + sub_val * P_COEFFICIENT + sub_D * D_COEFFICIENT;

        if(front_lift > 99)
        {
            frout_right = 100 + (sub_val * P_COEFFICIENT + sub_D * D_COEFFICIENT) * 2;
        }
        if(frout_right > 99)
        {
            front_lift = 100 + (-sub_val * P_COEFFICIENT - sub_D * D_COEFFICIENT) * 2;
        }
        if(behind_lift > 99)
        {
            behind_right = 100 + (sub_val * P_COEFFICIENT + sub_D * D_COEFFICIENT) * 2;
        }
        if(behind_right > 99)
        {
            behind_lift = 100 + (-sub_val * P_COEFFICIENT - sub_D * D_COEFFICIENT) * 2;
        }        
        
        //�м�ͣ�����綯���ƶ�
        if(mark == 1)
        {
            PBout(0) = 0;
            PBout(1) = 0;  
            PBout(10) = 0;
            PBout(11) = 0;
            PBout(12) = 0;
            PBout(13) = 0;
            PBout(14) = 0;
            PBout(15) = 0;
        }
        else
        {
            //�������ת�����ٵ��ռ�ձȾ�����ռ�ձȵĵ������һ����������ֵ
            if(front_lift < 0)
            {
                front_lift = -(100 + (-sub_val * P_COEFFICIENT - sub_D * D_COEFFICIENT) * 2);
                PBout(0) = 0;
                PBout(1) = 1;    
            }
            else
            {
                PBout(0) = 1;
                PBout(1) = 0;
            }
            if(frout_right < 0)
            {
                frout_right = -(100 + (sub_val * P_COEFFICIENT + sub_D * D_COEFFICIENT) * 2);
                PBout(10) = 0;
                PBout(11) = 1;
            }
            else
            {
                PBout(10) = 1;
                PBout(11) = 0;
            }
            if(behind_lift < 0)
            {
                behind_lift = -(100 + (-sub_val * P_COEFFICIENT - sub_D * D_COEFFICIENT) * 2);
                PBout(12) = 1;
                PBout(13) = 0;
            }
            else
            {
                PBout(12) = 0;
                PBout(13) = 1;
            }
            if(behind_right < 0)
            {
                behind_right = -(100 + (sub_val * P_COEFFICIENT + sub_D * D_COEFFICIENT) * 2);
                PBout(14) = 1;
                PBout(15) = 0;
            }
            else
            {
                PBout(14) = 0;
                PBout(15) = 1;    
            }
        }
        
        //ռ�ձ��޷�
        if(front_lift < 1)
        {
            front_lift = 0;
        }
        if(front_lift > 99)
        {
            front_lift = 100;
        }        
        if(frout_right < 1)
        {
            frout_right = 0;
        }
        if(frout_right > 99)
        {
            frout_right = 100;
        }
        if(behind_lift < 1)
        {
            behind_lift = 0;
        }
        if(behind_lift > 99)
        {
            behind_lift = 100;
        }
        if(behind_right < 1)
        {
            behind_right = 0;
        }
        if(behind_right > 99)
        {
            behind_right = 100;
        }        
        
        //С�����ܵ�ͣ��
        if((adcx1 < 200)&&(adcx2 < 200))
        {
            front_lift = frout_right = behind_lift = behind_right = 0;
        }
        
        //���µ��ռ�ձ�
        TIM_SetCompare1(TIM4, front_lift);
        TIM_SetCompare2(TIM4, frout_right);	
        TIM_SetCompare3(TIM4, behind_lift);	
        TIM_SetCompare4(TIM4, behind_right);	
    }
}



//TIM4 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM4_PWM_Init(u32 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOB����ʱ��
	    
  //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH3
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH4
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
  //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	//��ʼ��TIM4 Channel2 PWMģʽ	 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	//��ʼ��TIM4 Channel3 PWMģʽ	 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIM4 OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
	//��ʼ��TIM4 Channel4 PWMģʽ	 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIM4 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}


