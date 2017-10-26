
#ifndef __MB_SERVER_H__
#define __MB_SERVER_H__


#include    "mb.h"
#include    "mb_m.h"
#include    "user_mb_app.h"

#define USB_PACKAGE_SIZE      100 /* U�����ݵ��뵼�����Ĵ�С */


enum{
	
    R_TIME, /* ����ʱ�� */
    T_TIME, /* ����ʱ�� */
    F_TIME, /* �½�ʱ�� */
    D_TIME, /* ��ʱʱ�� */
    I_TIME, /* ���ʱ�� */
    C_TIME = I_TIME,/* ���û���ʱ�� ͬ���ʱ�乲�ñ��� */
	
/* ϵͳ���� */
	SYS_CONTRAST = 0,/* Һ���Աȶ� */
	SYS_VOLUME, /* ϵͳ���� */
	SYS_RES_ALLOWANCE_HINT,/* ������ʾ */
	SYS_RES_SAVE,/* ������� */
	SYS_RES_COVER,/* ���������� */
	SYS_GFI,/* GFI�������� */
	SYS_PHV,/* Ԥ����ѹ */
	SYS_TEST_SIGNAL,/* �����ź� */
	SYS_SELF_CHECK,/* �Լ����� */
	SYS_CHECK_HINT,/* ������ʾ */
	SYS_TEST_PORT,/* ���Զ˿� */
	SYS_LANGUAGE,/* ���� */
	SYS_FAIL_CONT,/* ʧ�ܼ��� */
	SYS_KEY_LOCK,/* ������ */
	SYS_NUM_RULER,/* ��Ź��� */
    
    
    RES_USED_NUM=0,
    RES_FREE_NUM,
    RES_TOTAL_NUM,
    RES_PASS_NUM,
    RES_FAIL_NUM,
};



extern USHORT   usSRegHoldBuf[S_REG_HOLDING_NREGS];
extern USHORT   usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];

extern void user_read_write_mult_hold_reg_hook_m(uint8_t slove_num);
extern void user_read_write_mult_hold_reg_hook_s(void);

extern void m_power_checkres_dispose(uint16_t cmd, uint16_t *data); /* ��Դ��� */

extern void m_comm_addr_dispose(uint16_t cmd, uint16_t *data);       /*����λ��ͨ�ŵ�ַ*/ 
extern void m_comm_ctrl_status_dispose(uint16_t cmd, uint16_t *data);       /*����״̬*/

extern void m_control_test_button_dispose(uint16_t cmd, uint16_t *data);       /*���ò���״̬*/
extern void m_delete_file_dispose(uint16_t cmd, uint16_t *data);       /*ɾ���ļ�������Ϊ0ɾ�������ļ�����Ϊ0ɾ�������ļ�*/
extern void m_query_active_file_num_dispose(uint16_t cmd, uint16_t *data);       /*��ȡ�ƶ��ļ�Ϊ��ļ�*/  
extern void m_query_active_step_num_dispose(uint16_t cmd, uint16_t *data);       /*��ȡ��ǰ����Ϊ�����*/
extern void m_catalog_file_dispose(uint16_t cmd, uint16_t *data);       /*��ѯ�ļ��Ƿ����*/ 
extern void m_insert_step_dispose(uint16_t cmd, uint16_t *data);       /*������Բ���*/    
extern void m_delete_step_dispose(uint16_t cmd, uint16_t *data);       /*ɾ����ǰ�����*/
extern void m_active_step_front_dispose(uint16_t cmd, uint16_t *data);       /*��ǰ�����ǰ��һλ*/  
extern void m_active_step_behind_dispose(uint16_t cmd, uint16_t *data);       /*��ǰ��������һλ*/
extern void m_inter_change_step_dispose(uint16_t cmd, uint16_t *data);       /*����������Ϣ*/
extern void m_updata_step_mode_dispose(uint16_t cmd, uint16_t *data);       /*�޸ĵ�ǰ��ļ���ģʽ*/

