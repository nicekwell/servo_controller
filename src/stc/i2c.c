/*********************************************************************************************
�ļ�˵����
���ļ�������ʱ��������STC��1T��Ƭ��12MHz����Ƶ�ʡ�
��ʵ�ʲ����з��֣���������ʱ���ȶԽ�������Ӱ��ܴ�����������ø��ٵ�Ƭ���Ļ�����ҪҪ�ӳ���ʱʱ�䣻
/*********************************************************************************************/
#include <i2c.h>

static void i2c_delay()
{
//	unsigned char i;
//	for(i=50;i>0;i--);
}

static void i2c_ack(bit ack)	//Ӧ���źţ������պ����ã�����ÿ���յ�һ���ֽ�Ҫ��ӻ�����һλӦ���źţ��������һ���ֽڲ���ҪӦ��
{
//����˺���֮ǰi2c_scl�ǵ͵�ƽ
	i2c_sda=ack;
	i2c_delay();
	i2c_scl=1;
	i2c_delay();
	i2c_scl=0;
	i2c_delay();
}

static void i2c_start()	//��ʼ�ź�
{
//����˺���ǰi2c_scl��i2c_sdaӦ���Ǹߵ�ƽ
	i2c_sda=1;
	i2c_delay();
	i2c_scl=1;
	i2c_delay();i2c_delay();
	i2c_sda=0;
	i2c_delay();i2c_delay();
	i2c_scl=0;	//ǯסi2c_scl�����������豸�Ͳ���ռ��I2C������
	i2c_delay();
}//����start����i2c_scl�ǵ͵�ƽ���Է���i2c_sda��ʱ���Ա仯

static void i2c_stop()	//ֹͣ�ź�
{
//����˺���֮ǰi2c_sclӦΪ�͵�ƽ
	i2c_sda=0;
	i2c_delay();
	i2c_scl=1;
	i2c_delay();i2c_delay();i2c_delay();
	i2c_sda=1;
	i2c_delay();i2c_delay();
}//�������������i2c_scl��i2c_sda���Ǹߵ�ƽ�������ͷ���

/*��������һ���ֽڣ�����Ӧ���źţ�0��Ӧ��*/
/*����������ȴ��ӻ���Ӧ��λ������ӻ���Ӧ��Ļ�����ֻ�Ƿ���һ����Ӧ���ֵ1�����ǲ���ȴ�����������Ӧ�ٶȡ�
*/
static bit i2c_sendB(unsigned char dat)	//����I2CЭ�飬�ȷ���λ�󷢵�λ
{
	void delay5(unsigned char);
	bit ack;	//Ӧ���ź�
	unsigned char i;
	for(i=0;i<=7;i++)
	{
		if(dat&0x80)
			i2c_sda=1;
		else
			i2c_sda=0;
		dat<<=1;
		i2c_delay();
		i2c_scl=1;
		i2c_delay();
		i2c_scl=0;
	}		//���ݷ������ˣ�����Ҫ�ȴ�Ӧ���ź�
	i2c_delay();
	i2c_sda=1;	//�ͷ���������������Ӧ���ź�
	i2c_delay();
	i2c_scl=1;
	i2c_delay();
	if(dat)
		ack=1;
	else
		ack=0;
	i2c_scl=0;
	i2c_delay();
	/*------------------------------------
	�������Ҫ��һ����ʱ����Ϊ�����������ȴ��ӻ�����Ӧ���źţ�����ӻ��ڽ�����һ���ֽ����ݺ��н϶������Ҫ����Ļ���
	�������Ҫһ�����ʵ���ʱʱ����ӻ��������ݣ���Ȼ���ݷ���̫�죬�ӻ�æ������ ---------------------------------*/

	return ack;
}/*�����������i2c_scl�ǵ͵�ƽ��i2c_sda������ʱ�仯*/

/*��������һ���ֽڣ�����Ӧ���źţ�0��Ӧ��*/
/*��������ȴ��ӻ���Ӧ��λ�Ž������ֽڵĴ��䣬����ӻ���Ӧ��Ļ�������������ʱ���źţ�֪����⵽Ӧ���źŲŻ������������
�ӻ����յ�����֮������Ȳ���Ӧ���źţ����Ƕ����ݽ��д����ȴ�������֮�󷵻�һ��Ӧ���źŸ�������������������һ���ֽڡ�
������������Ӧ�ٶȡ�		*
void i2c_sendB(unsigned char dat)	//����I2CЭ�飬�ȷ���λ�󷢵�λ
{
	void delay5(unsigned char);
	bit ack;	//Ӧ���ź�
	unsigned char i;
	for(i=0;i<=7;i++)
	{
		if(dat&0x80)
			i2c_sda=1;
		else
			i2c_sda=0;
		dat<<=1;
		i2c_delay();
		i2c_scl=1;
		i2c_delay();
		i2c_scl=0;
	}		//���ݷ������ˣ�����Ҫ�ȴ�Ӧ���ź�
	i2c_delay();
	i2c_sda=1;	//�ͷ���������������Ӧ���ź�
	for(;;)	//�����ȵ�Ӧ���źţ��Ȳ�����һֱ��
	{
		i2c_delay();
		i2c_scl=1;
		i2c_delay();
		if(i2c_sda==0)	//˵��������Ӧ��
			break;
		i2c_scl=0;
	}
	i2c_scl=0;
	i2c_delay();
}/*�����������i2c_scl�ǵ͵�ƽ��i2c_sda������ʱ�仯*/

