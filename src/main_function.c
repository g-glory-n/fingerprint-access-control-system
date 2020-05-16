#include "main.h"
#include "LCD12864.h"
#include "picture.h"
#include "ZW_SZM201.h"
#include "DS18B20.h"


#include "intrins.h"
#include "string.h"
#include "reg52.h"
#include "stdio.h"


/****************************************************
��ʼ������

****************************************************/


void timer0_interrupt (void) interrupt 1
{
    TH0 = 0x4C;	//50ms
    TL0 = 0x00;
/********************************************************************************************************
��������ʱ ռ�ձȼ��������
********************************************************************************************************/
	
	if(buzzer_quantity)	
	{	
		time0_buzzer++;
	
		if(time0_buzzer<buzzer_low_time&&time0_buzzer>=0)
			buzzer=0;
		if(time0_buzzer<=(buzzer_high_time+buzzer_low_time)&&time0_buzzer>=buzzer_low_time)
		{
			buzzer=1;
			if(time0_buzzer==(buzzer_high_time+buzzer_low_time))
			{
				time0_buzzer=0;
				buzzer_quantity--;

			}
		}
	}	
/********************************************************************************************************
�õ�Ƭ��ʱ
********************************************************************************************************/
	if(open_lock==0)
		time0_init_screen++;
/********************************************************************************************************
�����Ŷ�����ʱ
********************************************************************************************************/
	if(open_lock==1)
		time0_cartoon++;
/********************************************************************************************************
������ʱ
********************************************************************************************************/
	if(open_lock==1)
		time0_lock++;
	if(time0_lock>=100&&open_lock==1)
	{
//		while(buzzer_quantity);		   //�ȴ��������				  //����

		open_lock=0;				   //����

		time0_lock=0;				   //��ʱ����
		time0_init_screen=0;
		time0_buzzer=0;

		count_options=0;
		count_confirm=0;			   //��Ļ��ѡ������
		copy_count_options=0;
		time0_cartoon=0;

		receive_data[0]=0x00;
		receive_data[1]=0x00;		   
		receive_data[2]=0x00;
		receive_data[3]=0x00;
		receive_data[4]=0x00;
		receive_data[5]=0x00;
		receive_data[6]=0x00;
		receive_data[7]=0x00;		    //����Э������

		parameter_1=0;
		parameter_2=0;
		parameter_3=0;

		buzzer_quantity=3;
		buzzer_low_time=2;
		buzzer_high_time=2;

		flag_compare=1;
		
//		display_init_screen_00();
			
	}


}



void examine_lock (void)
{
	if(open_lock==0)
	{

		if(flag_compare==1)
		{
			delay_10us(5000);							  //ָ��ģ���������ݼ� ��ʱ
			ZW_SZW201_cmd(0,0,0,"compare");
			flag_compare=0;
		}

		if(receive_data[0]==0xf5&&receive_data[1]==0x0c&&receive_data[7]==0xf5)
		{
			if(receive_data[4]==0x01||receive_data[4]==0x02||receive_data[4]==0x03)
			{
				while(buzzer_quantity);			//�ȴ��������
				open_lock=1;
				flag_quick_door=1;
				flag_display_screen=1;			//��ʾһ�η�ֹ��˸
				flag_buzzer=1;					//��������־

//				receive_data[0]=0x00;
//				receive_data[1]=0x00;		   
//				receive_data[2]=0x00;
//				receive_data[3]=0x00;
//				receive_data[4]=0x00;
//				receive_data[5]=0x00;
//				receive_data[6]=0x00;
//				receive_data[7]=0x00;		    //����Э������
						   

				buzzer_quantity=3;
				buzzer_low_time=2;
				buzzer_high_time=2;
			}
			if(receive_data[4]==0x05)
			{
				while(buzzer_quantity);
				flag_compare=1;
				//��Ӵ���
				receive_data[0]=0x00;
				receive_data[1]=0x00;		   
				receive_data[2]=0x00;
				receive_data[3]=0x00;
				receive_data[4]=0x00;
				receive_data[5]=0x00;
				receive_data[6]=0x00;
				receive_data[7]=0x00;		    //����Э������

				buzzer_quantity=1;
				buzzer_low_time=3;
				buzzer_high_time=0;

			}
		}
	}
	

}

