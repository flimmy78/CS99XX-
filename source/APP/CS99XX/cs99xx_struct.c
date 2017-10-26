/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�99xx_struct.c
 * ժ  Ҫ  ��������Ҫ�����ݽṹ������������ʹ��
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

/********************** �����ļ� ************************/
#define _99xxSTR_GLOBALS
#include "cs99xx_struct.h"
#include "string.h"
#include "cs99xx_parameter.h"
#include "cs99xx_type.h"
#include "serve_test.h"
#include "cs99xx_mem_api.h"

/****************** �ڲ��������� *************************/

/****************** ȫ�ֱ����Ķ��弰��ʼ�� *****************/
/* ����λͼ��������ѯ���洢�ռ��Ѿ�ʹ�� */
static uint8_t  const  cs99xx_maptbl[256] = {
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x00 to 0x0F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x10 to 0x1F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x20 to 0x2F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,       /* 0x30 to 0x3F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x40 to 0x4F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x50 to 0x5F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x60 to 0x6F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,       /* 0x70 to 0x7F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0x80 to 0x8F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0x90 to 0x9F */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xA0 to 0xAF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,       /* 0xB0 to 0xBF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xC0 to 0xCF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,       /* 0xD0 to 0xDF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,       /* 0xE0 to 0xEF */
    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 0        /* 0xF0 to 0xFF */
};
TEST_FILE default_file = 
{
	0,/* Ĭ���ļ��ı��Ϊ0 */
	"DEFAULT",/* name */
// 	"????????",/* ����Ʒ�� */
	N_MODE,	/* ����ģʽΪN������ģʽ */
	0,	/* pass�źű���ʱ�� 1s */
	10,	/* pass����������ʱ�� 1s */
	ARC_CUR_MODEL,	/* �绡���Ϊ����ģʽ */
	1,	//�ļ��������˶��ٲ�
    0,
};


SYS_PAR sys_par =
{
	5,	/* Һ���Աȶ�(0-9) */
	5,	/* ������ʾ (0-9) */
	1,	/* �Ƿ񱣴�����0�����棬1���� */
	0,	/* �Ƿ�������� 0�����ǣ�1���� */
	GFI_OFF,	/* �Ƿ�GFI���� 0�رգ�1�� */
	0,	/* �Ƿ�򿪿����Լ칦�� 0�رգ�1�� */
	1,	/* �����л���1���ģ�0Ӣ�� */
	0,	/* ʧ�ܼ����Ƿ�ʧ�ܼ�����0��������1���� */
    1,
	
	FLOAT_MODE,	/* ���Է��� 0�ӵ� 1���� */
	0,	/* ���Ե�ƽ 0��ƽ 1���� */
	
	"2014-12-12 12:12:12",	/* ����ʱ�� */
	DISABLE,	/* �Ƿ���б���ʾ���� 0���رգ�1�� */
	0,	/* ��Ź��� (0-2) */
	1,	/* ���������ش� */
	
	ENABLE,	/* ͨ�ſ��� 1�� 0�ر�*/
	1,	/* �����ʣ�1��ʾ9600 2��ʾ14400 3��ʾ19200 */
	1,	/*������ַ (1-255) */
	
	ENABLE,	/* PLC���� 1�򿪣�0�ر� */
	
	"888888",	/* �������8λ������1λ */
	
	0,	/* һ��ʹ�õĽ���� ����ͳ�ƽ�� */
	0,	/* �ϸ�Ľ���� ����ͳ�ƽ�� */
	
	0,	/* ������ */
	0,/* ����Ʒ�� */
    DISABLE,/* �������� */
	
	0,/* plc �ź� ��ʧ�ܼ����򿪺����plc_signal=each_step ��ʾÿ�����ᷢPlc�ź�
                                   ���һ��Ҳ�ǣ���� plc_signal=all_steps ��ʾ���һ����������plc�ź�
                                   ������һ���ۺ��ж� */
	0,/* �����ʱ �û�Ҫ��������ȴ�һ��ʱ���������ѹ */
	200,/* IR�л���λ��ʱ */
};

/*********************** �����Ķ��� **************************/

