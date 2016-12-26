#ifndef LCM_H
#define LCM_H

#define LCM_ADDR 0x78	//����8Ϊ��дID������7λ��ַ
#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

void lcm_init();
void lcm_clean();//����

/*
  д��8*16�ַ������������ascii����ַ������ɡ�
  �������ַ������꣬һ��12864�������� 16*4 ���ַ���������ǰ���4��16�м���ġ�
 */
void lcm_write(unsigned char x, unsigned char y, unsigned char *chr);
#endif
