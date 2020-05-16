#include "main.h"
#include "LCD12864.h"
#include "ZW_SZM201.h"
#include "DS18B20.h"
//#include "picture.h"	ע�� ͼƬ���� �ظ�����!!!!

#include "intrins.h"
#include "string.h"
#include "reg52.h"
#include "stdio.h"

uchar receive_data[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};     //�ɹ�0xf5,0x0c,0x00,0x01,0x01,0x00,0x0c,0xf5

char temperature_string[8]={':',' ',' ',' ',' ',' ',' ',' '};

float temperature=0.0;
																	 
bit open_lock=0;

bit flag_compare=0;
bit flag_control_door=0;
bit flag_quick_door=0;
bit flag_display_screen=0;
bit flag_buzzer=0;

uchar parameter_1=0;
uchar parameter_2=0;
uchar parameter_3=0;

char array_parameter_1[4]={' ',' ',' ',' '};	  //���д��4�ֽ� ��ֹд����� ��ʾ����
char array_parameter_2[4]={' ',' ',' ',' '};
char array_parameter_3[4]={' ',' ',' ',' '};

uchar parameter=0;

uchar time0_lock=0;
uchar time0_init_screen=0;
uchar time0_cartoon=0;
uchar time0_buzzer=0;

uchar buzzer_quantity=0;
uchar buzzer_low_time=0;
uchar buzzer_high_time=0;


uchar quick_door=0;

uint count_options=0;
uchar count_confirm=0;
uchar copy_count_options=0;



uchar door=0;					  				//��¼��ѡ��
 