/*
 * ��������init_sys_flag
 * ����  ����ʼ��ϵͳ��־
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_sys_flag(void)
{
//     memset(&sys_flag, 0, sizeof sys_flag);
	sys_flag.mem_init = 1;	/* ����־λд1 */
	sys_flag.last_group = 0;
	sys_flag.boot_times = 1;
	sys_flag.amp_times = 0;
	sys_flag.key_start_times = 0;
	sys_flag.key_stop_times = 0;
	sys_flag.flag_flag = FLAG_FLAG;
}
/*
 * ��������init_sys_par
 * ����  ����ʼ��ϵͳ����
 * ����  ��SYS_PAR *p ����Ҫ��ʼ����ϵͳ����ָ��
 * ���  ����
 */
void init_sys_par(SYS_PAR *p)
{
	p->contrast = 5;	        /* Һ���Աȶ�(0-9) */
	p->allowance = 5;	        /* ������ʾ (0-9)*/
	p->is_save_res = 1;	        /* �Ƿ񱣴�����0�����棬1���� */
	p->is_overflow_cover = 0;	/* �Ƿ�������� */
	
	p->is_gfi_protect = 0;	    /* �Ƿ�GFI���� */
	p->is_self_check = 0;	    /* �Ƿ�򿪿����Լ칦�� */
	p->language = CHINESE;	    /* �����л���1���ģ�0Ӣ�� */
	p->is_falt_continue = 0;	/* ʧ�ܼ����Ƿ�ʧ�ܼ�����0��������1���� */
	p->fail_mode = FAIL_MODE_STOP;	/* ʧ�ܼ����Ƿ�ʧ�ܼ�����0��������1���� */
	
	p->is_table_dis = 0;	    /* �Ƿ���б���ʾ���� 0���رգ�1�� */
	p->test_method = 0;		    /* ���Է��� 1�ӵ� 0���� */
	p->test_level = 1;			/* ���Ե�ƽ 1��ƽ 0���� */
	p->buzzer_sw = ENABLE;      /* ���������� */
    p->silent_sw = DISABLE;     /* �������� */
	
    //�ò����ڻָ���������ʱ�ܵ�����
    if(app_flag.recover_flag == 0)
    {
        strcpy (p->data_of_survey, "2015-12-12 12:12:12");	/* ����ʱ�� */
    }
    
	p->num_rule = 0;	        /* ��Ź��� (0-2) */
	
	p->commu_on_off = ENABLE;	    /* ͨ�ſ��� 1�� 0�ر�*/
	p->baud_rate_gear = 1;	    /* �����ʣ�1��ʾ9600 2��ʾ14400 3��ʾ19200 */
	p->local_addr = 1;	        /*������ַ (1-255) */
	
	p->plc_on_off = 1;	        /* PLC���� 1�򿪣�0�ر� */
	strcpy(p->pass_word, "888888");	/* �������8λ������1λ */
	
	p->used_res_num = 0;        /* �Ѿ���¼�Ľ������ ����ͳ�ƽ��*/
	p->pass_res_num = 0;        /* �ϸ���� ����ͳ�ƽ�� */
	p->cover_res_num = 0;       /* ���Ǽ����� */
	
	p->keyboard_lock = 0;	    /* ������ 1�� 0�ر� */
	
	strcpy (p->dut_name, "????????");	/* ����Ʒ�� */
    
	p->plc_signal = ALL_STEP;        /* PLC�ź� */
	p->output_delay = 0;        /* ��ʱʱ�� */
	p->ir_gear_hold = 200;/* IR������ʱʱ�� */
	p->ir_gear_hold = IR_FILTERING_DEEP_FAST;/* IR�����ٶ�Ĭ��Ϊ���� */
	
	{
		uint32_t i = 0;
		
		for(i = 0; i < 16; i++)
		{
			if(p->dc_module_using_info.addr[i] > 15
				|| p->dc_module_using_info.count > 15)
			{
				memset(&p->dc_module_using_info, 0, sizeof(p->dc_module_using_info));
				break;
			}
		}
	}
}

void init_file_table(void)
{
	uint8_t i = 0;
    
	memset(file_table, 0, sizeof(file_table[0])*MAX_FILES);
    
	for(i = 0; i < MAX_FILES; i++)
	{
		file_table[i].num = i;  /* �����ļ�����һ��Ψһ�ı�ŷ�Χ(0 - MAX_FILES) */
	}
}

/*
 * ��������init_one_file
 * ����  ����ʼ��ָ���ļ�������
 * ����  ��n�ļ����
 * ���  ����
 * ����  ����
 */
