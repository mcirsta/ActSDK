/*********************************************************************************
 *                       213x---gsensor driver
 *                (c) Copyright 2007-2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * FileName:gsensor.h    Author:yangrui  Date:2009/05/11
 * Description: definition of macro.
 * Others:
 * History:
 *      <author>    <time>       <version >    <desc>
 *      yangrui     2009/05/11     1.0         build this moudle
 *********************************************************************************/
#ifndef _GSENSOR_H_
#define _GSENSOR_H_

#define GSENSOR_DISABLE   0    /*enable gsensor*/
#define GSENSOR_ENABLE    1    /*disable gsensor*/
#define GSENSOR_SET_INTERVAL      2    /*set the value of interval*/
#define GSENSOR_GET_INTERVAL      3    /*get the value of interval*/

typedef struct
{
    int acc_x; /*x方向的加速度值，单位:mg,取值范围：MX:[-4096,4096],ST:[-2286,2286]*/
    int acc_y; /*y方向的加速度值，单位:mg,取值范围：MX:[-4096,4096],ST[-2286,2286]*/
    int acc_z; /*z方向的加速度值，单位:mg,取值范围：MX:[-0,0],ST:[-2286,2286].*/
    int reserved; /*预留，未用.值为0*/
} gsensor_data_t;

#endif

