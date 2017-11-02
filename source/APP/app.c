/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�app.c
 * ժ  Ҫ  ���û���Ӧ�ó�����ڣ���ʼ����Ӳ�����������������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#define   APP_GLOBALS 

#include    "string.h"
#include    "stdio.h"
#include    "app.h"
#include	"BSP_config.h"
#include	"keyboard.h"
#include	"RTC_Config.h"
#include 	"DAC_VREF.h"
#include	"irq.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
#include 	"mb_server.h"

#include    "mb.h"
#include    "mb_m.h"
#include    "mb_cmd_define.h"
#include    "mb_master.h"
#include    "user_mb_app.h"
#include    "mbrtu.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_update.h"
#include    "cs99xx_it.h"
#include    "cs99xx_collect.h"
#include    "cs99xx_result.h"
#include    "cs99xx_plc.h"
#include    "auto_calibration.h"
#include    "dc_module.h"
#include 	"spi_flash.h"
#include    "test_com.h"


/***************************ȫ�ֱ���*************************/
uint8_t plain_code = 0; /* �������뿪�� 1Ϊ���� */
uint8_t gunlock = 0;	/* ���򿪼�����ʱ������ֵ��1��һ�ز����Ͳ��������������� */

void main_task(void* p_arg);
void keyscan_task(void* p_arg);
void sample_task(void* p_arg);

void schedule_task(void *p_arg);
void modbus_polling_task(void *p_arg);

/*
 * ��������get_cal_pin
 * ����  ����ȡ����״̬
 * ����  ����
 * ���  ����
 * ����  ������״̬
 */
uint8_t get_cal_pin(void)
{
    uint8_t res = 0;
    
    if(g_custom_sys_par.cal_medium == CAL_MEDIUM_FLASH)
    {
        res =  (CAL_PIN == SET);
    }
    else
    {
        res =  (CAL_PIN == RESET);
    }
    
    return res;
}
/*
 * ��������get_key_task_state
 * ����  ����ȡ����״̬
 * ����  ����
 * ���  ����
 * ����  ������״̬
 */
uint8_t get_key_task_state(void)
{
	return key_status;
}
/*
 * ��������off_keyboard
 * ����  ���رռ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void off_keyboard(void)
{
	INT8U err = 0;
	key_status = 1;
	OSTaskSuspend(Keyscan_TASK_PRIO);
	while(NULL != OSQAccept(KeyboardQSem, &err));
}

/*
 * ��������on_keyboard
 * ����  ���򿪼���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void on_keyboard(void)
{
	INT8U err = 0;
	key_status = 0;
	OSTaskResume(Keyscan_TASK_PRIO);
	while(NULL != OSQAccept(KeyboardQSem, &err));
}

/*
 * ��������clear_keyboard
 * ����  ����հ�������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void clear_keyboard(void)
{
	INT8U err = 0;
	while(NULL != OSQAccept(KeyboardQSem, &err));
}

/*
 * ��������off_sample_task
 * ����  ���رղ�������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void off_sample_task(void)
{
    set_sample_rate(0);/* ֹͣ���� */
    OSTaskSuspend(SAMPLE_TASK_PRIO);
}

void disable_sample_task(void)
{
    test_flag.sample_task_en = 0;
}
/*
 * ��������on_sample_task
 * ����  ���򿪲�������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void on_sample_task(void)
{
    test_flag.sample_task_en = 1;
}

void resume_sample_task(void)
{
	OSTaskResume(SAMPLE_TASK_PRIO);
    OSTimeDlyResume(SAMPLE_TASK_PRIO);
}

/*
 * ��������off_schedule_task
 * ����  ���رյ�������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void off_schedule_task(void)
{
    while(OSSemAccept(ScheduleSem));
	OSTaskSuspend(SCHEDULE_TASK_PRIO);
}

/*
 * ��������on_schedule_task
 * ����  ���򿪵�������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void on_schedule_task(void)
{
    while(OSSemAccept(ScheduleSem));
	OSTaskResume(SCHEDULE_TASK_PRIO);
}

/*
 * ��������mdbus_init
 * ����  ��MODBUS��ʼ����������ʹ�ܣ����ô���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void mdbus_init(void)
{
    p_prvvUARTRxISR = xMBRTUReceiveFSM;//�����жϴ�����
    p_prvvUARTTxReadyISR = xMBRTUTransmitFSM;
    
    /* �ӻ� */
	eMBInit(MB_RTU, 0x01, 2, 115200,  MB_PAR_EVEN);
	eMBEnable();
    /* ���� */
    eMBMasterInit(MB_RTU, 5, 115200,  MB_PAR_EVEN);
	eMBMasterEnable();
}

