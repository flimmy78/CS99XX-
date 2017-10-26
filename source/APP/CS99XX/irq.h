/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�irq.h
 * ժ  Ҫ  ���жϷ������ӿ�
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#ifndef	__IRQ_H__
#define	__IRQ_H__

void gfi_cycle_clear_count(void);


void stop_irq(void);
void arc_irq(void);
void short_irq(void);
void gfi_irq(void);

void uart5_irq_for_syn_roads(void);
void uart5_irq_for_dc_gr(void);

#endif //__IRQ_H__
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
