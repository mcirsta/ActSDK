/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_playing_ui.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/


#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"




/******************************************************************************/
/*!                    
* \par  Description:
*      将以ms为单位的时间转换成: "**:**:**"格式的字符串
* \param[in]    int: 时间, 单位: ms
* \param[out]   int *: 转换后的字符串格式:"**:**:**", 时:分:秒制
* \retval       true:成功 ; false:失败
* \ingroup      video
*******************************************************************************/
bool video_playing_time_to_str( unsigned int time, char *str )
{
    unsigned int tmp;
    unsigned int hour = 0;
    unsigned int minite = 0;
    unsigned int second = 0;
    
    if ( NULL == str )
    {
        print_warning("str is NULL");
        return false;
    }
    
    //计算秒
    second = time / 1000;
    if ( second >= 60 )
    {
        tmp = second;
        second %= 60;
        //计算分
        minite = tmp / 60;
        if ( minite >= 60 )
        {
            tmp = minite;
            minite %= 60;
            //计算小时
            hour = tmp / 60;
            if ( hour >= 100 )
            {
                print_warning("time is too long, and more than 100 houes");
                return false;
            }
        }
    }
    
    sprintf( str, "%.2d:%.2d:%.2d", hour, minite, second );
//    print_dbg("time:%d, time string:%s", time, str);
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      将当前播放时间,总时间 转换成进度条的x坐标值
* \param[in]    int: 当前播放时间, 单位: ms
* \param[in]    int: 总时间, 单位: ms
* \param[out]   int *: 计算后的高亮条最右侧 x坐标
* \retval       true:成功 ; false:失败
* \ingroup      video
*******************************************************************************/
bool video_playing_time_to_pbar_coordinate( unsigned int cur_time, unsigned int total_time, int *x_coordinate )
{
    static unsigned int saved_cur_time;
    static unsigned int saved_total_time;
    /* 高亮条最右侧的x坐标. 传给pbar */
    int x;
    
    if ( (NULL == x_coordinate) || (total_time < 1000) || (cur_time > total_time) )
    {
        print_warning("parameter err. cur_time:%d, total_time:%d", cur_time, total_time);
        return false;
    }
    
    //没有变化, 直接返回. 只有变化才继续计算
    if ( (saved_cur_time == cur_time) && (saved_total_time == total_time) )
    {
        return false;
    }
    
    cur_time /= 1000;
    total_time /= 1000;

    x = ( video_vars.scene_playing.pbar_length * (int)cur_time ) / (int)total_time;
    x += video_vars.scene_playing.pbar_xstart;

    *x_coordinate = x;
//    print_dbg("after cal. cur_time:%dS, total_time:%ds, x_coordinate:%d", cur_time, total_time, x);
    
    return true;
}


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    video_scene_playing_t *: 场景指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \retval       void
* \ingroup      video
*******************************************************************************/
void video_playing_draw_pbar_bg(video_scene_playing_t *scene_playing, char show)
{
    bool changed;
    int handle_gui_dc;
    
    if ( NULL == scene_playing->pic_pbar_bg )
    {
        return;
    }

    handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.pbar_canvas_id );
    if(handle_gui_dc <= 0)
    {
        print_err("gui_dc_get err\n");
        return;
    }
    
    changed = (scene_playing->saved_show.pbar_bg != show)? true:false;
//    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    if (1 == show)
    {
        //在pbar_canvas_id画图, 坐标相对于此canvas, 起点xy都为0
        sty_draw_picture_id( handle_gui_dc, scene_playing->pic_pbar_bg->pic_id, 0, 0 );
    }
    else if (0 == show)
    {
        print_warning("set bg hide is invalid\n\n");
    }
    else
    {
    }
    
    scene_playing->saved_show.pbar_bg = show;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      draw pbar pictrue
* \param[in]    video_scene_playing_t *: 指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 高亮条最右侧的x坐标. 此坐标改变需要重画
* \retval       void
* \ingroup      video
*   注意: 
*       高亮条变短, 每1像素的逐个递减画背景条. 拼合起来的是整个的背景条
*       高亮条变长, 每1像素的逐个递增画高亮条. 拼合起来的是整个的高亮条
*******************************************************************************/
void video_playing_draw_pbar(video_scene_playing_t *scene_playing, char show, int x)
{
    static int saved_x;
    bool changed;
    int i;
    sty_rect_t srect;
    int handle_gui_dc;


    if ( (NULL == scene_playing->pic_pbar_bg) || (NULL == scene_playing->pic_pbar) )
    {
        return;
    }
    if (x < 0)
    {
        print_warning("x:%d, param err", x);
        return;
    }
  
    changed = ( (scene_playing->saved_show.pbar != show) || (saved_x != x) )? true:false;
//    print_dbg("show:%d, changed:%d, x:%d, saved_x:%d", show, changed, x, saved_x);
    if ( false == changed )
    {
        return;
    }
    
    //第一次刷新或者全部重刷时, 复位saved_x
    if ( -1 == scene_playing->saved_show.pbar )
    {
        saved_x = video_vars.scene_playing.pbar_xstart;
    }
    if ( x < video_vars.scene_playing.pbar_xstart )
    {
        x = video_vars.scene_playing.pbar_xstart;
    }
    
    if (1 == show)
    {
        if (x < saved_x)
        {
            //在pbar_canvas_id画图, 坐标相对于此canvas, 所有坐标都要减去pic_pbar_bg
            srect.x     = (short)x;
            srect.y     = scene_playing->pic_pbar->y - scene_playing->pic_pbar_bg->y;
            srect.width = (short)( saved_x - x );
            srect.height= scene_playing->pic_pbar->height;
            canvas_erase( video_vars.blending.pbar_canvas_id, &srect );
            
            //高亮条变短, 从大到小画背景
            for (i=saved_x; i>=x; i--)
            {
                
                handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.pbar_canvas_id );
                if(handle_gui_dc <= 0)
                {
                    print_err("gui_dc_get err\n");
                    return;
                }

                sty_draw_picture_id( handle_gui_dc, scene_playing->pic_pbar->pic_id, i, srect.y );
            }
        }
        else if(x > saved_x)
        {
            //在pbar_canvas_id画图, 坐标相对于此canvas, 所有坐标都要减去pic_pbar_bg
            srect.x     = (short)saved_x;
            srect.y     = scene_playing->pic_pbar_hl->y - scene_playing->pic_pbar_bg->y;
            srect.width = (short)( x - saved_x );
            srect.height= scene_playing->pic_pbar_hl->height;
            canvas_erase( video_vars.blending.pbar_canvas_id, &srect );
            
            //高亮条变长, 从小到大画高亮
            for (i=saved_x; i<=x; i++)
            {
                handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.pbar_canvas_id );
                if(handle_gui_dc <= 0)
                {
                    print_err("gui_dc_get err\n");
                    return;
                }
                
                sty_draw_picture_id( handle_gui_dc, scene_playing->pic_pbar_hl->pic_id, i, srect.y );
            }
        }
        else
        {
        }
    }
    else if (0 == show)
    {
        print_warning("set pbar hide is invalid\n\n");
    }
    else
    {
    }
    
    scene_playing->saved_show.pbar = show;
    saved_x = x;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    video_scene_playing_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \retval       void
