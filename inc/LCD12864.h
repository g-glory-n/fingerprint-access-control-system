/*************************************************************
LCD12864接线方法:

P0  为输入数据口   p0.0...p0.7 -> DB0...DB7

sbit RS  = P2^4; //  数据/命令   当RS=0是,传送命令,当RS=1,传送数据.
sbit RW  = P2^3; //  读/写    RW=0写;RW=1读
sbit E   = P2^2; //  使能管脚   E=0  ->  E=1   ;下降沿   写入LCD12864

PSB -> VCC 		 //	 并口/串口选择   H-并行  L-串行
sbit RST = P2^0; //  LCD12864 复位 低电平有效

BL_A 背光电源+ ; BL_k 背光电源-
VCC 电源+ ; GND 电源-

**************************************************************/

#ifndef __LCD12864_h__
#define __LCD12864_h__

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#define RW_LCD P0


sbit RS  = P2^4; 													//  数据/命令   当RS=0是,传送命令,当RS=1,传送数据.
sbit RW  = P2^3; 													//  读/写    RW=0写;RW=1读
sbit E   = P2^2; 													//  使能管脚   E=0  ->  E=1   ;下降沿   写入LCD12864
				 													//PSB  并口/串口选择   H-并行  L-串行
sbit RST = P2^1; 													//  LCD12864 复位 低电平有效


extern void LCD12864_examine_busy (void);			 							 		 //LCD12864忙状态读取函数
extern void LCD12864_init_character (void);	 									 	  	 //LCD12864初始化(字符)
extern void LCD12864_init_picture (void);	 								  	 		 //LCD12864初始化(图像)
extern void LCD12864_restart (void);										 	 		 //LCD12864复位


extern void LCD12864_write_data (uchar dat);											 //写数据
extern void LCD12864_write_cmd (uchar cmd);												 //写指令

extern void LCD12864_display_character (uchar row,uchar column,uchar *string);			 //显示字符串
extern void LCD12864_display_picture (uchar row,uchar column,uchar *picture);			 //显示图片
/********************************************************************************************************
只显示图片时:
init_LCD12864_character();
write_cmd(0x01);
********************************************************************************************************/
extern void LCD12864_Clear_picture (void);												 //清除图片
extern void delay_10us(uint time);												         //延时函数:10us周期,12m晶振




#endif