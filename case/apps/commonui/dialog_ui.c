/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-02          1.0             build this file 
*******************************************************************************/
/*!
 * \file     dialog_ui.c
 * \brief    实现dialog控件各元素画图
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/02
*******************************************************************************/

#include "include_case.h"


/*本应用的头文件*/
#include "dialog_private.h"
#include "commonui_sty.h"




/*
* 由总行数映射到显示的第一行所在行数的数组. 值从0开始, 数组索引total_line从1开始
* 对于clip 共显示4行情况: {0, 1, 1, 0, 0, 0}表示:
* 共0行, 第一行显示在第0行(map_dialog_start_line数组的数值从0开始, 0就是第1行)
* 共1行, 第一行显示在第1行(map_dialog_start_line数组的数值从0开始, 1就是第2行)
* 共2行, 第一行显示在第1行(map_dialog_start_line数组的数值从0开始, 1就是第2行)
* 共3行, 第一行显示在第0行(map_dialog_start_line数组的数值从0开始, 0就是第1行)
* 共4行, 第一行显示在第0行(map_dialog_start_line数组的数值从0开始, 0就是第1行)
*/
static char map_clip_start_line[MAX_LINE_CLIP+2] = {0, 1, 1, 0, 0, 0};
//对于clip 共显示5行情况需定义如下数组
//static char map_clip_start_line[MAX_LINE_CLIP+2] = {0, 2, 1, 1, 0, 0, 0};

/*
* 对于dialog 共显示2行情况
*/
static char map_dialog_start_line[MAX_LINE_DIALOG+2] = {0, 0, 0, 0};
//对于dialog 共显示3行情况需定义如下数组
//static char map_dialog_start_line[MAX_LINE_DIALOG+2] = {0, 1, 0, 0, 0};




#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    dialog_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \retval       void
* \ingroup      common_dialog
*******************************************************************************/
void dialog_draw_bg(dialog_t *dialog, char show)
{
    bool changed;

    if ( NULL == dialog->private->pic_bg )
    {
        return;
    }
    
    changed = (dialog->private->saved_show.bg != show)? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    if (1 == show)
    {
        sty_draw_picture_res( dialog->private->handle_gui_dc, dialog->private->pic_bg );
    }
    else if (0 == show)
    {
        print_warning("set bg hide, is invalid");
    }
    else
    {
    }
    
    dialog->private->saved_show.bg = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of head
* \param[in]    dialog_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 显示的帧号.
* \retval       void
* \ingroup      common_dialog
*******************************************************************************/
void dialog_draw_head(dialog_t *dialog, char show, unsigned short frame)
{
    static unsigned short saved_frame = -1;
    bool changed;
    
    if ( (NULL == dialog->private->reg_head) || (NULL == dialog->private->pic_bg) )
    {
        return;
    }
    
    changed = ( (dialog->private->saved_show.head != show) || (saved_frame != frame) )? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != dialog->private->saved_show.head )
        {
            sty_rect_t srect;
            srect.x     = dialog->private->reg_head->x;
            srect.y     = dialog->private->reg_head->y;
            srect.width = dialog->private->reg_head->width;
            srect.height= dialog->private->reg_head->height;
            
            canvas_erase( dialog->canvas_id, &srect );
            sty_draw_sub_picture_id( dialog->private->handle_gui_dc, dialog->private->pic_bg->pic_id, \
                srect.x, srect.y, &srect );
        }
        sty_draw_picregion_res( dialog->private->handle_gui_dc, dialog->private->reg_head, frame );
    }
    else if (0 == show)
    {
        print_warning("set head hide, is invalid");
    }
    else
    {
    }
    
    dialog->private->saved_show.head = show;
    saved_frame = frame;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of yes
* \param[in]    dialog_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 显示的帧号.
* \retval       void
* \ingroup      common_dialog
*******************************************************************************/
void dialog_draw_yes(dialog_t *dialog, char show, unsigned short frame)
{
    static unsigned short saved_frame = -1;
    bool changed;
    sty_rect_t srect;
    
    if ( ( NULL == dialog->private->reg_yes ) || ( NULL == dialog->private->pic_bg ) )
    {
        return;
    }
    
    /* be clip, we will not draw it */
    if ( 0 == dialog->total_button )
    {
        return;
    }
    
    changed = ( (dialog->private->saved_show.yes != show) || (saved_frame != frame) )? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = dialog->private->reg_yes->x;
    srect.y     = dialog->private->reg_yes->y;
    srect.width = dialog->private->reg_yes->width;
    srect.height= dialog->private->reg_yes->height;
    
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != dialog->private->saved_show.yes )
        {
            canvas_erase( dialog->canvas_id, &srect );
            sty_draw_sub_picture_id( dialog->private->handle_gui_dc, dialog->private->pic_bg->pic_id, \
                srect.x, srect.y, &srect );
        }
        sty_draw_picregion_res( dialog->private->handle_gui_dc, dialog->private->reg_yes, frame );
    }
    else if (0 == show)
    {
        //处于隐藏状态的图标, 第一次创建控件先画的背景, 即已经是隐藏的, 则无需再画背景. 
        if ( -1 != dialog->private->saved_show.yes )
        {
            canvas_erase( dialog->canvas_id, &srect );
            //draw the background of yes region, to hide yes pic
            sty_draw_sub_picture_id(dialog->private->handle_gui_dc, dialog->private->pic_bg->pic_id, \
                dialog->private->reg_yes->x, dialog->private->reg_yes->y, &srect);
        }
    }
    else
    {
    }
    
    dialog->private->saved_show.yes = show;
    saved_frame = frame;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of no
