/********************************************************************************
 *                              USDK 1100
 *                            Module: PM
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     pm.h
 * \brief    ���Ĺ���궨��ͺ����ӿ�����
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __PM_H__
#define __PM_H__

/*!
 * \brief
 *      Ƶ��ȼ���
 */
#define FREQLEVEL   7

/*!
 * \brief
 *      �����Ӧ��ʱ����ֵ(video/emulator/dc/dv��)
 */
#define CLK_LOAD_MAX    0xff

/*!
 * \brief
 *      �߸���Ӧ��ʱ����ֵ3(record��)
 */
#define CLK_LOAD_HIGH_3    6
/*!
 * \brief
 *      �߸���Ӧ��ʱ����ֵ0
 */
#define CLK_LOAD_HIGH_0 3

/*!
 * \brief
 *      �еȸ���Ӧ��ʱ����ֵ(photo��)
 */
#define CLK_LOAD_MID_0  2
/*!
 * \brief
 *      �Ḻ��Ӧ��ʱ����ֵ(music/ebook/setting/browser��)
 */
#define CLK_LOAD_LOW_1  1
#define CLK_LOAD_LOW_0  0
/*!
 * \brief
 *      ��С����Ӧ��ʱ����ֵ
 */
#define CLK_LOAD_MIN    0

/*!
 * \brief
 *      ͬʱ֧��ʱ�ӻص�����ģ������ֵ
 */
#define NR_CLKS     16

#ifndef _ASSEMBLER_

#include "ucos/time.h"
/*!
 *  ioctl������ \n
 *  \li SET_APP_STATUS : ����Ӧ��״̬
 *  \li GET_APP_STATUS : ��ȡӦ��״̬
 *  \li CHANGE_COREPLL : ��Ƶ
 *  \li SET_VOLTAGE : ��ѹ
 *  \li GET_CLOCK : ��ȡʱ����Ϣ���ο�GET_CLOCK�����֣�
 *  \li SET_CLK_DOMAIN : ����ʱ����
 *  \li CLEAR_CLK_DOMAIN : ���ʱ����
 *
 */
enum
{
    SET_APP_STATUS,
    GET_APP_STATUS,
    CHANGE_COREPLL,
    SET_VOLTAGE,
    GET_CLOCK,
    SET_CLK_DOMAIN,
    CLEAR_CLK_DOMAIN
};

/*!
 *  ��ѹ���� \n
 *  \li VDD : vcc
 *  \li VCC : vdd
 *
 */
enum
{
    VDD,
    VCC
};
/*!
 *  vdd������ֵ \n
 *  \li VDD13 : 1.30v
 *  \li VDD135: 1.35v
 *  \li ����  : �մ�����
 *
 */
enum
{
    VDD13,
    VDD135,
    VDD14,
    VDD145,
    VDD15,
    VDD155,
    VDD16,
    VDD165,
    VDD17,
    VDD175,
    VDD18,
    VDD185,
    VDD19,
    VDD195,
    VDD20
};
/*!
 *  vcc������ֵ \n
 *  \li VCC26 : 2.6v
 *  \li VCC27 : 2.7v
 *  \li ����  : �մ�����
 *
 */
enum
{
    VCC26,
    VCC27,
    VCC28,
    VCC29,
    VCC30,
    VCC31,
    VCC32,
    VCC33
};

/*!
 *  ��Ƶģ��ʱ����� \n
 *  \li 0~15 : ���֧��16��ģ��
 *
 */
enum
{
    CLK_NAND,
    CLK_YVU,
    CLK_SD,
    CLK_MHA,
    CLK_MCA,
    CLK_SERIAL,
    CLK_TIMER,
    CLK_COUNT,
    CLK_I2C,
};

/*!
 *  GET_CLOCK������ \n
 *  \li GET_COREPLL : ��ȡcorepllֵ
 *  \li GET_CCLK : ��ȡcpu clk
 *  \li GET_SCLK  : ��ȡbus clk
 *  \li GET_PCLK : ��ȡ��ȡ perpheral clk
 *  \li GET_CLK_LEVEL  : ��ȡ��ǰƵ�ʵ�ֵ(���磺0/1/2/3/4/5)
 *
 */
enum
{
    GET_COREPLL,
    GET_CCLK,
    GET_SCLK,
    GET_PCLK,
    GET_CLK_LEVEL,
};

/*!
 *  ��Ƶ������ \n
 *  \li CHANGEPLL_START : ��ʼ��Ƶ
 *  \li CHANGEPLL_END : ��Ƶ����
 *
 */
enum
{
    CHANGEPLL_START,
    CHANGEPLL_END
};

/*!
 *  ��Ƶ���� \n
 *  \li CHANGEPLL_DOWN : ��Ƶ
 *  \li CHANGEPLL_UP : ��Ƶ
 *
 */
enum
{
    CHANGEPLL_DOWN,
    CHANGEPLL_UP
};

/*!
 * \brief
 *      ��ǰƵ��ʱ����Ϣ���ݽṹ
 */