//void buzzer_time_quantity (uint low_time,uint high_time,uchar quantity)
//{
//	if(quantity)	
//	{
//		if(time0_buzzer<low_time&&time0_buzzer>=0)
//			buzzer=0;
//		if(time0_buzzer<high_time&&time0_buzzer>=low_time)
//		{
//			buzzer=1;
//			if(time0_buzzer==high_time)
//				time0_buzzer=0;
//			quantity--;
//		}
//	}	
//}

void examine_key (void)
{
	if(options==0)
	{
		delay_10us(1000);
		if(options==0)
		{
			buzzer=0;
			time0_lock=0;
			count_options++;
			flag_display_screen=1;
			if(count_confirm==4&&(copy_count_options==20||copy_count_options==21||copy_count_options==22||copy_count_options==24||copy_count_options==25||copy_count_options==29))
			{
				parameter=0;

				if(copy_count_options==20)
				{
					count_confirm=3;
					copy_count_options=16;
					count_options=21;
				}
				if(copy_count_options==21)
				{
					count_confirm=3;
					copy_count_options=16;
					count_options=22;
				}
				if(copy_count_options==22)
				{
					count_confirm=3;
					copy_count_options=16;
					count_options=23;
				}



				if(copy_count_options==24)
				{
					count_confirm=3;
					copy_count_options=17;
					count_options=25;
				}
				if(copy_count_options==25)
				{
					count_confirm=3;
					copy_count_options=17;
					count_options=26;
				}



				if(copy_count_options==29)
				{
					count_confirm=3;
					copy_count_options=19;
					count_options=30;
				}
			}



			if(count_options==4&&count_confirm==0)	 //����00��ͷѭ��				��ѡ��
				count_options=0;
			if(count_options==10&&count_confirm==1)	 //����01��ͷѭ��				һ���Ų���
				count_options=4;
			if(count_options==16&&count_confirm==2)	 //����02_02��ͷѭ��			��������
				count_options=10;
			if(count_options==20&&count_confirm==2)	 //����02_01��ͷѭ��			ָ������ѡ��
				count_options=16;


			if(count_options==24&&count_confirm==3)	 //����03_00��ͷѭ��			¼��ָ��
				count_options=20;
			if(count_options==27&&count_confirm==3)	 //����03_01��ͷѭ��			ɾ��ָ��
				count_options=24;
			if(count_options==29&&count_confirm==3)	 //����03_01��ͷѭ��			ɾ������
				count_options=27;
			if(count_options==31&&count_confirm==3)	 //����03_01��ͷѭ��			�ȶԵȼ�
				count_options=29;

			if(count_confirm==2&&(copy_count_options==4||copy_count_options==5||copy_count_options==6))
			{
				count_options=0;
				count_confirm=0;			   //��Ļ��ѡ������
				copy_count_options=0;
				time0_lock=0;

  				buzzer_quantity=0;
				buzzer_low_time=0;
				buzzer_high_time=0;		   	   //��ͣ����

			}
		}
		while(!options);
		buzzer=1;
	}
	if(confirm==0)
	{
		delay_10us(1000);
		if(confirm==0)
		{
			buzzer=0;
			time0_lock=0;
			flag_display_screen=1;
			if(copy_count_options==20||copy_count_options==21||copy_count_options==22||copy_count_options==24||copy_count_options==25||copy_count_options==29)
			{
				parameter++;

				if(copy_count_options==22&&parameter==0x04)
					parameter=0;
				if(copy_count_options==29&&parameter==0x0a)
					parameter=0;

			}
			else
			{
			
				count_confirm++;

				copy_count_options=count_options;

				if(count_confirm==1)
					count_options=4;
				if(count_confirm==2&&copy_count_options==9)		//����������ý��� count_options++
					count_options=10;
				if(count_confirm==2&&copy_count_options==8)		//����ָ�����ý��� count_options++
					count_options=16;
		  	
				if(count_confirm==3&&copy_count_options==16)	//�������ָ�ƽ��� count_options++
					count_options=20;
				if(count_confirm==3&&copy_count_options==17)	//����ɾ��ָ�ƽ��� count_options++
					count_options=24;
				if(count_confirm==3&&copy_count_options==18)	//����ɾ������ָ�ƽ��� count_options++
					count_options=27;
				if(count_confirm==3&&copy_count_options==19)	//����ȶԵȼ����ý��� count_options++
					count_options=29;

				if(copy_count_options==20||copy_count_options==21||copy_count_options==22||copy_count_options==24||copy_count_options==25||copy_count_options==29)
					parameter=1;
			 	
						
				flag_control_door=1;
			}
		}
		while(!confirm);
		buzzer=1;
	}

	if(back_stop==0)
	{
		delay_10us(1000);
		if(back_stop==0)
		{
			buzzer=0;
			time0_lock=0;
			flag_display_screen=1;

			count_options=0;
			copy_count_options=0;
			count_confirm=0;			   //��Ļ��ѡ������

			relay_A_stop=0;
			relay_B_stop=0;
			relay_C_stop=0;
			relay_D_stop=0;

			delay_10us(2000);

			relay_A_stop=1;
			relay_B_stop=1;
			relay_C_stop=1;
			relay_D_stop=1;

  			buzzer_quantity=0;
			buzzer_low_time=0;
			buzzer_high_time=0;		   	   //��ͣ����


//			control_door('A',"stop");
//			control_door('B',"stop");
//			control_door('C',"stop");
//			control_door('D',"stop");
			parameter_1=0;
			parameter_2=0;
			parameter_3=0;
		}
		while(!back_stop);
		buzzer=1;
	}

}



