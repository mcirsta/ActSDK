/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_control.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_control.h
* \brief    music control operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef   __MUSIC_CONTROL_H
#define   __MUSIC_CONTROL_H

#include "music.h"


/************************************************************************
* 
*                      ��Ӻ궨��
*           
*************************************************************************/
bool music_open_engine( bool waiting );
bool music_close_engine( void );

bool music_get_engine_info(mengine_t *engine);
bool music_get_filepath(char *filepath, int max_len);

/************************************************************************
* Function:         music_set_eq
* Description:      ����EQ 
* para1��           eq �Ӹõ�ַ��ʼ���Ҫ����EQ����Ϣ
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_set_eq(mengine_eq_t *eq_val);

bool music_set_playmode(play_mode_e playmode);
bool music_set_displaymode(display_mode_e displaymode);
bool music_set_fade(bool fade);

/************************************************************************
* Function:         music_set_playspeed
* Description:      ���������ٶȣ�ֵ��Χ[-8��8] 
* para1��           play_speed  �����ٶ�
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_set_playspeed(int playspeed);

bool music_set_ab_auto(bool auto_play);

/************************************************************************
* Function:         music_set_ab_count
* Description:      ����ABѭ��������ֵ��Χ[0��+��] 
* para1��           count ABѭ�����Ŵ���
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_set_ab_count(int count);

/************************************************************************
* Function:         music_set_ab_time
* Description:      ���ô�B��ص�A��ĵȴ�ʱ��
* para1��           wait_time ��B�㷵�ص�A��ĵȴ�ʱ��
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_set_ab_time(int time);

bool music_start_ab(void);
bool music_clear_ab(void);
bool music_set_ab_next(void);
bool music_set_ab_prev(void);

/************************************************************************
* Function:      music_play
* Description:   ��ʼ��������
* Return��       bool   true: �ɹ�   false:ʧ��
*************************************************************************/
bool music_play(void);

bool music_stop(void);
bool music_pause(void);
bool music_resume(void);

/************************************************************************
* Function:         music_seek_play
* Description:      seek����,����ʱ����msΪ��λ 
* para1��           seek_position  �Ӹ�ʱ�俪ʼ����
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_seek(int ms);

/************************************************************************
* Function:         music_fast_forward
* Description:      ��������ݲ���Ϊ���
*                   ���ٶȵȼ�����ΧΪ[0��4]
*
* para1��           none
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_fast_forward(void);

/************************************************************************
* Function:         music_fast_backward
* Description:      ���ˣ����ݲ���Ϊ���
*                   ���ٶȵȼ�����ΧΪ[0��4]
*
* para1��           none
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_fast_backward(void);

bool music_cancel_ffb(void);

/************************************************************************
* Function:         music_set_ffb_step
* Description:      ���ÿ�������ٶȵȼ�����ΧΪ[0��4] 
* para1��           step   ������˵��ٶȵȼ�
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_set_ffb_step(int speed);

/************************************************************************
* Function:          music_set_ffb_time
* Description:       �����ڿ�����˹����У�
*                    ÿһ��������֮������
*                    ���ŵ�ʱ�䳤��
*                          
* para1��           time  ���ŵ�ʱ�䳤��
* Return��          bool   true:  �ɹ�   false: ʧ��
*************************************************************************/
bool music_set_ffb_time(int time);

/************************************************************************
* Function:           music_set_filepath
* Description:        ѡ��һ���ȴ����ŵ������ļ�
* para1��             pFile ָ��Ҫ���ŵ��ļ���·��
* Return��            true or false
*************************************************************************/ 
bool music_set_filepath(char *filepath);
bool music_set_fileindex(int fileindex);
bool music_play_next(void);
bool music_play_prev(void);

bool  music_make_bookmark(eg_audio_tag_t *audio_tag);

bool music_set_last_file( void );

bool music_delete_file(char * fullpath);


/* album image decode */
bool album_open_engine( void );
bool album_close_engine( void );
bool album_get_status( mmm_image_status_t *status );
bool album_get_image_info( mmm_img_info_t *img_info );
bool album_check_file( plist_detail_info_t *id3_info );
bool album_start_decode( plist_detail_info_t *id3_info, gui_bitmap_info_t *bmp, void (*callback)() );
bool album_abort_decode( void );

/* music pthread operation */
bool music_create_thread( void *proc, void *param );

/* keymap control */
bool music_keymap_control( key_val_e key_val, bool is_map );

/* fusion control */
void scene_fusion_set_first_frame( scene_t *scene );
void scene_fusion_set_last_frame( scene_t *scene );
void option_fusion_set_first_frame( menuview_t *mv );
void option_fusion_set_last_frame( menuview_t *mv );

bool music_get_nowplaying( void );
bool music_set_nowplaying( bool nowplaying );

#endif

