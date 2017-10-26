
#ifndef __AUTO_CAL_COMM_H__
#define AUTO_CAL_COMM_H__

typedef enum{
AUTOCAL_AC_V, //������ѹ
AUTOCAL_AC_A, //��������
AUTOCAL_DC_V, //ֱ����ѹ
AUTOCAL_DC_A, //ֱ������
AUTOCAL_RES , //����
AUTOCAL_GR_A, //�ӵص���
}CAL_MODE_T;
extern uint8_t *cal_mode_pool[][2];

//�ӻ�״̬��
typedef enum{
	AUTOCAL_WAIT_CONNECT,	//δ����У׼����
	AUTOCAL_CONNECTED,		//�ѽ���У׼����
	AUTOCAL_WAIT_DATA,		//�ȴ�У׼����
	AUTOCAL_OUTPUTING,		//�������
	AUTOCAL_SAVEDATA,		//��������
	AUTOCAL_WAIT_OUTPUT,	//�ȴ����
	AUTOCAL_WAIT_STOP,		//�ȴ�ֹͣ
}TESTER_ST;

void enter_auto_cal_ui(void);
void exit_auto_cal_ui(void);
uint8_t *get_cur_point_mode(void);
void get_auto_cal_status(uint16_t*data);
uint8_t *get_cur_point_range(uint8_t *buf);
void auto_cal_start_output(void);
void auto_cal_stop_output(void);
void set_auto_cal_measure(uint64_t measure);
void get_auto_cal_points(uint16_t *data);
int16_t load_auto_cal_point(uint16_t data);

#endif //AUTO_CAL_COMM_H__
