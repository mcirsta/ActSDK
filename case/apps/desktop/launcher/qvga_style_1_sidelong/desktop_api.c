/********************************************************************************
*                            Module: libc_general
*                 Copyright(c) 2001-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>         <time>           <version >             <desc>
*       chencm      2009-08-7 10:00     1.0             build this file 
********************************************************************************/
/*!
 * 
 * \brief  libc����Ҫ��פ�ڴ�Ľӿڵ�ע��
 * \author wurui
 * \par �������: 
 * libcʵ������Ҫ��פ�ڴ沿�ֵĽӿڷ�װ
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/


#include "include_case.h"
#include "./../launcher_scene_desktop.h"

static const unsigned int desktop_apitbl[]=
{
    (unsigned int)launcher_scene_desktop,

};                
                  
/******************************************************************************/
/*!                    
* \par  Description:
*      init����
* \note
* ʹ�ú�__initȷ��������λ��.ctors��
*******************************************************************************/
int __attribute__((constructor)) desktop_api_init(void)
{
    api_install(APINO_DESKTOP, (unsigned)desktop_apitbl);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      exit����
* \note
* ʹ�ú�__exitȷ��������λ��.dtors��
*******************************************************************************/
void __attribute__((destructor)) desktop_api_exit(void)
{
    api_uninstall(APINO_DESKTOP);
    return;
}