void init_one_file(uint8_t n)
{
    if(n >= MAX_FILES)
    {
        return;
    }
    
	file_table[n].num = n; /* �ļ���ţ�ÿ���ļ�����Ψһ�ı�� 0 Ĭ���ļ� 1-30�û��ļ� */
	file_table[n].work_mode = N_MODE; /* 1��ʾN 2��ʾG */
	file_table[n].hold_pass_time = 10; /* pass�źű���ʱ�� */
	file_table[n].pass_buzzing_time = 10; /* pass����������ʱ�� */
	file_table[n].arc_mode = ARC_CUR_MODEL; /* 1����ģʽ2�ȼ�ģʽ */
	file_table[n].total = 0; /* ��Ÿ��ļ���һ�������˼��� */
}

/*
 * ��������clear_one_file
 * ����  �����ָ���ļ�������
 * ����  ��n�ļ����
 * ���  ����
 * ����  ����
 */
void clear_one_file(uint8_t n)
{
    if(n >= MAX_FILES)
    {
        return;
    }
    
    memset(&file_table[n], 0, sizeof file_table[0]);
    
	file_table[n].num = n; /* �ļ���ţ�ÿ���ļ�����Ψһ�ı�� 0 Ĭ���ļ� 1-30�û��ļ� */
}

/*
 * ��������init_default_file
 * ����  ����ʼ��Ĭ���ļ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_default_file(void)
{
	default_file.num = 0; /* �ļ���ţ�ÿ���ļ�����Ψһ�ı�� 0 Ĭ���ļ� 1-30�û��ļ� */
    strcpy(default_file.name, "DEFAULT"); /* �ļ����� */
// 	strcpy(default_file.dut_name,"????????"); /* ����Ʒ���� */
// 	default_file.work_mode = N_MODE; /* 1��ʾN 2��ʾG */
// 	default_file.hold_pass_time = 0; /* pass�źű���ʱ�� */
// 	default_file.pass_buzzing_time = 10; /* pass����������ʱ�� */
// 	default_file.arc_mode = ARC_CUR_MODEL; /* 1����ģʽ2�ȼ�ģʽ */
	default_file.total = 0; /* ��Ÿ��ļ���һ�������˼��� */
	default_file.create_time = 0; /* ����ʱ�� */
}

/*
 * ��������del_file
 * ����  ��ɾ��ָ���ļ�
 * ����  ��file_numҪɾ�����ļ����
 * ���  ����
 * ����  ����
 */
void del_file(const uint8_t file_num)
{
    if(file_num >= MAX_FILES)
    {
        return;
    }
    
    clear_step_used_flag();/* ��ձ�� */
    clear_group_table();/* ����ļ���ӳ��� */
    
    clear_one_file(file_num);/* ����ļ����� */
    
    save_group_info(file_num);
    
    if(g_cur_file->num == file_num)
    {
        g_cur_file = &file_table[0];
        
		sys_flag.last_group = g_cur_file->num;/* �������ʹ�õ��ļ���־ */
		save_sys_flag();
    }
    
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
}

/*
 * ��������del_all_file
 * ����  ��ɾ�����е��ļ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void del_all_file(void)
{
    int32_t i = 0;
    
    default_file = file_table[0];/* ���� */
    init_file_table();
    
    clear_step_used_flag();/* ��ձ�� */
    clear_group_table();/* ����ļ���ӳ��� */
    
    for(i = 1; i < MAX_FILES; i++)
    {
        if(file_table[i].name[0] != 0)
        {
            save_step_used_flag(i);
            save_group_table(i);
        }
    }
    
    /* ȫ��ɾ��ʱ��ֱ�ӽ���ǰ�ļ�ָ��ָ��Ĭ���ļ� */
    file_table[0] = default_file;/* �ָ����� */
    g_cur_file = &file_table[0];
    read_step_used_flag(g_cur_file->num);
    read_group_table(g_cur_file->num);
    
    save_all_file();/* ���������ļ� */
    
    sys_flag.last_group = 0;/* �����һ��ʹ�õ��ļ���ΪĬ�� */
    save_sys_flag();
}

