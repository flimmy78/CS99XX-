#ifndef _SPI_CPLD_h
#define _CPI_CPLD_h

#include "stm32f10x.h"

/* ���� */
#define	CPLD_W					0X01000000	/* д */

#define CPLD_SET_FREQUENCY		0x01000000 /* �������Ƶ�� ���е�16λΪƵ�ʿ����� */
#define CPLD_START_SINEWAVE		0x01020066 /* �������Ҳ���� */
#define CPLD_STOP_SINEWAVE		0x01020000 /* ֹͣ���Ҳ���� */


#define CPLD_START				0X0101AAAA 	/* ���������� */
#define CPLD_STOP				0X01015555	/* ֹͣ���� */
#define	CPLD_READ_PHASE			0X02000000 	/* ����λ�� */
#define CPLD_VOL_CUNT			0x02010000	/* ����������ֵ */
#define CPLD_R_COUNT			0x02010000	/* ����ѹ���μ��� */
#define CPLD_COMM_CHECK			0x02020000	/* ��У��ֵ��ͨ�ż��ʹ�� ���� 0x5555��ʾͨ������ */

#define COUNT_SIN_FREQ_WORD(F)	((F)*pow(2, 25)/pow(10, 6)) /* ����Ƶ�ʹؼ��� */

/**************************��·��ѹ�����*********************************************/
#define CMD_CPLD_HV_H				0X01000000  /* �߶˿��� */
#define CMD_CPLD_HV_L				0X01010000  /* �Ͷ˿��� */
/* ״̬���� */
#define HV_PORT_H           (0)
#define HV_PORT_L           (1)

#define CPLD_HV_H1           ((uint8_t)(0))
#define CPLD_HV_H2           ((uint8_t)(1))
#define CPLD_HV_H3           ((uint8_t)(2))
#define CPLD_HV_H4           ((uint8_t)(3))
#define CPLD_HV_H5           ((uint8_t)(4))
#define CPLD_HV_H6           ((uint8_t)(5))
#define CPLD_HV_H7           ((uint8_t)(6))
#define CPLD_HV_H8           ((uint8_t)(7))

#define CPLD_HV_L1           ((uint8_t)(0))
#define CPLD_HV_L2           ((uint8_t)(1))
#define CPLD_HV_L3           ((uint8_t)(2))
#define CPLD_HV_L4           ((uint8_t)(3))
#define CPLD_HV_L5           ((uint8_t)(4))
#define CPLD_HV_L6           ((uint8_t)(5))
#define CPLD_HV_L7           ((uint8_t)(6))
#define CPLD_HV_L8           ((uint8_t)(7))
/*************************************************************************************/

extern void init_spi_cpld(void);

extern void reset_posrts_cpld_ctrl(void);
extern void cpld_write(u32 SendData);
extern u16 ReadDataFromCPLD(u32 SendData);

extern void hv_cpld_write(u32 SendData);
extern void cmd_cpld_hv_port(uint8_t ID, uint8_t bits, uint8_t status);
extern void cpld_count_angle(double *sin, double *cos);

#endif

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/

