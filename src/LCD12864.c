#include "LCD12864.h"

#include "reg52.h"
#include "intrins.h"
#include "string.h"
#include "stdio.h"


uchar code dis_position[4][8]=
{
	{0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87},
	{0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97},
	{0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f},
	{0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f}
};


/*********************************************
LCD12864��λ
*********************************************/
void LCD12864_restart (void)
{
//	PSB=1;                 //����Ϊ8BIT���ڹ���ģʽ
	RST=0;                 //��λ
	delay_10us(100);
	RST=1;                 //��λ�ø�
	LCD12864_write_cmd(0x30);//�趨8λMPU���ƽ���(�����趨��)

}


/*********************************************
LCD12864�ַ���ʼ��
*********************************************/
void LCD12864_init_character (void)
{
	delay_10us(5000);

	LCD12864_write_cmd(0x30);//�趨��չָ�!!!
	LCD12864_write_cmd(0x34);//��ͼ��ʾ  ��


	LCD12864_write_cmd(0x30);//�趨����ָ�!!!
    LCD12864_write_cmd(0x03);//��ַ��λ
    LCD12864_write_cmd(0x0c);//����ʾ���������
    LCD12864_write_cmd(0x06);//��ʾ����ƶ�����
	LCD12864_write_cmd(0x01);//��ʾ����	
}


/*********************************************
LCD12864ͼƬ��ʼ��
*********************************************/
void LCD12864_init_picture (void)
{
	delay_10us(5000);

	LCD12864_write_cmd(0x30);//�趨����ָ�!!!
	LCD12864_write_cmd(0x08);// ��ʾ ��


	LCD12864_write_cmd(0x34);//�趨��չָ�!!!
	LCD12864_write_cmd(0x36);//����ָ��� ��ͼ��ʾ ��

	LCD12864_Clear_picture();//����
		
}


/*********************************************
���ͼƬ
*********************************************/
void LCD12864_Clear_picture (void)
{
   int i,j;
//*******��ʾ�ϰ�����������
   for(i=0;i<32;i++)              //
    { 
      LCD12864_write_cmd(0x80 + i); //SET  ��ֱ��ַ VERTICAL ADD
      LCD12864_write_cmd(0x80);       //SET  ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         LCD12864_write_data(0x00);
       }
    }	   
//*******��ʾ�°�����������
   for(i=0;i<32;i++)              //
    {
      LCD12864_write_cmd(0x80 + i); //SET ��ֱ��ַ VERTICAL ADD
      LCD12864_write_cmd(0x88);       //SET ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         LCD12864_write_data(0x00);
       }
    }	
	
}


/*********************************************
��ʾͼƬ
*********************************************/
void LCD12864_display_picture (uchar row,uchar column,uchar *picture)	//��ʾͼƬ�о���
{
   int i,j;
//*******��ʾ�ϰ�����������
   for(i=0;i<(32-16*row);i++)             
    { 
      LCD12864_write_cmd(0x80 + (i+16*row)); 		 //SET  ��ֱ��ַ VERTICAL ADD
      LCD12864_write_cmd(0x80+column);       //SET  ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<(16-2*column);j++)
       {
         LCD12864_write_data(*picture++);
       }
    }	   
//*******��ʾ�°�����������
   for(i=0;i<32;i++)             
    {
      LCD12864_write_cmd(0x80 + i); 		 //SET ��ֱ��ַ VERTICAL ADD
      LCD12864_write_cmd(0x88+column);       //SET ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<(16-2*column);j++)
       {
         LCD12864_write_data(*picture++);
       }
    }	
}


/*********************************************
��ʾ�ַ���
*********************************************/
void LCD12864_display_character (uchar row,uchar column,uchar *string)  //row ��;column ��
{
	uchar addr;
	uchar i;
	uchar length;
	length=strlen(string);
	addr=dis_position[row][column];
	LCD12864_write_cmd(addr);
	if(length>(16-2*column))
		for(i=0;i<(16-2*column);i++)
		{
			LCD12864_write_data(*string++);
		}
	else
		for(i=0;i<length;i++)
		{
			LCD12864_write_data(*string++);
		}		
}


/*********************************************
д����
*********************************************/
void LCD12864_write_data (uchar dat)
{
	LCD12864_examine_busy();
	RS=1;
	RW=0;
	_nop_();
	E=1;
	RW_LCD=dat;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E=0;		
}


/*********************************************
дָ��
*********************************************/
void LCD12864_write_cmd (uchar cmd)
{
	LCD12864_examine_busy();
	RS=0;
	RW=0;
	_nop_();
	E=1;
	RW_LCD=cmd;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E=0;		
}


/**********************************************
LCD12864æ״̬��ȡ����
**********************************************/
void LCD12864_examine_busy (void)
{
	RW_LCD=0xff;
	RS=0;	 	 //����
	RW=1;	 	 //��
	E=1; 	 	 //��æ��������E
	while(1)
	{
		if((RW_LCD&0x80)==0)
			break;	
	}			
}


/***********************************************
��ʱ����:10us����,12m����
***********************************************/
void delay_10us(uint time)
{
	while(time--);
}

























