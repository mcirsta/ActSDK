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
 * \defgroup   applib_system_pm   ϵͳ��Դ����ӿ� 
 */

/*!
 *  \brief 
 *  ϵͳƵ����Ϣ�ṹ����
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
 *     ����һ��Ƶ�ʵ������� 
 * \ingroup applib_system_pm
 * \param[in] domain -- Ƶ�ʲ���(������ͺ����Ƶ�ʵ���ֵ)
 * \return �ɹ�����true, ʧ�ܷ���false 
 *******************************************************************************/
bool sys_pm_set_clk_domain(const clk_domain_t * domain);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ���һ��Ƶ�ʵ������� 
 * \ingroup applib_system_pm
 * \param[in] domain -- Ƶ�ʲ���(������ͺ����Ƶ�ʵ���ֵ)
 * \return �ɹ�����true, ʧ�ܷ���false 
 * \note ����һ��Ӧ��ʱ��������һ��Ƶ�ʵ������ԣ������˳�ʱ����������ò���
 *******************************************************************************/
bool sys_pm_clear_clk_domain(const clk_domain_t * domain);

/******************************************************************************/
/*!                    
 * \par  Description:
 *      ��ȡ��ǰϵͳ�����ߵ�Ƶ����Ϣ 
 * \ingroup applib_system_pm
 * \param[in] sys_clk_info -- ��Ÿ���Ƶ����Ϣ�Ľṹ��ָ�� 
 * \return �ɹ�����true, ʧ�ܷ���false 
 *******************************************************************************/
bool sys_pm_get_clk_info(sys_clk_info_t * sys_clk_info);

#endif /*#ifndef __SYS_PM_H__*/
