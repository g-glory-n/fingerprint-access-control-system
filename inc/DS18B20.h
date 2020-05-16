#ifndef __DS18B20_h__
#define __DS18B20_h__

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit DSPORT=P3^2;


//--声明全局函数--//
extern uchar ds18b20_init();

extern void ds18b20_write_byte(uchar com);
extern uchar ds18b20_read_byte();

extern void  ds18b20_chang_temp_cmd();
extern void  ds18b20_read_temp_cmd();
extern int ds18b20_read_temp();

extern float ds18b20_count_temperature(void);



#endif
