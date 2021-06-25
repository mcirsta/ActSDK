
/*******************************************************************************
 *                                  US213F
 *                            Module: Fusion
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       carrie     2009-04-07 10:00     1.0             build this file
*******************************************************************************/
/*!
 * \file     Fusion.h
 * \brief    the ui display fusion header file
 * \author   Carrie
 * \version 1.0
 * \date  2009/4/7
*******************************************************************************/
#ifndef  __FUSION_H__
#define  __FUSION_H__

#include "include_case.h"
#include "transimage.h"

/*!
*\brief
	特效种类
*/
typedef enum
{
    /*! 无特效*/
    FUSION_NORMAL = 0,

    /*! 推送 */
    FUSION_PUSH,
    /*! 翻转 */
    FUSION_REVERSE,
    /*! 立方体 */
    FUSION_CUBE,

    /*! GUI的RGB界面弹出透明子菜单*/
    FUSION_POPUPALPHA,
    /*! 在视频的YUV界面上弹出子菜单 */
    FUSION_MOVEALPHA,

    /*! 旋转 */
    FUSION_ROTATE,
    /*! 弹出子菜单*/
    FUSION_POPUP,
    /*! 淡出*/
    FUSION_FADE,
    /*! 棋盘*/
    FUSION_CHECK,
    /*! 百叶窗*/
    FUSION_LOUVER,
    /*! 随机线条*/
    FUSION_RANDOMLINE,
    /*! 水波纹*/
    FUSION_WAVE,
    /*! PPT*/
    FUSION_PPT,
    /*! 翻页 分平移上翻和立体卷曲翻页 */
    FUSION_PAGE,
    /*! 随机特效*/
    FUSION_RANDOM,
    /*! 切割 */
    FUSION_CUT,
    /*! 擦除 */
    FUSION_ERASE,

    /*! 特效最大值 */
    FUSION_MODE_MAX,
}fusion_mode_e;

/*!
 * \brief
 *   特效回调函数，当完成一帧的特效时，调用一次，应用可以自己叠加图片
 * \param[in]   framebuffer framebuffer首地址
 * \param[in]   width       framebuffer宽
 * \param[in]   height      framebuffer高
 */
typedef void (*fusion_callback_t)(unsigned char *framebuffer, int width, int height);

/*!
 * \brief
 *     特效帧信息
 */
typedef struct
{
    sty_rect_t      rect;   /*! 屏幕矩形区 */
    unsigned char   *pdata; /*! 区域数据 */
    bool            alpha;  /*! 数据是否带alpha */
}fusion_frame_t;

/*!
 * \brief
 *     特效模式及方向
 */
typedef struct
{
    fusion_mode_e   mode;
    trans_direct_t  direct;
}fusion_effect_t;

/*!
 * \brief
 *     特效的页面进入情况
 */
typedef enum
{
	/*! 从前一级进入 */
    FUSION_FORWARD = 0,
    /*! 从后一级返回 */     
    FUSION_BACKWARD,       
    /*! 最大值 */     
    FUSION_MAX,
}fusion_enter_e;

/*!
 * \brief
 *     特效类型
 */
typedef enum
{
	/*! 普通页面 */
    FUSION_TYPE_COMMON = 0,
    /*! 应用切换 */
    FUSION_TYPE_APP_CHANGE,
    /*! Option菜单RGB */
    FUSION_TYPE_OPTION_RGB,
    /*! Option菜单YUV */
    FUSION_TYPE_OPTION_YUV,
    /*! 旋转屏 */
    FUSION_TYPE_ROTATE,
    /*! 对话框 */
    FUSION_TYPE_DIALOG,
    /*! 类型的最大值 */
    FUSION_TYPE_MAX,        
}fusion_type_e;


/******************************************************************************/
/*!
 *\par  Description:
 *      设置特效参数
 *\param[in]   effect 特效结构体指针（包含特效模式， 特效移动方向）
 *\retval  true  设置成功
 *\retval  false 设置失败
 *\par
 *\note
 *******************************************************************************/
bool fusion_set_effect(fusion_effect_t *effect);


/******************************************************************************/
/*!
 *\par  Description:
 *      设置特效的第一帧
 *\param[in]   frame    fusion_frame_t结构体指针，可以为NULL，
 *                      如果为NULL，则取当前全屏的framebuffer数据，
 *                      做MOVEALPHA操作时, frame->alpha必须为true
 *\retval  true  设置成功
 *\retval  false 设置失败
 *\par
 *\note 必须与fusion_set_last_frame、fusion_display一组出现
 *\note
 *******************************************************************************/
bool fusion_set_first_frame(fusion_frame_t *frame);