/*
 * ��������init_99xx_list
 * ����  ����ʼ�����Բ�����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void init_99xx_list(void)
{
	int8_t i = 0;
	NODE_STEP	*first = NULL;
	NODE_STEP	*second = NULL;
    
	//���յĲ��Բ��س�ʼ��Ϊһ������������ͷ��free_list_steps   file_table[MAX_FILES];
	memset(test_group.test_steps,0,sizeof(NODE_STEP)*STEP_LIST_SIZE);
	
	for(i = 0; i < STEP_LIST_SIZE - 1; i++)
	{
		first  = &test_group.test_steps[i];
		second = &test_group.test_steps[i + 1];
		first->next = second;
	}
    
	first  = &test_group.test_steps[STEP_LIST_SIZE - 1];//С��Խ�磬֮ǰд��MAX_STEPS��������벻���ĺ���������������Ե�
	
	first->next = NULL;
	
	free_list_steps = &test_group.test_steps[0];	//���ղ�����ͷָ��test_steps[0]
	
	list_99xx.head = NULL;
	list_99xx.tail = NULL;
	list_99xx.size = 0;
}

/*
 * ��������init_acw_step
 * ����  ����ʼ��ACW���Բ�
 * ����  ��pָ��ָ�����ڵ�����
 * ���  ����
 * ����  ����
 */
void init_acw_step(NODE_STEP * p)
{
	ACW_STRUCT 	l_acw;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	
	define_cur_kinds(ACW, l_gear_buf, l_flag);/* �������� */
	
	memset(&l_acw, 0, sizeof l_acw);
	
	l_acw.step = p->one_step.com.step;
	l_acw.mode = ACW;
	l_acw.testing_voltage = ACW_VOL_L;
	l_acw.gear_i = l_flag[0];
	l_acw.upper_limit = 500;
	l_acw.lower_limit = 0;
	l_acw.ac_real_cur = 0;
	l_acw.arc_sur = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_acw.output_freq = 1;/* 50hz */
	}
	/* ����ģʽ */
	else
	{
		l_acw.output_freq = 500;/* 50hz */
	}
	
	l_acw.rise_time = type_spe.rise_time_min_value;
	l_acw.testing_time = 30;
	l_acw.fall_time = 0;
	l_acw.interval_time = 0;
	l_acw.steps_pass = 1;
	l_acw.steps_cont = 0;
	
	l_acw.offset_cur = 0;
	l_acw.offset_real = 0;
	l_acw.offset_result = 0;
    
	if(g_cur_file->work_mode == G_MODE)
	{
		l_acw.steps_pass = 0;
		l_acw.steps_cont = 1;
	}
    
    l_acw.port.num = type_spe.port_num;
    
	p->one_step.acw = l_acw;
}

/*
 * ��������init_cc_step
 * ����  ����ʼ��CC���Բ�
 * ����  ��pָ��ָ�����ڵ�����
 * ���  ����
 * ����  ����
 */
void init_cc_step(NODE_STEP * p)
{
	CC_STRUCT 	l_cc;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	
	define_cur_kinds(CC, l_gear_buf, l_flag);/* �������� */
	
	memset(&l_cc, 0, sizeof l_cc);
	
	l_cc.step = p->one_step.com.step;
	l_cc.mode = CC;
	l_cc.testing_voltage = CC_VOL_L;
	l_cc.gear_i = l_flag[0];
	l_cc.upper_limit = 500;//ac_gear[get_cc_max_gear()].high_max;//;
	l_cc.lower_limit = 50;
	l_cc.ac_real_cur = 0;
	l_cc.cur_intensity = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_cc.output_freq = 1;/* 50hz */
	}
	/* ����ģʽ */
	else
	{
		l_cc.output_freq = 500;/* 50hz */
	}
	
	l_cc.rise_time = 0;
	l_cc.testing_time = 1;
	l_cc.fall_time = 0;
	l_cc.interval_time = 0;
	l_cc.steps_pass = 1;
	l_cc.steps_cont = 0;
	
	l_cc.offset_cur = 0;
	l_cc.offset_real = 0;
	l_cc.offset_result = 0;
    
	if(g_cur_file->work_mode == G_MODE)
	{
		l_cc.steps_pass = 0;
		l_cc.steps_cont = 1;
	}
    
    l_cc.port.num = type_spe.port_num;
    
	p->one_step.cc = l_cc;
}
/*
 * ��������init_dcw_step
 * ����  ����ʼ��DCW���Բ�
 * ����  ��pָ��ָ�����ڵ�����
 * ���  ����
 * ����  ����
 */