void display_init_screen_00 (void)
{
	LCD12864_init_character();
	LCD12864_display_character(0,0,"��ָ");
	LCD12864_display_character(1,0,"����");
	LCD12864_display_character(2,0,"ϵ��");
	LCD12864_display_character(3,0,"ͳ��");

	LCD12864_init_picture();
	LCD12864_display_picture(0,2,picture_init_screen00);	
}

void display_init_screen_01 (void)
{
	LCD12864_init_character();
	LCD12864_display_character(0,0,"�������߹���");
	LCD12864_display_character(1,0,"��ϵ��ʽ��");
	LCD12864_display_character(2,0,"13966085530");
	LCD12864_display_character(3,0,"�����¶�");	  					
	LCD12864_display_character(3,4,temperature_string);
	LCD12864_display_character(3,7,"��");		
}












void display_control_screen_00 (uchar i)
{
	LCD12864_init_character();
	LCD12864_display_character(0,2,"��բ��_A");
	LCD12864_display_character(1,2,"��բ��_B");
	LCD12864_display_character(2,2,"��բ��_C");
	LCD12864_display_character(3,2,"��բ��_D");
	if(i=='A')
		LCD12864_display_character(0,0," ->");
	if(i=='B')
		LCD12864_display_character(1,0," ->");
	if(i=='C')
		LCD12864_display_character(2,0," ->");
	if(i=='D')
		LCD12864_display_character(3,0," ->");		
}

void display_control_screen_01 (uchar *cmd)
{
	LCD12864_init_character();
	LCD12864_display_character(0,1,"ѡ�������ʽ");
	LCD12864_display_character(1,1,"��");
	LCD12864_display_character(2,1,"��ͣ");
	LCD12864_display_character(3,1,"�ر�");
	LCD12864_display_character(1,6,"����");
	LCD12864_display_character(2,6,"ָ��");
	LCD12864_display_character(3,6,"����");
	if(strcmp(cmd,"open")==0)
		LCD12864_display_character(1,0,"->");
	if(strcmp(cmd,"stop")==0)
		LCD12864_display_character(2,0,"->");
	if(strcmp(cmd,"down")==0)
		LCD12864_display_character(3,0,"->");
	if(strcmp(cmd,"back")==0)
		LCD12864_display_character(1,5,"->");
	if(strcmp(cmd,"fingerprint")==0)
		LCD12864_display_character(2,5,"->");
	if(strcmp(cmd,"setting")==0)
		LCD12864_display_character(3,5,"->");

}

