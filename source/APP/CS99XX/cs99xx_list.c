/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�99xx_listS.c
 * ժ  Ҫ  ��������Ҫ�����ݽṹ������������ʹ��
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#define _CS99xxLIST_GLOBALS
#include "cs99xx_list.h"
#include "string.h"



void init_list(void)
{
	int8_t i;
	NODE_STEP	*first;
	NODE_STEP	*second;
    
	//���յĲ��Բ��س�ʼ��Ϊһ������������ͷ��free_list_steps   file_table[MAX_FILES];
	memset(steps_buf,0,sizeof steps_buf);
	
	for(i = 0; i < 5 - 1; i++)
	{
		first  = &steps_buf[i];
		second = &steps_buf[i+1];
		first->next = second;
	}
    
	first  = &steps_buf[5-1];
    
	first->next = (NODE_STEP*)0;
	
	free_list_head = &steps_buf[0];	//���ղ�����ͷָ��test_steps[0]
	
	cs99xx_list.head = NULL;
	cs99xx_list.tail = NULL;
	cs99xx_list.size = 0;
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