* \ingroup      video
*******************************************************************************/
void video_playing_draw_status_bg(video_scene_playing_t *scene_playing, char show)
{
    bool changed;
    int handle_gui_dc;

    if ( NULL == scene_playing->pic_status_bg )
    {
        return;
    }
    
    changed = (scene_playing->saved_show.status_bg != show)? true:false;
//    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }

    
    handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.status_canvas_id );
    if(handle_gui_dc <= 0)
    {
        print_err("gui_dc_get err\n");
        return;
    }

    
    if (1 == show)
    {
        //在status_canvas_id画图, 坐标相对于此canvas, 起点xy都为0
        sty_draw_picture_id( handle_gui_dc, scene_playing->pic_status_bg->pic_id, 0, 0 );
    }
    else if (0 == show)
    {
        print_warning("set bg hide is invalid\n\n");
    }
    else
    {
    }
    
    scene_playing->saved_show.status_bg = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of play_pause
* \param[in]    video_scene_playing_t *: 指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 显示的帧号.
* \retval       void
* \ingroup      video
*******************************************************************************/
void video_playing_draw_status(video_scene_playing_t *scene_playing, char show, video_status_frame_e frame)
{
    static int saved_frame = -1;
    bool changed;
    sty_rect_t srect;
    picregion_resource_t tmp_picreg;
    int handle_gui_dc;
    
    if ( (NULL == scene_playing->pic_status_bg) || (NULL == scene_playing->reg_status) )
    {
        return;
    }
    
    changed = ( (scene_playing->saved_show.status != show) || (saved_frame != frame) )? true:false;
//    print_dbg("show:%d, changed:%d, frame:%d, saved_frame:%d", show, changed, frame, saved_frame);
    if ( false == changed )
    {
        return;
    }

      
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != scene_playing->saved_show.status )
        {
            //在status_canvas_id画图, 坐标相对于此canvas, 所有坐标都要减去pic_status_bg
            srect.x     = scene_playing->reg_status->x - scene_playing->pic_status_bg->x;
            srect.y     = scene_playing->reg_status->y - scene_playing->pic_status_bg->y;
            srect.width = scene_playing->reg_status->width;
            srect.height= scene_playing->reg_status->height;
            
            canvas_erase( video_vars.blending.status_canvas_id, &srect );
            
            handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.status_canvas_id );
            if(handle_gui_dc <= 0)
            {
                print_err("gui_dc_get err\n");
                return;
            }
            //先画图片所在区域的背景, 否则前景的png图片会重叠在一起
            sty_draw_sub_picture_id( handle_gui_dc, scene_playing->pic_status_bg->pic_id, srect.x, srect.y, &srect );
        }
        
        //在status_canvas_id画图, 坐标相对于此canvas, 所有坐标都要减去pic_status_bg
        memcpy( &tmp_picreg, scene_playing->reg_status, sizeof(tmp_picreg) );
        scene_playing->reg_status->x -= scene_playing->pic_status_bg->x;
        scene_playing->reg_status->y -= scene_playing->pic_status_bg->y;

        handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.status_canvas_id );
        if(handle_gui_dc <= 0)
        {
            print_err("gui_dc_get err\n");
            return;
        }

        sty_draw_picregion_res( handle_gui_dc, scene_playing->reg_status, frame );
        memcpy( scene_playing->reg_status, &tmp_picreg, sizeof(tmp_picreg) );
    }
    else if (0 == show)
    {
        print_warning("set play_pause hide is invalid\n\n");
    }
    else
    {
    }
    
    scene_playing->saved_show.status = show;
    saved_frame = (int)frame;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str of time_left