void main (void)
{
	init_UART();				   				//��ʼ������
	init_time0();				   				//��ʼ��T0
	LCD12864_restart();			   				//�ϵ縴λLCD12864

	P0=0xff;  
	P1=0xff;
	P2=0xff;
	P3=0xff;					   				//�˿��ø�

  	buzzer_quantity=3;
	buzzer_low_time=10;
	buzzer_high_time=10;		   				//������ʾ��

	control_door('A',"stop");
	control_door('B',"stop");
	control_door('C',"stop");
	control_door('D',"stop");	   				//�ϵ�ͣ��

	temperature=ds18b20_count_temperature();	//�����¶�

	delay_10us(50000);			   				//�ϵ���ʱ  �ȴ�ָ��ģ��
	delay_10us(50000);
	ZW_SZW201_cmd(0,0,0,"compare");				//ָ��ģ��1:Nָ��

	display_init_screen_00();
	time0_init_screen=0;
  
	while(1)
	{
		examine_lock();

		if(open_lock==0)
		{

   			if(time0_init_screen==100)
			{ 
				temperature=ds18b20_count_temperature();							//Ӱ��stop������������ �¶ȸı�ʱ ds18b20��Ҫ��Ӧʱ��
				sprintf((temperature_string+1),"%2.2f",temperature);
 	 									
				display_init_screen_01();											// 5s �õ�Ƭ1;
			}
			if(time0_init_screen>=200)												//10s �õ�Ƭ2;
			{	
				temperature=ds18b20_count_temperature();								
				sprintf((temperature_string+1),"%2.2f",temperature);

				time0_init_screen=0;					 							//��ʱ����				
				ZW_SZW201_cmd(0,0,0,"compare");										//ÿ5s�ӷ���1:N�ȶ�ָ��
				display_init_screen_00();
			}

			if(back_stop==0)														//����ʱ��ͣ
			{																		
				delay_10us(1000);
				if(back_stop==0)
				{
					buzzer=0;
					relay_A_stop=0;
					relay_B_stop=0;
					relay_C_stop=0;
					relay_D_stop=0;

					delay_10us(2000);

					relay_A_stop=1;
					relay_B_stop=1;
					relay_C_stop=1;
					relay_D_stop=1;
				}
				while(!back_stop);
				buzzer=1;
			}
		}

		if(open_lock==1)
		{
			examine_key ();

			if(count_confirm==0)
			{
				if(flag_display_screen)
				{
					if(count_options==0)
						display_control_screen_00('A');
					if(count_options==1)
						display_control_screen_00('B');
					if(count_options==2)
						display_control_screen_00('C');
					if(count_options==3)
						display_control_screen_00('D');
					flag_display_screen=0;
				}
			}
			







			if(count_confirm==1)
			{
				if(flag_display_screen)
				{
					if(copy_count_options==0)
						door='A';
					if(copy_count_options==1)
						door='B';
					if(copy_count_options==2)
						door='C';
					if(copy_count_options==3)
						door='D';
					if(count_options==4)				
						display_control_screen_01("open");					
					if(count_options==5)
						display_control_screen_01("stop");	
					if(count_options==6)
						display_control_screen_01("down");	
					if(count_options==7)
						display_control_screen_01("back");	
					if(count_options==8)
						display_control_screen_01("fingerprint");
					if(count_options==9)
						display_control_screen_01("setting");
					flag_display_screen=0;
				}
			}








			if(count_confirm==2)
			{
				if(copy_count_options==4)
				{
					if(flag_control_door==1)
					{
						control_door(door,"stop");
						delay_10us(2000);
						control_door(door,"open");
						
						buzzer_quantity=20;
						buzzer_low_time=2;
						buzzer_high_time=2;

						time0_cartoon=0;
						flag_control_door=0;
					}
					display_control_screen_02_00 (door,"open");
				}
				if(copy_count_options==5)
				{
					if(flag_control_door==1)
					{
						control_door(door,"stop");
						
						buzzer_quantity=20;
						buzzer_low_time=2;
						buzzer_high_time=2;

						time0_cartoon=0;
						flag_control_door=0;
					}
					display_control_screen_02_00 (door,"stop");
				}
				if(copy_count_options==6)
				{
					if(flag_control_door==1)
					{
						control_door(door,"stop");
						delay_10us(2000);
						control_door(door,"down");

						buzzer_quantity=20;
						buzzer_low_time=2;
						buzzer_high_time=2;

						time0_cartoon=0;
						flag_control_door=0;
					}
					display_control_screen_02_00 (door,"down");
				}
				if(copy_count_options==7)
				{
					count_confirm=0;
					copy_count_options=0;
					count_options=0;

				}




				if(copy_count_options==8&&receive_data[4]==0x03)
				{
					if(flag_display_screen)
					{
						if(count_options==16)				
							display_control_screen_02_01("add");					
						if(count_options==17)
							display_control_screen_02_01("delete");	
						if(count_options==18)
							display_control_screen_02_01("delete_all");	
						if(count_options==19)
							display_control_screen_02_01("security_level");
						flag_display_screen=0;
					}
				}


				if(copy_count_options==8&&(receive_data[4]==0x01||receive_data[4]==0x02))
				{
					LCD12864_init_character();
					LCD12864_display_character(1,2,"���ṩ");	  
					LCD12864_display_character(2,1,"����Աָ��!!");

					buzzer_quantity=1;
					buzzer_low_time=20;
					buzzer_high_time=0;

					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					count_confirm=0;
					copy_count_options=0;
					count_options=0;
					time0_lock=100;
					continue;
				}





				if(copy_count_options==9)
				{
					if(flag_display_screen)
					{
						if(count_options==10)				
							display_control_screen_02_02("B");					
						if(count_options==11)
							display_control_screen_02_02("C");	
						if(count_options==12)
							display_control_screen_02_02("BC");	
						if(count_options==13)
							display_control_screen_02_02("ABCD");	
						if(count_options==14)
							display_control_screen_02_02("��");
						if(count_options==15)
							display_control_screen_02_02("back");
						if(quick_door==0)
							LCD12864_display_character(0,6,"  ��");						
						if(quick_door==1)
							LCD12864_display_character(0,6,"   B");						
						if(quick_door==2)
							LCD12864_display_character(0,6,"   C");						
						if(quick_door==3)
							LCD12864_display_character(0,6,"  BC");						
						if(quick_door==4)
							LCD12864_display_character(0,6,"ABCD");
						flag_display_screen=0;	
					}						
						
				}
			}









			if(count_confirm==3)
			{
				if(copy_count_options==4||copy_count_options==5||copy_count_options==6)
				{
					count_confirm=0;
					copy_count_options=0;
					count_options=0;			   //��Ļ��ѡ������
					time0_lock=0;

  					buzzer_quantity=0;
					buzzer_low_time=0;
					buzzer_high_time=0;		   	   //��ͣ����

				}



				if(copy_count_options==16)
				{
					sprintf(array_parameter_1,"%02d",(uint)parameter_1);
					sprintf(array_parameter_2,"%02d",(uint)parameter_2);
					sprintf(array_parameter_3,"%02d",(uint)parameter_3);
					if(count_options==20)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,3,"��λ: ");
						LCD12864_display_character(1,3,"��λ: ");
						LCD12864_display_character(2,3,"Ȩ��: ");
						LCD12864_display_character(3,3,"ȷ��");

						LCD12864_display_character(0,6,array_parameter_1);
						LCD12864_display_character(1,6,array_parameter_2);
						LCD12864_display_character(2,6,array_parameter_3);

						LCD12864_display_character(0,0," -> ");
					}
					if(count_options==21)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,3,"��λ: ");
						LCD12864_display_character(1,3,"��λ: ");
						LCD12864_display_character(2,3,"Ȩ��: ");
						LCD12864_display_character(3,3,"ȷ��");

						LCD12864_display_character(0,6,array_parameter_1);
						LCD12864_display_character(1,6,array_parameter_2);
						LCD12864_display_character(2,6,array_parameter_3);

						LCD12864_display_character(1,0," -> ");
					}
					if(count_options==22)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,3,"��λ: ");
						LCD12864_display_character(1,3,"��λ: ");
						LCD12864_display_character(2,3,"Ȩ��: ");
						LCD12864_display_character(3,3,"ȷ��");

						LCD12864_display_character(0,6,array_parameter_1);
						LCD12864_display_character(1,6,array_parameter_2);
						LCD12864_display_character(2,6,array_parameter_3);

						LCD12864_display_character(2,0," -> ");
					}
					if(count_options==23)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,3,"��λ: ");
						LCD12864_display_character(1,3,"��λ: ");
						LCD12864_display_character(2,3,"Ȩ��: ");
						LCD12864_display_character(3,3,"ȷ��");

						LCD12864_display_character(0,6,array_parameter_1);
						LCD12864_display_character(1,6,array_parameter_2);
						LCD12864_display_character(2,6,array_parameter_3);

						LCD12864_display_character(3,0," -> ");
					}


				}
				if(copy_count_options==17)
				{
					sprintf(array_parameter_1,"%02d",(uint)parameter_1);
					sprintf(array_parameter_2,"%02d",(uint)parameter_2);
					if(count_options==24)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,1,"delete  user");
						LCD12864_display_character(1,3,"��λ: ");
						LCD12864_display_character(2,3,"��λ: ");
						LCD12864_display_character(3,3,"ȷ��");

						LCD12864_display_character(1,6,array_parameter_1);
						LCD12864_display_character(2,6,array_parameter_2);

						LCD12864_display_character(1,0," -> ");
					}
					if(count_options==25)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,1,"delete  user");
						LCD12864_display_character(1,3,"��λ: ");
						LCD12864_display_character(2,3,"��λ: ");
						LCD12864_display_character(3,3,"ȷ��");

						LCD12864_display_character(1,6,array_parameter_1);
						LCD12864_display_character(2,6,array_parameter_2);

						LCD12864_display_character(2,0," -> ");
					}
					if(count_options==26)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,1,"delete  user");
						LCD12864_display_character(1,3,"��λ: ");
						LCD12864_display_character(2,3,"��λ: ");
						LCD12864_display_character(3,3,"ȷ��");

						LCD12864_display_character(1,6,array_parameter_1);
						LCD12864_display_character(2,6,array_parameter_2);

						LCD12864_display_character(3,0," -> ");
					}
				}



				if(copy_count_options==18)
				{
					if(count_options==27)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,0," delete all user");
						LCD12864_display_character(2,2,"ȷ��");
						LCD12864_display_character(2,6,"ȡ��");

						LCD12864_display_character(2,0," -> ");
					}
					if(count_options==28)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,0," delete all user");
						LCD12864_display_character(2,2,"ȷ��");
						LCD12864_display_character(2,6,"ȡ��");

						LCD12864_display_character(2,4," -> ");
					}

				}
				if(copy_count_options==19)
				{
					sprintf(array_parameter_1,"%02d",(uint)parameter_1);
					if(count_options==29)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,0,"  setting level ");
						LCD12864_display_character(1,3,"�ȼ�: ");
						LCD12864_display_character(2,3,"ȷ��");

						LCD12864_display_character(1,6,array_parameter_1);

						LCD12864_display_character(1,1," -> ");
					}
					if(count_options==30)
					{
						LCD12864_init_character();
						LCD12864_display_character(0,0,"  setting level ");
						LCD12864_display_character(1,3,"�ȼ�: ");
						LCD12864_display_character(2,3,"ȷ��");

						LCD12864_display_character(1,6,array_parameter_1);

						LCD12864_display_character(2,1," -> ");
					}
				}








				if(copy_count_options==10)		  						//B		 ���ÿ���  ���ֿ���  ���ֹ���
				{		  
					quick_door=1;
					LCD12864_init_character();
					LCD12864_display_character(0,1,"�������óɹ�");
					LCD12864_display_character(2,1,"��ָ���ٿ���");
					LCD12864_display_character(3,1,"��ָ���ٹ���");

					buzzer_quantity=1;
					buzzer_low_time=20;
					buzzer_high_time=0;

					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
//					count_confirm=2;
//					copy_count_options=9;
//					count_options=10;
					count_confirm=0;
					copy_count_options=0;
					count_options=0;
					time0_lock=100;
					continue;

				}
				if(copy_count_options==11)		  						//C
				{		  
					quick_door=2;
					LCD12864_init_character();
					LCD12864_display_character(0,1,"�������óɹ�");
					LCD12864_display_character(2,1,"��ָ���ٿ���");
					LCD12864_display_character(3,1,"��ָ���ٹ���");

					buzzer_quantity=1;
					buzzer_low_time=20;
					buzzer_high_time=0;

					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					count_confirm=0;
					copy_count_options=0;
					count_options=0;
					time0_lock=100;
					continue;
				}
				if(copy_count_options==12)		  						//BC
				{		  
					quick_door=3;
					LCD12864_init_character();
					LCD12864_display_character(0,1,"�������óɹ�");
					LCD12864_display_character(2,1,"��ָ���ٿ���");
					LCD12864_display_character(3,1,"��ָ���ٹ���");

					buzzer_quantity=1;
					buzzer_low_time=20;
					buzzer_high_time=0;

					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					count_confirm=0;
					copy_count_options=0;
					count_options=0;
					time0_lock=100;
					continue;
				}
				if(copy_count_options==13)		  						//ABCD
				{		  
					quick_door=4;
					LCD12864_init_character();
					LCD12864_display_character(0,1,"�������óɹ�");
					LCD12864_display_character(2,1,"��ָ���ٿ���");
					LCD12864_display_character(3,1,"��ָ���ٹ���");

					buzzer_quantity=1;
					buzzer_low_time=20;
					buzzer_high_time=0;

					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					count_confirm=0;
					copy_count_options=0;
					count_options=0;
					time0_lock=100;
					continue;
				}
				if(copy_count_options==14)		 						 //��
				{
					quick_door=0;		  
					LCD12864_init_character();
					LCD12864_display_character(1,1,"��������ȡ��");

					buzzer_quantity=1;
					buzzer_low_time=20;
					buzzer_high_time=0;

					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					count_confirm=0;
					copy_count_options=0;
					count_options=0;
					time0_lock=100;
					continue;
				}
				if(copy_count_options==15)								  //����
				{
					count_confirm=1;
					copy_count_options=0;
					count_options=4;
				}
			}










			if(count_confirm==4)
			{

				if(copy_count_options==20)
				{
					parameter_1=parameter;

					sprintf(array_parameter_1,"%02d",(uint)parameter_1);		 //��#�����0x
					LCD12864_display_character(0,7,"  ");						 //�������λ��
					LCD12864_display_character(0,6,array_parameter_1);
		
				}
				if(copy_count_options==21)
				{
					parameter_2=parameter;	
				
					sprintf(array_parameter_2,"%02d",(uint)parameter_2);
					LCD12864_display_character(1,7,"  ");
					LCD12864_display_character(1,6,array_parameter_2);
				}
				if(copy_count_options==22)
				{
					parameter_3=parameter;
					sprintf(array_parameter_3,"%02d",(uint)parameter_3);
					LCD12864_display_character(2,6,array_parameter_3);
				}
				if(copy_count_options==23)
				{

					receive_data[0]=0x00;
					receive_data[1]=0x00;		   
					receive_data[2]=0x00;
					receive_data[3]=0x00;
					receive_data[4]=0x00;
					receive_data[5]=0x00;
					receive_data[6]=0x00;
					receive_data[7]=0x00;		   									 //����Э������
							   

					LCD12864_init_character();
					LCD12864_display_character(0,1,"�밴ѹ��Ҽ��");					 //����һ���2FF  ���������EF  �û��� 1~FFEF
					ZW_SZW201_cmd (parameter_1,parameter_2,parameter_3,"add_1");
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					time0_lock=0;
					delay_10us(50000);
					delay_10us(50000);
					if(receive_data[1]==0x01&&receive_data[4]==0x07&&receive_data[6]==0x06)
					{
						LCD12864_display_character(0,1,"��ָ���Ѵ���");
						delay_10us(50000);										 //F5 03 00 00 01 00 02 F5		¼��ʧ��
						delay_10us(50000);
						delay_10us(50000);										 //F5 01 00 00 07 00 06 F5		ָ���Ѵ���
						delay_10us(50000);
						delay_10us(50000);
						delay_10us(50000);

						parameter_1=0;
						parameter_2=0;
						parameter_3=0;

						receive_data[4]=0x03;

						count_confirm=2;
						copy_count_options=8;
						count_options=16;		
						time0_lock=0;
						continue;
					}
					LCD12864_init_character();	
					LCD12864_display_character(1,1,"�밴ѹ�ڷ���");
					ZW_SZW201_cmd (parameter_1,parameter_2,parameter_3,"add_2");
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					time0_lock=0;
					delay_10us(50000);
					delay_10us(50000);
					LCD12864_init_character();
					LCD12864_display_character(2,1,"�밴ѹ������");
					ZW_SZW201_cmd (parameter_1,parameter_2,parameter_3,"add_3");
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					time0_lock=0;
					delay_10us(50000);
					delay_10us(50000);
					if(receive_data[1]==0x03&&receive_data[4]==0x01&&receive_data[6]==0x02)
					{
						LCD12864_display_character(2,1,"ָ��¼��ʧ��");
						delay_10us(50000);										 
						delay_10us(50000);
						delay_10us(50000);										
						delay_10us(50000);
						delay_10us(50000);
						delay_10us(50000);

						parameter_1=0;
						parameter_2=0;
						parameter_3=0;

						receive_data[4]=0x03;

						count_confirm=2;
						copy_count_options=8;
						count_options=16;		
						time0_lock=0;
						continue;
					}	
					if(receive_data[1]==0x03&&receive_data[4]==0x00&&receive_data[6]==0x03)
					{
						LCD12864_display_character(2,1,"ָ��¼��ɹ�");
						delay_10us(50000);										 
						delay_10us(50000);
						delay_10us(50000);										
						delay_10us(50000);
						delay_10us(50000);
						delay_10us(50000);

						parameter_1=0;
						parameter_2=0;
						parameter_3=0;

						receive_data[4]=0x03;

						count_confirm=2;
						copy_count_options=8;
						count_options=16;		
						time0_lock=0;
						continue;
					}
					LCD12864_display_character(2,1,"ģ������Ӧ!!");
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);


					parameter_1=0;
					parameter_2=0;
					parameter_3=0;

					receive_data[4]=0x03;
					
					count_confirm=2;
					copy_count_options=8;
					count_options=16;
					time0_lock=0;
					continue;		
						
				}
				if(copy_count_options==24)
				{
					parameter_1=parameter;

					sprintf(array_parameter_1,"%02d",(uint)parameter_1);
					LCD12864_display_character(1,7,"  ");
					LCD12864_display_character(1,6,array_parameter_1);
				}
				if(copy_count_options==25)
				{
					parameter_2=parameter;

					sprintf(array_parameter_2,"%02d",(uint)parameter_2);
					LCD12864_display_character(2,7,"  ");
					LCD12864_display_character(2,6,array_parameter_2);
				}
				if(copy_count_options==26)
				{
					receive_data[0]=0x00;
					receive_data[1]=0x00;		   
					receive_data[2]=0x00;
					receive_data[3]=0x00;
					receive_data[4]=0x00;
					receive_data[5]=0x00;
					receive_data[6]=0x00;
					receive_data[7]=0x00;		   									 //����Э������

					LCD12864_init_character();
					LCD12864_display_character(1,1,"ɾȥ�ض��û�");					 
					ZW_SZW201_cmd (parameter_1,parameter_2,0,"delete");
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					LCD12864_init_character();
					LCD12864_display_character(1,2,"���Ժ󡭡�");				 
					delay_10us(50000);
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);

					time0_lock=0;

					if(receive_data[1]==0x04&&receive_data[4]==0x00&&receive_data[6]==0x04)
					{
						LCD12864_init_character();
						LCD12864_display_character(1,2,"�����ɹ�");
						delay_10us(50000);										 
						delay_10us(50000);
						delay_10us(50000);										
						delay_10us(50000);
						delay_10us(50000);
						delay_10us(50000);

						parameter_1=0;
						parameter_2=0;
						parameter_3=0;

						receive_data[4]=0x03;

						count_confirm=2;
						copy_count_options=8;
						count_options=16;		
						time0_lock=0;
						continue;
					}
					if(receive_data[1]==0x04&&receive_data[4]==0x05&&receive_data[6]==0x01)
					{
						LCD12864_init_character();
						LCD12864_display_character(1,2,"����ʧ��");
						delay_10us(50000);										 
						delay_10us(50000);
						delay_10us(50000);										
						delay_10us(50000);
						delay_10us(50000);
						delay_10us(50000);

						parameter_1=0;
						parameter_2=0;
						parameter_3=0;

						receive_data[4]=0x03;

						count_confirm=2;
						copy_count_options=8;
						count_options=16;		
						time0_lock=0;
						continue;
					}

					LCD12864_init_character();
					LCD12864_display_character(1,1,"ģ������Ӧ!!");
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);										
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);

					parameter_1=0;
					parameter_2=0;
					parameter_3=0;

					receive_data[4]=0x03;

					count_confirm=2;
					copy_count_options=8;
					count_options=16;		
					time0_lock=0;
					continue;
						
				}
				if(copy_count_options==27)
				{
					receive_data[0]=0x00;
					receive_data[1]=0x00;		   
					receive_data[2]=0x00;
					receive_data[3]=0x00;
					receive_data[4]=0x00;
					receive_data[5]=0x00;
					receive_data[6]=0x00;
					receive_data[7]=0x00;		   									 //����Э������

					LCD12864_init_character();
					LCD12864_display_character(1,1,"ɾȥ�����û�");					 
					ZW_SZW201_cmd (0,0,0,"delete_all");
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					LCD12864_init_character();
					LCD12864_display_character(1,2,"���Ժ󡭡�");				 
					delay_10us(50000);
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);

					time0_lock=0;
				
					if(receive_data[1]==0x05&&receive_data[4]==0x00&&receive_data[6]==0x05)
					{
						LCD12864_init_character();
						LCD12864_display_character(1,2,"�����ɹ�");
						delay_10us(50000);										 
						delay_10us(50000);
						delay_10us(50000);										
						delay_10us(50000);
						delay_10us(50000);
						delay_10us(50000);

						parameter_1=0;
						parameter_2=0;
						parameter_3=0;

						receive_data[4]=0x03;

						count_confirm=2;
						copy_count_options=8;
						count_options=16;		
						time0_lock=0;
						continue;
					}
					LCD12864_init_character();
					LCD12864_display_character(1,1,"ģ������Ӧ!!");
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);										
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);

					parameter_1=0;
					parameter_2=0;
					parameter_3=0;

					receive_data[4]=0x03;

					count_confirm=2;
					copy_count_options=8;
					count_options=16;		
					time0_lock=0;
					continue;

				}
				if(copy_count_options==28)
				{
					count_confirm=2;
					copy_count_options=8;
					count_options=16;		
				}
				if(copy_count_options==29)
				{
					parameter_1=parameter;
					sprintf(array_parameter_1,"%02d",(uint)parameter_1);
					LCD12864_display_character(1,6,array_parameter_1);
				}
				if(copy_count_options==30)
				{
					receive_data[0]=0x00;
					receive_data[1]=0x00;		   
					receive_data[2]=0x00;
					receive_data[3]=0x00;
					receive_data[4]=0x00;
					receive_data[5]=0x00;
					receive_data[6]=0x00;
					receive_data[7]=0x00;		   									 //����Э������

					LCD12864_init_character();
					LCD12864_display_character(1,1,"���ñȶԵȼ�");					 
					ZW_SZW201_cmd (parameter_1,0,0,"security_level");
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);
					LCD12864_init_character();
					LCD12864_display_character(1,2,"���Ժ󡭡�");	
					ZW_SZW201_cmd (parameter_1,0,0,"security_level");			 
					delay_10us(50000);
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);

					time0_lock=0;

					if(receive_data[1]==0x28&&receive_data[3]==parameter_1&&receive_data[6]==(0x28^receive_data[3]))
					{
						LCD12864_init_character();
						LCD12864_display_character(1,2,"�����ɹ�");
						delay_10us(50000);										 
						delay_10us(50000);
						delay_10us(50000);										
						delay_10us(50000);
						delay_10us(50000);
						delay_10us(50000);

						parameter_1=0;
						parameter_2=0;
						parameter_3=0;

						receive_data[4]=0x03;

						count_confirm=2;
						copy_count_options=8;
						count_options=16;		
						time0_lock=0;
						continue;
					}

					LCD12864_init_character();
					LCD12864_display_character(1,1,"ģ������Ӧ!!");
					delay_10us(50000);										 
					delay_10us(50000);
					delay_10us(50000);										
					delay_10us(50000);
					delay_10us(50000);
					delay_10us(50000);

					parameter_1=0;
					parameter_2=0;
					parameter_3=0;

					receive_data[4]=0x03;

					count_confirm=2;
					copy_count_options=8;
					count_options=16;		
					time0_lock=0;
					continue;
						
				}
				if(parameter_1==0xff&&parameter_2>=0xf0)
				{
					parameter_1=0;
					parameter_2=0;
					parameter_3=0;
					continue;
				}

					
			}










