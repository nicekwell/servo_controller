/*********************************************************************************************
文件说明：
本文件所有延时函数基于STC的1T单片机12MHz晶振频率。
在实际测试中发现，主机的延时长度对接收数据影响很大，如果主机换用高速单片机的话，需要要延长延时时间；
/*********************************************************************************************/
#include <12C5A60S2.h>
sbit iic_scl=P1^7;
sbit iic_sda=P2^7;

void iic_delay()
{
//	unsigned char i;
//	for(i=50;i>0;i--);
}

void iic_ack(bit ack)	//应答信号，给接收函数用，主机每接收到一个字节要向从机发送一位应答信号，但是最后一个字节不需要应答
{
//进入此函数之前iic_scl是低电平
	iic_sda=ack;
	iic_delay();
	iic_scl=1;
	iic_delay();
	iic_scl=0;
	iic_delay();
}

void iic_start()	//起始信号
{
//进入此函数前iic_scl和iic_sda应都是高电平
	iic_sda=1;
	iic_delay();
	iic_scl=1;
	iic_delay();iic_delay();
	iic_sda=0;
	iic_delay();iic_delay();
	iic_scl=0;	//钳住iic_scl，这样其它设备就不会占用IIC总线了
	iic_delay();
}//出了start函数iic_scl是低电平，以方便iic_sda随时可以变化

void iic_stop()	//停止信号
{
//进入此函数之前iic_scl应为低电平
	iic_sda=0;
	iic_delay();
	iic_scl=1;
	iic_delay();iic_delay();iic_delay();
	iic_sda=1;
	iic_delay();iic_delay();
}//出了这个函数后iic_scl和iic_sda都是高电平，都被释放了

/*主机发送一个字节，返回应答信号，0是应答。*/
/*本函数不会等待从机的应答位，如果从机不应答的话这里只是返回一个不应答的值1，但是不会等待，不能自适应速度。
*/
bit iic_sendB(unsigned char dat)	//根据IIC协议，先发高位后发低位
{
	void delay5(unsigned char);
	bit ack;	//应答信号
	unsigned char i;
	for(i=0;i<=7;i++)
	{
		if(dat&0x80)
			iic_sda=1;
		else
			iic_sda=0;
		dat<<=1;
		iic_delay();
		iic_scl=1;
		iic_delay();
		iic_scl=0;
	}		//数据发送完了，下面要等待应答信号
	iic_delay();
	iic_sda=1;	//释放数据线用来接收应答信号
	iic_delay();
	iic_scl=1;
	iic_delay();
	if(dat)
		ack=1;
	else
		ack=0;
	iic_scl=0;
	iic_delay();
	/*------------------------------------
	这里可能要有一点延时，因为这个函数不会等待从机发送应答信号，如果从机在接受完一个字节数据后有较多的事情要处理的话，
	这里就需要一个合适的延时时间给从机处理数据，不然数据发送太快，从机忙不过来 ---------------------------------*/

	return ack;
}/*出了这个函数iic_scl是低电平，iic_sda可以随时变化*/

/*主机发送一个字节，返回应答信号，0是应答。*/
/*本函数会等待从机的应答位才结束本字节的传输，如果从机不应答的话这里会持续产生时钟信号，知道检测到应答信号才会结束本函数，
从机接收到数据之后可以先不发应答信号，而是对数据进行处理，等处理完了之后返回一个应答信号给主机，主机将发送下一个字节。
这样可以自适应速度。		*
void iic_sendB(unsigned char dat)	//根据IIC协议，先发高位后发低位
{
	void delay5(unsigned char);
	bit ack;	//应答信号
	unsigned char i;
	for(i=0;i<=7;i++)
	{
		if(dat&0x80)
			iic_sda=1;
		else
			iic_sda=0;
		dat<<=1;
		iic_delay();
		iic_scl=1;
		iic_delay();
		iic_scl=0;
	}		//数据发送完了，下面要等待应答信号
	iic_delay();
	iic_sda=1;	//释放数据线用来接收应答信号
	for(;;)	//誓死等到应答信号，等不到就一直等
	{
		iic_delay();
		iic_scl=1;
		iic_delay();
		if(iic_sda==0)	//说明进行了应答
			break;
		iic_scl=0;
	}
	iic_scl=0;
	iic_delay();
}/*出了这个函数iic_scl是低电平，iic_sda可以随时变化*/

//主机接收到的1个字节数据，形参是应答信号，0是应答
unsigned char iic_recB(bit ack)	//先传高位，后传低位
{
	unsigned char get=0;
	unsigned char i;
	iic_sda=1;	//释放iic_sda以接收数据
	for(i=0;i<=7;i++)
	{
		iic_delay();
		iic_scl=1;
		iic_delay();
		get<<=1;
		if(iic_sda)
			get|=0x01;
		iic_scl=0;
	}
	iic_delay();
	iic_ack(ack);	//对从机进行应答，此时从机应该已经释放sda了

	return get;
}
/*	本程序适用于IIC主机。
	上面4个终极函数在调用时也要遵循规则，不像其它例程那样简单。
	在整个程序一开始要让iic_scl和iic_sda都置1，让IIC总线空闲。
	一个设备想要通信要先发出起始信号，然后发送数据，发送多少个数据无限制，但是起始信号后面的第一个字节数据代表的是从机地址和读写信号。
	主机发送数据时，从机每接收到一个字节就会给主机一个应答位，应答位由iic_sendB函数返回。
	主机接收数据时，主机每接收到一个数据都应给从机一个应答位，应答位作为接收函数iic_rec的形参在执行接收动作时同时指定是否应答。

	在所有数据传送完毕后主机发送停止信号释放IIC总线。
	在对一个设备操作完成后想要对另一个设备进行操作，主机可以不发送停止信号，而是再发送一个起始信号，在这个起始信号后面紧接着在发送一个从机地址和读写信息。
*/
