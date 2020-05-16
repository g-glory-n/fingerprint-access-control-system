#include "ZW_SZM201.h"
#include "main.h"

#include "string.h"
#include "intrins.h"
#include "reg52.h"
#include "stdio.h"





void ZW_SZW201_cmd (uchar parameter_1,uchar parameter_2,uchar parameter_3,uchar *cmd)	   //parameter_1用户号高八位;parameter_2用户号低八位;parameter_3权限
{
	uchar CHK=0;									//CHK为2-6数据字节异或值
	if(strcmp(cmd,"compare")==0)
	{
		send_one_byte(0xf5);
		send_one_byte(0x0c);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x0c);
		send_one_byte(0xf5);
	}
	if(strcmp(cmd,"sleep")==0)
	{
		send_one_byte(0xf5);
		send_one_byte(0x2c);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x2c);
		send_one_byte(0xf5);
	}
	
		
	if(strcmp(cmd,"add_1")==0)
	{
		CHK=0x01^parameter_1^parameter_2^parameter_3;
		send_one_byte(0xf5);
		send_one_byte(0x01);
		send_one_byte(parameter_1);
		send_one_byte(parameter_2);
		send_one_byte(parameter_3);
		send_one_byte(0x00);
		send_one_byte(CHK);
		send_one_byte(0xf5);
	}
	if(strcmp(cmd,"add_2")==0)
	{
		CHK=0x02^parameter_1^parameter_2^parameter_3;
		send_one_byte(0xf5);
		send_one_byte(0x02);
		send_one_byte(parameter_1);
		send_one_byte(parameter_2);
		send_one_byte(parameter_3);
		send_one_byte(0x00);
		send_one_byte(CHK);
		send_one_byte(0xf5);
	}	
	if(strcmp(cmd,"add_3")==0)
	{
		CHK=0x03^parameter_1^parameter_2^parameter_3;
		send_one_byte(0xf5);
		send_one_byte(0x03);
		send_one_byte(parameter_1);
		send_one_byte(parameter_2);
		send_one_byte(parameter_3);
		send_one_byte(0x00);
		send_one_byte(CHK);
		send_one_byte(0xf5);
	}	
	if(strcmp(cmd,"delete")==0)
	{
		CHK=0x04^parameter_1^parameter_2;
		send_one_byte(0xf5);
		send_one_byte(0x04);
		send_one_byte(parameter_1);
		send_one_byte(parameter_2);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(CHK);
		send_one_byte(0xf5);
	}
	if(strcmp(cmd,"delete_all")==0)
	{
		send_one_byte(0xf5);
		send_one_byte(0x05);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(0x05);
		send_one_byte(0xf5);
	}
	if(strcmp(cmd,"security_level")==0)
	{
		CHK=0x28^parameter_1;
		send_one_byte(0xf5);
		send_one_byte(0x28);
		send_one_byte(0x00);
		send_one_byte(parameter_1);		//parameter_1新的比对等级;0-9级;9级最高;默认为5级
		send_one_byte(0x00);
		send_one_byte(0x00);
		send_one_byte(CHK);
		send_one_byte(0xf5);
	}
		
	
}




