typedef struct
{
    /*! corepllֵ*/
    unsigned int corepll;
    /*! cclk��Ƶϵ��*/
    unsigned int cclk_div;
    /*! sclk��Ƶϵ��*/
    unsigned int sclk_div;
    /*! pclk��Ƶϵ��*/
    unsigned int pclk_div;
    /*! Ƶ��cp0_count���ʱ��*/
    struct timeval overflow_value;
} curclk_t;

/*!
 * \brief
 *      ��Ƶ�ӿڲ�������Ӧioctl������CHANGE_COREPLL��
 */
typedef struct
{
    /*! ��Ƶ�ʵ�*/
    unsigned int newcorepll;
    /*! ��Ƶ�ʵ�*/
    unsigned int oldcorepll;
} change_corepll_t;

/*!
 * \brief
 *      Ƶ��ʱ�ӵ�ѹ��Ϣ���ݽṹ
 */
typedef struct
{
    /*! Ƶ��ʱ����Ϣ*/
    curclk_t clk;
    /*! ��Ƶϵ����Ӧ��BUSCLK�Ĵ�����ֵ*/
    unsigned int div;
    /*! Ƶ��Ҫ���vddֵ*/
    unsigned int vdd;
    /*! Ƶ��Ҫ���vccֵ*/
    unsigned int vcc;
} clk_info_t;

/*!
 * \brief
 *      ��Ƶ�ص����ݽṹ
 */
typedef struct
{
    /*!��Ƶ�ص�����ָ�� */
    int (*handler)(unsigned int clk, curclk_t *curclk, void *dev_id,
            unsigned int cmd, unsigned int direction);
    /*!��Ƶ�ص�ģ��Id */
    void *dev_id;
    /*!��Ƶ�ص���־ */
    unsigned int flags;
} clk_action_t;

///*!
// * \brief
// *      Ӧ��״̬���ݽṹ
// */
//typedef struct{
//    /*!Ӧ�õļ��� */
//    int level;
//    /*!Ӧ�õ�״̬��1 Ϊæ��0��ʾ�У����Խ���standby״̬ */
//    int status;
//    /*!Ӧ�õı���״̬��1 ��ʾ ��Ҫһֱ�򿪣� 0 ��ʾ���Թر� */
//    int backlight;
//} app_status_t;

/*!
 * \brief
 *      ���õ�ѹ���ݽṹ
 */
typedef struct
{
    /*!Ӧ�õļ��� */
    unsigned int value;
    /*!Ӧ�õ�״̬��1 Ϊæ��0��ʾ�У����Խ���standby״̬ */
    unsigned int voltage_type;
} voltage_info_t;

/*!
 * \brief
 *      ��ȡʱ�ӵ����ݽṹ
 */
typedef struct
{
    /*!��ȡʱ�ӵ�������(��GET_CLOCK������) */
    unsigned int cmd;
    /*!��ȡ��� */
    unsigned int result;
} clock_info_t;

/*!
 * \brief
 *      ʱ�������ݽṹ
 */
typedef struct
{
    /*! ʱ������Сֵ*/
    unsigned int min;
    /*! ʱ�������ֵ*/
    unsigned int max;
} clk_domain_t;

extern clk_action_t clk_action[NR_CLKS];
extern clk_info_t clk_info[];
//extern unsigned int current_level;

#ifndef FUCNTION_SYMBOL //ͨ������ָ�뷽ʽ���ýӿ�ʱ��������FUCNTION_SYMBOL�궨�壬�����������ظ��������
extern int request_clkadjust(unsigned int clk, int(*handler)(unsigned int clk,
        curclk_t *curclk, void *dev_id, unsigned int cmd,
        unsigned int direction), void *dev_id);
extern int free_clkadjust(unsigned int clk, void *dev_id);

extern int set_clk_domain(unsigned int min, unsigned int max);
extern int clear_clk_domain(unsigned int min, unsigned int max);

extern unsigned int get_corepll(void);
extern int change_corepll(unsigned int newcorepll, unsigned int oldcorepll);//0/1/2/3/4/5---48MHz/72MHz/120MHz/144MHz/156MHz/168MHz;

extern int set_voltage(unsigned int value, unsigned int voltage_type);//����vdd/vcc,voltageType---VDD,VCC;value---VDD13~VDD20/VCC26~VCC33
extern int dis_change_voltage(unsigned int flag);

extern int get_clk_vol_cfg(clk_info_t *buf, unsigned int no);/*No:0~freqLevel-1*/
extern int set_clk_vol_cfg(clk_info_t *buf, unsigned int no);/*No:0~freqLevel-1*/

extern int set_dsp_flag(int flag);
extern int get_dsp_clk(void);
extern int set_dsp_clk(int dsp_clk);

extern unsigned int get_app_status(void);
//extern int set_app_status(app_status_t *app_status);
extern int set_app_status(unsigned int *app_status);

#endif /*FUCNTION_SYMBOL*/

#endif /*_ASSEMBLER_*/
#endif /*__PM_H__*/
