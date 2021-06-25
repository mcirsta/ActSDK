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

/*��Ӧ�õ�ͷ�ļ�*/
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

    /*����ͼƬ������,��(i-1)��ͼƬ����ʧ����*/
    if(i < 6)
    {
        for(j=0; j<(i-1); j++)
        {
            sty_delete_bitmap(prec->status_bitmap[j]);
        }
    }

    /*û�г���ɾ��6��ͼƬ*/
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
*      ����ǰ����ʱ��,��ʱ�� ת���ɽ�������x����ֵ
* \param[in]    int: ��ǰ����ʱ��, ��λ: s
* \param[in]    int: ��ʱ��, ��λ: s
* \param[out]   int *: �����ĸ��������Ҳ� x����
* \retval       
* \ingroup      tvin
*******************************************************************************/
#if 0
//���ε�������������Ҳ���������صĺ���
static bool _time_to_pbar_coordinate( unsigned int cur_time, unsigned int total_time, unsigned int *x_coordinate )
{
    /* ���������Ҳ��x����. ����pbar */
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
*      ����msΪ��λ��ʱ��ת����: "**:**:**"��ʽ���ַ���
* \param[in]    int: ʱ��, ��λ: ms
* \param[out]   int *: ת������ַ�����ʽ:"**:**:**", ʱ:��:����
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
        
    //������
    //second = time / 1000;
    second = time;
    if ( second >= 60 )
    {
        tmp = second;
        second %= 60;
        //�����
        minite = tmp / 60;
        if ( minite >= 60 )
        {
            tmp = minite;
            minite %= 60;
            //����Сʱ
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
* \param[in]    tvin_scene_record_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_draw_pbar_bg(tvin_scene_record_t *scene_record)
{  
    #if 0
    //��Ϊ���м����ȡ��ʣ���¼ʱ�䲻׼ȷ����������ʾ��һ�£�����ȥ����
    
    if ( NULL == scene_record->pic_pbar_bg )
    {
        return;
    }

    /* �����ǰ��ͼƬ�Ѿ���ʾ�����ˣ���û�б�Ҫ�ٻ��� */
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
* \param[in]    tvin_scene_record_t *: ָ��
* \param[in]    int: ���������Ҳ��x����. ������ı���Ҫ�ػ�
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_draw_pbar(tvin_scene_record_t *scene_record)
{
    #if 0
    //��Ϊ���м����ȡ��ʣ���¼ʱ�䲻׼ȷ����������ʾ��һ�£�����ȥ����
    
    static unsigned int saved_cur_time = 0;
    static unsigned int saved_total_time = 0;  

    //��ʼΪ�������������������    
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

    //û�б仯, ֱ�ӷ���. ֻ�б仯�ż�������
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

    //�������䳤, ��С���󻭸���
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
* \param[in]    video_scene_playing_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
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
* \param[in]    tvin_scene_record_t *: ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    int: ��ʾ��֡��.
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
* \param[in]    tvin_scene_record_t *: �ؼ�ָ��
* \param[in]    char *: ��ʾ���ַ���.
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
* \param[in]    tvin_scene_record_t *: �ؼ�ָ��
* \param[in]    int: ��ʾ״̬. 1:show, 0:hide
* \param[in]    char *: ��ʾ���ַ���.
* \retval       void
* \ingroup      video
*******************************************************************************/
void tvin_draw_remaintime(tvin_scene_record_t *scene_record)
{
    #if 0
    //��ʣ��ʱ����ӵ���ߵ���¼ʱ���ϣ���ʾ��00:00:00 / 01:30:48
    //�������ε���Щ����
    
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