void display_control_screen_02_00 (uchar i,uchar *cmd)					  //���Ŷ���
{
	if(i=='A')
	{
		if(strcmp(cmd,"open")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"A   ->  open");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_A_10);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_A_09);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_A_08);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_A_07);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_A_06);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_A_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_A_04);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_A_03);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_A_02);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_A_01);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_A_00);		
		}
		if(strcmp(cmd,"stop")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"A   ->  stop");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_A_05);
			}
			if(time0_cartoon==20)
				LCD12864_init_picture();
			if(time0_cartoon==40)
				LCD12864_display_picture(1,0,picture_door_A_05);
			if(time0_cartoon==60)
				LCD12864_init_picture();
			if(time0_cartoon==80)
				LCD12864_display_picture(1,0,picture_door_A_05);
		}	
		if(strcmp(cmd,"down")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"A   ->  down");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_A_00);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_A_01);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_A_02);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_A_03);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_A_04);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_A_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_A_06);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_A_07);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_A_08);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_A_09);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_A_10);		
		}	
	}
	
	if(i=='B')
	{
		if(strcmp(cmd,"open")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"B   ->  open");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_B_10);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_B_09);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_B_08);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_B_07);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_B_06);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_B_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_B_04);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_B_03);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_B_02);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_B_01);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_B_00);		
		}
		if(strcmp(cmd,"stop")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"B   ->  stop");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_B_05);
			}
			if(time0_cartoon==20)
				LCD12864_init_picture();
			if(time0_cartoon==40)
				LCD12864_display_picture(1,0,picture_door_B_05);
			if(time0_cartoon==60)
				LCD12864_init_picture();
			if(time0_cartoon==80)
				LCD12864_display_picture(1,0,picture_door_B_05);
		}	
		if(strcmp(cmd,"down")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"B   ->  down");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_B_00);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_B_01);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_B_02);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_B_03);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_B_04);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_B_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_B_06);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_B_07);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_B_08);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_B_09);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_B_10);		
		}	
	}		

	if(i=='C')
	{
		if(strcmp(cmd,"open")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"C   ->  open");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_C_10);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_C_09);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_C_08);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_C_07);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_C_06);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_C_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_C_04);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_C_03);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_C_02);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_C_01);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_C_00);		
		}
		if(strcmp(cmd,"stop")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"C   ->  stop");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_C_05);
			}
			if(time0_cartoon==20)
				LCD12864_init_picture();
			if(time0_cartoon==40)
				LCD12864_display_picture(1,0,picture_door_C_05);
			if(time0_cartoon==60)
				LCD12864_init_picture();
			if(time0_cartoon==80)
				LCD12864_display_picture(1,0,picture_door_C_05);
		}	
		if(strcmp(cmd,"down")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"C   ->  down");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_C_00);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_C_01);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_C_02);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_C_03);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_C_04);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_C_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_C_06);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_C_07);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_C_08);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_C_09);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_C_10);		
		}	
	}		

	if(i=='D')
	{
		if(strcmp(cmd,"open")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"D   ->  open");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_D_10);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_D_09);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_D_08);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_D_07);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_D_06);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_D_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_D_04);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_D_03);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_D_02);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_D_01);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_D_00);		
		}
		if(strcmp(cmd,"stop")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"D   ->  stop");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_D_05);
			}
			if(time0_cartoon==20)
				LCD12864_init_picture();
			if(time0_cartoon==40)
				LCD12864_display_picture(1,0,picture_door_D_05);
			if(time0_cartoon==60)
				LCD12864_init_picture();
			if(time0_cartoon==80)
				LCD12864_display_picture(1,0,picture_door_D_05);
		}	
		if(strcmp(cmd,"down")==0)
		{
			if(time0_cartoon==0)
			{
				LCD12864_init_character();
				LCD12864_display_character(0,1,"D   ->  down");
					
				LCD12864_init_picture();
				LCD12864_display_picture(1,0,picture_door_D_00);
			}
			if(time0_cartoon==9)
				LCD12864_display_picture(1,0,picture_door_D_01);
			if(time0_cartoon==18)
				LCD12864_display_picture(1,0,picture_door_D_02);		
			if(time0_cartoon==27)
				LCD12864_display_picture(1,0,picture_door_D_03);		
			if(time0_cartoon==36)
				LCD12864_display_picture(1,0,picture_door_D_04);		
			if(time0_cartoon==45)
				LCD12864_display_picture(1,0,picture_door_D_05);		
			if(time0_cartoon==54)
				LCD12864_display_picture(1,0,picture_door_D_06);		
			if(time0_cartoon==63)
				LCD12864_display_picture(1,0,picture_door_D_07);		
			if(time0_cartoon==72)
				LCD12864_display_picture(1,0,picture_door_D_08);		
			if(time0_cartoon==81)
				LCD12864_display_picture(1,0,picture_door_D_09);		
			if(time0_cartoon==90)
				LCD12864_display_picture(1,0,picture_door_D_10);		
		}	
	}		
		
}



