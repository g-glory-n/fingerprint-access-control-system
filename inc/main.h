/***********************************************************************
ZW_SZW201����:
�ڣ�GND
�죺VCC
�ң�EN
�ƣ�IRQ
�ף�RX -> MCU-UART_TX 
�ȣ�TX -> MCU-UART_RX

����ģ�黽��ʱ����������Բ���������
***********************************************************************/

/************************************************************************
LCD12864���߷���:

P0  Ϊ�������ݿ�   p0.0...p0.7 -> DB0...DB7

sbit RS  = P2^4; //  ����/����   ��RS=0��,��������,��RS=1,��������.
sbit RW  = P2^3; //  ��/д    RW=0д;RW=1��
sbit E   = P2^2; //  ʹ�ܹܽ�   E=0  ->  E=1   ;�½���   д��LCD12864

PSB 	  		 //	 ����/����ѡ��   H-����  L-����
sbit RST = P2^1; //  LCD12864 ��λ �͵�ƽ��Ч

BL_A �����Դ+ ; BL_k �����Դ-
VCC ��Դ+ ; GND ��Դ-

*************************************************************************/

/*************************************************************************
�̵������߷���:
sbit relay_A_up  =P1^0;
sbit relay_A_stop=P1^1;
sbit relay_A_down=P1^2;

sbit relay_B_up  =P1^3;
sbit relay_B_stop=P1^4;
sbit relay_B_down=P1^5;

sbit relay_C_up  =P1^6;
sbit relay_C_stop=P1^7;
sbit relay_C_down=P3^4;  //T0

sbit relay_D_up  =P3^5;	 //T1
sbit relay_D_stop=P3^6;	 //WR
sbit relay_D_down=P3^7;  //WD
*************************************************************************/

/*************************************************************************
ѡ������߷���:
sbit options=P2^7;
sbit confirm=P2^6;
sbit back_stop=P2^5;
sbit buzzer=P2^0;			 //	  ������
*************************************************************************/

#ifndef __main_h__
#define __main_h__

#include "reg52.h"


#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

/******************************************
�˻���������,��������
******************************************/
sbit options=P2^7;
sbit confirm=P2^6;
sbit back_stop=P2^5;
sbit buzzer=P2^0;			 //	  ������

/******************************************
�̵������Ŷ���
******************************************/
sbit relay_A_open=P1^0;
sbit relay_A_stop=P1^1;
sbit relay_A_down=P1^2;

sbit relay_B_open=P1^3;
sbit relay_B_stop=P1^4;
sbit relay_B_down=P1^5;

sbit relay_C_open=P1^6;
sbit relay_C_stop=P1^7;
sbit relay_C_down=P3^4;  //T0

sbit relay_D_open=P3^5;	 //T1
sbit relay_D_stop=P3^6;	 //WR
sbit relay_D_down=P3^7;  //WD


/******************************************
ָ��ģ�����Ŷ���
******************************************/
//sbit finger_signal=P3^3;//ģ�鴦������ʱ����ָ�Ӵ��ɼ�������ἤ��ģ�飬�˽����������
//sbit sleep=P2^0;		//ģ�����߿��ƽӿڣ�����ߵ�ƽ��ģ�鹩��-��������͵�ƽ��ģ�����-����

extern char temperature_string[8];
extern uchar receive_data[8];

extern bit flag_compare;
extern bit flag_control_door;
extern bit flag_quick_door;
extern bit flag_buzzer;
extern bit flag_display_screen;				 

extern bit open_lock;
											  //��
extern uchar time0_lock;										  //������ʱ
extern uchar time0_cartoon;										  //�����Ŷ�����ʱ
extern uchar time0_buzzer;										  //��������ʱ
extern uchar time0_init_screen;									  //��ʼ��Ļ��ʱ

extern uchar buzzer_quantity;
extern uchar buzzer_low_time;
extern uchar buzzer_high_time;

extern uchar parameter_1;
extern uchar parameter_2;
extern uchar parameter_3;

extern char array_parameter_1[4];
extern char array_parameter_2[4];
extern char array_parameter_3[4];

extern uchar parameter;

extern uint count_options;
extern uchar count_confirm;
extern uchar copy_count_options;

extern uchar door;
extern uchar quick_door;

extern float temperature;
									   
								  //success_validation=0A;1B;2C;3D


extern void init_UART (void);							  		  //���ڳ�ʼ�� ������19200 �������
extern void init_time0 (void);							  		  //��ʱ��T0 ��ʽһ 16λ  50ms  ��ʼ��

extern void display_init_screen_00 (void);						  //��ʾ��ʼͼƬ0
extern void display_init_screen_01 (void);						  //��ʾ��ʼͼƬ1

extern void display_control_screen_00 (uchar i);				  //��ʾ����ͼƬ00

extern void display_control_screen_01 (uchar *cmd);				  //��ʾ����ͼƬ01
	
extern void display_control_screen_02_00 (uchar i,uchar *cmd);	  //��ʾ������ͼƬ02_00		  	  
extern void display_control_screen_02_01 (uchar *cmd);			  //ָ������ͼƬ02_01
extern void display_control_screen_02_02 (uchar *cmd);			  //��ʾ���ÿ��ٿ���ͼƬ02_02		  
		  

extern void examine_lock (void);
extern void send_one_byte (unsigned char c);			  		  //����һ�ֽ�����

extern void examine_key (void);
extern void control_door (uchar door,uchar *cmd);		  		  //ABCD�ſ��ƺ���

//extern void buzzer_time_quantity (uint low_time,uint high_time,uchar quantity);




















#endif