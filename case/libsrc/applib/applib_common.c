/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : applib_common.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/


#include "applib_common.h"

const app_info_t * g_this_app_info = NULL;

bool g_applib_inited = false;

inline bool is_applib_inited(void)
{
    if(false == g_applib_inited)
    {
        print_warning("applib has not initialized!");
        return false;
    }

    return true;
}



