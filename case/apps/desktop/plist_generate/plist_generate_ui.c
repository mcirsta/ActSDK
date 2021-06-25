/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-25          1.0             build this file 
*******************************************************************************/
/*!
 * \file     plist_generate_ui.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/25
*******************************************************************************/

#include "include_case.h"


/* include file of this application, 本应用的头文件 */
#include "plist_generate.h"
#include "plist_generate_scene.h"
#include "plist_generate_ui.h"





/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    plist_generate_scene_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \retval       void
* \ingroup      video
*******************************************************************************/
void plist_generate_draw_bg(plist_generate_scene_t *plist_generate_scene, char show)
{
    bool changed;
    
    if ( NULL == plist_generate_scene->pic_bg )
    {
        return;
    }
    
    changed = (plist_generate_scene->saved_show.bg != show)? true:false;
//    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    if (1 == show)
    {
        sty_draw_picture_res( plist_generate_scene->app_scene.handle_gui_dc, plist_generate_scene->pic_bg );
    }
    else if (0 == show)
    {
        print_warning("set bg hide is invalid\n\n");
    }
    else
    {
    }
    
    plist_generate_scene->saved_show.bg = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of play_pause
* \param[in]    plist_generate_scene_t *: 指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 显示的帧号.
* \retval       void
* \ingroup      video
*******************************************************************************/
void plist_generate_draw_wait_animate(plist_generate_scene_t *plist_generate_scene, char show, char frame)
{
    static char saved_frame = -1;
    bool changed;
    sty_rect_t srect;
    gui_rect_t grect;
    
    if ( (NULL == plist_generate_scene->pic_bg) || (NULL == plist_generate_scene->reg_wait) )
    {
        return;
    }
    
    changed = ( (plist_generate_scene->saved_show.wait != show) || (saved_frame != frame) )? true:false;
//    print_dbg("show:%d, changed:%d, frame:%d, saved_frame:%d", show, changed, frame, saved_frame);
    if ( false == changed )
    {
        return;
    }
    
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != plist_generate_scene->saved_show.wait )
        {
            grect.x0 = plist_generate_scene->reg_wait->x;
            grect.y0 = plist_generate_scene->reg_wait->y;
            grect.x1 = (grect.x0 + plist_generate_scene->reg_wait->width) - 1;
            grect.y1 = (grect.y0 + plist_generate_scene->reg_wait->height) - 1;
            gui_dc_clear_rect( plist_generate_scene->app_scene.handle_gui_dc, grect.x0, grect.y0, grect.x1, grect.y1 );
            
            srect.x     = plist_generate_scene->reg_wait->x;
            srect.y     = plist_generate_scene->reg_wait->y;
            srect.width = plist_generate_scene->reg_wait->width;
            srect.height= plist_generate_scene->reg_wait->height;
            //先画图片所在区域的背景, 否则前景的png图片会重叠在一起
            sty_draw_sub_bitmap(plist_generate_scene->app_scene.handle_gui_dc, \
                plist_generate_scene->bg_bitmap_info, \
                plist_generate_scene->reg_wait->x, plist_generate_scene->reg_wait->y, &srect);
        }
        sty_draw_bitmap( plist_generate_scene->app_scene.handle_gui_dc, plist_generate_scene->wait_bitmap_info[frame], \
            plist_generate_scene->reg_wait->x, plist_generate_scene->reg_wait->y );
    }
    else if (0 == show)
    {
        print_warning("set hide is invalid\n\n");
    }
    else
    {
    }
    
    plist_generate_scene->saved_show.wait = show;
    saved_frame = frame;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    plist_generate_scene_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \retval       void
