#ifndef LCM_H
#define LCM_H

#define LCM_ADDR 0x78
#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
#define high 1
#define low 0

void lcm_init();
void lcm_clean();//清屏
void lcm_write(unsigned char x, unsigned char y, unsigned char *chr);//写入8*16字符，输入坐标和ascii码的字符串即可
#endif
