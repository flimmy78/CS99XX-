
#include "test_tools.h"


/*
 * 函数名：soft_delay_us
 * 描述  ：软件阻塞延时10us 个数
 * 输入  ：dly_us 个数
 * 输出  ：无
 * 返回  ：无
 */
void cs_delay_us(int time)
{
	int i = 0;
	
	while(time--)
	{
		i = 10; //自己定义
		
		while(i--);
	}
}
void cs_delay_ms(int time)
{
	int i = 0;
	
	while(time--)
	{
		i = 12000;  //自己定义
		while(i--);
	}
}


/******************* (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
