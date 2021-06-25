/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-16          v1.0               build this file 
********************************************************************************/
/*!
 * \file     load_menuitem.c
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/16
*******************************************************************************/
#include "include_case.h"


/*��Ӧ�õ�ͷ�ļ�*/
#include "include_video.h"




/******************************************************************************/
/*!                    
* \par  Description:
*      ��resgroup��, ��ȡnum��video_menu_info_t������Ϣ, ���͵���menuitemָ��������ʾ
*      ���style_str_id����, ��ui-editor��Դ��style_str_id�ҵ�str res, ��ת����strid, ��������ʾ
*      ���style_pic_id����, ��ui-editor��Դ��style_pic_id�ҵ�pic res, ��ת����picid, ��������ʾ
* \param[in]    unsigned int: resgroup_id, ui-editor��Դ��
* \param[in]    video_menu_info_t: Ԥ�ȶ���ľ�̬�˵�, ���л�ȡstyle_id, ����menu��ʾ
* \param[in]    int: ��video_menu_info_t��ȡ�ĸ���
* \param[out]   menuitem_t *: ������ʾ�� menu ָ��
* \retval       true:��ȡ�ɹ�; false:��ȡʧ��
*******************************************************************************/
bool video_load_menuitem( unsigned int resgroup_id, const video_menu_info_t *video_menu_info, \
                        int num, menuitem_t *items )
{
    scene_t* scene;
    resgroup_resource_t *resgroup;
    string_resource_t *str_res = NULL;
    picture_resource_t *pic_res = NULL;
    int i;
    
    if ( (NULL == items) || (num <= 0) )
    {
        return false;
    }
    
    //get scene
    scene = sty_get_app_scene();
    if ( scene == NULL )
    {
        print_err("cann't find app scene!");
        return false;
    }

    //load resource resgroup
    resgroup = ( resgroup_resource_t * )get_scene_child( scene, resgroup_id );
    
    //load resource
    for ( i=0; i<num; i++ )
    {
        if ( 0 != video_menu_info[i].style_str_id )
        {
            str_res = ( string_resource_t * )get_resgroup_child( resgroup, video_menu_info[i].style_str_id ); 
            if ( str_res != NULL )
            {
                items[i].text_id = str_res->str_id;
            }
            else
            {
                items[i].text_id = 0;
            }
        }
        
        if ( 0 != video_menu_info[i].style_unsel_pic_id )
        {
            pic_res = ( picture_resource_t * )get_resgroup_child( resgroup, video_menu_info[i].style_unsel_pic_id ); 
            if ( pic_res != NULL )
            {
                items[i].unsel_icon_id = pic_res->pic_id;
            }
            else
            {
                items[i].unsel_icon_id = 0;
            }
        }
        if ( 0 != video_menu_info[i].style_sel_pic_id )
        {
            pic_res = ( picture_resource_t * )get_resgroup_child( resgroup, video_menu_info[i].style_sel_pic_id ); 
            if ( pic_res != NULL )
            {
                items[i].sel_icon_id = pic_res->pic_id;
            }
            else
            {
                items[i].sel_icon_id = 0;
            }
        }
    }
    
    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      ��taglist, ��ȡnum�� tag������Ϣ, ��������menuitemָ��������ʾ
* \param[in]    int: ��taglist��ȡ��tag����
* \param[out]   video_tag_info_t *: tag���� ����ָ��
* \param[out]   menuitem_t *: ������ʾ�� tag menu ָ��
* \retval       true:��ȡ�ɹ�; false:��ȡʧ��
*******************************************************************************/
bool video_load_taglist_menuitem( int num, video_tag_info_t *tag_info, menuitem_t *menuitem )
{
    int i;
    
    if ( (NULL == tag_info) || (NULL == menuitem) )
    {
        return false;
    }
    
    if ( num <= 0 )
    {
        return false;
    }
    else if ( num > VIDEO_TAGLIST_MAX_NUM )
    {
        print_warning("video taglist item:%d, out of range", num);
        num = VIDEO_TAGLIST_MAX_NUM;
    }
    else
    {
    }
    
    //��ȡfile_name��time������ʾ
    for ( i=0; i<num; i++ )
    {
        video_taglist_get_tag_info( i, &tag_info[i] );
        
        menuitem[i].text = tag_info[i].file_name;
        
        memset( tag_info[i].time_buffer, 0x00, sizeof(tag_info[i].time_buffer) );
        video_playing_time_to_str( tag_info[i].time, tag_info[i].time_buffer );
        menuitem[i].info1 = tag_info[i].time_buffer;
        print_dbg("tag:%d, filename:%s, time:%s", i, tag_info[i].file_name, tag_info[i].time_buffer);
    }
    
    return true;
}

