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
 * \brief    ʵ��dialog�ؼ���Ԫ�ػ�ͼ
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/02
*******************************************************************************/

#include "include_case.h"


/*��Ӧ�õ�ͷ�ļ�*/
#include "dialog_private.h"
#include "commonui_sty.h"




/*
* ��������ӳ�䵽��ʾ�ĵ�һ����������������. ֵ��0��ʼ, ��������total_line��1��ʼ
* ����clip ����ʾ4�����: {0, 1, 1, 0, 0, 0}��ʾ:
* ��0��, ��һ����ʾ�ڵ�0��(map_dialog_start_line�������ֵ��0��ʼ, 0���ǵ�1��)
* ��1��, ��һ����ʾ�ڵ�1��(map_dialog_start_line�������ֵ��0��ʼ, 1���ǵ�2��)
* ��2��, ��һ����ʾ�ڵ�1��(map_dialog_start_line�������ֵ��0��ʼ, 1���ǵ�2��)
* ��3��, ��һ����ʾ�ڵ�0��(map_dialog_start_line�������ֵ��0��ʼ, 0���ǵ�1��)
* ��4��, ��һ����ʾ�ڵ�0��(map_dialog_start_line�������ֵ��0��ʼ, 0���ǵ�1��)
*/
static char map_clip_start_line[MAX_LINE_CLIP+2] = {0, 1, 1, 0, 0, 0};
//����clip ����ʾ5������趨����������
//static char map_clip_start_line[MAX_LINE_CLIP+2] = {0, 2, 1, 1, 0, 0, 0};

/*
* ����dialog ����ʾ2�����
*/
static char map_dialog_start_line[MAX_LINE_DIALOG+2] = {0, 0, 0, 0};
//����dialog ����ʾ3������趨����������
//static char map_dialog_start_line[MAX_LINE_DIALOG+2] = {0, 1, 0, 0, 0};




#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*      draw back ground pictrue
* \param[in]    dialog_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
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
* \param[in]    dialog_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    int: ��ʾ��֡��.
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
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
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
* \param[in]    dialog_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    int: ��ʾ��֡��.
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
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
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
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
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
* \param[in]    dialog_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    int: ��ʾ��֡��.
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
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
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
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
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
*      ��dialog��Ҫ��ʾ���ַ������ά����, ʵ�ֳ��ַ���������ʾ. �Իس�ʵ�ֻ���
*      ��ά����ĵ�һά������һһ��Ӧ
* \param[in]    dialog_t *: �ؼ�ָ��
* \param[in]    char *: ��Ҫ��ʾ���ַ���
* \param[out]   dialog_clew_param_t *: ������ʾ�ַ����Ĳ������
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
        //���ַ���ĩβ, ������һ��, ���˳�
        if (0 == *clew)
        {
            line_num++;
            break;
        }
        
        //���ǻس�, ��װbuffer_clew������ʾ: ��clew������buffer_clew��line_num����
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
        //�ǻس�, ����һ��, ��������clew������ַ�
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
    
    //����Ƿ񳬹������������, �������ֱ��ɾ��������
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
    //��Ҫ��ʾ��������
    param->total_line = line_num;
    
    //ͨ����Ҫ��ʾ��������, ��map����, ӳ�����ʼ��ʾ���к�
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
*       �����str_id����"\n"����, dialog�Զ���⵽"\n"��ʵ�ֻ�����ʾ����
* \param[in]    dialog_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    char *: ��ʾ���ַ���.
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
        //��һ�δ����ؼ��Ѿ����˱���, �������ٻ�����. ��֮����Ҫ�Ȼ������ٻ����ϵ�ͼƬ
        if ( -1 != dialog->private->saved_show.clew )
        {
            canvas_erase( dialog->canvas_id, &srect );
            sty_draw_sub_picture_id( dialog->private->handle_gui_dc, \
                dialog->private->pic_bg->pic_id, srect.x, srect.y, &srect );
        }
        
        //������ʾ�ĵ�һ��y����
        srect.y += (clew_param->start_line * LINE_HEIGHT);
        srect.height = LINE_HEIGHT;
        
        //����str�ĸ�ʽ, ���ڻ���û�и�ʽ��str. һ����sty_restore_string_format���ʹ��
        sty_set_string_format( dialog->private->handle_gui_dc, dialog->private->str_clew );
        //����ÿһ��y����, ������ʾ
        for (i=0; i<clew_param->total_line; i++)
        {
            sty_draw_string_buf(dialog->private->handle_gui_dc, clew_param->buffer_clew[i], \
                    &srect, dialog->private->str_clew->text_align );
            srect.y += LINE_HEIGHT;
        }
        //�ָ�str�ĸ�ʽ. һ����sty_set_string_format���ʹ��
        sty_restore_string_format( dialog->private->handle_gui_dc );
    }
    else if (0 == show)
    {
        //��������״̬��ͼ��, ��һ�δ����ؼ��Ȼ��ı���, ���Ѿ������ص�, �������ٻ�����. 
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

