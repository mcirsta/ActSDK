/*******************************************************************************
 *                              NOYA1100
 *                            Module: tvin
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-04-01          1.0             build this file 
*******************************************************************************/
/*!
 * \file     tvin_record_ui.c
 * \brief    vin record ui 
 * \author   Jerry Fu
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/

/*本应用的头文件*/
#include "tvin.h"
#include "tvin_record_ui.h"


bool tvin_create_bitmap(void)
{
    unsigned short i;
    int j;
    
    tvin_scene_record_t  *prec = NULL;
    picregion_frame_t  pic_frame;

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    for(i=0; i<6; i++)
    {
        get_picregion_frame(prec->picregion_record_status,i, &pic_frame);
        prec->status_bitmap[i] = sty_create_bitmap(pic_frame.pic_id);
        if(NULL == prec->status_bitmap[i])
        {
            goto create_status_bitmap_err;            
        }
    }
    
    prec->stabg_bitmap = sty_create_bitmap(prec->pic_record_status_bg->pic_id);
    if(NULL == prec->stabg_bitmap)
    {
        goto create_statusbg_bitmap_err;
    }

    prec->pbar_hl_bitmap = sty_create_bitmap(prec->pic_pbar_hl->pic_id);
    if(NULL == prec->pbar_hl_bitmap)
    {
        goto create_pbar_hl_bitmap_err;
    }
    
    return true;

    create_pbar_hl_bitmap_err:
    sty_delete_bitmap(prec->stabg_bitmap);

    create_statusbg_bitmap_err:
    create_status_bitmap_err:

    /*创建图片出错了,第(i-1)个图片创建失败了*/
    if(i < 6)
    {
        for(j=0; j<(i-1); j++)
        {
            sty_delete_bitmap(prec->status_bitmap[j]);
        }
    }

    /*没有出错，删除6个图片*/
    if(i == 6)
    {
        for(j=0; j<i; j++)
        {
            sty_delete_bitmap(prec->status_bitmap[j]);
        }
    }
    return false;    
}

bool tvin_delete_bitmap(void)
{
    int i;
    tvin_scene_record_t  *prec = NULL;

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    for(i=0; i<6; i++)
    {        
        if(NULL != prec->status_bitmap[i])
        {
            sty_delete_bitmap(prec->status_bitmap[i]); 
        }
    }

    if(NULL != prec->stabg_bitmap)
    {
        sty_delete_bitmap(prec->stabg_bitmap);
    }

    if(NULL != prec->pbar_hl_bitmap)
    {
        sty_delete_bitmap(prec->pbar_hl_bitmap);
    }    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      将当前播放时间,总时间 转换成进度条的x坐标值
* \param[in]    int: 当前播放时间, 单位: s
* \param[in]    int: 总时间, 单位: s
* \param[out]   int *: 计算后的高亮条最右侧 x坐标
* \retval       
* \ingroup      tvin
*******************************************************************************/
#if 0
//屏蔽掉进度条，所以也屏蔽这个相关的函数
static bool _time_to_pbar_coordinate( unsigned int cur_time, unsigned int total_time, unsigned int *x_coordinate )
{
    /* 高亮条最右侧的x坐标. 传给pbar */
    unsigned int x;
    unsigned int pbar_length;
    tvin_scene_record_t *prec = NULL;

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    if ( (NULL == x_coordinate) || (total_time <= 0) || (cur_time > total_time) )
    {
        print_err("parameter err. cur_time:%d, total_time:%d", cur_time, total_time);
        return false;
    }

    pbar_length = (unsigned int)prec->pbar_hl_position_resgroup->width;
    x = (pbar_length * cur_time) / total_time;
    x += prec->xstart_pos;
    *x_coordinate = x;
    return true;
}
#endif

/******************************************************************************/
/*!                    
* \par  Description:
*      将以ms为单位的时间转换成: "**:**:**"格式的字符串
* \param[in]    int: 时间, 单位: ms
* \param[out]   int *: 转换后的字符串格式:"**:**:**", 时:分:秒制
* \retval       
* \ingroup      video
*******************************************************************************/
static bool _time2str( unsigned int time, char *str )
{
    unsigned int tmp;
    unsigned int hour = 0;
    unsigned int minite = 0;
    unsigned int second = 0;
    
    if ( NULL == str )
    {
        print_err("str is NULL");
        return false;
    }
        
    //计算秒
    //second = time / 1000;
    second = time;
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
        }
    }
    
    sprintf( str, "%.2d:%.2d:%.2d", hour, minite, second );
    return true;
}



