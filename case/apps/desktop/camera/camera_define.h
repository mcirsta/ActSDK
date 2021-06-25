/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-6-17  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_define.h
 * \brief    camera中需要用到的一些宏定义
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-6-17
 *******************************************************************************/

#ifndef __CAMERA_DEFINE_H__
#define __CAMERA_DEFINE_H__

#ifndef CFG_TEXT_LEN
#define CFG_TEXT_LEN  512
#endif

#ifndef MOUNT_DRV   //注意下面的return false是否有问题
#define	MOUNT_DRV(_result,_name,_arg,_result2,_nod,_mode,_dev)	do{\
                                     (_result) = insmod((_name),(void *)(_arg));\
                                     if((_result) != 0)\
                                     {\
                                        print_err(_name"\ninsmod err%d errnumber=%d\n",(_result),errno);\
                                        /*return false;\*/\
                                     };\
                                      (_result2) = sys_mknod((_nod), (_mode), makedev((_dev), 0));\
                                      if((_result2) != 0)\
                                      {\
                                        print_err(_name"\nmknod err%d errnumber=%d\n",(_result2),errno);\
                                        /*return false;\*/\
                                      }\
                                      }while(0)

#define  SET_CAM_RECT(name,x,y,w,h) do{\
                            (name).xpos =  (x);\
                            (name).ypos =  (y);\
                            (name).width =  (w);\
                            (name).height =  (h);\
                            }while(0)
#endif  /*  MOUNT_DRV*/

#ifndef  CAMERA_DEF_RET
#define CAMERA_DEF_RET
#define	HANDLEERR(result,value,str)	do{\
if ( (value) == (result) )\
{\
print_err(str);\
return false;\
}\
}while(0)

#define	RET_UNEQUAL_ERR(result,value,str) do{\
if ( (value) != (result) )\
{\
print_err(str);\
return false;\
}\
}while(0)

//mmm use 0 == true ; -1 == err; app 0 == false;1 == true;
//exit( -1 );

#define MMM_RET_JUDGE(_result,value,str) do{\
    if ( (_result) != (value) )\
    {\
        /*print_err*/print_warning(str"ERR\n");\
        (_result) = false;\
    }\
    else\
    {\
              print_info(str"TEST ok\n");\
              (_result) = true;\
    }\
    }while(0)
#endif  /*CAMERA_DEF_RET*/

#ifndef MENU_SELECT_OK_HIDE
#define MENU_SELECT_OK_HIDE 0
#endif   /* MENU_SELECT_OK_HIDE */

#ifndef USE_MODAL_DIALOG
#define USE_MODAL_DIALOG    0
#endif   /* USE_MODAL_DIALOG */

#ifndef CAM_THRESHOLD_SPACE
#define CAM_THRESHOLD_SPACE 0//测试用,剩余张数或者秒数，这里用0
#endif  /*CAM_THRESHOLD_SPACE*/

#ifndef DBG_REMNANT_MEMORY
#define DBG_REMNANT_MEMORY  0   //打印剩余空间和使用内存
#endif   /* DBG_REMNANT_MEMORY */

#ifndef DV_ACTIVITY_DETECT
#define DV_ACTIVITY_DETECT  1
#endif  /* DV_ACTIVITY_DETECT */

#ifndef DV_ICON_STATUS2SDRAM
#define DV_ICON_STATUS2SDRAM    1
#endif   /* DV_ICON_STATUS2SDRAM */

#ifndef BLENDING_AREA
#define BLENDING_AREA
#define	CAM_DEFAULT_BLENDING_COLOR_KEY     (-1)//0xffff00
#define   CAM_LETF_BLENDING_COLOR_KEY 0//0xf81f //(0)//(0xff00ff)
#define   CAM_BOTTON_BLENDING_COLOR_KEY   0//0xf81f

/*
1.目前界面分GUI层(所有绘制的osd层，例如主界面，music界面)，
YUV层(所有输入层,TVIN,SENSOR IN,VIDEO PLAY,PHOTO PLAY)
2.要在YUV上显示OSD菜单ICON图标等就需要做alpha blending混合

3.范围: 0-255. 越大越透明, 前景越淡, YUV层画面越清晰, 但目前只有5级

*/

