/***********************************************************************
ZW_SZW201接线:
黑：GND
红：VCC
灰：EN
黄：IRQ
白：RX -> MCU-UART_TX 
橙：TX -> MCU-UART_RX

由于模块唤醒时间过长；所以不采用休眠
***********************************************************************/

/************************************************************************
LCD12864接线方法:

P0  为输入数据口   p0.0...p0.7 -> DB0...DB7

sbit RS  = P2^4; //  数据/命令   当RS=0是,传送命令,当RS=1,传送数据.
sbit RW  = P2^3; //  读/写    RW=0写;RW=1读
sbit E   = P2^2; //  使能管脚   E=0  ->  E=1   ;下降沿   写入LCD12864

PSB 	  		 //	 并口/串口选择   H-并行  L-串行
sbit RST = P2^1; //  LCD12864 复位 低电平有效

BL_A 背光电源+ ; BL_k 背光电源-
VCC 电源+ ; GND 电源-

*************************************************************************/

/*************************************************************************
继电器接线方法:
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
选项按键接线方法:
sbit options=P2^7;
sbit confirm=P2^6;
sbit back_stop=P2^5;
sbit buzzer=P2^0;			 //	  蜂鸣器
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
人机交互界面,按键定义
******************************************/
sbit options=P2^7;
sbit confirm=P2^6;
sbit back_stop=P2^5;
sbit buzzer=P2^0;			 //	  蜂鸣器

/******************************************
继电器引脚定义
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
指纹模块引脚定义
******************************************/
//sbit finger_signal=P3^3;//模块处于休眠时，手指接触采集器表面会激活模块，此脚输出高脉冲
//sbit sleep=P2^0;		//模块休眠控制接口：输入高电平：模块供电-启动输入低电平：模块掉电-休眠

extern char temperature_string[8];
extern uchar receive_data[8];

extern bit flag_compare;
extern bit flag_control_door;
extern bit flag_quick_door;
extern bit flag_buzzer;
extern bit flag_display_screen;				 

extern bit open_lock;
											  //锁
extern uchar time0_lock;										  //上锁定时
extern uchar time0_cartoon;										  //开关门动画定时
extern uchar time0_buzzer;										  //蜂鸣器定时
extern uchar time0_init_screen;									  //初始屏幕定时

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


extern void init_UART (void);							  		  //串口初始化 波特率19200 允许接受
extern void init_time0 (void);							  		  //定时器T0 方式一 16位  50ms  初始化

extern void display_init_screen_00 (void);						  //显示初始图片0
extern void display_init_screen_01 (void);						  //显示初始图片1

extern void display_control_screen_00 (uchar i);				  //显示控制图片00

extern void display_control_screen_01 (uchar *cmd);				  //显示控制图片01
	
extern void display_control_screen_02_00 (uchar i,uchar *cmd);	  //显示开关门图片02_00		  	  
extern void display_control_screen_02_01 (uchar *cmd);			  //指纹设置图片02_01
extern void display_control_screen_02_02 (uchar *cmd);			  //显示设置快速开门图片02_02		  
		  

extern void examine_lock (void);
extern void send_one_byte (unsigned char c);			  		  //发送一字节数据

extern void examine_key (void);
extern void control_door (uchar door,uchar *cmd);		  		  //ABCD门控制函数

//extern void buzzer_time_quantity (uint low_time,uint high_time,uchar quantity);




















#endif