* \param[in]    dialog_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    int: 显示的帧号.
* \retval       void
* \ingroup      common_dialog
*******************************************************************************/
void dialog_draw_no(dialog_t *dialog, char show, unsigned short frame)
{
    static unsigned short saved_frame = -1;
    bool changed;
    sty_rect_t srect;
    
    if ( ( NULL == dialog->private->reg_no ) || ( NULL == dialog->private->pic_bg ) )
    {
        return;
    }
    
    /* be clip, we will not draw it */
    if ( 0 == dialog->total_button )
    {
        return;
    }
    
    changed = ( (dialog->private->saved_show.no != show) || (saved_frame != frame) )? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = dialog->private->reg_no->x;
    srect.y     = dialog->private->reg_no->y;
    srect.width = dialog->private->reg_no->width;
    srect.height= dialog->private->reg_no->height;
    
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != dialog->private->saved_show.no )
        {
            canvas_erase( dialog->canvas_id, &srect );
            sty_draw_sub_picture_id( dialog->private->handle_gui_dc, dialog->private->pic_bg->pic_id, \
                srect.x, srect.y, &srect );
        }
        sty_draw_picregion_res( dialog->private->handle_gui_dc, dialog->private->reg_no, frame );
    }
    else if (0 == show)
    {
        //处于隐藏状态的图标, 第一次创建控件先画的背景, 即已经是隐藏的, 则无需再画背景. 
        if ( -1 != dialog->private->saved_show.no )
        {
            canvas_erase( dialog->canvas_id, &srect );
            //draw the background of no region, to hide no pic
            sty_draw_sub_picture_id(dialog->private->handle_gui_dc, dialog->private->pic_bg->pic_id, \
                dialog->private->reg_no->x, dialog->private->reg_no->y, &srect);
        }
    }
    else
    {
    }
    
    dialog->private->saved_show.no = show;
    saved_frame = frame;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      将dialog需要显示的字符放入二维数组, 实现长字符串多行显示. 以回车实现换行
