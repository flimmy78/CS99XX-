#ifndef __MB_CMD_DEFINE_H__
#define __MB_CMD_DEFINE_H__

#define MB_READ_PAR     0X5A    //������
#define MB_SET_PAR      0XA5    //д����
#define MB_EXE_OK       0XF5    //ִ������ɹ�
#define MB_EXE_NG       0XF5    //ִ������ʧ��

typedef enum{
    USB_ACCESS_NO,//���ܷ���U��
    USB_ACCESS_OK,//u�̷���OK
    
    USB_CON_NG,//U��δ����
    USB_CON_OK,//U������OK
}USB_ST;

typedef enum{
    MB_YES_WAIT_ACK,/* MODBUS�ȴ�������Ӧ */
    MB_NO_WAIT_ACK,/* MODBUS���ȴ�������Ӧ */
}MB_WAIT_ACK;

typedef enum{
    NO_ERR,/* ָ��ִ�гɹ� */
    SYNTAX_ERR,/* ָ���а������Ϸ����ַ���ָ���ʽ���� */
    EXE_NOT_ALLOWED,/* �ڵ�ǰ״̬�²�����ִ�д�ָ�� */
    PAR_NOT_ALLOWED,/* ���յ�ָ����������Ĳ��� */
    MISS_PARAMETER,/* ָ�����©���� */
    UNDEFINED_HEADER,/* δ�����ָ���ͷ */
    PAR_TYPE_ERR,/* �������ʹ��� */
    INVALID_STR_DATA,/* ��������ַ������� */
    EXE_TIME_OUT,/* ָ��ִ�г�ʱ */
    DATA_OUT_RANGE,/* ����ֵ��������Χ */
}MB_COMM_ERR_T;

typedef enum{
    CTRL_LOCAL,/* ���ؿ��� */
    CTRL_REMOTE,/* Զ�̿��� */
}CONTROL_ST_T;