//�������յ���1���ֽ����ݣ��β���Ӧ���źţ�0��Ӧ��
static unsigned char i2c_recB(bit ack)	//�ȴ���λ���󴫵�λ
{
	unsigned char get=0;
	unsigned char i;
	i2c_sda=1;	//�ͷ�i2c_sda�Խ�������
	for(i=0;i<=7;i++)
	{
		i2c_delay();
		i2c_scl=1;
		i2c_delay();
		get<<=1;
		if(i2c_sda)
			get|=0x01;
		i2c_scl=0;
	}
	i2c_delay();
	i2c_ack(ack);	//�Դӻ�����Ӧ�𣬴�ʱ�ӻ�Ӧ���Ѿ��ͷ�sda��

	return get;
}
/*	������������I2C������
	����4���ռ������ڵ���ʱҲҪ��ѭ���򣬲����������������򵥡�
	����������һ��ʼҪ��i2c_scl��i2c_sda����1����I2C���߿��С�
	һ���豸��Ҫͨ��Ҫ�ȷ�����ʼ�źţ�Ȼ�������ݣ����Ͷ��ٸ����������ƣ�������ʼ�źź���ĵ�һ���ֽ����ݴ�����Ǵӻ���ַ�Ͷ�д�źš�
	������������ʱ���ӻ�ÿ���յ�һ���ֽھͻ������һ��Ӧ��λ��Ӧ��λ��i2c_sendB�������ء�
	������������ʱ������ÿ���յ�һ�����ݶ�Ӧ���ӻ�һ��Ӧ��λ��Ӧ��λ��Ϊ���պ���i2c_rec���β���ִ�н��ն���ʱͬʱָ���Ƿ�Ӧ��

	���������ݴ�����Ϻ���������ֹͣ�ź��ͷ�I2C���ߡ�
	�ڶ�һ���豸������ɺ���Ҫ����һ���豸���в������������Բ�����ֹͣ�źţ������ٷ���һ����ʼ�źţ��������ʼ�źź���������ڷ���һ���ӻ���ַ�Ͷ�д��Ϣ��
*/





/*
  ��ʼ + �ӻ���ַд + �ڲ���ַ��cmd + length������ + ����
  device_addr��7λ��i2c�ӻ���ַ��
  lengthָ����data�����ݳ��ȣ���������뷢��cmd_addr��ͽ�������lengthָΪ0���ɡ�
 */
void i2c_master_write(unsigned char device_addr, unsigned char cmd_addr, unsigned char *dat, unsigned char length)
{
    i2c_start();
    i2c_sendB(device_addr<<1);	//���λ��0����ʾд
    i2c_sendB(cmd_addr);	//����д���ӻ��ĵ�һ�ֽڣ�һ����Ϊ�ڲ���ַ����cmd
    while(length>0)
    {
	i2c_sendB(*dat);
	dat++;
	length--;
    }
    i2c_stop();
}

/*  ֻдһ�������д�����ݡ� */
void i2c_writeCmd(unsigned char device_addr, unsigned char cmd)
{
    i2c_master_write(device_addr, cmd, 0, 0);
}

/*  ���ڲ�ָ���Ĵ���дһ���ֽ����ݡ� */
void i2c_writeReg8(unsigned char device_addr, unsigned char cmd_addr, unsigned char dat)
{
    i2c_master_write(device_addr, cmd_addr, &dat, 1);
}

/*  ���ڲ�ָ���Ĵ���д2���ֽ����ݣ���8λ�ڵ͵�ַ����8λ�ڸߵ�ַ�� */
void i2c_writeReg16(unsigned char device_addr, unsigned char cmd_addr, unsigned int dat)
{
    unsigned char dat_temp[2];
    dat_temp[0] = dat;
    dat_temp[1] = dat>>8;
    i2c_master_write(device_addr, cmd_addr, dat_temp, 2);
}

/*
  ��ʼ + �ӻ���ַд + �ڲ���ַ��cmd + ��ʼ + �ӻ���ַ�� + ��ȡlength������ + ����
  ��ȡ�ӻ���ַָ���Ĵ�����ָ�����ȵ����ݡ�
  device_addr��7λ��i2c�ӻ���ַ��
  cmd_addr���ڲ���ַ����һ���ֽ���ʱҲ����Ϊ��cmd��
  ��ȡlength�����ݷŵ�datָ��ĵط�
 */
void i2c_master_read(unsigned char device_addr, unsigned char cmd_addr, unsigned char *dat, unsigned char length)
{
    i2c_start();
    i2c_sendB(device_addr<<1);	//�ӻ���ַ�����λ��0����ʾд
    i2c_sendB(cmd_addr);
    
    i2c_start();
    i2c_sendB(device_addr<<1 | 0x01);	//�ӻ���ַ�����λ��1����ʾ��
    while(length>1)	//��ȡ���ݣ��������һ���ֽ�
    {
	*dat = i2c_recB(0);
	dat++;
	length--;
    }
    //�����ȡ���һ���ֽڣ�������Ӧ��
    *dat = i2c_recB(1);
    i2c_stop();
}

/*  ��ȡ�ӻ���ַָ���Ĵ�����һ���ֽ����ݡ� */
unsigned char i2c_readReg8(unsigned char device_addr, unsigned char cmd_addr)
{
    unsigned char temp;
    i2c_master_read(device_addr, cmd_addr, &temp, 1);
    return temp;
}

/*
  ��ȡָ���Ĵ���2���ֽڣ�����һ��16λ�����ص�ַ�ڵ�8λ���ߵ�ַ�ڸ�8λ�� */
unsigned int i2c_readReg16(unsigned char device_addr, unsigned char cmd_addr)
{
    unsigned char temp[2];
    unsigned int temp16;
    i2c_master_read(device_addr, cmd_addr, temp, 2);
    temp16 = temp[1]<<8;
    temp16 |= temp[0];
    return temp16;
}


