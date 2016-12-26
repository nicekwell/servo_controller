/*
本延时函数适用于1T单片机，24MHz晶振
*/

/******************************************
精确的us级延时，延时时间 3n-3 us
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
