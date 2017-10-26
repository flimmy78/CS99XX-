#include "Com_Send_Ctrl.h"
#include "USART.h"

#include "cs99xx_config.h"
#include "usart2.h"

#if SYNCHRO_ROADS_EN>0

#define     QUEUE_SIZE      ( 2 )

struct { 
    uint8_t Send_Data[64];
    uint8_t end_place ;
}send_msg[QUEUE_SIZE];

static uint8_t head;        //����ָ��
static uint8_t rear;        //��βָ��

static void connect_end_data(void)
{
    send_msg[rear].Send_Data[send_msg[rear].end_place ++] = over_data[0];
    send_msg[rear].Send_Data[send_msg[rear].end_place ++] = over_data[1];
}

static void connect_check_data(void)
{
    uint16_t temp_check_data;
   
    temp_check_data = read_check_data(send_msg[rear].Send_Data, send_msg[rear].end_place);
   
    send_msg[rear].Send_Data[send_msg[rear].end_place] =  temp_check_data & 0xff; 
	
    send_msg[rear].end_place ++;
	
    send_msg[rear].Send_Data[send_msg[rear].end_place] = (temp_check_data >> 8) & 0xff;
	
    send_msg[rear].end_place ++;
}

void Send_Data(void)
{
    /*ͷ��㲻����β�ڵ�*/
    if(rear != head)
	{
        Com_Client_PutStr(send_msg[head].Send_Data, send_msg[head].end_place) ;
    }
}


void Write_Cmd(CTRL_CMD_LIST cmd)
{
    send_msg[rear].Send_Data[CMD_PLACE] = (uint8_t)cmd;
    send_msg[rear].end_place = CMD_PLACE + 1;
}

void Send_Dispose(void)
{
    connect_check_data();
    connect_end_data();
	
    /*����д�������β�ڵ����һλ*/
    rear = (rear+1) % QUEUE_SIZE;
    Send_Data();
}

void Send_Rear_Dispose(void)
{
    connect_check_data();
    connect_end_data();
	
    /*����д�������β�ڵ����һλ*/
	head = rear;
    rear = (rear+1) % QUEUE_SIZE;
    Send_Data();
}


/*ͨ��û�г����쳣*/
void Comm_OK(void)
{
    /*����ͷ������һλ*/
    head = (head+1) % QUEUE_SIZE;
	
    Send_Data();
}


void Write_char_data(uint8_t *data, uint8_t num)
{
    uint8_t i;
    
    for(i = 0; i < num; i++) 
	{
        send_msg[rear].Send_Data[send_msg[rear].end_place + i] = data[i];
    }
	
    send_msg[rear].end_place += num;
}
#endif

