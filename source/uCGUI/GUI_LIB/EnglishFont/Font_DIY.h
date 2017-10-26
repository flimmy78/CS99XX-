/******************************************************************************
 * �ļ���Ϣ :  
 *
 * �� �� �� :  
 *
 * �������� :  
 * 
 * ԭʼ�汾 : 
 *     
 * �޸İ汾 :  
 *    
 * �޸����� : 
 *
 * �޸����� :
 * 
 * �� �� �� :
 *
 * ��    ע :
 *
 * ��    �� :   Դ����
 *
 * ��    Ȩ :   �Ͼ���ʢ�������޹�˾ , Copyright Reserved
 * 
******************************************************************************/

/******************************************************************************
 *                              ͷ�ļ���ʿ
******************************************************************************/
 
#ifndef    FONT_DIY_H
#define    FONT_DIY_H


#if defined(__cplusplus)

    extern "C" {     /* Make sure we have C-declarations in C++ programs */

#endif

/******************************************************************************
 *                             �����ļ�����
******************************************************************************/ 

#ifndef    INCLUDES_H

#include "GUI.H" 
#include "picture.h"
    
#endif

// Ӣ������
extern const GUI_FONT GUI_FontSize1;
extern const GUI_FONT GUI_FontSize2;
extern const GUI_FONT GUI_FontSize3;
extern const GUI_FONT GUI_FontSize4;
        

// ��������
extern const GUI_FONT GUI_FontHZ_SimSun_12;     /* ��Ҫ�������ý�����ʾ���� */
extern const GUI_FONT GUI_FontHZ_SimSun_13;     /* ��Ҫ�������ý�����ʾӢ�� */
extern const GUI_FONT GUI_FontHZ_SimSun_15;     /* ���Գ���ʹ������ */
extern const GUI_FONT GUI_FontHZ_SimSunF_15;    /* ���Գ���ʹ�õ�λ */
extern const GUI_FONT GUI_FontHZ_SimSun_16;     /* ���Խ���״̬��ʾ */

extern const GUI_FONT GUI_FontHZ_Garamond_16;   /* ���� Ӣ�������� */
extern const GUI_FONT GUI_FontHZ_FangZhengXingKai_24;	/* �����桮��ʢ������ */
extern const GUI_FONT GUI_FontHZ_MS_Sans_Serif_24;//�Զ�У׼������ʾPASS


/******************************************************************************
 *                            �ļ��ӿں궨��
******************************************************************************/

// Ӣ������1X��Y����ߴ�
#define    FONT_SIZE1_NUM_XSIZE                          8	    // ����
#define    FONT_SIZE1_NUM_XDIST			                 8
#define    FONT_SIZE1_NUM_BYTES_PER_LINE                 1

#define    FONT_SIZE1_LOWERCASE_XSIZE                    8	    // Сд��ĸ
#define    FONT_SIZE1_LOWERCASE_XDIST			         8
#define    FONT_SIZE1_LOWERCASE_BYTES_PER_LINE           1

#define    FONT_SIZE1_CAPITAL_XSIZE                      8	    // ��д��ĸ
#define    FONT_SIZE1_CAPITAL_XDIST			             8
#define    FONT_SIZE1_CAPITAL_BYTES_PER_LINE             1

#define    FONT_SIZE1_YSIZE			                     16
#define    FONT_SIZE1_YDIST			                     16


// Ӣ������2X��Y����ߴ�
#define    FONT_SIZE2_LOWERCASE_XSIZE                    24	    // Сд��ĸ
#define    FONT_SIZE2_LOWERCASE_XDIST			         24
#define    FONT_SIZE2_LOWERCASE_BYTES_PER_LINE           3

#define    FONT_SIZE2_CAPITAL_XSIZE                      24	    // ��д��ĸ
#define    FONT_SIZE2_CAPITAL_XDIST			             24
#define    FONT_SIZE2_CAPITAL_BYTES_PER_LINE             3

#define    FONT_SIZE2_YSIZE			                     41
#define    FONT_SIZE2_YDIST			                     38
 

// Ӣ������3X��Y����ߴ�
#define    FONT_SIZE3_NUM_XSIZE                          32	    // ����
#define    FONT_SIZE3_NUM_XDIST			                 32
#define    FONT_SIZE3_NUM_BYTES_PER_LINE                 4

#define    FONT_SIZE3_LOWERCASE_XSIZE                    32	    // Сд��ĸ
#define    FONT_SIZE3_LOWERCASE_XDIST			         32
#define    FONT_SIZE3_LOWERCASE_BYTES_PER_LINE           4

#define    FONT_SIZE3_CAPITAL_XSIZE                      40	    // ��д��ĸ
#define    FONT_SIZE3_CAPITAL_XDIST			             40
#define    FONT_SIZE3_CAPITAL_BYTES_PER_LINE             5

#define    FONT_SIZE3_YSIZE			                     65
#define    FONT_SIZE3_YDIST			                     65


// Ӣ������4X��Y����ߴ�
#define    FONT_SIZE4_NUM_XSIZE                          50	    // ����
#define    FONT_SIZE4_NUM_XDIST			                 50
#define    FONT_SIZE4_NUM_BYTES_PER_LINE                 7

#define    FONT_SIZE4_YSIZE			                     111
#define    FONT_SIZE4_YDIST			                     100


// ��������1X��Y����ߴ�
#define    FONT_CHINESE_SIZE1_XSIZE			             16
#define    FONT_CHINESE_SIZE1_XDIST			             16
#define    FONT_CHINESE_SIZE1_BYTES_PER_LINE             2

#define    FONT_CHINESE_SIZE1_YSIZE			             16
#define    FONT_CHINESE_SIZE1_YDIST			             16


// ��������2X��Y����ߴ�
#define    FONT_CHINESE_SIZE2_XSIZE			             24
#define    FONT_CHINESE_SIZE2_XDIST			             21
#define    FONT_CHINESE_SIZE2_BYTES_PER_LINE             3

#define    FONT_CHINESE_SIZE2_YSIZE			             21
#define    FONT_CHINESE_SIZE2_YDIST			             21


// ��������3X��Y����ߴ�
#define    FONT_CHINESE_SIZE3_XSIZE			             32
#define    FONT_CHINESE_SIZE3_XDIST			             28
#define    FONT_CHINESE_SIZE3_BYTES_PER_LINE             4

#define    FONT_CHINESE_SIZE3_YSIZE			             27
#define    FONT_CHINESE_SIZE3_YDIST			             27


// ��������4X��Y����ߴ�
#define    FONT_CHINESE_SIZE4_XSIZE			             40
#define    FONT_CHINESE_SIZE4_XDIST			             37
#define    FONT_CHINESE_SIZE4_BYTES_PER_LINE             5

#define    FONT_CHINESE_SIZE4_YSIZE			             37
#define    FONT_CHINESE_SIZE4_YDIST			             37
/******************************************************************************
 *                            �ļ��ӿں�������
******************************************************************************/




#if defined(__cplusplus)

    }

#endif 


#endif
/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
