#include <12C5A60S2.h>
#include <delay.h>
#include <lcm.h>

sbit test = P2^7;

sbit ch1 = P1^6;
sbit ch2 = P1^5;
sbit ch3 = P1^4;
sbit ch4 = P1^3;
sbit ch5 = P1^2;
sbit ch6 = P1^1;
sbit ch7 = P1^0;

sbit led1 = P0^6;
sbit led2 = P0^5;
sbit led3 = P0^4;
sbit led4 = P0^3;
sbit led5 = P0^2;
sbit led6 = P0^1;
sbit led7 = P0^0;

sbit key_ch1 = P2^0;
sbit key_ch2 = P2^1;
sbit key_ch3 = P2^2;
sbit key_ch4 = P2^3;
sbit key_ch5 = P2^4;
sbit key_ch6 = P2^5;
sbit key_ch7 = P2^6;
sbit key_m100 = P3^7;
sbit key_m10 = P3^6;
sbit key_reset = P3^5;
sbit key_p10 = P3^4;
sbit key_p100 = P3^3;

unsigned char mk[7]={150, 150, 150, 150, 150, 150, 150};	//保存7个通道的脉宽，单位是10us
unsigned char focus = 1;	//当前焦点通道，1~7

unsigned char disp_tick = 0;	//定时器通过此变量通知主循环进行显示更新

void task_key1()	//5ms调用一次
{
    static bit key_ch1_last=1, key_ch2_last=1, key_ch3_last=1, key_ch4_last=1, key_ch5_last=1, key_ch6_last=1, key_ch7_last=1;
    if(key_ch1_last == 1 && key_ch1 ==0)	{focus = 1;}
    else if(key_ch2_last == 1 && key_ch2 ==0)	{focus = 2;}
    else if(key_ch3_last == 1 && key_ch3 ==0)	{focus = 3;}
    else if(key_ch4_last == 1 && key_ch4 ==0)	{focus = 4;}
    else if(key_ch5_last == 1 && key_ch5 ==0)	{focus = 5;}
    else if(key_ch6_last == 1 && key_ch6 ==0)	{focus = 6;}
    else if(key_ch7_last == 1 && key_ch7 ==0)	{focus = 7;}
    key_ch1_last = key_ch1;
    key_ch2_last = key_ch2;
    key_ch3_last = key_ch3;
    key_ch4_last = key_ch4;
    key_ch5_last = key_ch5;
    key_ch6_last = key_ch6;
    key_ch7_last = key_ch7;
}
void task_key2()	//5ms一次
{
    static bit key_m100_last=1, key_m10_last=1, key_reset_last=1;
    static unsigned char key_m10_continue_num = 0;

    if(key_m100_last == 1 && key_m100 == 0)
    {
	if(mk[focus-1]>=10) mk[focus-1]-=10;	
    }
    if(key_reset_last == 1 && key_reset == 0)
    {
	mk[focus-1] = 150;
    }

    if(key_m10 ==0)
    {
	if(key_m10_last == 1)
	{
	    if(mk[focus-1]!=0)	mk[focus-1]--;
	}
	else
	{
	    if(key_m10_continue_num == 80)	//连按等待时间
	    {
		static unsigned char continue_num=0;
		if(continue_num == 0)
		{
		    if(mk[focus-1]!=0) mk[focus-1]--;
		}
		continue_num++;
		if(continue_num == 20)	//连按增加间隔时间
		    continue_num =0;
	    }
	    else
	    {
		key_m10_continue_num++;
	    }
	}
    }
    else
	key_m10_continue_num=0;
    
    key_m100_last = key_m100;
    key_m10_last = key_m10;
    key_reset_last = key_reset;
}
void task_key3()
{
    static bit key_p10_last=1, key_p100_last=1;
    static unsigned char key_p10_continue_num = 0;
    
    if(key_p100_last == 1 && key_p100 == 0)
    {
	if(mk[focus-1]<=245) mk[focus-1]+=10;
    }
	
    if(key_p10 ==0)
    {
	if(key_p10_last == 1)
	{
	    if(mk[focus-1]!=255) mk[focus-1]++;
	}
	else
	{
	    if(key_p10_continue_num == 80)	//连按等待时间
	    {
		static unsigned char continue_num=0;
		if(continue_num == 0)
		{
		    if(mk[focus-1]!=255) mk[focus-1]++;
		}
		continue_num++;
		if(continue_num == 20)	//连按增加间隔时间
		    continue_num =0;
	    }
	    else
	    {
		key_p10_continue_num++;
	    }
	}
    }
    else
	key_p10_continue_num=0;
    
    key_p10_last = key_p10;
    key_p100_last = key_p100;
}

