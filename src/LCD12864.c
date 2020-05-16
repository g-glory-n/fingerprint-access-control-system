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
LCD12864复位
*********************************************/
void LCD12864_restart (void)
{
//	PSB=1;                 //设置为8BIT并口工作模式
	RST=0;                 //复位
	delay_10us(100);
	RST=1;                 //复位置高
	LCD12864_write_cmd(0x30);//设定8位MPU控制界面(功能设定内)

}


/*********************************************
LCD12864字符初始化
*********************************************/
void LCD12864_init_character (void)
{
	delay_10us(5000);

	LCD12864_write_cmd(0x30);//设定拓展指令集!!!
	LCD12864_write_cmd(0x34);//绘图显示  关


	LCD12864_write_cmd(0x30);//设定基本指令集!!!
    LCD12864_write_cmd(0x03);//地址归位
    LCD12864_write_cmd(0x0c);//开显示及光标设置
    LCD12864_write_cmd(0x06);//显示光标移动设置
	LCD12864_write_cmd(0x01);//显示清屏	
}


/*********************************************
LCD12864图片初始化
*********************************************/
void LCD12864_init_picture (void)
{
	delay_10us(5000);

	LCD12864_write_cmd(0x30);//设定基本指令集!!!
	LCD12864_write_cmd(0x08);// 显示 关


	LCD12864_write_cmd(0x34);//设定拓展指令集!!!
	LCD12864_write_cmd(0x36);//扩充指令动作 绘图显示 开

	LCD12864_Clear_picture();//清屏
		
}


/*********************************************
清除图片
*********************************************/
void LCD12864_Clear_picture (void)
{
   int i,j;
//*******显示上半屏内容设置
   for(i=0;i<32;i++)              //
    { 
      LCD12864_write_cmd(0x80 + i); //SET  垂直地址 VERTICAL ADD
      LCD12864_write_cmd(0x80);       //SET  水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         LCD12864_write_data(0x00);
       }
    }	   
//*******显示下半屏内容设置
   for(i=0;i<32;i++)              //
    {
      LCD12864_write_cmd(0x80 + i); //SET 垂直地址 VERTICAL ADD
      LCD12864_write_cmd(0x88);       //SET 水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         LCD12864_write_data(0x00);
       }
    }	
	
}


/*********************************************
显示图片
*********************************************/
void LCD12864_display_picture (uchar row,uchar column,uchar *picture)	//显示图片有局限
{
   int i,j;
//*******显示上半屏内容设置
   for(i=0;i<(32-16*row);i++)             
    { 
      LCD12864_write_cmd(0x80 + (i+16*row)); 		 //SET  垂直地址 VERTICAL ADD
      LCD12864_write_cmd(0x80+column);       //SET  水平地址 HORIZONTAL ADD
      for(j=0;j<(16-2*column);j++)
       {
         LCD12864_write_data(*picture++);
       }
    }	   
//*******显示下半屏内容设置
   for(i=0;i<32;i++)             
    {
      LCD12864_write_cmd(0x80 + i); 		 //SET 垂直地址 VERTICAL ADD
      LCD12864_write_cmd(0x88+column);       //SET 水平地址 HORIZONTAL ADD
      for(j=0;j<(16-2*column);j++)
       {
         LCD12864_write_data(*picture++);
       }
    }	
}


/*********************************************
显示字符串
*********************************************/
void LCD12864_display_character (uchar row,uchar column,uchar *string)  //row 行;column 列
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
写数据
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
写指令
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
LCD12864忙状态读取函数
**********************************************/
void LCD12864_examine_busy (void)
{
	RW_LCD=0xff;
	RS=0;	 	 //命令
	RW=1;	 	 //读
	E=1; 	 	 //读忙必须拉高E
	while(1)
	{
		if((RW_LCD&0x80)==0)
			break;	
	}			
}


/***********************************************
延时函数:10us周期,12m晶振
***********************************************/
void delay_10us(uint time)
{
	while(time--);
}

























