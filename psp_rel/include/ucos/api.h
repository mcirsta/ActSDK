/********************************************************************************
 *                               USDK(1100)
 *                            Module: API
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     api.h
 * \brief    各驱动起始API号定义
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __API_H__
#define __API_H__

#define SYSCALL_API_START   0x10000
#define KMODULE_API_START   0x20000
#define GUI_API_START       0x30000
#define TEST_API_START      0x40000

#define LIBC_SYSINDEP_API_START 0x50000
#define LIBC_FS_API_START       0x60000
#define LIBC_GENERAL_API_START  0x70000

#define MMM_API_START       0x80000
#define SQLITE_API_START    0x90000
#define FSEL_API_START      0xa0000
#define PLIST_API_START     0xb0000

#define MUTILANG_API_START  0xc0000
#define STYLE_API_START     0xd0000
#define SPECIAL_API_START   0xe0000
#define FFT_API_START       0xf0000

#define ACT_MEM_API_START   0x100000
#define APCONFIG_API_START  0x110000

#define APPLIB_API_START    0x120000
#define COMMONUI_API_START  0x130000
#define DESKTOP_API_START   0x140000

#define SOUND_API_START     0x150000
#define ALC_API_START       0x160000

#define FUSION_API_START    0x170000

#ifndef _ASSEMBLER_
#include <ucos/types.h>
/*!
 *  API序号 \n
 *  \li 0~31 : 最大支持32个API
 *
 */
enum
{
    APINo_SYSCALL,
    APINO_KMODULE,
    APINO_GUI,
    APINO_TEST,
    APINO_LIBC_SYSINDEP,
    APINO_LIBC_FS,
    APINO_LIBC_GENERAL,
    APINO_MMM,
    APINO_SQLITE,
    APINO_FSEL,
    APINO_PLIST,
    APINO_MUTILANG,
    APINO_STYLE,
    APINO_SPECIAL,
    APINO_FFT,
    APINO_ACT_MEM,
    APINO_APCONFIG,
    APINO_APPLIB,
    APINO_COMMONUI,
    APINO_DESKTOP,
    APINO_SOUND,
    APINO_ALC,
    APINO_FUSION,
    APINO_R24,
    APINO_R25,
    APINO_R26,
    APINO_R27,
    APINO_R28,
    APINO_R29,
    APINO_R30,
    APINO_R31,
    APINO_R32,
    APINO_MAX
//APINO_MAX=32
};

extern unsigned int total_api_entry[];

extern int api_install(unsigned char api_no, unsigned int addr);
extern int api_uninstall(unsigned char api_no);
extern int apino_valid(unsigned char api_no);

#endif /*_ASSEMBLER_*/
#endif /*__API_H__ */