void t0 () interrupt 1	//10us 一次
{
    test = 1;
    //先进行本次脉宽和周期判断
    {
	static int num_mk = 0;
	num_mk ++;
	if(num_mk == mk[0]) ch1 = 0;
	if(num_mk == mk[1]) ch2 = 0;
	if(num_mk == mk[2]) ch3 = 0;
	if(num_mk == mk[3]) ch4 = 0;
	if(num_mk == mk[4]) ch5 = 0;
	if(num_mk == mk[5]) ch6 = 0;
	if(num_mk == mk[6]) ch7 = 0;
	if(num_mk == 2000)	//周期结束
	{
	    num_mk = 0;
	    ch1 = 1;
	    ch2 = 1;
	    ch3 = 1;
	    ch4 = 1;
	    ch5 = 1;
	    ch6 = 1;
	    ch7 = 1;
	}
    }
    //定时任务分配
    {
	static int num1 = 0;
	num1 ++;
	/*按键扫描由于涉及到连按操作，处理比较复杂，为了保证每次都能在10us以内处理完，把按键扫描分成3份，在不同的时间段内进行。*/
	if(num1 == 500)	//5ms一次，进行按键扫描
	{
	    num1 = 0;
	    task_key1();
	}
	else if(num1 == 100)	//5ms一次，进行按键扫描
	    task_key2();
	else if(num1 == 150)	//5ms一次，进行按键扫描
	    task_key3();
	else if(num1 == 200)
	{
	    static unsigned char num2 = 0;
	    num2++;
	    if(num2 == 20)	//进行显示刷新
	    {
		num2 = 0;
		disp_tick = 1;
	    }
	}
    }
    test =0;
}

void main()
{
    // 初始化中断
    ET0 = 1;
    EA = 1;
    
    // 初始化T0，中断周期10us
    AUXR &= 0x7F;		//定时器0工作在12T模式
    TMOD &= 0xF0; TMOD |= 0x02;	//定时器0工作在8为自动填装模式
    TL0 = 236;
    TH0 = 236;
    TF0 = 0;		//清除TF0标志
    TR0 = 1;		//定时器0开始计时

    //初始化IO，P0.0~P0.6 推挽输出
    P0M0=0x7F;
    P0M1=0x00;
    P0=0x00;

    lcm_init();
    delay5(10);
    lcm_clean();
    delay5(10);

    while(1)
    {
	if(disp_tick)	//50ms调用一次
	{
	    unsigned char str[9];
	    unsigned char i;
	    disp_tick = 0;

	    P0 &= 0x80;
	    P0 |= 0x01<<(7-focus);	//点亮当前focus的led
	    for(i=0; i<7; i++)
	    {
		if(i+1 == focus)
		    str[0]='>';
		else
		    str[0]=' ';
		str[1] = 0x30+i+1;
		str[2] = '-';
		str[3] = 0x30 + mk[i]/100;
		str[4] = 0x30 + (mk[i]%100)/10;
		str[5] = 0x30 + mk[i]%10;
		str[6] = '0';
		str[7] = ' ';
		str[8] = '\0';
		lcm_write((i%2)*8, i/2, str);
	    }
	}
    }
}