/*
 * ��������soft_init
 * ����  �������ʼ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
static void soft_init(void)
{
	init_file_table(); /* ��ʼ���ļ��� */
	init_99xx_list(); /* ��test_group��ʼ��Ϊһ������һ���������� */
	g_cur_file = &default_file; /* ��ʱ����ǰ�ļ�ָ��Ĭ�ϵ��ļ����ȿ������Ĭ���ļ�������Ҫ��Ϊ���һ��ʹ�õ��ļ� */
	init_ratio_all(); /* ��ʼ��У׼���� */
    
    mdbus_init();/* mdbus ��ʼ�� */
}
/*
 * ��������startup_task
 * ����  ����ʼ����
 * ����  ��void* p_arg û����
 * ���  ����
 * ����  ����
 */
void startup_task(void* p_arg)
{
void test_md5(void);
    test_md5();
    bsp_init();/* Ӳ����ʼ�� */
	soft_init();/* �����ʼ�� */
    
	#if(OS_TASK_STAT_EN>0)
		OSStatInit();//ʹ��ucos ��ͳ������
	#endif
	/* ���������� */
	while(OS_ERR_NONE != OSTaskCreateExt(main_task,
					(void*)0,
					&Main_Task_STK[MAIN_TASK_STK_SIZE-1],
					Main_TASK_PRIO,
					Main_TASK_ID,
					&Main_Task_STK[0],
					MAIN_TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
					));
	OSTaskDel(OS_PRIO_SELF);  //ɾ����������
}

/*
 * ��������schedule_task
 * ����  �����Ե�������
 * ����  ��void* p_arg û����
 * ���  ����
 * ����  ����
 */
void schedule_task(void *p_arg)
{
	INT8U err = 0;
	
	while(1)
	{
		/* �ȴ��ź��� */
		OSSemPend(ScheduleSem, 0, &err);
        
		if(STOP)
		{
			continue;
		}
        
		if(test_irq_fun != NULL)
		{
            if(test_flag.gradation == STAGE_TEST)
            {
                if(ERR_NUM_OVER != ERR_NONE && g_dis_time + 1 == tes_t)
                {
                    close_test_timer();
                    g_dis_time = tes_t;
                }
            }
            
            test_irq_fun();
		}
        
        /* ���Ե���˸ */
        if(test_flag.gradation != STAGE_INTER)
        {
            if(test_flag.test_led_flag)
            {
                LED_TEST = !LED_TEST;
            }
        }
        else
        {
            LED_TEST = LED_OFF;
        }
	}
}


/*
 * ��������sample_task
 * ����  ����������
 * ����  ��void* p_arg û����
 * ���  ����
 * ����  ����
 */
void sample_task(void* p_arg)
{
    off_sample_task();//�رղ�������
    
    while(1)
    {
        if(test_flag.sample_task_en)
        {
            OSTimeDlyHMSM(0,0,0,app_flag.sample_cycle);//���Ʋ�����
            adc_sample();
        }
        else
        {
            off_sample_task();//�رղ�������
        }
    }
}

/*
 * ��������keyscan_task
 * ����  ������ɨ������
 * ����  ��void* p_arg û����
 * ���  ����
 * ����  ����
 */
