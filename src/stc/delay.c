/*
����ʱ����������1T��Ƭ����24MHz����
*/

/******************************************
��ȷ��us����ʱ����ʱʱ�� 3n-3 us
******************************************/
void delayus(unsigned char n)
{
	while(--n);
}

void delayms(unsigned char n)
{
	unsigned int i;
	for(;n>0;n--)
		for(i=940*2;i>0;i--);
}

void delay5(unsigned char n)
{
	unsigned int i;
	for(;n>0;n--)
		for(i=4700*2;i>0;i--);
}
