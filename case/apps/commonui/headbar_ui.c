/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-17          1.0             build this file 
*******************************************************************************/
/*!
 * \file     headbar_ui.c
 * \brief    ʵ��headbar�ؼ���Ԫ�ػ�ͼ
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/17
*******************************************************************************/

#include "include_case.h"


/*��Ӧ�õ�ͷ�ļ�*/
#include "headbar_private.h"
#include "commonui_sty.h"


/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_bg(headbar_t *headbar, char show)
{
    bool changed;

    if ( NULL == headbar->private->pic_bg )
    {
        return;
    }
    
    changed = (headbar->private->saved_show.bg != show)? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    
    if (1 == show)
    {
        if (true == changed)
        {
            sty_draw_picture_res( headbar->private->handle_gui_dc, headbar->private->pic_bg );
        }
    }
    else if (0 == show)
    {
        print_warning("set bg hide is invalid");
    }
    else
    {
    }
    
    headbar->private->saved_show.bg = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw music play icon pictrue
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_music_play(headbar_t *headbar, char show)
{
    bool changed;
    sty_rect_t srect;
    
    if ( NULL == headbar->private->pic_music_play )
    {
        return;
    }
    
    changed = (headbar->private->saved_show.music_play != show)? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->pic_music_play->x;
    srect.y     = headbar->private->pic_music_play->y;
    srect.width = headbar->private->pic_music_play->width;
    srect.height= headbar->private->pic_music_play->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.music_play )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id( headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect );
            }
        }
        sty_draw_picture_res( headbar->private->handle_gui_dc, headbar->private->pic_music_play );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.music_play )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //draw the background of music_play region, to hide music_play
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    srect.x, srect.y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.music_play = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw keylock icon pictrue
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_keylock(headbar_t *headbar, char show)
{
    bool changed;
    sty_rect_t srect;
    
    if ( NULL == headbar->private->pic_keylock )
    {
        return;
    }
    
    changed = (headbar->private->saved_show.keylock != show)? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->pic_keylock->x;
    srect.y     = headbar->private->pic_keylock->y;
    srect.width = headbar->private->pic_keylock->width;
    srect.height= headbar->private->pic_keylock->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.keylock )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id( headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect );
            }
        }
        sty_draw_picture_res( headbar->private->handle_gui_dc, headbar->private->pic_keylock );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.keylock )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //draw the background of keylock, to hide keylock
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->pic_keylock->x, headbar->private->pic_keylock->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.keylock = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw card icon pictrue
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_card(headbar_t *headbar, char show)
{
    bool changed;
    sty_rect_t srect;
    
    if ( NULL == headbar->private->pic_card )
    {
        return;
    }
    
    changed = (headbar->private->saved_show.card != show)? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->pic_card->x;
    srect.y     = headbar->private->pic_card->y;
    srect.width = headbar->private->pic_card->width;
    srect.height= headbar->private->pic_card->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.card )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id( headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect );
            }
        }
        sty_draw_picture_res( headbar->private->handle_gui_dc, headbar->private->pic_card );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.card )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //draw the background of chard, to hide card pic
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->pic_card->x, headbar->private->pic_card->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.card = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw usb icon pictrue
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_usb(headbar_t *headbar, char show)
{
    bool changed;
    sty_rect_t srect;
    
    if ( NULL == headbar->private->pic_usb )
    {
        return;
    }
    
    changed = (headbar->private->saved_show.usb != show)? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->pic_usb->x;
    srect.y     = headbar->private->pic_usb->y;
    srect.width = headbar->private->pic_usb->width;
    srect.height= headbar->private->pic_usb->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.usb )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id( headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect );
            }
        }
        sty_draw_picture_res( headbar->private->handle_gui_dc, headbar->private->pic_usb );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.usb )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //draw the background of usb, to hide usb
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->pic_usb->x, headbar->private->pic_usb->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.usb = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of pic title
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    int: ����ap�����pic_id, ֱ����ʾ
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_head(headbar_t *headbar, char show, unsigned short pic_id)
{
    static unsigned short saved_pic_id = -1;
    bool changed;
    sty_rect_t srect;
    
    if ( NULL == headbar->private->reg_head )
    {
        return;
    }
    
    changed = ( (headbar->private->saved_show.head != show) || (saved_pic_id != pic_id) )? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->reg_head->x;
    srect.y     = headbar->private->reg_head->y;
    srect.width = headbar->private->reg_head->width;
    srect.height= headbar->private->reg_head->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.head )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id( headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect );
            }
        }
        //����ap�����pic_id, ֱ����ʾ
        sty_draw_picture_id( headbar->private->handle_gui_dc, \
            pic_id, headbar->private->reg_head->x, headbar->private->reg_head->y);
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.head )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //draw the background of title region, to hide title
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->reg_head->x, headbar->private->reg_head->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.head = show;
    saved_pic_id = pic_id;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw region of pic battery
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    int: ��ʾ��֡��.
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_battery(headbar_t *headbar, char show, unsigned short frame)
{
    static unsigned short saved_frame = -1;
    bool changed;
    sty_rect_t srect;
    
    if ( NULL == headbar->private->reg_battery )
    {
        return;
    }
    
    changed = ( (headbar->private->saved_show.battery != show) || (saved_frame != frame) )? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->reg_battery->x;
    srect.y     = headbar->private->reg_battery->y;
    srect.width = headbar->private->reg_battery->width;
    srect.height= headbar->private->reg_battery->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.battery )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id( headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect );
            }
        }
        sty_draw_picregion_res( headbar->private->handle_gui_dc, headbar->private->reg_battery, frame );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.battery )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //draw the background of battery region, to hide battery icon
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->reg_battery->x, headbar->private->reg_battery->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.battery = show;
    saved_frame = frame;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str of title
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    unsigned short: ��ʾ���ַ���id.
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_title_by_id(headbar_t *headbar, char show, unsigned short str_id)
{
    static unsigned short saved_str_id;
    bool changed;
    sty_rect_t srect;
    
    if ( NULL == headbar->private->str_title )
    {
        return;
    }
    
    changed = ( (headbar->private->saved_show.title != show) || (saved_str_id != str_id) )? true:false;
    print_dbg("show:%d, changed:%d, str_id:%d", show, changed, str_id);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->str_title->x;
    srect.y     = headbar->private->str_title->y;
    srect.width = headbar->private->str_title->width;
    srect.height= headbar->private->str_title->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.title )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�Ȼ��ַ���������ı���, ����ǰ����ַ����ص���һ��
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect);
            }
        }
        //ʹ��headbar��str��Դ����, ���ⲿ�����strid��ʾ����. ����ֻ��stridû������, ��ʾ��Ч��û��������
        headbar->private->str_title->str_id = str_id;
        sty_draw_string_res( headbar->private->handle_gui_dc, headbar->private->str_title);
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.title )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //���ַ���������ı���, ����ַ�
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->str_title->x, headbar->private->str_title->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.title = show;
    saved_str_id = str_id;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str of title
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    char *: ��ʾ���ַ���.
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_title_by_buffer(headbar_t *headbar, char show, char *buffer)
{
    bool changed;
    sty_rect_t srect;
    
    if ( (NULL == headbar->private->str_title) || (NULL == buffer) )
    {
        return;
    }
    
    changed = ( headbar->private->saved_show.title != show )? true:false;
    print_dbg("show:%d, changed:%d", show, changed);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->str_title->x;
    srect.y     = headbar->private->str_title->y;
    srect.width = headbar->private->str_title->width;
    srect.height= headbar->private->str_title->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.title )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�Ȼ��ַ���������ı���, ����ǰ����ַ����ص���һ��
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect);
            }
        }
        //ʹ��headbar��str��Դ����, ���ⲿ�����strid��ʾ����. ����ֻ��stridû������, ��ʾ��Ч��û��������
        sty_draw_string_res_ext( headbar->private->handle_gui_dc, headbar->private->str_title, buffer );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.title )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //���ַ���������ı���, ����ַ�
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->str_title->x, headbar->private->str_title->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.title = show;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      draw str of time
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    char *: ��ʾ���ַ���.
* \retval       void
* \ingroup      common_headbar
*******************************************************************************/
void headbar_draw_time(headbar_t *headbar, char show, char *p_time)
{
    static char saved_buffer[HEADBAR_BUFFER_LENGTH_STR_TIME];
    bool changed;
    int tmp;
    sty_rect_t srect;
    
    if ( ( NULL == headbar->private->str_time ) || (NULL == p_time) )
    {
        return;
    }
    
    tmp = strcmp(saved_buffer, p_time);
    changed = ( (headbar->private->saved_show.time != show) || (tmp != 0) )? true:false;
    print_dbg("show:%d, changed:%d, time:%s, savedtime:%s", show, changed, p_time, saved_buffer);
    if ( false == changed )
    {
        return;
    }
    
    srect.x     = headbar->private->str_time->x;
    srect.y     = headbar->private->str_time->y;
    srect.width = headbar->private->str_time->width;
    srect.height= headbar->private->str_time->height;
    
    if (1 == show)
    {
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != headbar->private->saved_show.time )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //�Ȼ��ַ���������ı���, ����ǰ����ַ����ص���һ��
            //redraw string background, then draw new string. or strings will �ص���һ��
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, \
                    headbar->private->pic_bg->pic_id, srect.x, srect.y, &srect);
            }
        }
        sty_draw_string_res_ext( headbar->private->handle_gui_dc, headbar->private->str_time, p_time );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
        if ( -1 != headbar->private->saved_show.time )
        {
            canvas_erase( headbar->canvas_id, &srect );
            //���ַ���������ı���, ����ַ�
            //�б�����draw, �����canvas_erase
            if ( NULL != headbar->private->pic_bg )
            {
                sty_draw_sub_picture_id(headbar->private->handle_gui_dc, headbar->private->pic_bg->pic_id, \
                    headbar->private->str_time->x, headbar->private->str_time->y, &srect);
            }
        }
    }
    else
    {
    }
    
    headbar->private->saved_show.time = show;
    memset(saved_buffer, 0x00, sizeof(saved_buffer));
    strcpy(saved_buffer, p_time);
}

