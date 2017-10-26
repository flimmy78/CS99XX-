
#include "stm32f10x.h"
#include "app.h"
#include "mb_master.h"
#include "mb_cmd_define.h"
#include "mb.h"
#include "mb_m.h"


QUEUE_T send_queue;/* ���Ͷ��� */

/*
 * ��������mb_send_cmd
 * ����  ��modbus�������ͺ���
 * ����  ��mb_pack ��������֡��
 * ���  ����
 * ����  ����
 */
void mb_send_cmd(MB_PACK_T *mb_pack)
{
    if(mb_pack == NULL)
    {
        return;
    }
    
    eMBMasterReqReadWriteMultipleHoldingRegister(mb_pack->slave_addr,
                           mb_pack->read_addr, mb_pack->read_lon,
                           mb_pack->data_f,
                           mb_pack->write_addr, mb_pack->write_lon);
//     OSTimeDlyResume(MODBUS_SEND_TASK_PRIO);
}

/*
 * ��������mb_query_slave_info
 * ����  ��modbus��������ѯ�ʴӻ���Ϣָ���������֡����modbus�ķ��Ͷ�����
 * ����  ��slave_addr �ӻ���ַ
 *          cmd ����
 *          data �����ӻ�������
 *          data_len �����ӻ����ݳ���
 *          len ѯ�ʴӻ����ݵĳ̶� -2
 * ���  ����
 * ����  ����
 */
int32_t mb_query_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t* data, uint16_t data_len, uint16_t len)
{
    uint16_t i = 0;
    uint32_t j = 0;
    MB_PACK_T  *mb_pack;
    uint8_t type;
    uint8_t err = 0;
    
    OSMutexPend(MdbusMutex, 0, &err);
    mb_pack = &send_queue.node[send_queue.top];
    send_queue.top = (send_queue.top + 1) % QUEUE_DEEP;
    
    if(slave_addr == 0)
    {
        type = MB_NO_WAIT_ACK;
    }
    else
    {
        type = MB_YES_WAIT_ACK;
    }
    
    mb_pack->slave_addr = slave_addr;
    mb_pack->read_addr = 0;
    mb_pack->read_lon = len + 2;
    mb_pack->write_addr = 0;
    mb_pack->write_lon = data_len + 2;
    mb_pack->type = type;
    
    mb_pack->data_f[i++] = cmd;
    mb_pack->data_f[i++] = MB_READ_PAR;
    
    for(j = 0; j < data_len; j++)
    {
        mb_pack->data_f[i++] = data[j];
    }
    
    OSQPost(MdbusQSem, mb_pack);
	OSMutexPost(MdbusMutex);
    
    return 0;
}


/*
 * ��������mb_set_slave_info
 * ����  ��modbus�����������ôӻ���Ϣָ�����֡���ݷ���modbus�ķ��Ͷ�����
 * ����  ��slave_addr �ӻ���ַ
 *          cmd ����
 *          data �����ӻ�������
 *          data_len �����ӻ����ݳ���
 *          len ѯ�ʴӻ����ݵĳ̶� -2
 * ���  ����
 * ����  ����
 */
int32_t mb_set_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t *data, uint16_t len)
{
    uint16_t i = 0;
    uint32_t j = 0;
    MB_PACK_T  *mb_pack;
    uint8_t type;
    uint8_t err = 0;
    
    OSMutexPend(MdbusMutex, 0, &err);
    mb_pack = &send_queue.node[send_queue.top];
    send_queue.top = (send_queue.top + 1) % QUEUE_DEEP;
    
    if(slave_addr == 0)
    {
        type = MB_NO_WAIT_ACK;
    }
    else
    {
        type = MB_YES_WAIT_ACK;
    }
    
    mb_pack->slave_addr = slave_addr;
    mb_pack->read_addr = 0;
    mb_pack->read_lon = 3;
    mb_pack->write_addr = 0;
    mb_pack->write_lon = len + 2;
    mb_pack->type = type;
    
    mb_pack->data_f[i++] = cmd;
    mb_pack->data_f[i++] = MB_SET_PAR;
    
    for(j = 0; j < len; j++)
    {
        mb_pack->data_f[i++] = data[j];
    }
    
    OSQPost(MdbusQSem, mb_pack);
	OSMutexPost(MdbusMutex);
    
    return 0;
}


/******************* (C) COPYRIGHT 2016 ��ʢ���� *****END OF FILE*******************/


