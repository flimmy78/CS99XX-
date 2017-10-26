
#ifndef __MB_MASTER_H__
#define __MB_MASTER_H__

#ifndef NULL
#define NULL 0
#endif

#include "app.h"

#define DATA_MAX_LON  100

typedef enum{
    MB_MASETER_SENDING,/* ���ڷ��� */
    MB_MASETER_SEND_END,/* ���ͽ��� */
    MB_MASETER_SEND_BREAK,/* ��ֹ���Ͳ���ն��� */
}MB_MASTER_SEND_ST;/* MODBUS��������״̬ */

typedef struct{
    uint16_t data_f[DATA_MAX_LON];/* ���ݻ��� */
    uint8_t slave_addr;/* �ӻ���ַ */
    uint8_t read_addr;/* �����ĵ�ַ */
    uint8_t read_lon;/* �����ĳ��� ���ֵĸ��� = �ֽ���/2 */
    uint8_t write_addr;/* д��ĵ�ַ */
    uint8_t write_lon;/* д������ݳ��� ���ֵĸ��� = �ֽ���/2 */
    uint8_t type;/* ͨ�Ű��������ǹ㲥֡������ͨ֡ */
}MB_PACK_T;


#define QUEUE_DEEP    MDBUS_BUF_SIZE  /* ���е���� */
typedef struct{
    MB_PACK_T node[QUEUE_DEEP];
    uint32_t top;/* ���еĶ��� */
}QUEUE_T;


extern void mb_send_cmd(MB_PACK_T *mb_pack);
extern int32_t mb_query_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t* data, uint16_t data_len, uint16_t len);
extern int32_t mb_set_slave_info(uint8_t slave_addr, uint16_t cmd, uint16_t *data, uint16_t len);


#endif //__MB_MASTER_H__