/* 0-51; 52-102; 103-153; 154-204; 205-255 */
#define CAM_DEFAULT_BLENDING_ALPHA    (0) //52

#endif   /* BLENDING_AREA */

//=======================================================================
//ARRAY_LENGTH

#ifndef ARRAY_LENGTH_TIME
#define ARRAY_LENGTH_TIME 64
#endif   /* ARRAY_LENGTH_TIME */

#ifndef ARRAY_LENGTH_RESOLUTION
#define ARRAY_LENGTH_RESOLUTION  32
#endif   /* ARRAY_LENGTH_RESOLUTION */

#ifndef PATH_STICKER
#define PATH_STICKER
#define  DEF_PATH_STICKER_1  "/mnt/diska/misc/C_STIK1.png"
#define  DEF_PATH_STICKER_2  "/mnt/diska/misc/C_STIK2.png"
#define  DEF_PATH_STICKER_3  "/mnt/diska/misc/C_STIK3.png"
#define  DEF_PATH_STICKER_TEMP1  "/mnt/diska/misc/C_STIK1.tmp"
#define  DEF_PATH_STICKER_TEMP2  "/mnt/diska/misc/C_STIK2.tmp"
#define  DEF_PATH_STICKER_TEMP3  "/mnt/diska/misc/C_STIK3.tmp"
#endif   /* PATH_STICKER */

#ifndef PATH_SHUTTER
#define PATH_SHUTTER
#define DEF_PATH_SHUTTER_1 "/mnt/diska/misc/shutter1.wav"
#define DEF_PATH_SHUTTER_2 "/mnt/diska/misc/shutter2.wav"
#define DEF_PATH_SHUTTER_3 "/mnt/diska/misc/shutter3.wav"
#endif   /* PATH_SHUTTER */

#ifndef EIO
#define EIO            5  /* I/O error */
#endif   /* EIO */

#ifndef ENOMEM
#define ENOMEM     12  /* Out of memory */
#endif   /* ENOMEM */

#ifndef DBG_TIME_OPTIMIZE
#define DBG_TIME_OPTIMIZE   0
#endif   /* DBG_TIME_OPTIMIZE */

#ifndef DBG_MMM
#define DBG_MMM 0
#endif   /* DBG_MMM */

#if 0
extern int dbg_time;
extern int get_time;

#if 0
//测试路径使用时间用的调试宏
#define DBG_TIME(void) \
{\
    int get_time = get_ab_timer();\
    printf("%s,%s,%d,( %d )-- %d ms\n",__FILE__,__FUNCTION__,__LINE__,get_time,get_time - dbg_time);\
    dbg_time = get_time;\
}
#endif

#if 0
#define DBG_TIME(str) \
{\
    int get_time = get_ab_timer();\
    printf(str"----,( %d )-- %d ms\n",get_time,get_time - dbg_time);\
    printf("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);\
    dbg_time = get_time;\
}
#endif
#define DBG_TIME_A(void)  do{}while(0)

#define DBG_TIME_At(void) \
{\
    dbg_time = get_ab_timer();\
}
#define DBG_TIME(str)  do{}while(0)

#define DBG_TIMEt(str) \
{\
    get_time = get_ab_timer();\
    printf(str"----,( %d )-- %d ms\n",get_time,get_time - dbg_time);\
    /*printf("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);*/\
    /*dbg_time = get_time;*/\
}

#define MY_FREE(p) \
{\
    if ((p) != NULL)\
    {\
        free((p));\
        (p) = NULL;\
    }\
}
#endif

#ifndef SHORTCUT_CAMERA_DSC
#define SHORTCUT_CAMERA_DSC 1
#endif   /* SHORTCUT_CAMERA_DSC */

#ifndef SHORTCUT_CAMERA_DV
#define SHORTCUT_CAMERA_DV 0
#endif   /* SHORTCUT_CAMERA_DV */

#ifndef SHORTCUT_PLAYBACK_DSC
#define SHORTCUT_PLAYBACK_DSC 1
#endif   /* SHORTCUT_PLAYBACK_DSC */

#ifndef SHORTCUT_PLAYBACK_DV
#define SHORTCUT_PLAYBACK_DV 0
#endif   /* SHORTCUT_PLAYBACK_DV */

#endif   /* __CAMERA_DEFINE_H__ */