void display_control_screen_02_01 (uchar *cmd)					  //ָ������
{
	LCD12864_init_character();
	LCD12864_display_character(0,2,"���ָ��");
	LCD12864_display_character(1,2,"ɾȥָ��");
	LCD12864_display_character(2,2,"ɾȥ����");
	LCD12864_display_character(3,2,"�ȶԵȼ�");
	if(strcmp(cmd,"add")==0)
		LCD12864_display_character(0,0," -> ");
	if(strcmp(cmd,"delete")==0)
		LCD12864_display_character(1,0," -> ");
	if(strcmp(cmd,"delete_all")==0)
		LCD12864_display_character(2,0," -> ");
	if(strcmp(cmd,"security_level")==0)
		LCD12864_display_character(3,0," -> ");
}

void display_control_screen_02_02 (uchar *cmd)					  //���ٿ���
{
	LCD12864_init_character();
	LCD12864_display_character(0,0,"��ǰ���� -> ");
	LCD12864_display_character(1,1," B");
	LCD12864_display_character(2,1," C");
	LCD12864_display_character(3,1,"BC");
	LCD12864_display_character(1,6,"ABCD");
	LCD12864_display_character(2,6,"  ��");
	LCD12864_display_character(3,6,"����");
	if(strcmp(cmd,"B")==0)
		LCD12864_display_character(1,0,"->");
	if(strcmp(cmd,"C")==0)
		LCD12864_display_character(2,0,"->");
	if(strcmp(cmd,"BC")==0)
		LCD12864_display_character(3,0,"->");
	if(strcmp(cmd,"ABCD")==0)
		LCD12864_display_character(1,5,"->");
	if(strcmp(cmd,"��")==0)
		LCD12864_display_character(2,5,"->");
	if(strcmp(cmd,"back")==0)
		LCD12864_display_character(3,5,"->");

}


void control_door (uchar door,uchar *cmd)
{
	if(door=='A')
	{
		if(strcmp(cmd,"open")==0)
		{
			relay_A_open=0;
			delay_10us(1000);
			relay_A_open=1;	
		}
		if(strcmp(cmd,"stop")==0)
		{
			relay_A_stop=0;
			delay_10us(1000);
			relay_A_stop=1;	
		}	
		if(strcmp(cmd,"down")==0)
		{
			relay_A_down=0;
			delay_10us(1000);
			relay_A_down=1;	
		}		
	}
	if(door=='B')
	{
		if(strcmp(cmd,"open")==0)
		{
			relay_B_open=0;
			delay_10us(1000);
			relay_B_open=1;	
		}
		if(strcmp(cmd,"stop")==0)
		{
			relay_B_stop=0;
			delay_10us(1000);
			relay_B_stop=1;	
		}	
		if(strcmp(cmd,"down")==0)
		{
			relay_B_down=0;
			delay_10us(1000);
			relay_B_down=1;	
		}		
	}	
	if(door=='C')
	{
		if(strcmp(cmd,"open")==0)
		{
			relay_C_open=0;
			delay_10us(1000);
			relay_C_open=1;	
		}
		if(strcmp(cmd,"stop")==0)
		{
			relay_C_stop=0;
			delay_10us(1000);
			relay_C_stop=1;	
		}	
		if(strcmp(cmd,"down")==0)
		{
			relay_C_down=0;	
			delay_10us(1000);
			relay_C_down=1;	
		}		
	}	
	if(door=='D')
	{
		if(strcmp(cmd,"open")==0)
		{
			relay_D_open=0;
			delay_10us(1000);
			relay_D_open=1;	
		}
		if(strcmp(cmd,"stop")==0)
		{
			relay_D_stop=0;
			delay_10us(1000);
			relay_D_stop=1;	
		}	
		if(strcmp(cmd,"down")==0)
		{
			relay_D_down=0;
			delay_10us(1000);
			relay_D_down=1;	
		}		
	}	
	
}


void init_UART (void)
{
    TMOD = 0x21; //T0��ʱ�� T1 �����ʷ�����
    SCON = 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x80;
    EA = 1;
    ES = 1;
    TR1 = 1;

	PS=1;			//�������ȼ���
}
void init_time0 (void)
{
    TMOD = 0x21; //T0��ʱ�� T1 �����ʷ�����
    TH0 = 0x4C;	 //50ms
    TL0 = 0x00;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void send_one_byte (uchar c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

void UART_interrupt (void) interrupt 4
{
	static uchar i=0;
    if(RI)
    {
        RI = 0;
		receive_data[i]=SBUF;
		i++;
		if(i==8)
			i=0;
    }
}




