*      二维数组的第一维与行数一一对应
* \param[in]    dialog_t *: 控件指针
* \param[in]    char *: 需要显示的字符串
* \param[out]   dialog_clew_param_t *: 计算显示字符串的参数结果
* \retval       
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_get_clew_param( dialog_t *dialog, unsigned short str_id, dialog_clew_param_t *param )
{
    static unsigned short saved_str_id = -1;
    bool changed;
    int i = 0;
    char line_num = 0;
    char *p_tmp = NULL;
    char *clew = NULL;
    
    if ( NULL == param )
    {
        print_err("param is NULL");
        return false;
    }
    changed = (saved_str_id != str_id)? true:false;
    if ( false == changed )
    {
        return false;
    }
    
    clew = (char *)sty_get_string( str_id );
    if ( NULL == clew )
    {
        return false;
    }
    
    print_dbg("clew:%s", clew);
    memset(param->buffer_clew, 0x00, sizeof(param->buffer_clew));
    p_tmp = param->buffer_clew[(int)line_num];
    
    while (1)
    {
        //是字符串末尾, 则增加一行, 并退出
        if (0 == *clew)
        {
            line_num++;
            break;
        }
        
        //不是回车, 组装buffer_clew用于显示: 将clew拷贝到buffer_clew的line_num行中
//        if (*clew != '\n')
        if (*clew != '\t')
        {
            *p_tmp = *clew;
//            print_dbg("line:%d, char:%c, char num:%d", line_num, *clew, i);
            p_tmp++;
            clew++;
            i++;
            if (i >= MAX_CHAR_PER_LINE)
            {
                print_warning("clew is too long, it must't be more than %d chars per line\n\n\n", MAX_CHAR_PER_LINE);
                break;
            }
        }
        //是回车, 增加一行, 继续查找clew下面的字符
        else
        {
            line_num++;
            print_dbg("add to:%dlines, last line str:%s\n", line_num, param->buffer_clew[line_num-1]);
            if (line_num > MAX_LINE_CLIP)
            {
                print_warning("line num is too long, it must't be more than %d lines\n\n\n", MAX_LINE_CLIP);
                break;
            }
            i = 0;
            p_tmp = param->buffer_clew[(int)line_num];
            clew++;
        }
    }/* end while (1) */
    
    //检查是否超过最大行数限制, 如果超过直接删掉多余行
    if ( (0 == dialog->total_button) && (line_num > MAX_LINE_CLIP) )
    {
        print_warning("clip line num too long, must less than %d\n\n\n", MAX_LINE_CLIP);
        line_num = MAX_LINE_CLIP;
    }
    if ( (0 != dialog->total_button) && (line_num > MAX_LINE_DIALOG) )
    {
        print_warning("dialog line num too long, must less than %d\n\n\n", MAX_LINE_DIALOG);
        line_num = MAX_LINE_DIALOG;
    }
    //需要显示的总行数
    param->total_line = line_num;
    
    //通过需要显示的总行数, 由map数组, 映射出起始显示的行号
    if (0 == dialog->total_button)
    {
        param->start_line = map_clip_start_line[(int)param->total_line];
    }
    else
    {
        param->start_line = map_dialog_start_line[(int)param->total_line];
    }
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str of title
*       传入的str_id需以"\n"换行, dialog自动检测到"\n"后实现换行显示功能
* \param[in]    dialog_t *: 控件指针
* \param[in]    int: 显示状态. 1:show, 0:hide
* \param[in]    char *: 显示的字符串.
* \retval       void
* \ingroup      common_dialog
*******************************************************************************/
void dialog_draw_clew(dialog_t *dialog, char show, dialog_clew_param_t *clew_param)
{
    static dialog_clew_param_t saved_clew_param;
    bool changed;
    int i;
    sty_rect_t srect;
    
    if ( ( NULL == dialog->private->str_clew ) || ( NULL == dialog->private->pic_bg ) )
    {
        return;
    }
    if ( NULL == clew_param )
    {
        return;
    }
    
    changed = (dialog->private->saved_show.clew != show)? true:false;
    if ( false == changed )
    {
        if ((saved_clew_param.start_line != clew_param->start_line) || \
            (saved_clew_param.total_line != clew_param->total_line) || \
            ( 0 != strcmp(saved_clew_param.buffer_clew[0], clew_param->buffer_clew[0]) ) )
        {
            changed = true;
        }
    }
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x      = dialog->private->str_clew->x;
    srect.y      = dialog->private->str_clew->y;
    srect.width  = dialog->private->str_clew->width;
    srect.height= dialog->private->str_clew->height;
    
    if (1 == show)
    {
        //第一次创建控件已经画了背景, 则无需再画背景. 但之后都需要先画背景再画其上的图片
        if ( -1 != dialog->private->saved_show.clew )
        {
            canvas_erase( dialog->canvas_id, &srect );
            sty_draw_sub_picture_id( dialog->private->handle_gui_dc, \
                dialog->private->pic_bg->pic_id, srect.x, srect.y, &srect );
        }
        
        //计算显示的第一行y坐标
        srect.y += (clew_param->start_line * LINE_HEIGHT);
        srect.height = LINE_HEIGHT;
        
        //设置str的格式, 用于绘制没有格式的str. 一定与sty_restore_string_format配对使用
        sty_set_string_format( dialog->private->handle_gui_dc, dialog->private->str_clew );
        //计算每一行y坐标, 逐行显示
        for (i=0; i<clew_param->total_line; i++)
        {
            sty_draw_string_buf(dialog->private->handle_gui_dc, clew_param->buffer_clew[i], \
                    &srect, dialog->private->str_clew->text_align );
            srect.y += LINE_HEIGHT;
        }
        //恢复str的格式. 一定与sty_set_string_format配对使用
        sty_restore_string_format( dialog->private->handle_gui_dc );
    }
    else if (0 == show)
    {
        //处于隐藏状态的图标, 第一次创建控件先画的背景, 即已经是隐藏的, 则无需再画背景. 
        if ( -1 != dialog->private->saved_show.clew )
        {
            canvas_erase( dialog->canvas_id, &srect );
            sty_draw_sub_picture_id(dialog->private->handle_gui_dc, dialog->private->pic_bg->pic_id, \
                dialog->private->str_clew->x, dialog->private->str_clew->y, &srect);
        }
    }/* end if (1 == show) */
    else
    {
    }
    
    dialog->private->saved_show.clew = show;
    saved_clew_param = *clew_param;
//shaofeng    memcpy( &saved_clew_param, clew_param, sizeof(saved_clew_param) );
}