void keyscan_task(void* p_arg)
{
	off_keyboard();	/* �رռ��� */
	
	while(1)
	{
		report_key_value();/* ������ɨ�谴�� */
		OSTimeDlyHMSM(0,0,0,5);
	}
}


/*
 * ��������modbus_polling_task
 * ����  ��MODBUS��������
 * ����  ��void* p_arg û����
 * ���  ����
 * ����  ����
 */
void modbus_polling_task(void *p_arg)
{
	while(1)
	{
		eMBPoll();
        OSTimeDlyHMSM(0,0,0,3);
   	}
}
void modbus_master_polling_task(void *p_arg)
{
	while(1)
	{
        eMBMasterPoll();
        OSTimeDlyHMSM(0,0,0,10);
   	}
}
void clear_mdbus_send_queue(void)
{
    uint8_t err = 0;
    
    OSMutexPend(MdbusMutex, 0, &err);
    while(NULL != OSQAccept(MdbusQSem, &err));
    OSMutexPost(MdbusMutex);
    
    test_flag.mb_master_timeout = 0;
}
/*
 * ��������modbus_master_send_task
 * ����  ��modbus������������
 * ����  ��void* p_arg û����
 * ���  ����
 * ����  ����
 */
void modbus_master_send_task(void* p_arg)
{
    #define RETRY_TIMES_MAX   2 /* ����ط����� */
    MB_PACK_T *mb_pack;
    uint16_t retry_times = 0;
    uint16_t count = 0;
    uint8_t err = 0;
    
	while(1)
	{
        test_flag.mb_break_send = MB_MASETER_SEND_END;
        OSMutexPend(MdbusMutex, 0, &err);
		mb_pack = OSQPend(MdbusQSem,10,&err);
        OSMutexPost(MdbusMutex);
        
        if(test_flag.mb_break_send == MB_MASETER_SEND_BREAK)/* ��� */
        {
            clear_mdbus_send_queue();
            test_flag.mb_break_send = MB_MASETER_SEND_END;
            continue;
        }
        
        test_flag.mb_break_send = MB_MASETER_SENDING;
        
        if(mb_pack != NULL)
        {
			app_flag.dc_gr_task_send_idle = 0;
            OSTimeDlyHMSM(0,0,0,10);
            test_flag.mb_syn = 0;
            retry_times = 0;
            
            do
            {
                mb_send_cmd(mb_pack);
                /* modbus����Ҫ�ȴ���Ӧ��Ϊ���͵��ǹ㲥ָ�� */
                if(mb_pack->type == MB_NO_WAIT_ACK)
                {
                    break;
                }
                
                for(count = 0; count < 80; count++)
                {
                    OSTimeDlyHMSM(0,0,0,10);
                    
                    if(test_flag.mb_break_send == MB_MASETER_SEND_BREAK)/* ��� */
                    {
                        clear_mdbus_send_queue();
                        break;
                    }
                    
                    if(test_flag.mb_syn == 1)
                    {
                        break;
                    }
                }
            }while(++retry_times <= RETRY_TIMES_MAX && test_flag.mb_syn == 0);
            
            
            if(retry_times == RETRY_TIMES_MAX + 1 && test_flag.mb_syn == 0)
            {
                test_flag.mb_master_timeout = 1;
            }
        }
		else
		{
			app_flag.dc_gr_task_send_idle = 1;
		}
   	}
}