void init_dcw_step(NODE_STEP * p)
{
	DCW_STRUCT 	l_dcw;
	const uint8_t *l_gear_buf[CUR_KINDS_MAX];
	uint8_t l_flag[CUR_KINDS_MAX];
	uint8_t l_kinds;
	
	l_kinds = define_cur_kinds(DCW, l_gear_buf, l_flag);/* �������� */
	memset(&l_dcw, 0, sizeof l_dcw);
	
	l_dcw.step = p->one_step.com.step;
	l_dcw.mode = DCW;
	l_dcw.testing_voltage = DCW_VOL_L;
	l_dcw.gear_i = l_flag[l_kinds-1];
	l_dcw.upper_limit = 500;
	l_dcw.lower_limit = 0;
	l_dcw.charge_cur = 0;
	l_dcw.arc_sur = 0;
	l_dcw.delay_time = 0;
	l_dcw.rise_time = 0;
	l_dcw.stab_time = 0;
	l_dcw.testing_time = 30;
	l_dcw.fall_time = 0;
	l_dcw.interval_time = 0;
	l_dcw.steps_pass = 1;
	l_dcw.steps_cont = 0;
	
	l_dcw.offset_cur = 0;
	l_dcw.offset_result = 0;
	
	if(g_cur_file->work_mode == G_MODE)
	{
		l_dcw.steps_pass = 0;
		l_dcw.steps_cont = 1;
	}
    
    l_dcw.port.num = type_spe.port_num;
	p->one_step.dcw = l_dcw;
}

/*
 * ��������init_ir_step
 * ����  ����ʼ��IR���Բ�
 * ����  ��pָ��ָ�����ڵ�����
 * ���  ����
 * ����  ����
 */
void init_ir_step(NODE_STEP * p)
{
	IR_STRUCT 	l_ir;
	
	memset(&l_ir, 0, sizeof l_ir);
	
	l_ir.step = p->one_step.com.step;
	l_ir.mode = IR;
	l_ir.testing_voltage = IR_VOL_L;
	l_ir.auto_shift_gears = 1;
	l_ir.upper_limit = 0;
	l_ir.lower_limit = IR_RES_L;
	l_ir.rise_time = 0;
	l_ir.testing_time = 30;
	l_ir.delay_time = 0;
	l_ir.interval_time = 0;
	l_ir.steps_pass = 1;
	l_ir.steps_cont = 0;
	
    l_ir.port.num = type_spe.port_num;
	p->one_step.ir = l_ir;
}

/*
 * ��������init_gr_step
 * ����  ����ʼ��GR���Բ�
 * ����  ��pָ��ָ�����ڵ�����
 * ���  ����
 * ����  ����
 */
void init_gr_step(NODE_STEP * p)
{
	GR_STRUCT 	l_gr;
	const uint8_t *l_gear_buf[VOL_KINDS_MAX];
	uint8_t l_flag[VOL_KINDS_MAX];
	
	memset(l_flag, 0, sizeof l_flag);
	memset(&l_gr, 0, sizeof l_gr);
	define_vol_kinds(GR, l_gear_buf, l_flag);/* �������� */
	
	l_gr.step = p->one_step.com.step;
	l_gr.mode = GR;
	l_gr.voltage_gear = l_flag[0];
	l_gr.testing_cur = GR_CUR_L;
	l_gr.upper_limit = 1000;
	l_gr.lower_limit = 0;
	l_gr.testing_time = 30;
	l_gr.interval_time = 0;
	l_gr.steps_pass = 1;
	l_gr.steps_cont = 0;
    
	if(type_spe.gr_hz_type == HZ_TYPE_GRADE)
	{
		l_gr.output_freq = 1;
	}
	else
	{
		l_gr.output_freq = 500;
	}
    
	l_gr.test_method = DC_GR_CONT;
	
	l_gr.offset_res = 0;
	l_gr.offset_result = 0;
	
	p->one_step.gr = l_gr;
}

/*
 * ��������init_bbd_step
 * ����  ����ʼ��BBD���Բ�
 * ����  ��pָ��ָ�����ڵ�����
 * ���  ����
 * ����  ����
 */