/******************************************************************************/
/*!
 *\par  Description:
 *      设置特效的最后一帧
 *\param[in]   frame    fusion_frame_t结构体指针，可以为NULL，
 *                      如果为NULL，则取当前全屏的framebuffer数据，
 *                      做POPUPALPHA、MOVEALPHA操作时, frame->alpha必须为true
 *\retval  true  设置成功
 *\retval  false 设置失败
 *\par
 *\note 必须与fusion_set_first_frame、fusion_display一组出现
 *\note
 *******************************************************************************/
bool fusion_set_last_frame(fusion_frame_t *frame);


/******************************************************************************/
/*!
 *\par  Description:
 *      指定特效模式显示特效（忽略fusion_set_type设定的类型及方向）
 *\param[in]   callback     特效回调函数指针，每完成一帧即调用，可以为NULL
 *\retval  true  显示成功
 *\retval  false 显示失败
 *\par
 *\note 必须与fusion_set_first_frame、fusion_set_last_frame一组出现
 *\par Example:
 *\code
 *  1. 全屏特效
 *      fusion_effect_t eff;
 *      fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &eff);
 *      fusion_set_effect(&eff);
 *      fusion_set_first_frame(NULL);
 *      gui_paint_somthing(...)
 *      fusion_set_last_frame(NULL);
 *      fusion_display(NULL);
 *
 *  2. 非全屏
 *      fusion_effect_t eff;
 *      fusion_frame_t frame;
 *      frame.rect.x = 10;
 *      frame.rect.y = 20;
 *      frame.rect.width = 100;
 *      frame.rect.height = 100;
 *      frame.pdata = NULL;
 *      frame.alpha = false;
 *      fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &eff);
 *      fusion_set_effect(&eff);
 *      fusion_set_first_frame(&frame);
 *      gui_paint_somthing(...)
 *      fusion_set_last_frame(&frame);
 *      fusion_display(NULL);
 *
 *  3. Option菜单
 *      fusion_effect_t eff;
 *      canvas_info_t info;
 *      fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &eff);
 *      fusion_set_effect(&eff);
 *      fusion_set_first_frame(NULL);
 *      paint_option...;
 *      int result = canvas_get_info(canvas_id, &info); // 先获取子菜单图层数据
 *      fusion_frame_t frame;
 *      frame.rect.x = info.rect.x;
 *      frame.rect.y = info.rect.y;
 *      frame.rect.width = info.rect.width;
 *      frame.rect.height = info.rect.height;
 *      frame.pdata = info.pdata;
 *      frame.alpha = true;
 *      fusion_set_last_frame(&frame);
 *      fusion_get_config(FUSION_TYPE_OPTION_RGB, FUSION_FORWARD, &eff);
 *      fusion_display(NULL);
 * \endcode
 *******************************************************************************/
bool fusion_display(fusion_callback_t callback);


/******************************************************************************/
/*!
 *\par  Description:
 *      设置特效配置值
 *\param[in]   type     特效类型
 *\param[in]   enter    进入当前页面情况：前一级页面进入、后一级页面返回
 *\param[in]   effect   特效类型
 *\retval  true  设置成功
 *\retval  false 设置失败
 *\par Example:
 *\code
 *    fusion_effect_t eff;
 *    eff.type = TRANS_LEFT;
 *    eff.mode = FUSION_CUBE;
 *    fusion_set_config(FUSION_TYPE_COMMON, FUSION_DIR_FORWARD, &eff);
 * \endcode
 *******************************************************************************/
bool fusion_set_config(fusion_type_e type, fusion_enter_e enter, const fusion_effect_t *effect);


/******************************************************************************/
/*!
 *\par  Description:
 *      读取特效配置值
 *\param[in]   type     特效类型
 *\param[in]   enter    进入当前页面情况：前一级页面进入、后一级页面返回
 *\param[out]  effect   特效类型
 *\retval  true  设置成功
 *\retval  false 设置失败
 *\par Example:
 *\code
 *    fusion_effect_t eff;
 *    fusion_get_config(FUSION_TYPE_COMMON, FUSION_DIR_FORWARD, &eff);
 *    fusion_display(&eff, NULL);
 * \endcode
 *******************************************************************************/
bool fusion_get_config(fusion_type_e type, fusion_enter_e enter, fusion_effect_t *effect);

/******************************************************************************/
/*!
 *\par  Description:
 *      更新特效配置值（setting恢复了默认配置之后，需调用此接口）
 *\retval  true  设置成功
 *\retval  false 设置失败
 *\par Example:
 *\code
 *    reset_all_config();
 *    fusion_update_config();
 * \endcode
 *******************************************************************************/
bool fusion_update_config(void);

#endif   /* ----- #ifndef FUSION_H  ----- */


