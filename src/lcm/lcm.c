#include <lcm.h>
#include <i2c.h>
#include "codetab.c"

/*这两个函数和I2C接口相关。*/
static void lcm_writeDat(unsigned char dat)//往lcm写入一个字节数据
{
    i2c_writeReg8(LCM_ADDR>>1, 0x40, dat);	//0x40表示后面写入的是数据
}
static void lcm_writeCmd(unsigned char cmd)//往lcm写入命令
{
    i2c_writeReg8(LCM_ADDR>>1, 0x00, cmd);	//0x00表示后面写入的是命令
}

static void lcm_setPos(unsigned char x,unsigned char y)//设置坐标
{
	lcm_writeCmd(0xb0+y);
	lcm_writeCmd(((x&0xf0)>>4)|0x10);
	lcm_writeCmd((x&0x0f)|0x01);
}

void lcm_clean()//清屏
{
	unsigned char x,y;
	for(y=0;y<8;y++)
	{
		lcm_writeCmd(0xb0+y);
		lcm_writeCmd(0x01);
		lcm_writeCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			lcm_writeDat(0);
	}
}

void lcm_init()
{
	lcm_writeCmd(0xae);//--turn off oled panel
	lcm_writeCmd(0x00);//---set low column address
	lcm_writeCmd(0x10);//---set high column address
	lcm_writeCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	lcm_writeCmd(0x81);//--set contrast control register
	lcm_writeCmd(Brightness); // Set SEG Output Current Brightness
	lcm_writeCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	lcm_writeCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	lcm_writeCmd(0xa6);//--set normal display
	lcm_writeCmd(0xa8);//--set multiplex ratio(1 to 64)
	lcm_writeCmd(0x3f);//--1/64 duty
	lcm_writeCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	lcm_writeCmd(0x00);//-not offset
	lcm_writeCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	lcm_writeCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	lcm_writeCmd(0xd9);//--set pre-charge period
	lcm_writeCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	lcm_writeCmd(0xda);//--set com pins hardware configuration
	lcm_writeCmd(0x12);
	lcm_writeCmd(0xdb);//--set vcomh
	lcm_writeCmd(0x40);//Set VCOM Deselect Level
	lcm_writeCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	lcm_writeCmd(0x02);//
	lcm_writeCmd(0x8d);//--set Charge Pump enable/disable
	lcm_writeCmd(0x14);//--set(0x10) disable
	lcm_writeCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	lcm_writeCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	lcm_writeCmd(0xaf);//--turn on oled panel
	lcm_clean();
	lcm_setPos(0,0);
}

/*
  写入8*16字符，输入坐标和ascii码的字符串即可。
  坐标是字符的坐标，一个12864屏可容纳 16*4 个字符，坐标就是按照4行16列计算的。
 */
void lcm_write(unsigned char px, unsigned char py, unsigned char *chr)
{
	unsigned char c=0,i=0,j=0;
	unsigned char x, y;
	x = px*8;
	y = py*2;
	while(chr[j] != '\0')
	{
		c=chr[j]-32;
		if(x>120)
		{
			x=0;
			y++;
		}
		lcm_setPos(x,y);
		for(i=0;i<8;i++)
			lcm_writeDat(F8X16[c*16+i]);
		lcm_setPos(x,y+1);
		for(i=0;i<8;i++)
			lcm_writeDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