void init_bbd_step(NODE_STEP * p)
{
	BBD_STRUCT 	l_bbd;
	
	memset(&l_bbd, 0, sizeof l_bbd);
	
	l_bbd.step = p->one_step.com.step;
	l_bbd.mode = BBD;
	l_bbd.testing_voltage = 100;
	l_bbd.open_ratio = 50;/* ��ʾ10% ��������Ϊ�� ���ưٷֱ���1%���� */
	l_bbd.short_ratio = 120;/* ��ʾ120% */
	l_bbd.cap_value = 0;
	l_bbd.testing_time = 30;
	l_bbd.interval_time = 0;
	l_bbd.steps_pass = 1;
	l_bbd.steps_cont = 0;
	l_bbd.output_freq = 0;
    
	if(type_spe.hz_type == HZ_TYPE_GRADE)
	{
		l_bbd.output_freq = 9;/* 400hz */
	}
	/* ����ģʽ */
	else
	{
		l_bbd.output_freq = 4000;/* 400hz */
	}
    
	l_bbd.gear = AC_20mA;
	l_bbd.get_cs_ok = 0;
	l_bbd.offset_cap = 0;/* ƫ�Ƶ��� */
	l_bbd.offset_result = 0;/* ƫ�Ƶ���������� 0Ĭ�� 1ʧ�� 2�ϸ� */
	
    l_bbd.port.num = type_spe.port_num;
	p->one_step.bbd = l_bbd;
}

/*
 * ��������insert_node
 * ����  ������ڵ�
 * ����  ��step�����
 * ���  ����
 * ����  ����
 */
void insert_node(uint8_t step)
{
    NODE_STEP nod;
	NODE_STEP	*node = NULL;
	NODE_STEP	*cur = NULL;
	
	cur = free_list_steps;	/* ��ȡ�ղ� */
	nod.one_step.com.step = step+1;//���뵽��ǰ���ĺ���
	init_acw_step(&nod);
    
	if(cur)
	{
		free_list_steps = cur->next;
		cur->one_step = nod.one_step;
	}
	else
	{
		return;//������
	}
	
    
    if(NULL != list_99xx.head)
    {
        for(node = list_99xx.head; NULL != node; node = node->next)
        {
            if(node->one_step.com.step == step)
            {
                break;
            }
        }
    }
    else
    {
        list_99xx.head = cur;
    }
	
	cur->prev = node;
    
    if(NULL != node)
    {
        cur->next = node->next;
    }
    else
    {
        cur->next = NULL;
    }
    
    if(NULL != node)
    {
        if(node->next)
        {
            node->next->prev = cur;
        }
        else
        {
            list_99xx.tail = cur;
        }
        
        node->next = cur;
    }
    else
    {
        list_99xx.tail = cur;
    }
    
	for(node=cur->next; node; node = node->next)
	{
		node->one_step.com.step++;//���½ڵ��Ľڵ�Ĳ���ȫ����1
	}
	
	list_99xx.size++;//�ܲ�����1
}

/*
 * ��������insert_node
 * ����  ��������Բ�
 * ����  ��pos �����λ�� 
 *         mode ����ģʽ
 * ���  ����
 * ����  ����
 */
void insert_step(uint8_t pos, uint8_t mode)
{
	NODE_STEP node;
    uint16_t l_index;
    int16_t i = 0;
    uint16_t free_step = TABLE_VALUE_NULL;
    
    if(pos > (MAX_STEPS-1))
    {
        return;
    }
    
    if(g_cur_file == NULL)
    {
        return;
    }
    
    if(g_cur_file->total == 0)
    {
        clear_step_used_flag();
    }
    
    for(i = 0; i < (MAX_STEPS/8+1); i++)
    {
        l_index = step_used_flag[i];
        
        if(l_index != 0xff)
        {
            free_step = cs99xx_maptbl[l_index];
            step_used_flag[i] |= (1<<free_step);/* ����־��һ */
            free_step = free_step + i * 8;/* ��ȡ�ղ� */
            break;
        }
    }
    
    if(free_step == TABLE_VALUE_NULL)
    {
        return;
    }
    
//     memcpy(&cur_group_table[pos + 1], &cur_group_table[pos], (g_cur_file->total - pos) * 2); //Ī�����������
//     cur_group_table[pos] = free_step;
    
    for(i = g_cur_file->total; i > pos; i--)
    {
        cur_group_table[i] = cur_group_table[i - 1];
    }
    
    cur_group_table[pos] = free_step;
    
    g_cur_file->total++;
    
    node.one_step.com.step = pos + 1;
	node.one_step.com.mode = mode;
    
    init_mode(&node);/* ��ʼ���ող���Ĳ� */
    save_one_step(&node, node.one_step.com.step);/* ���� */
}

