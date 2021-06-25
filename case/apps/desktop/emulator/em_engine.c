/************************************************************************
 *                             USDK1100
 *                            music engin
 *
 *                (c) Copyright 2003-2015, Actions Co,Ld.
 *                          All Right Reserved
 *
 * File: em_engin.c
 * description:
 *             emulator 加载声音、图像驱动
 * By  : dupengcheng
 * Version: 1> v1.00     first version     2009-03-10
 ************************************************************************/
#include "include_case.h"

void em_mount_drv(void);
void em_umount_drv(void);

static void _mount_dsp_dec(void);
static void _umount_dsp_dec(void);


/*************************************************************************
 * Function:          em_mount_drv()
 * Description:       Emulator 加载所有驱动
 * para1：            none
 * Return：           none
 **************************************************************************/
void em_mount_drv(void)
{
    _mount_dsp_dec();
}

/*************************************************************************
 * Function:          em_mount_drv()
 * Description:       Emulator 加载所有驱动
 * para1：            none
 * Return：           none
 **************************************************************************/
void em_umount_drv(void)
{
    _umount_dsp_dec();
}

///************************************************************************
// * Function:             mount_ada
// * Description:         load ada driver
// * Para1:
// * Return：           none
// *************************************************************************/
//static void _mount_ada(void)
//{
//    insmod("ada.ko", 0);
//    sys_mknod("/dev/ada", S_IFCHR, makedev(MAJOR_ADA, 0));
//}
//
///************************************************************************
// * Function:             mount_ada
// * Description:         load ada driver
// * Para1:
// * Return：           none
// *************************************************************************/
//static void _umount_ada(void)
//{
//    int ret;
//    ret = rmmod("ada.ko", 0);
//    print_dbg("ada ret=%d", ret);
//}

/*************************************************************************
 * Function:            mount_dsp_dec
 * Description:        加载音频解码驱动
 * para1：            none
 * Return：           none
 **************************************************************************/
static void _mount_dsp_dec(void)
{
    insmod("dspdec.ko", 0);
    sys_mknod(DEVICE_DSP_AD, S_IFCHR, makedev(MAJOR_DSP_AD, 0));
}

/*************************************************************************
 * Function:            mount_dsp_dec
 * Description:        加载音频解码驱动
 * para1：            none
 * Return：           none
 **************************************************************************/
static void _umount_dsp_dec(void)
{
    int ret;
    ret = rmmod("dspdec.ko", 0);
}

///*************************************************************************
// * Function:            mount_vram
// * Description:        加载vram
// * para1：            none
// * Return：           none
// *************************************************************************/
//static void _mount_vram(void)
//{
//    insmod("vram.ko", 0);
//    sys_mknod("/dev/vram", S_IFCHR, makedev(MAJOR_VRAM, 0));
//}
//
///*************************************************************************
// * Function:            mount_vram
// * Description:        加载vram
// * para1：            none
// * Return：           none
// *************************************************************************/
//static void _umount_vram(void)
//{
//    int ret;
//    ret = rmmod("vram.ko", 0);
//    print_dbg("vram ret=%d", ret);
//}