/*
 * ��������create_other_task
 * ����  ��������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void create_other_task(void)
{
    /* ��������ɨ������ */
	while(OS_ERR_NONE != OSTaskCreateExt(keyscan_task,
					(void*)0,
					&Keyscan_Task_STK[KEY_TASK_STK_SIZE-1],
					Keyscan_TASK_PRIO,
					Keyscan_TASK_ID,
					&Keyscan_Task_STK[0],
					KEY_TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
					));
    /* ���Ե������� */
	while(OS_ERR_NONE != OSTaskCreateExt(schedule_task,
					(void*)0,
					&Startup_Task_STK[SCHEDULE_TASK_STK_SIZE-1],
					SCHEDULE_TASK_PRIO,
					SCHEDULE_TASK_ID,
					&Startup_Task_STK[0],
					SCHEDULE_TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
					));
    /* �������� */
	while(OS_ERR_NONE != OSTaskCreateExt(sample_task,
					(void*)0,
					&Sample_Task_STK[SAMPLE_TASK_STK_SIZE-1],
					SAMPLE_TASK_PRIO,
					SAMPLE_TASK_ID,
					&Sample_Task_STK[0],
					SAMPLE_TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
					));
    /* MODBUS�������� */
 	while(OS_ERR_NONE != OSTaskCreateExt(modbus_polling_task,
 					(void*)0,
 					&Modbus_Send_STK[MODBUS_SEND_STK_SIZE-1],
 					MODBUS_SEND_TASK_PRIO,
 					MODBUS_SEND_TASK_ID,
 					&Modbus_Send_STK[0],
 					MODBUS_SEND_STK_SIZE,
 					(void*)0,
 					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
 					));
 	while(OS_ERR_NONE != OSTaskCreateExt(modbus_master_polling_task,
 					(void*)0,
 					&Modbus_M_Send_STK[MODBUS_M_SEND_STK_SIZE-1],
 					MODBUS_M_SEND_TASK_PRIO,
 					MODBUS_M_SEND_TASK_ID,
 					&Modbus_M_Send_STK[0],
 					MODBUS_M_SEND_STK_SIZE,
 					(void*)0,
 					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
 					));
    /* MODBUS������������ */
 	while(OS_ERR_NONE != OSTaskCreateExt(modbus_master_send_task,
 					(void*)0,
 					&Modbus_Master_Send_STK[MODBUS_MASTER_SEND_STK_SIZE-1],
 					MODBUS_MASTER_SEND_TASK_PRIO,
 					MODBUS_MASTER_SEND_TASK_ID,
 					&Modbus_Master_Send_STK[0],
 					MODBUS_MASTER_SEND_STK_SIZE,
 					(void*)0,
 					OS_TASK_OPT_STK_CHK| OS_TASK_OPT_STK_CLR
 					));
}

/*
 * ��������exe_new_status
 * ����  ��ִ�е�ǰ״̬����
 * ����  ��exe_status �����µ�״̬
 * ���  ��exe_status ����µ�״̬
 * ����  ����
 */
void exe_cur_status(EXE_STATUS_ENUM *exe_status)
{
    EXE_STATUS_ENUM  t_exe_status = EXE_NULL;
    
    if(NULL == exe_status)
    {
        return;
    }
    
    t_exe_status = *exe_status;
    *exe_status = EXE_NULL;
    
    switch(t_exe_status)
    {
        case RE_MAIN://�ػ�������
        {
            clear_();   /* ���� */
            gui_draw_main();/* ��������� */
            break; 
        }
        case INTO_TEST://������Խ���
        {
            off_rtc_int();/* �ر�RTC�ж� */
            
            app_flag.into_test_ui = 1;/* ��λ */
            test_subtask();/* ���� ���������� */
			plc_signal_cmd(PLC_SIG_EXIT);/* PLC״̬����Ϊδ���� */
            app_flag.into_test_ui = 0;/* �����־ */
            on_rtc_int();/* ��RTC ʱ�� */
            
            /* �ж��Ƿ�Ҫ������Խ��� */
            if(test_flag.into_set)
            {
                test_flag.into_set = 0;
                g_return = 0;
                g_exit = 0;
                *exe_status = INTO_SET;//�������ý���
            }
            /* �ж��Ƿ��ٴν�����Գ��� */
            if(test_flag.into_test)
            {
                test_flag.into_test = 0;
                g_return = 0;
                g_exit = 0;
                *exe_status = INTO_TEST;//������Խ���
            }
            break;
        }
        case INTO_SET://�������ý���
            set_subtask(0);/* �������ý��� 0��ʾ����������룬�����1��ʾ�Ӳ��Խ������ �����뿴 set_subtask ���� */
            break;
        case INTO_HELP://�����������
            help_subtask();
            break;
        case INTO_MENU://����˵�����
            menu_subtask();
            cursor_pos(-5,0);//������Ƴ���Ļ ��ֹ������λ�û�Խ�����ʾ���
            break;
        case INTO_AUTO_CAL:
            app_flag.calibration = 1;/* ����У׼ģʽ */
            auto_calibration();//����У׼����
            app_flag.calibration = 0;/* �˳�У׼ģʽ */
            *exe_status = RE_MAIN;/* �ػ������� */
            break;
        case INTO_SCAN_MODULES:
            app_flag.scan_module_flag = 1;
            scan_modules();
            app_flag.scan_module_flag = 0;
            break;
        default:
            Time_Show(20,40);//ˢ��ʱ��
            break;
    }
}