/*
 * ��������del_step
 * ����  ��ɾ�����Բ�
 * ����  ��step Ҫɾ���Ĳ����
 * ���  ����
 * ����  ����
 */
void del_step(uint8_t step)
{
    uint16_t l_index = TABLE_VALUE_NULL;
    uint16_t l_index1 = 0;
    uint16_t pos = step - 1;
    
    if(step > MAX_STEPS)
    {
        return;
    }
    
    if(g_cur_file->total == 1)
    {
        return;
    }
    
    if(g_cur_step == NULL)
    {
        return;
    }
    
    l_index = cur_group_table[pos];
    l_index1 = l_index / 8;
    
    if(l_index == TABLE_VALUE_NULL)
    {
        return;
    }
    
    step_used_flag[l_index1] &= ~(1<<(l_index%8));/* ����־���� */
    
    memcpy(&cur_group_table[pos], &cur_group_table[pos + 1], (g_cur_file->total - step) * 2);
    cur_group_table[g_cur_file->total - 1] = TABLE_VALUE_NULL;
    
    g_cur_file->total--;
}
/*
 * ��������del_all_steps
 * ����  ��ɾ�����в��Բ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void del_all_steps(void)
{
    int32_t i = 0;
    int32_t num = g_cur_file->total;
    
    for(i = 0; i < num; i++)
    {
        del_step(i + 1);
    }
}

/*
 * ��������swap_step
 * ����  �������������Բ�����
 * ����  ��one ��һ������
 *         two �ڶ�������
 * ���  ����
 * ����  ����
 */
void swap_step(const uint16_t one, const uint16_t two)
{
    uint16_t temp = 0;
    uint16_t l_addr1 = TABLE_VALUE_NULL;
    uint16_t l_addr2 = TABLE_VALUE_NULL;
    
    if(one == two)
    {
        return;
    }
    
    if(one > g_cur_file->total || one == 0)
    {
        return;
    }
    
    if(two > g_cur_file->total || two == 0)
    {
        return;
    }
    
    l_addr1 = cur_group_table[one - 1];
    l_addr2 = cur_group_table[two - 1];
    
    if(l_addr1 == TABLE_VALUE_NULL || l_addr2 == TABLE_VALUE_NULL)
    {
        return;
    }
    
    /* �������� */
    temp = l_addr1;
    l_addr1 = l_addr2;
    l_addr2 = temp;
    
    /* �ѽ������ӳ�����ݱ��浽ӳ����� */
    cur_group_table[one - 1] = l_addr1;
    cur_group_table[two - 1] = l_addr2;
    
    save_group_table(g_cur_file->num);/* �Ѹ��Ĺ���ӳ���д�뵽�洢�� */
}

/*
 * ��������check_step_exist_for_comm
 * ����  �������Բ��Ƿ����
 * ����  ��pos Ҫ���Ĳ����
 * ���  ��0 ����
 *         -1 �洢��Ϣ����
 *         -2 ����ŷǷ�
 * ����  ����
 */
int32_t check_step_exist_for_comm(uint16_t pos)
{
    uint16_t l_addr = TABLE_VALUE_NULL;
    
    if(pos > MAX_STEPS)
    {
        return -2;
    }
    
    l_addr = cur_group_table[pos - 1];
    
    if(l_addr == TABLE_VALUE_NULL)
    {
        return -1;
    }
    
    return 0;
}
/*
 * ��������position_step
 * ����  ����λָ���Ĳ��Բ�
 * ����  ��pos Ҫ��λ�Ĳ����
 * ���  ����
 * ����  ����λ���Ĳ��ڵ����ݽṹָ�� ���ΪNULL����Ĳ���pos�Ƿ�
 */
NODE_STEP *position_step(uint16_t pos)
{
    uint16_t l_addr = TABLE_VALUE_NULL;
    
    if(pos > MAX_STEPS)
    {
        return NULL;
    }
    
    l_addr = cur_group_table[pos - 1];
    
    if(l_addr != TABLE_VALUE_NULL)
    {
        load_steps_to_list(pos);
    }
    
	return list_99xx.head;
}

/*
 * ��������get_group_total_step
 * ����  ����ȡָ����������ܲ���
 * ����  ��note_num ��������
 * ���  ����
 * ����  ��������
 */
uint32_t get_group_total_step(uint16_t note_num)
{
    return file_table[note_num % MAX_FILES].total;
}

uint16_t get_cur_file_total(void)
{
    return g_cur_file->total;
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