typedef enum{
    M_POWER_CHECKRES, /* ��Դ��� */
    M_COMM_ADDR, /*����λ��ͨ�ŵ�ַ*/ 
    M_COMM_CTRL_STATUS, /*����״̬*/
    M_CONTROL_TEST_BUTTON, /*���ò���״̬*/
    M_DELETE_FILE, /* ɾ���ļ� */
    M_QUERY_ACTIVE_FILE_NUM,/*��ѯ��ǰ�ļ��ı��*/
    M_QUERY_ACTIVE_STEP_NUM,/*��ѯ��ǰ������*/
    M_CATALOG_FILE, /* ��ѯ�ļ��Ƿ���ڴ��ھͷ������ļ����� */
    M_INSERT_STEP, /*������Բ���*/
    M_DELETE_STEP, /*ɾ����ǰ�����*/
    M_ACTIVE_STEP_FRONT, /*��ǰ�����ǰ��һλ*/
    M_ACTIVE_STEP_BEHIND, /*��ǰ��������һλ*/
    M_INTER_CHANGE_STEP, /*����������Ϣ*/
    M_UPDATA_STEP_MODE, /*�޸ĵ�ǰ��ļ���ģʽ*/
    
    /********** ϵͳ���� ***************/
    M_SYSTEM_NRULE,/* ���Խ����Ź��� */
    M_SYSTEM_TIME,/* ϵͳʱ�� */
    M_PASS_WORD,/* ϵͳ���� */
    M_KEYKLOCK,/* ������ */
    M_FAIL_CONTINUE,/* ʧ�ܼ��� */
    M_SYS_LANGUAGE,/* ϵͳ���� */
    M_TEST_PORT,/* ���Զ˿� �ӵ� ����*/
    M_RESET_STOP,/* ��λ��ͣ */
    M_CHECK_HINT, /* ������ʾ */
    M_SELF_CHECK,/* �Լ� */
    M_TEST_SIGNAL,/* �����ź� */
    M_PHV,/* */
    M_GFI,/* GFI���� */
    M_RESULT_RECOVER,/* ������� */
    M_RESULT_SAVE,/* ������� */
    M_RES_REMAIN_HINT, /* ������ʾ */
    M_SYS_VOLUME,/* ϵͳ���� */
    M_SYS_CONTRAST,/* ��Ļ�Աȶ� */
    M_DUT_NAME,/* ����Ʒ�� */
    
    /********* ���Խ�� *****************/
    M_RESULT_FETCH,/* ��ѯ�����Ϣ */
    M_RESULT_CLEAR,/* ��ս�� */
    M_RESULT_FAIL,/* ��ѯʧ������ */
    M_RESULT_PASS,/* ��ѯ�ɹ����� */
    M_RESULT_USED_NUM,/* ��ѯʹ������ */
    M_RESULT_REMAIN_NUM,/* ��ѯʣ������ */
    M_RESULT_TOTAL_NUM,/* ��ѯ������ */
    
    /********** ����������� ************/
    M_TEST_PORTS,/* ��·ɨ��˿� */
    M_IR_AUTO_SHIFT,/* IR�Զ����� */
    M_COM_STEPS_CONT,/* �������� */
    M_COM_STEPS_PASS,/* ����PASS */
    M_COM_DELAY_T,/* ��ʱʱ�� */
    M_COM_CHANGE_T,/* ���ʱ�� */
    M_COM_INTER_T,/* ���ʱ�� */
    M_COM_FALL_T,/* �½�ʱ�� */
    M_COM_TEST_T,/* ����ʱ�� */
    M_COM_RISE_T,/* ����ʱ�� */
    M_COM_FREQUENCY,/* ���Ƶ�� */
    M_COM_ARC,/* �绡��� */
    M_COM_CURRENT,/* ������� GR */
    M_COM_CHARGE_CURRENT,/* ������ */
    M_COM_REAL_CURRENT,/* ��ʵ���� */
    M_COM_CUR_LOW,/* �������� */
    M_COM_CUR_HIGH,/* �������� */
    M_COM_CUR_GEAR,/* ������λ */
    M_COM_VOLTAGE,/* �����ѹ */
    M_COM_MODE,/* ����ģʽ */
    
    M_BBD_OPEN_CHECK,/* BBD��·��� */
    M_BBD_SHORT_CHECK,/* BBD��·��� */
    
    /************* �����༭ ***************/
    M_SWAP_STEPS,/* �������Բ� */
    M_MOVE_FORWARD,/* ǰ�� */
    M_MOVE_BACKWARD,/* ���� */
    
    /************* �������� ***************/
    M_TEST_MODE,/* ����ģʽ */
    M_TEST_FETCH_MSG,/* ���Թ����е����ݲ�ѯ */
    M_GET_CAP,/* ���Ի�ȡBBD����ֵ */
    M_GET_OFFSET,/* ���Ի�ȡ������ƫ��ֵ */
    M_PLC_STATUS,/* PLC״̬ */
    M_TEST_STATUS,/* ����״̬ */
    
    M_DEL_ALL_FILE,/* ɾ�������ļ� */
    M_STEP_MSG,/* ���Բ���Ϣ */
    M_FILE_MSG,/* �ļ���Ϣ */
    M_SET_ACTIVE_STEP,/* ��ȡָ����Ϊ��ǰ��� */
    M_SET_ACTIVE_FILE,/* ��ȡָ���ļ�Ϊ��ǰ�Ļ�ļ� */
    M_SAVE_FILE,/* ��ǰ�ļ����Ϊ ���� ͨ���ļ��ṹ���ļ������ļ���Ŵ����� */
    M_EDIT_FILE,/* �༭�ļ� */
    M_NEW_FILE,/* �½��ļ� */
    M_ID_MSG,/* �����Ǳ�ʶ��ѯ */
    M_RST,/* �����Ǹ�λָ�� */
    M_TESTER_INFO,/* ������Ϣ */
    M_COMM_BPS,/* ͨ�Ų����� */
    M_COMM_SW_STATUS,/* ͨ�ſ��� */
    M_PLC_SW_STATUS,/* PLC���� */
    M_CYLINDER_CTRL_STATUS,/* ���׿���״̬ */
    
/******************************************************************/
    M_SYS_PAR_USB,    /* ϵͳ�����ļ� ������1����¼ */
    M_SYS_FLAG_USB,   /* ϵͳ����ļ� ������1����¼ */
    M_FILE_LIST_USB,  /* �ļ���Ŀ¼ ����31���Ӽ�¼�� �ֱ��� 31���ļ� */
    
    M_STEP_USED_FLAG_BASE,/* λͼ�� */
    M_GROUP_TABLE_BASE,/* ����ӳ��� */
    M_MEM_GROP_BASE_USB,/* ��������Բ����� */
    M_DATA_LOADING_END,/* ���ݴ������ */
    
    M_RESULT_USB,    /* ����ļ��Ű���n����¼ nΪ�������Ѵ��ڵļ�¼���� */
    M_RESULT_INFO,   /* ��ȡ�����ǽ�������Ϣ */
    M_RESULT_ONE_USB, /* ʵʱ��ȡ�����Ϣʹ�� */
    M_SYS_PAR_LEN_USB,/* ��ѯϵͳ���ݳ��� */
    M_FILE_STEPS_NUM_USB,/* ��ѯָ���ļ��Ĳ��� */
    M_GET_TESTER_TYPE_USB,/* ��ȡ�����ǵ��ͺ� */
    M_USB_CONNECT_STATUS,/* U������״̬ */
    
/******************************************************************/
    M_AUTO_CAL,/* �Զ�У׼ */
	
	M_COM_OFFSET,/* ƫ�� */
	
	M_GET_INTERFACE_INFO,/* ��ȡ�ӿڰ���Ϣ */
/******************************************************************/
	//У׼ָ�
	M_CAL_ENTER,/* ����У׼���� */
	M_CAL_EXIT,/* �˳�У׼���� */
	M_CAL_STATUS,/* ��ȡУ׼״̬ */
	M_CAL_MODE,/* ��ȡУ׼ģʽ */
	M_CAL_RANGE,/* ��ȡУ׼���� */
	M_CAL_START,/* ����У׼��� */
	M_CAL_STOP,/* ֹͣУ׼��� */
	M_CAL_MEASURE,/* ����У׼����ֵ */
	M_CAL_POINTS,/* ��ȡУ׼���ܸ��� */
	M_CAL_LOAD,/* ����ָ����У׼�� */
/******************************************************************/
	
}MB_CMD_T;

/* ֱ��ģ��ָ� */
enum{
    M_DC_MODULE_CAL_DATA,   /* У׼���� */
    M_DC_MODULE_DAC,    /* ���DA */
    M_DC_MODULE_START,  /* �������� */
    M_DC_MODULE_STOP,   /* ֹͣ���� */
    M_DC_MODULE_TEST_FETCH, /* ��ȡֱ��ģ��Ĳ�����Ϣ ��ѹAD ����AD ����״̬ ST */
    M_DC_MODULE_VOL_GEAR,   /* ��ѹ��λ�趨 */
    M_DC_MODULE_TEST_METHOD,/* ���ò��Է��� ������� �������� */
    M_DC_MODULE_INFO,   /* ��ȡֱ��ģ�����Ϣ */
    M_DC_MODULE_SET_DATA,//ģ����������ݰ�����λ�Ͳ��Է���...
    M_DC_MODULE_TEST_FETCH_SHORT,//��ָ��
};
#endif //__MB_CMD_DEFINE_H__

/******************* (C) COPYRIGHT 2016 ��ʢ���� *****END OF FILE*******************/

