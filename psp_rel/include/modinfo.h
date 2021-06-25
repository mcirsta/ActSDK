/********************************************************************************
 *                              usdk130
 *                            Module: FWINFO
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xueguokun  2009-11-12      1.0                    build this file
 ********************************************************************************/
/*!
 * \file			modinfo.h
 * \brief		模块使用的VRAM
 * \author       薛国昆
 \par Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *
 * \version		1.0
 * \date			2009/11/12
 *******************************************************************************/

#ifndef __MODINFO_H__
#define __MODINFO_H__

/*!
 * \brief	plist_flag的数据结构
 */
typedef struct
{
    /*! 结构体的长度，sector为单位 */
    unsigned int plist_flag;
    char reserve2[508];

} modinfo_t;

/*!modinfo的长度，byte为单位 */
#define MODINFO_LENGTH	(sizeof (modinfo_t))

/*! modinfo的长度，sector为单位 */
#define MODINFO_SECTOR_LENGTH	(MODINFO_LENGTH >> 9)

#endif	//__MODINFO_H__
