#ifndef LCM_H
#define LCM_H

#define LCM_ADDR 0x78	//这是8为的写ID，不是7位地址
#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

void lcm_init();
void lcm_clean();//清屏

/*
  写入8*16字符，输入坐标和ascii码的字符串即可。
  坐标是字符的坐标，一个12864屏可容纳 16*4 个字符，坐标就是按照4行16列计算的。
 */
void lcm_write(unsigned char x, unsigned char y, unsigned char *chr);
#endif