extern void m_system_nrule_dispose(uint16_t cmd, uint16_t *data);
extern void m_system_time_dispose(uint16_t cmd, uint16_t *data);
extern void m_pass_word_dispose(uint16_t cmd, uint16_t *data);
extern void m_keyklock_dispose(uint16_t cmd, uint16_t *data);
extern void m_fail_continue_dispose(uint16_t cmd, uint16_t *data);
extern void m_sys_language_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_port_dispose(uint16_t cmd, uint16_t *data);
extern void m_reset_stop_dispose(uint16_t cmd, uint16_t *data);/* ��λ��ͣ */
extern void m_check_hint_dispose(uint16_t cmd, uint16_t *data); /* ������ʾ */
extern void m_self_check_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_signal_dispose(uint16_t cmd, uint16_t *data);
extern void m_phv_dispose(uint16_t cmd, uint16_t *data);
extern void m_gfi_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_recover_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_save_dispose(uint16_t cmd, uint16_t *data);
extern void m_res_remain_hint_dispose(uint16_t cmd, uint16_t *data); /* ������ʾ */
extern void m_sys_volume_dispose(uint16_t cmd, uint16_t *data);/* ϵͳ���� */
extern void m_sys_contrast_dispose(uint16_t cmd, uint16_t *data);/* ��Ļ�Աȶ� */
extern void m_dut_name_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_fetch_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_clear_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_fail_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_pass_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_used_num_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_remain_num_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_total_num_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_ports_dispose(uint16_t cmd, uint16_t *data);
extern void m_ir_auto_shift_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_steps_cont_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_steps_pass_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_delay_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_change_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_inter_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_fall_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_test_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_rise_t_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_frequency_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_arc_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_current_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_charge_current_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_real_current_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_offset_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_low_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_high_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_cur_gear_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_voltage_dispose(uint16_t cmd, uint16_t *data);
extern void m_com_mode_dispose(uint16_t cmd, uint16_t *data);
extern void m_bbd_open_check_dispose(uint16_t cmd, uint16_t *data);
extern void m_bbd_short_check_dispose(uint16_t cmd, uint16_t *data);
extern void m_swap_steps_dispose(uint16_t cmd, uint16_t *data);
extern void m_move_forward_dispose(uint16_t cmd, uint16_t *data);
extern void m_move_backward_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_mode_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_fetch_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_cap_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_offset_dispose(uint16_t cmd, uint16_t *data);
extern void m_plc_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_test_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_del_all_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_step_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_file_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_set_active_step_dispose(uint16_t cmd, uint16_t *data);
extern void m_set_active_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_save_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_edit_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_new_file_dispose(uint16_t cmd, uint16_t *data);
extern void m_id_msg_dispose(uint16_t cmd, uint16_t *data);
extern void m_rst_dispose(uint16_t cmd, uint16_t *data);
extern void m_tester_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_comm_bps_dispose(uint16_t cmd, uint16_t *data);
extern void m_comm_sw_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_plc_sw_status_dispose(uint16_t cmd, uint16_t *data);









extern void m_sys_par_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_sys_flag_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_file_list_usb_dispose(uint16_t cmd, uint16_t *data);
// extern void m_map_table_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_step_used_flag_base_dispose(uint16_t cmd, uint16_t *data);
// extern void m_step_used_flag_end_dispose(uint16_t cmd, uint16_t *data);
extern void m_group_table_base_dispose(uint16_t cmd, uint16_t *data);
// extern void m_group_table_end_dispose(uint16_t cmd, uint16_t *data);
// extern void m_group_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_mem_group_base_usb_dispose(uint16_t cmd, uint16_t *data);
// extern void m_mem_grop_end_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_data_loading_end_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_info_dispose(uint16_t cmd, uint16_t *data);
extern void m_result_one_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_sys_par_len_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_file_steps_num_usb_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_tester_type_usb_dispose(uint16_t cmd, uint16_t *data);


extern void m_usb_connect_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_cylinder_ctrl_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_auto_cal_dispose(uint16_t cmd, uint16_t *data);
extern void m_get_interface_info_dispose(uint16_t cmd, uint16_t *data);



extern void m_cal_enter_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_exit_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_status_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_mode_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_range_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_start_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_stop_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_measure_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_points_dispose(uint16_t cmd, uint16_t *data);
extern void m_cal_load_dispose(uint16_t cmd, uint16_t *data);

#endif //__MB_SERVER_H__
