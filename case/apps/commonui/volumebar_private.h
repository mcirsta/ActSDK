/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : softkeyboard_private.h
 * History:         
 *      <author>         <time>           <version >             <desc>
 *    wangsheng    2009-4-14             v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     softkeyboard_private.h
 * \brief    softkeyboard widget
 * \author   wangsheng
 *               
 * \version 1.0
 * \date  2009/04/22
 *******************************************************************************/

#ifndef __VOLUMEBAR_PRIVATE_H__
#define __VOLUMEBAR_PRIVATE_H__

#include "include_case.h"


/* define volumebar param */
#define VBAR_MIN_VOL	0
#define VBAR_MAX_VOL	31

/* define resource status*/
enum
{
    need_paint,
    not_need_paint,
};

/*volumebar 内部私有数据结构*/
struct volumebar_private_data_s
{    
    int hdc;    //!< hdc

    int dec_pressed;    //!< the flag for decreasing the volume
    int inc_pressed;    //!< the flag for increasing the volume   
    
    int vol_bg_status;
    int vol_dec_status;
    int vol_inc_status;
    int vol_icon_status;
    int vol_pb_status;
    int vol_logo_status;
    int vol_val_status;
	
    /* resource for painting */
    resgroup_resource_t *vb;
    picture_resource_t *vb_bg;
    picture_resource_t *vb_dec;
    picture_resource_t *vb_dech;
    picture_resource_t *vb_inc;
    picture_resource_t *vb_inch;
    picture_resource_t *vb_icon;
    picture_resource_t *vb_pbbg;
    picture_resource_t *vb_pb[VBAR_MAX_VOL];
    string_resource_t *vb_logo;
    string_resource_t *vb_val;
};

#endif