/*
 * ��������main_task
 * ����  �������񣬸���������ã����Ե���Ҫ����
 * ����  ��void* p_arg û����
 * ���  ���� 
 * ����  ���� 
 */
void main_task(void* p_arg)
{
	INT8U err = 0;
	uint16_t* pkey_value = NULL;
	int16_t start_delay = 0;
    EXE_STATUS_ENUM exe_status = RE_MAIN;
    uint8_t l_cal_lock = 0;/* ����У׼������־ */
    
    exit_off();/* �ر��ж� */
    read_custom_par();/* �������Ʋ��� �����ȶ����Ʋ����ڶ��������� */
    check_type();/* ���Ӳ���ͺ� */
	init_par_info();/* ������ͺź�ͶԽ��������Ϣ��ʼ�� */
    create_other_task();/* ������������ */
    
	
	/* �Ӵ洢���ж������������Ͳ�Ҫ������Խ�����ǽ��������� */
	if(0 != read_paramenter())
	{
		TimeDisplay = 1;/* ����ʱ����ʾ��־λ */
        on_keyboard();	/* �򿪼��� */
	}
    /* �Ӵ洢���ж�ȡ����û�г��� */
    else
    {
        if(DC_GR_EN == ENABLE)
        {
            get_dc_gr_cal_info();
        }
        
        cur_node = *g_cur_step;//���ص�ǰ�����ݵ�ȫ�ֱ���cur_node��
        init_par_info_list(&cur_node);/* ģʽ���������ʼ�� */
        
        clear_();	/* ���� */
        Time_Show(20,40); /* ��ʾʱ�� */
        gui_draw_main();  /* ��ʾ������ */
        
        while(1)
        {
            Time_Show(20,40);
            
            /* ����ʱ���Ÿ�λ���Ͳ��������Խ��� */
            if(STOP_PIN == 0)
            {
                on_keyboard();/* �򿪼��� */
                break;
            }
            
            /* ������ʱ�ȴ�ֱ�ӽ�����Խ��� */
            if(++start_delay > 6000)
            {
                on_keyboard();/* �򿪼��� */
                exe_status = INTO_TEST;//������Խ���
                break;
            }
        }
    }
    
    rtc_init(); /* RTC��ʼ�� ע��:֮����������ų�ʼ��rtc��Ϊ,��rtc����bsp_init��ʱĬ��ʱ����������ʧ��
                     �����飬ȷ��Ϊ��ʼ����Ҫһ��ʱ����ʱ�����������ʱ̫���ˣ�����Ῠ�٣���˷������� */
    
	while(1)
	{
        /* �����־ */
		if(g_return || g_exit)
		{
			g_return = 0;
			g_exit = 0;
			exe_status = RE_MAIN;/* �ػ������� */
		}
        
        /* ���������ý���ı�־ */
        if(test_flag.into_set)
        {
            test_flag.into_set = 0;
            exe_status = INTO_SET;//�������ý���
        }
        
        /* ��������Խ���ı�־ */
        if(test_flag.into_test)
        {
            test_flag.into_test = 0;
            exe_status = INTO_TEST;//������Խ���
        }
        
        /* �������Զ�У׼�ı�־ */
        if(test_flag.into_auto_cal_ui)
        {
            test_flag.into_auto_cal_ui = 0;
            exe_status = INTO_AUTO_CAL;
        }
        
        exe_cur_status(&exe_status);//ִ���µĽ���״̬
        
		pkey_value = OSQAccept(KeyboardQSem, &err);
        
		if(NULL == pkey_value)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_F1:/* �������ý��� */
                exe_status = INTO_SET;
                break;
			case KEY_F2:/* ����˵����� */
                exe_status = INTO_MENU;
				break;
			case KEY_F3:/* ������Խ��� */
                exe_status = INTO_TEST;
				break;
			case KEY_F4:/* �������� */
                exe_status = INTO_HELP;
				break;
			case KEY_ENTER & KEY_0:/* ��Ӣ���л� */
				sys_par.language = !sys_par.language;
				save_sys_par();
				exe_status = RE_MAIN;/* �ػ������� */
				break;
			case KEY_ENTER & KEY_1:/* �ָ�ϵͳĬ������ */
				strcpy(sys_par.pass_word, PASS_WORD);
				save_sys_par();
				prompt_recover_pw();
				exe_status = RE_MAIN;/* �ػ������� */
				break;
			case KEY_0 & KEY_1:/* ��ʼ��eep ��Ҫ���� */
				if(!get_cal_pin())
				{
					break;
				}
                
				sys_flag.mem_init = 0;
				save_sys_flag();/* ����ϵͳ��־ */
				break;
			case KEY_F4 & KEY_8:/* ����У׼ */
				if(!get_cal_pin())
				{
					break;
				}
                
                if(l_cal_lock == 0)
                {
                    /* Ȩ�޼�飬��֤�û����� */
					if(0 != authenticate_pw(AUTHEN_PW_CAL))	/* ������֤ʧ�� */
					{
                        exe_status = RE_MAIN;/* �ػ������� */
                        break;
					}
					else
					{
						l_cal_lock = 1;
					}
                }
                
				app_flag.calibration = 1;/* ����У׼ģʽ */
				correctness();//����У׼����
				app_flag.calibration = 0;/* �˳�У׼ģʽ */
				break;
			case KEY_F4 & KEY_9:/* �����ͺ�ѡ�� */
#ifndef BOOT_LOADER
				if(!get_cal_pin())
				{
					break;
				}
                /* Ȩ�޼�飬��֤�û����� */
				if(-1 == authenticate_pw(1))
				{
					exe_status = RE_MAIN;/* �ػ������� */
					break;
				}
				select_type();
#endif
				break;
			case KEY_F4 & KEY_5:/* �������ڸ��� */
                /* Ȩ�޼�飬��֤�û����� */
				if(-1 == authenticate_pw(1))
				{
					exe_status = RE_MAIN;/* �ػ������� */
					break;
				}
				sys_check_date();
				break;
			case KEY_0 & KEY_2:/* ����ϵͳ */
				if(!get_cal_pin())
				{
					break;
				}
				NVIC_SystemReset();
				while(1);
			case KEY_0 & KEY_3:/* ���������������Ϣ */
				if(!get_cal_pin())
				{
					break;
				}
				dis_program_info();
                exe_status = RE_MAIN;/* �ػ������� */
                break;
			case KEY_0 & KEY_4:/* �鿴ϵͳ�쳣��Ϣ��־ */
				if(!get_cal_pin())
				{
					break;
				}
				dis_exception_log();
                exe_status = RE_MAIN;/* �ػ������� */
                break;
			case KEY_0 & KEY_5:/* �Զ�У׼ */
				if(!get_cal_pin())
				{
					break;
				}
                exe_status = INTO_AUTO_CAL;
                break;
			case KEY_0 & KEY_6:/* ģ��ɨ����� */
				if(!get_cal_pin())
				{
					break;
				}
                exe_status = INTO_SCAN_MODULES;
                break;
		}
	}
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
