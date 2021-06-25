/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_pm.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-07-17             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     sys_pm.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/07/17
 *******************************************************************************/


#ifndef __SYS_PM_H__
#define __SYS_PM_H__

#include <include_psp.h>

/*! 
 * \defgroup   applib_system_pm   系统电源管理接口 
 */

/*!
 *  \brief 
 *  系统频率信息结构定义
 */
typedef struct 
{
    int corepll;
    int cclk;
    int sclk;
    int pclk;
    int clk_level;
}sys_clk_info_t;

/******************************************************************************/
/*!                    
 * \par  Description:
 *     设置一个频率调整策略 
 * \ingroup applib_system_pm
 * \param[in] domain -- 频率策略(包含最低和最大频率调整值)
 * \return 成功返回true, 失败返回false 
 *******************************************************************************/
bool sys_pm_set_clk_domain(const clk_domain_t * domain);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     清除一个频率调整策略 
 * \ingroup applib_system_pm
 * \param[in] domain -- 频率策略(包含最低和最大频率调整值)
 * \return 成功返回true, 失败返回false 
 * \note 进入一个应用时，可设置一个频率调整策略，当其退出时，必须清除该策略
 *******************************************************************************/
bool sys_pm_clear_clk_domain(const clk_domain_t * domain);

/******************************************************************************/
/*!                    
 * \par  Description:
 *      获取当前系统、总线等频率信息 
 * \ingroup applib_system_pm
 * \param[in] sys_clk_info -- 存放各种频率信息的结构体指针 
 * \return 成功返回true, 失败返回false 
 *******************************************************************************/
bool sys_pm_get_clk_info(sys_clk_info_t * sys_clk_info);

#endif /*#ifndef __SYS_PM_H__*/
