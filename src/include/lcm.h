#ifndef LCM_H
#define LCM_H

#define LCM_ADDR 0x78
#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
#define high 1
#define low 0

void lcm_init();
void lcm_clean();//����
void lcm_write(unsigned char x, unsigned char y, unsigned char *chr);//д��8*16�ַ������������ascii����ַ�������
#endif