* \ingroup      video
*******************************************************************************/
void plist_generate_draw_pbar_bg(plist_generate_scene_t *plist_generate_scene, char show)
{
    bool changed;
    
    if ( NULL == plist_generate_scene->pic_pbar_bg )
    {
        return;
    }
    
    changed = (plist_generate_scene->saved_show.pbar_bg != show)? true:false;
//    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    if (1 == show)
    {
        sty_draw_picture_res( plist_generate_scene->app_scene.handle_gui_dc, plist_generate_scene->pic_pbar_bg );
    }
    else if (0 == show)
    {
        print_warning("set bg hide is invalid\n\n");
    }
    else
    {
    }
    
    plist_generate_scene->saved_show.pbar_bg = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      将当前播放时间,总时间 转换成进度条的x坐标值
* \param[in]    int: 当前进度, 单位: 1%
* \param[in]    int: 总进度, 100%
* \param[out]   int *: 计算后的高亮条最右侧 x坐标
* \retval       
* \ingroup      video
*******************************************************************************/
bool plist_generate_progress_to_pbar_coordinate( int cur_percent, int total_percent, int *x_coordinate )
{
    static int saved_cur_percent = -1;
    static int saved_total_percent = -1;
    /* 高亮条最右侧的x坐标. 传给pbar */
    int x;
    
    if ( (NULL == x_coordinate) || (cur_percent < 0) || (total_percent <= 0) || (cur_percent > total_percent) )
    {
        print_warning("parameter err. cur_percent:%d, total_percent:%d", cur_percent, total_percent);
        return false;
    }
    
    //没有变化, 直接返回. 只有变化才继续计算
    if ( (saved_cur_percent == cur_percent) && (saved_total_percent == total_percent) )
    {
        return false;
    }
    
    //计算高亮条右端坐标, 并考虑到高亮条左侧缩进的坐标
    x = (plist_generate_vars.scene.pbar_length * cur_percent) / total_percent;
    x += plist_generate_vars.scene.pbar_xstart;

    *x_coordinate = x;
    saved_cur_percent = cur_percent;
    saved_total_percent = total_percent;
//    print_dbg("after cal. cur_percent:%dS, total_percent:%ds, x_coordinate:%d", cur_percent, total_percent, x);
    
    return true;
}

//shaofeng saved_x的初始化
/******************************************************************************/
/*!                    
* \par  Description:
*      draw pbar pictrue
* \param[in]    plist_generate_scene_t *: 指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 高亮条最右侧的x坐标. 此坐标改变需要重画
* \retval       void
* \ingroup      video
*******************************************************************************/
void plist_generate_draw_pbar(plist_generate_scene_t *plist_generate_scene, char show, int x)
{
    static int saved_x;
    bool changed;
    int i;
    gui_rect_t grect;

    if ( NULL == plist_generate_scene->pic_pbar_hl )
    {
        return;
    }
    if (x < 0)
    {
        print_warning("x:%d, param err", x);
        return;
    }
    
    changed = ( (plist_generate_scene->saved_show.pbar != show) || (saved_x != x) )? true:false;
//    print_dbg("show:%d, changed:%d, x:%d, saved_x:%d", show, changed, x, saved_x);
    if ( false == changed )
    {
        return;
    }
    
    //第一次刷新或者全部重刷时, 复位saved_x
    if ( -1 == plist_generate_scene->saved_show.pbar )
    {
        saved_x = plist_generate_scene->pbar_xstart;
    }
    if ( x < plist_generate_scene->pbar_xstart )
    {
        x = plist_generate_scene->pbar_xstart;
    }
    
    if (1 == show)
    {
        if(x > saved_x)
        {
            grect.x0 = (short)saved_x;
            grect.y0 = plist_generate_scene->pic_pbar_hl->y;
            grect.x1 = (short)x;
            grect.y1 = (grect.y0 + plist_generate_scene->pic_pbar_hl->height) - 1;
            gui_dc_clear_rect( plist_generate_scene->app_scene.handle_gui_dc, grect.x0, grect.y0, grect.x1, grect.y1 );
            
            //高亮条变长, 从小到大画高亮
            for (i=saved_x; i<=x; i++)
            {
                sty_draw_picture_id( plist_generate_scene->app_scene.handle_gui_dc, \
                    plist_generate_scene->pic_pbar_hl->pic_id, \
                    i, plist_generate_scene->pic_pbar_hl->y);
            }
        }
    }
    else if (0 == show)
    {
        print_warning("set hide is invalid\n\n");
    }
    else
    {
    }
    
    plist_generate_scene->saved_show.pbar = show;
    saved_x = x;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str wait
* \param[in]    plist_generate_scene_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    char *: 显示的字符串.
* \retval       void
* \ingroup      video
*******************************************************************************/
void plist_generate_draw_str(plist_generate_scene_t *plist_generate_scene, char show)
{
    bool changed;
    sty_rect_t srect;
    gui_rect_t grect;
    
    if ( NULL == plist_generate_scene->str_wait )
    {
        return;
    }
    
    changed = (plist_generate_scene->saved_show.str_wait != show)? true:false;
//    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != plist_generate_scene->saved_show.str_wait )
        {
            grect.x0 = plist_generate_scene->str_wait->x;
            grect.y0 = plist_generate_scene->str_wait->y;
            grect.x1 = (grect.x0 + plist_generate_scene->str_wait->width) - 1;
            grect.y1 = (grect.y0 + plist_generate_scene->str_wait->height) - 1;
            gui_dc_clear_rect( plist_generate_scene->app_scene.handle_gui_dc, grect.x0, grect.y0, grect.x1, grect.y1 );
            
            srect.x     = plist_generate_scene->str_wait->x;
            srect.y     = plist_generate_scene->str_wait->y;
            srect.width = plist_generate_scene->str_wait->width;
            srect.height= plist_generate_scene->str_wait->height;
            //先画字符所在区域的背景, 否则前后的字符会重叠在一起
            sty_draw_sub_picture_id(plist_generate_scene->app_scene.handle_gui_dc, \
                plist_generate_scene->pic_bg->pic_id, \
                plist_generate_scene->str_wait->x, plist_generate_scene->str_wait->y, &srect);
        }
        sty_draw_string_res( plist_generate_scene->app_scene.handle_gui_dc, plist_generate_scene->str_wait );
    }
    else if (0 == show)
    {
        print_warning("set hide is invalid\n\n");
    }
    else
    {
    }
    
    plist_generate_scene->saved_show.str_wait = show;
}

