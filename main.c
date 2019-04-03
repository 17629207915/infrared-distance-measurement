#include"reg51.h"
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

//定义adc模块端口
#define GPIO_DIG P1
sbit CS = P1^2;
sbit DI = P1^3;
sbit DO = P1^3;
sbit Clk = P1^4;

//定义1602连接端口
#define DATA P0
#define Busy 0x80
sbit RS = P3^0;
sbit RW = P3^1;
sbit E  = P3^2;
uint D;

//定义延迟函数
void delayms(char ms);
//LCD1602忙判断
char is_busy();
//定义写命令函数(1602=>stc89c52)
void write_cmd(unsigned char cmd);
//定义写数据函数(1602=>stc89c52)
void write_data(unsigned char dat);
//定义1602写数据行选择函数
void pos(unsigned char pos);
//定义ADC模数转换函数
uchar ADC0832(uchar CH);


main()
{  
	write_cmd(0x38);
	delayms(1);
	write_cmd(0x0c);
	delayms(1);
	write_cmd(0x06);
	delayms(1);
	write_cmd(0x01);
	delayms(1);
	
	pos(0);
	write_data(' ');
	write_data(' ');
	write_data('V');
	write_data('L');
	write_data('T');
	write_data('A');
	write_data('G');
	write_data('E');
	write_data(':');
	while(1)
	{
		D =  ADC0832(0); 
		pos(0x40);
		write_data(' ');
		write_data(' ');
		write_data(D/1000%10+0x30);
		write_data(D/100%10+0x30);
		write_data(D/10%10+0x30);
		write_data(D%10+0x30);
		delayms(100);
	}
}


uchar ADC0832(uchar CH)
{
	uchar i,dis0,dis1;
	Clk=0;
	DI=1;
		
	_nop_();
	CS=0;
	_nop_();
	Clk=1;
	_nop_();
		
	if(CH==0)
	{
		Clk=0;
		DI=1;
		_nop_();
		Clk=1;
		_nop_();
		Clk=0;
		DI=0;
		_nop_();
		Clk=1;
		_nop_();	
	}
	else
	{
		Clk=0;
		DI=1;
		_nop_();
		Clk=1;
		_nop_();
		Clk=0;
		DI=1;
		_nop_();
		Clk=1;
		_nop_();
	}
	Clk=0;
	DI=1;
	
	for(i=0;i<8;i++)
	{
		_nop_();
		dis0<<=1;
		Clk=1;
		_nop_();
		Clk=0;
		if(DO)
			dis0|=0x01;
		else
			dis0|=0x00;
	}
	for(i=0;i<8;i++)
	{
		dis1>>=1;
		if(DO)
			dis1|=0x80;
		else
			dis1|=0x00;
		_nop_();
		Clk=1;
		_nop_();
		Clk=0;
	}
	if(dis0==dis1)
		D=dis0;
	_nop_();
	CS=1;
	DO=1;
	Clk=1;
	return D;
}


void pos(unsigned char pos)
{
	write_cmd(pos|0x80);
}

void write_data(unsigned char dat)
{
	while(is_busy());
	RS = 1;
	RW = 0;
	E  = 0;
	DATA = dat;
	_nop_();
	E = 1;
	E = 0;
}

void write_cmd(unsigned char cmd)
{
	while(is_busy());
	RS = 0;
	RW = 0;
	E  = 0;
	DATA = cmd;
	_nop_();
	E = 1;
	E = 0;
}


char is_busy()
{
	int a=0;
	char result;
	RS = 0;
	RW = 1;
	E  = 1;
	_nop_();
	result = (1<<7 & DATA);
	E = 0;
	return result;
}


void delayms(char ms)
{
	while(ms--)
	{
		unsigned char i, j;
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 
		while (--i);
	}
}

