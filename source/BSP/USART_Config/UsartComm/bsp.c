// #include "stm32f10x.h"
// #include "stm32f10x_conf.h"
// #include "comm_usart2.h"


// void bps_config(void)
// {
//     Usart2Config();
// }

// /*
//     ����    ��ȡ���������յ�������
//     ����    ����Ϊ�������ˣ����տͻ������͵�ָ�����ݣ��ӽ��ն��е�ͷ����ȡ�����ݵ�β�ڵ㣬
//                 �޸Ľ������ݻ�����е�ͷ���Ϊ��ǰβ�ڵ����һ���ڵ�
//     ����    ���ڽ������ݵ�ͷָ��
//     ����    �������ݵĸ���
// */
// uint8_t bspReadServerData(uint8_t * data)
// {
//     uint16_t i, j;

//     i = Usart2_front;
//     j = 0;
//     while(1) {
//         data[j] = UsartReceive_Data[i];
//         i = (i + 1) % BUFFER_SIZE_USART2;
//         j++;
//         
//         if(i == Usart2_rear)
//             break;
//     }
//     Usart2_front = Usart2_rear;
//     return j;
// }

// /*
//     ����    �������˷�������
// */
// void bspSendServerData(uint8_t * data, uint8_t data_num)
// {
//     Usart2_SendData((char *)data, data_num);
// }

// /*
//     ����    �ͻ��˷�������
// */
// void bspSendClientData(uint8_t * data, uint8_t data_num)
// {
//     Usart2_SendData((char *)data, data_num);
// }

// /*
//     ����    �ͻ��˽�������
// */
// uint8_t bspReadClientData(uint8_t * data)
// {
//     uint16_t i, j;

//     i = Usart2_front;
//     j = 0;
//     while(1) {
//         data[j] = UsartReceive_Data[i];
//         i = (i + 1) % BUFFER_SIZE_USART2;
//         j++;

//         if(i == Usart2_rear)
//             break;
//     }

//     return j;
// }







