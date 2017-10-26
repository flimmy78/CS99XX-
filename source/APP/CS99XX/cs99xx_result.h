/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_result.h
 * ժ  Ҫ  �������Ϣ��������ʾ
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#ifndef __CS99XX_RESULT_H__
#define __CS99XX_RESULT_H__

#include 	"app.h"

extern uint8_t getTestMode(uint8_t mode_num);
extern uint32_t get_res_total(void);
extern void res_list_dis(void);
extern void save_cur_result(RESULT_STR *res);
extern void save_result(RESULT_STR *res, const int8_t stat);
extern int32_t wait_usb_connect(void);
extern void updata_result(uint8_t mode);
extern void result_acw(void);
extern void result_dcw(void);
extern void result_ir(void);
extern void result_gr(void);
extern void result_bbd(void);
extern void result_cc(void);
extern void ch_out_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str);
extern void ch_loop_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str);
extern void change_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str, uint16_t n);
extern void change_result_to_string_comm(const RESULT_STR *res, char* res_buf);


#endif //__CS99XX_RESULT_H__

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
