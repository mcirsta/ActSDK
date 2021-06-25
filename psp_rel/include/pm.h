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
 * \brief    功耗管理宏定义和函数接口声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __PM_H__
#define __PM_H__

/*!
 * \brief
 *      频点等级数
 */
#define FREQLEVEL   7

/*!
 * \brief
 *      最大负载应用时钟域值(video/emulator/dc/dv等)
 */
#define CLK_LOAD_MAX    0xff

/*!
 * \brief
 *      高负载应用时钟域值3(record等)
 */
#define CLK_LOAD_HIGH_3    6
/*!
 * \brief
 *      高负载应用时钟域值0
 */
#define CLK_LOAD_HIGH_0 3

/*!
 * \brief
 *      中等负载应用时钟域值(photo等)
 */
#define CLK_LOAD_MID_0  2
/*!
 * \brief
 *      轻负载应用时钟域值(music/ebook/setting/browser等)
 */
#define CLK_LOAD_LOW_1  1
#define CLK_LOAD_LOW_0  0
/*!
 * \brief
 *      最小负载应用时钟域值
 */
#define CLK_LOAD_MIN    0

/*!
 * \brief
 *      同时支持时钟回调函数模块的最大值
 */
#define NR_CLKS     16

#ifndef _ASSEMBLER_

#include "ucos/time.h"
/*!
 *  ioctl命令字 \n
 *  \li SET_APP_STATUS : 设置应用状态
 *  \li GET_APP_STATUS : 获取应用状态
 *  \li CHANGE_COREPLL : 调频
 *  \li SET_VOLTAGE : 调压
 *  \li GET_CLOCK : 获取时钟信息（参考GET_CLOCK命令字）
 *  \li SET_CLK_DOMAIN : 设置时钟域
 *  \li CLEAR_CLK_DOMAIN : 清除时钟域
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
 *  电压类型 \n
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
 *  vdd可设置值 \n
 *  \li VDD13 : 1.30v
 *  \li VDD135: 1.35v
 *  \li 其他  : 照此类推
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
 *  vcc可设置值 \n
 *  \li VCC26 : 2.6v
 *  \li VCC27 : 2.7v
 *  \li 其他  : 照此类推
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
 *  调频模块时钟序号 \n
 *  \li 0~15 : 最大支持16个模块
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
 *  GET_CLOCK命令字 \n
 *  \li GET_COREPLL : 获取corepll值
 *  \li GET_CCLK : 获取cpu clk
 *  \li GET_SCLK  : 获取bus clk
 *  \li GET_PCLK : 获取获取 perpheral clk
 *  \li GET_CLK_LEVEL  : 获取当前频率档值(例如：0/1/2/3/4/5)
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
 *  调频命令字 \n
 *  \li CHANGEPLL_START : 开始调频
 *  \li CHANGEPLL_END : 调频结束
 *
 */
enum
{
    CHANGEPLL_START,
    CHANGEPLL_END
};

/*!
 *  调频方向 \n
 *  \li CHANGEPLL_DOWN : 降频
 *  \li CHANGEPLL_UP : 升频
 *
 */
enum
{
    CHANGEPLL_DOWN,
    CHANGEPLL_UP
};

/*!
 * \brief
 *      当前频点时钟信息数据结构
 */
typedef struct
{
    /*! corepll值*/
    unsigned int corepll;
    /*! cclk分频系数*/
    unsigned int cclk_div;
    /*! sclk分频系数*/
    unsigned int sclk_div;
    /*! pclk分频系数*/
    unsigned int pclk_div;
    /*! 频点cp0_count溢出时间*/
    struct timeval overflow_value;
} curclk_t;

/*!
 * \brief
 *      调频接口参数（对应ioctl命令字CHANGE_COREPLL）
 */
typedef struct
{
    /*! 新频率点*/
    unsigned int newcorepll;
    /*! 旧频率点*/
    unsigned int oldcorepll;
} change_corepll_t;

/*!
 * \brief
 *      频点时钟电压信息数据结构
 */
typedef struct
{
    /*! 频点时钟信息*/
    curclk_t clk;
    /*! 分频系数对应的BUSCLK寄存器的值*/
    unsigned int div;
    /*! 频点要求的vdd值*/
    unsigned int vdd;
    /*! 频点要求的vcc值*/
    unsigned int vcc;
} clk_info_t;

/*!
 * \brief
 *      调频回调数据结构
 */
typedef struct
{
    /*!调频回调函数指针 */
    int (*handler)(unsigned int clk, curclk_t *curclk, void *dev_id,
            unsigned int cmd, unsigned int direction);
    /*!调频回调模块Id */
    void *dev_id;
    /*!调频回调标志 */
    unsigned int flags;
} clk_action_t;

///*!
// * \brief
// *      应用状态数据结构
// */
//typedef struct{
//    /*!应用的级别 */
//    int level;
//    /*!应用的状态：1 为忙，0表示闲，可以进入standby状态 */
//    int status;
//    /*!应用的背光状态：1 表示 需要一直打开， 0 表示可以关闭 */
//    int backlight;
//} app_status_t;

/*!
 * \brief
 *      设置电压数据结构
 */
typedef struct
{
    /*!应用的级别 */
    unsigned int value;
    /*!应用的状态：1 为忙，0表示闲，可以进入standby状态 */
    unsigned int voltage_type;
} voltage_info_t;

/*!
 * \brief
 *      获取时钟的数据结构
 */
typedef struct
{
    /*!获取时钟的命令字(将GET_CLOCK命令字) */
    unsigned int cmd;
    /*!获取结果 */
    unsigned int result;
} clock_info_t;

/*!
 * \brief
 *      时钟域数据结构
 */
typedef struct
{
    /*! 时钟域最小值*/
    unsigned int min;
    /*! 时钟域最大值*/
    unsigned int max;
} clk_domain_t;

extern clk_action_t clk_action[NR_CLKS];
extern clk_info_t clk_info[];
//extern unsigned int current_level;

#ifndef FUCNTION_SYMBOL //通过函数指针方式调用接口时，需增加FUCNTION_SYMBOL宏定义，否则编译会有重复定义错误
extern int request_clkadjust(unsigned int clk, int(*handler)(unsigned int clk,
        curclk_t *curclk, void *dev_id, unsigned int cmd,
        unsigned int direction), void *dev_id);
extern int free_clkadjust(unsigned int clk, void *dev_id);

extern int set_clk_domain(unsigned int min, unsigned int max);
extern int clear_clk_domain(unsigned int min, unsigned int max);

extern unsigned int get_corepll(void);
extern int change_corepll(unsigned int newcorepll, unsigned int oldcorepll);//0/1/2/3/4/5---48MHz/72MHz/120MHz/144MHz/156MHz/168MHz;

extern int set_voltage(unsigned int value, unsigned int voltage_type);//设置vdd/vcc,voltageType---VDD,VCC;value---VDD13~VDD20/VCC26~VCC33
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
