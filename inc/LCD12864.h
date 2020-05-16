/*************************************************************
LCD12864���߷���:

P0  Ϊ�������ݿ�   p0.0...p0.7 -> DB0...DB7

sbit RS  = P2^4; //  ����/����   ��RS=0��,��������,��RS=1,��������.
sbit RW  = P2^3; //  ��/д    RW=0д;RW=1��
sbit E   = P2^2; //  ʹ�ܹܽ�   E=0  ->  E=1   ;�½���   д��LCD12864

PSB -> VCC 		 //	 ����/����ѡ��   H-����  L-����
sbit RST = P2^0; //  LCD12864 ��λ �͵�ƽ��Ч

BL_A �����Դ+ ; BL_k �����Դ-
VCC ��Դ+ ; GND ��Դ-

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


sbit RS  = P2^4; 													//  ����/����   ��RS=0��,��������,��RS=1,��������.
sbit RW  = P2^3; 													//  ��/д    RW=0д;RW=1��
sbit E   = P2^2; 													//  ʹ�ܹܽ�   E=0  ->  E=1   ;�½���   д��LCD12864
				 													//PSB  ����/����ѡ��   H-����  L-����
sbit RST = P2^1; 													//  LCD12864 ��λ �͵�ƽ��Ч


extern void LCD12864_examine_busy (void);			 							 		 //LCD12864æ״̬��ȡ����
extern void LCD12864_init_character (void);	 									 	  	 //LCD12864��ʼ��(�ַ�)
extern void LCD12864_init_picture (void);	 								  	 		 //LCD12864��ʼ��(ͼ��)
extern void LCD12864_restart (void);										 	 		 //LCD12864��λ


extern void LCD12864_write_data (uchar dat);											 //д����
extern void LCD12864_write_cmd (uchar cmd);												 //дָ��

extern void LCD12864_display_character (uchar row,uchar column,uchar *string);			 //��ʾ�ַ���
extern void LCD12864_display_picture (uchar row,uchar column,uchar *picture);			 //��ʾͼƬ
/********************************************************************************************************
ֻ��ʾͼƬʱ:
init_LCD12864_character();
write_cmd(0x01);
********************************************************************************************************/
extern void LCD12864_Clear_picture (void);												 //���ͼƬ
extern void delay_10us(uint time);												         //��ʱ����:10us����,12m����




#endif