/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    tvin_scene_record_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_draw_pbar_bg(tvin_scene_record_t *scene_record)
{  
    #if 0
    //因为从中间件获取的剩余可录时间不准确，进度条显示不一致，所以去掉。
    
    if ( NULL == scene_record->pic_pbar_bg )
    {
        return;
    }

    /* 如果当前的图片已经显示出来了，就没有必要再绘制 */
    if(true == (scene_record->pbar_bg_is_show))
    {
        return;
    }

    sty_draw_picture_id( gui_dc_get((unsigned short)(scene_record->blending.pbar_canvas_id)),\
            (int)(scene_record->pic_pbar_bg->pic_id), 0, 0 );
        
    scene_record->pbar_bg_is_show = true;
    #endif
}


void tvin_draw_headbar(tvin_scene_record_t *scene_record)
{
    if(true == (scene_record->headbar_is_show))
    {
        return;
    }

    headbar_paint(scene_record->headbar);
    scene_record->headbar_is_show = true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw pbar pictrue
* \param[in]    tvin_scene_record_t *: 指针
* \param[in]    int: 高亮条最右侧的x坐标. 此坐标改变需要重画
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_draw_pbar(tvin_scene_record_t *scene_record)
{
    #if 0
    //因为从中间件获取的剩余可录时间不准确，进度条显示不一致，所以去掉。
    
    static unsigned int saved_cur_time = 0;
    static unsigned int saved_total_time = 0;  

    //初始为高亮条左侧缩进的坐标    
    unsigned int i;
    sty_rect_t srect;    
    unsigned int x_pos;
    
    if(false == (scene_record->first_draw))
    {
        saved_cur_time = -1;
        saved_total_time = -1;
        
        scene_record->saved_x = scene_record->xstart_pos;
    }

    if ((NULL == scene_record->pic_pbar) || (NULL == scene_record->pic_pbar_hl))
    {
        return;
    }

    //没有变化, 直接返回. 只有变化才继续计算
    if ( (saved_cur_time == scene_record->cur_record_time) && (saved_total_time == scene_record->total_time) )
    {
        return;                
    }

    saved_cur_time = scene_record->cur_record_time;
    saved_total_time = scene_record->total_time;

    if(_time_to_pbar_coordinate(scene_record->cur_record_time, scene_record->total_time,&x_pos) == false)
    {
        return;
    }
                                  
    srect.x = (short)(scene_record->saved_x);
    srect.y = scene_record->pic_pbar_hl->y - scene_record->pic_pbar_bg->y;
    srect.width = (short)(x_pos - scene_record->saved_x);
    srect.height = scene_record->pic_pbar_hl->height;
    canvas_erase(scene_record->blending.pbar_canvas_id, &srect);        

    //高亮条变长, 从小到大画高亮
    for (i=scene_record->saved_x; i<=x_pos; i++)
    {
        sty_draw_bitmap((int)(gui_dc_get((unsigned short)(scene_record->blending.pbar_canvas_id))),\
            scene_record->pbar_hl_bitmap, (int)i, (int)(srect.y));
    }
    scene_record->saved_x = x_pos;
    #endif
}


/******************************************************************************/
/*!                    * \par  Description:*      draw back ground pictrue
* \param[in]    video_scene_playing_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \retval       void
* \ingroup      video
*******************************************************************************/
void tvin_draw_status_bg(tvin_scene_record_t *scene_record)
{    
    if ( NULL == scene_record->pic_record_status_bg )
    {
        return;
    }
    

    if(true == (scene_record->record_statusbg_is_show))
    {
        return;
    }

    sty_draw_picture_id( gui_dc_get((unsigned short)(scene_record->blending.button_canvas_id)),\
        (int)(scene_record->pic_record_status_bg->pic_id), 0, 0);            
    scene_record->record_statusbg_is_show = true;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of play_pause
* \param[in]    tvin_scene_record_t *: 指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 显示的帧号.
* \retval       void
* \ingroup      video
*******************************************************************************/
void tvin_draw_status(tvin_scene_record_t *scene_record)
{    
    tvin_record_status_e frame;
    sty_rect_t  srect;
    
    if ( (NULL == scene_record->pic_record_status_bg) ||
         (NULL == scene_record->picregion_record_status) )
    {
        return;
    }
    
    if(true == scene_record->record_status_is_show)
    {
        return;
    }
    
    srect.x = scene_record->picregion_record_status->x - scene_record->pic_record_status_bg->x;
    srect.y = scene_record->picregion_record_status->y - scene_record->pic_record_status_bg->y;

    canvas_erase_all(scene_record->blending.button_canvas_id);

    sty_draw_bitmap(gui_dc_get((unsigned short)(scene_record->blending.button_canvas_id)),\
        scene_record->stabg_bitmap, 0, 0);

    frame = scene_record->tvin_record_sta;
    sty_draw_bitmap(gui_dc_get((unsigned short)(scene_record->blending.button_canvas_id)),\
        scene_record->status_bitmap[frame], srect.x, srect.y);
    
    scene_record->record_status_is_show = true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw record time display with string,
*      show as "00:00:00 / 01:30:48"
* \param[in]    tvin_scene_record_t *: 控件指针
* \param[in]    char *: 显示的字符串.
* \retval       void
* \ingroup      video
*******************************************************************************/
void tvin_draw_recordtime(tvin_scene_record_t *scene_record)
{
    static  unsigned int save_record_time = -1;
    static  unsigned int save_remain_time = -1;
    char time_buf[40];
    char tmp_buf[16];
    size_t time_len;
    string_resource_t relative_str;

    if(scene_record->first_draw == false)
    {
        save_record_time = -1;
    }
    
    if ( NULL == scene_record->str_record_time )
    {
        return;
    }
       
    if(true == (scene_record->record_time_is_show))
    {
        return;
    }
    
    if( (save_record_time == scene_record->cur_record_time) && \
        (save_remain_time == scene_record->record_remain_time))
    {
        return;
    }

    save_record_time = scene_record->cur_record_time;
    save_remain_time = scene_record->record_remain_time;

    if(_time2str(scene_record->cur_record_time, time_buf) == false)
    {
        return;    
    }
  
    if(_time2str(scene_record->record_remain_time, tmp_buf) == false)
    {
        return;    
    }

    strncat(time_buf, " ", 1);
    strncat(time_buf, "/", 1);
    strncat(time_buf, " ", 1);
    time_len = strlen(tmp_buf);
    strncat(time_buf, tmp_buf, time_len);
    print_dbg("time_buf = %s\n",time_buf);
                                   
    canvas_erase_all(scene_record->blending.recordtime_canvas_id);

    memcpy( &relative_str, scene_record->str_record_time, sizeof(relative_str) );
    relative_str.x = 0;
    relative_str.y = 0;
    
    sty_draw_string_res_ext( gui_dc_get((unsigned short)(scene_record->blending.recordtime_canvas_id)),\
        &relative_str, time_buf );
    
    scene_record->record_time_is_show = true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str of time_right
* \param[in]    tvin_scene_record_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    char *: 显示的字符串.
* \retval       void
* \ingroup      video
*******************************************************************************/
void tvin_draw_remaintime(tvin_scene_record_t *scene_record)
{
    #if 0
    //把剩余时间叠加到左边的已录时间上，显示成00:00:00 / 01:30:48
    //所以屏蔽掉这些代码
    
    static unsigned int save_remain_time = -1;
    char tmp_buf[16];
    string_resource_t relative_str;

    if(scene_record->first_draw == false)
    {
        save_remain_time = 0;
    }
    
    if ( NULL == scene_record->str_remain_time )
    {
        return;
    }
       
    if(true == scene_record->remain_time_is_show)
    {
        return;
    }
    
    if((save_remain_time == (scene_record->record_remain_time))\
        && (scene_record->first_draw))
    {
        return;
    }

    save_remain_time = scene_record->record_remain_time;

    if(_time2str(scene_record->record_remain_time, tmp_buf) == false)
    {
        return;
    }
               
    canvas_erase_all(scene_record->blending.remaintime_canvas_id);

    memcpy( &relative_str, scene_record->str_remain_time, sizeof(relative_str) );
    relative_str.x = 0;
    relative_str.y = 0;
    
    sty_draw_string_res_ext( gui_dc_get((unsigned short)(scene_record->blending.remaintime_canvas_id)),\
        &relative_str, tmp_buf );            
    scene_record->remain_time_is_show = true;
    #endif
}