//			if(count_confirm==5)
//			{
//					
//			}
//
//
//
//
//
//
//
//
//
//			if(count_confirm==6)
//			{
//					
//			}








			if(flag_quick_door==1)
			{
				if(quick_door==0&&(receive_data[4]==0x01||receive_data[4]==0x02||receive_data[4]==0x03))
				{
					flag_quick_door=0;
					continue;					
				}

				if(quick_door==1&&(receive_data[4]==0x01||receive_data[4]==0x03))
				{
					control_door('B',"stop");
					delay_10us(2000);
					control_door('B',"open");
				}
				if(quick_door==1&&receive_data[4]==0x02)
				{
					control_door('B',"stop");
					delay_10us(2000);
					control_door('B',"down");
				}

				if(quick_door==2&&(receive_data[4]==0x01||receive_data[4]==0x03))
				{
					control_door('C',"stop");
					delay_10us(2000);
					control_door('C',"open");
				}
				if(quick_door==2&&receive_data[4]==0x02)
				{
					control_door('C',"stop");
					delay_10us(2000);
					control_door('C',"down");
				}

				if(quick_door==3&&(receive_data[4]==0x01||receive_data[4]==0x03))
				{
					control_door('B',"stop");
					control_door('C',"stop");
					delay_10us(2000);
					control_door('C',"open");
					control_door('B',"open");
				}
				if(quick_door==3&&receive_data[4]==0x02)
				{
					control_door('B',"stop");
					control_door('C',"stop");
					delay_10us(2000);
					control_door('C',"down");
					control_door('B',"down");
				}

				if(quick_door==4&&(receive_data[4]==0x01||receive_data[4]==0x03))
				{
					control_door('A',"stop");
					control_door('B',"stop");
					control_door('C',"stop");
					control_door('D',"stop");
		  	
					delay_10us(2000);

					control_door('A',"open");
					control_door('B',"open");
					control_door('C',"open");
					control_door('D',"open");

				}  	
				if(quick_door==4&&receive_data[4]==0x02)
				{
					control_door('A',"stop");
					control_door('B',"stop");
					control_door('C',"stop");
					control_door('D',"stop");

					delay_10us(2000);

					control_door('A',"down");
					control_door('B',"down");
					control_door('C',"down");
					control_door('D',"down");			 					
				}
				flag_quick_door=0;
				flag_display_screen=1;
			}

			delay_10us(10);			//ȷ������if(open_lock==0);

			if(open_lock==0)		//��ִֹ�к���ʱ  ��ʱ���ж�  �޷�ˢ�� display_init_screen0(); �����ú󣡣���
				display_init_screen_00();


		}	
	}

}








//	LCD12864_restart();
//
//	init_LCD12864_character();
//	display_character(0,0,"");
//	delay_10us(60000);
//	delay_10us(60000);
//	delay_10us(60000);
//	delay_10us(60000);
//
//	init_LCD12864_character();
//	write_cmd(0x01);
//	init_LCD12864_picture();
//	display_picture(picture0);
//
//	delay_10us(60000);
//	delay_10us(60000);
//	delay_10us(60000);
//	delay_10us(60000);
//
//	init_LCD12864_character();
//	display_character(0,0,"����");
//	display_character(3,0,"��");
//	init_LCD12864_picture();
//	display_picture(picture0);
//
//	delay_10us(60000);
//	delay_10us(60000);
//	delay_10us(60000);
//	delay_10us(60000);
//
//	init_LCD12864_character();
//	write_cmd(0x01);
//
//	init_LCD12864_picture();
//	Clear_picture();