* \param[in]    video_scene_playing_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    char *: 显示的字符串.
* \retval       void
* \ingroup      video
*******************************************************************************/
void video_playing_draw_time_left(video_scene_playing_t *scene_playing, char show, unsigned int time)
{
    static unsigned int saved_time = 0;
    bool changed;
    bool result;
    char time_buffer[VIDEO_BUFFER_LENGTH_STR_TIME];
    string_resource_t relative_str;
    int handle_gui_dc;
    
    if ( NULL == scene_playing->str_time_left )
    {
        return;
    }
    
    changed = ( (scene_playing->saved_show.time_left != show) || (saved_time != time) )? true:false;
//    print_dbg("show:%d, changed:%d, title:%s, savedtitle:%s", show, changed, p_str, saved_buffer);
    if ( false == changed )
    {
        return;
    }
    
    result = video_playing_time_to_str( time, time_buffer );
    if ( false == result )
    {
        return;
    }

    
    handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.time_left_canvas_id );
    if(handle_gui_dc <= 0)
    {
        print_err("gui_dc_get err\n");
        return;
    }

    
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != scene_playing->saved_show.time_left )
        {
            canvas_erase_all( video_vars.blending.time_left_canvas_id );
        }
        
        //在time_left_canvas_id画图, 坐标相对于此canvas, 起点xy都为0
        memcpy( &relative_str, scene_playing->str_time_left, sizeof(relative_str) );
        relative_str.x = 0;
        relative_str.y = 0;
        sty_draw_string_res_ext( handle_gui_dc, &relative_str, time_buffer );
    }
    else if (0 == show)
    {
        print_warning("set time hide is invalid\n\n");
    }
    else
    {
    }
    
    scene_playing->saved_show.time_left = show;
    saved_time = scene_playing->cur_time;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str of time_right
* \param[in]    video_scene_playing_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    char *: 显示的字符串.
* \retval       void
* \ingroup      video
*******************************************************************************/
void video_playing_draw_time_right(video_scene_playing_t *scene_playing, char show, unsigned int time)
{
    static unsigned int saved_time = 0;
    bool changed;
    bool result;
    char time_buffer[VIDEO_BUFFER_LENGTH_STR_TIME];
    string_resource_t relative_str;
    int handle_gui_dc;
    
    if ( NULL == scene_playing->str_time_right )
    {
        return;
    }
    
    changed = ( (scene_playing->saved_show.time_right != show) || (saved_time != time) )? true:false;
//    print_dbg("show:%d, changed:%d, title:%s, savedtitle:%s", show, changed, p_str, saved_buffer);
    if ( false == changed )
    {
        return;
    }

    
    handle_gui_dc = gui_dc_get( (unsigned short)video_vars.blending.time_right_canvas_id );
    if(handle_gui_dc <= 0)
    {
        print_err("gui_dc_get err\n");
        return;
    }

    
    result = video_playing_time_to_str( time, time_buffer );
    if ( false == result )
    {
        return;
    }
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != scene_playing->saved_show.time_right )
        {
            canvas_erase_all( video_vars.blending.time_right_canvas_id );
        }
        memcpy( &relative_str, scene_playing->str_time_right, sizeof(relative_str) );
        relative_str.x = 0;
        relative_str.y = 0;
        sty_draw_string_res_ext( handle_gui_dc, &relative_str, time_buffer );
    }
    else if (0 == show)
    {
        print_warning("set time hide is invalid\n\n");
    }
    else
    {
    }
    
    scene_playing->saved_show.time_right = show;
    saved_time = time;
}

