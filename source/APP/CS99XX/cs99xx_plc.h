
#ifndef __CS99XX_PLC_H__
#define __CS99XX_PLC_H__

enum{
    PLC_SIG_TESTING,/* PLC�������ڲ����ź� */
    PLC_SIG_RESET,/* PLC��������ź� */
    PLC_SIG_PASS,/* PLC����PASS��Ϣ�� */
    PLC_SIG_FAIL,/* PLC����FAIL�ź� */
	PLC_SIG_EXIT,/* PLC�˳����Խ��� */
};

/* PLC״̬ */
enum{
    PLC_NO_ERR,
	PLC_HIGH_FAIL,
	PLC_LOW_FAIL,
	
    PLC_NO_READY,
	PLC_READY,
	PLC_TESTING,
	PLC_PASS,
	PLC_FAIL,
};

extern void plc_delay_off(void);
extern void plc_signal_cmd(uint8_t cmd);
extern void cancel_plc_test_signal(void);

#endif //__CS99XX_PLC_H__
