/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_config.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_config.h
* \brief    music config
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MENGIN_CONFIG_H__
#define __MENGIN_CONFIG_H__

#include "include_case.h"


#include "mengine_playlist.h"

typedef enum
{
	EG_PLAY,    //normal play
	EG_PAUSE,
	EG_STOP,
	
	EG_PLAY_AB,     //null, a, b , do -->ab_status
	EG_PLAY_FAST,   //fb, ff -->fast_status_t	
	
    EG_ERROR,   //in error status
    
}play_status_e;

typedef enum
{
	EG_PLAY_AB_NULL,
	EG_PLAY_AB_A,
	EG_PLAY_AB_B,
	EG_PLAY_AB_DO,
	
}ab_status_e;

typedef enum
{
	EG_FFPLAY,
	EG_FBPLAY
	
}fast_status_e;

typedef enum
{
	EG_SPECTRUM,
	EG_LYRIC,
	EG_EFFECT1,
	EG_EFFECT2,
	EG_EFFECT3,
	
}display_mode_e;

typedef enum
{
	EG_SEQUENCE,
	EG_REPEAT_ONE,
	EG_REPEAT_ALL,
	EG_SHUFFLE,
	EG_SHUFFLE_REPEAT,
	EG_INTRO,
	
}play_mode_e;

typedef enum
{
    EG_ERR_NONE = 0,
        
    /*! ���ļ�ʧ�� */
    EG_ERR_OPEN_FILE,    
    /*! �ļ���ʽ��֧�� */
    EG_ERR_NOT_SUPPORT,    
    /*! ������� */
    EG_ERR_DECODER_ERROR,

    /*! ��license�����Ŵ����������ǲ���ʱ���Ѿ����� */
    EG_ERR_NO_LICENSE,
    /*! DRMʱ�Ӵ��� */
    EG_ERR_SECURE_CLOCK,
    /*! DRM��Ϣ���� */
    EG_ERR_DRM_INFO
	
}eg_err_e;

/*!
 *  \brief
 *     EQģʽ
 */
typedef enum
{
    /*! ��ͨģʽ*/
    EQ_NORMAL=0,
    /*! ҡ��ģʽ*/
    EQ_ROCK,
    /*! ����ģʽ*/
    EQ_POP,
    /*! �ŵ�ģʽ*/
    EQ_CLASSIC,
    /*! ���ģʽ*/
    EQ_SOFT,
    /*! ��ʿģʽ*/
    EQ_JAZZ,
    /*! �ص���ģʽ*/
    EQ_DBB,
    /*! �û�ģʽ*/
    EQ_USR_MODE,
    /*! SRS�µ�WOWģʽ*/
    EQ_SRSWOW,
    /*! SRS�µ�WOW HDģʽ*/
    EQ_SRSWOW_HD,
    /*! SRS�û�ģʽ*/
    EQ_SRSUSR_MODE
}eq_mode_e;

/* EQ���� */
typedef struct
{
    //Eq���ͣ�0-Norma, 1-Pop, 2-Classic, 3-Soft, 4-Jazz, 5-Rock, 6-DBB, 7-User EQ, 
    //                    8-SRSWOW, 9-SRSWOW HD, 10-SRS User Mode
    //����EQ����
    eq_mode_e eq_mode;
    
    /*! ���û�ģʽʱ�û���Ҫ���õĲ���*/
    int user_param[10];
    
    /*! ��SRS�û�ģʽʱ�û���Ҫ���õĲ���*/
    int srs_param[10];
    
}mengine_eq_t;

/*!
 *  \brief
 *     �ϵ㡢��ǩ��Ϣ�ṹ
 */
typedef struct
{
	/*! �ϵ�ʱ�� xxms*/
	unsigned int time;
	/*! buf��Ϣ*/
	char buf[AUDIO_TAG_SIZE];
}eg_audio_tag_t;

/******************************************************************
*                                            ������Ϣ(��������)
*******************************************************************/
typedef struct
{
    //��ʾģʽ��0-Spectrum, 1-Lyric, 2-Effect1, 3-Effect2, 4-Effect3
    display_mode_e display_mode;

    //��ʾ��Ч����
    bool display_effect;
    
    //ѭ��ģʽ��0-Sequence, 1-Repeat One, 2-Repeat All, 3-Shuffle, 4-Shuffle + Repeat, 5-Intro
    play_mode_e play_mode;

    //Eqģʽ
    mengine_eq_t eq_val;
    
    //���뵭����0-disable, 1-enable
    bool fade;
    
    //���ٲ��ŵ��ٶȣ�-4~+4
    int play_speed;

    //����ٶȣ�0-2X, 1-4X, 2-8X, 3-16X, 4-32X
    int ffb_speed;

    //������̲�������ʱ��
    int ffb_time;
    
    //ABѭ���Ĵ�����1��10
    int ab_count;

    //ABѭ���ļ����1��6��
    int ab_time;

    //�Զ�AB���ŵı�־��0-�ֶ�, 1-�Զ�
    bool ab_autoplay;

    //�ڲ����б���ļ�����
    int file_total;
    
    //��ǰ�ļ��ڲ����б��е�����
    int file_index;
    
    //�ļ���ǩ��Ϣ
    eg_audio_tag_t audio_tag;

    //fsel mode
    bool fsel_mode;
    
}mengine_config_t;


/******************************************************************
 *                                            ����״̬��Ϣ
*******************************************************************/
typedef struct
{
    //����״̬
    play_status_e play_status;

    //AB����״̬
    ab_status_e ab_status;
    
    //���/����״̬
    fast_status_e fast_status;
    
    //����״̬
    eg_err_e err_status;

}mengine_status_t;


/* music engine ������Ϣ */
typedef struct
{
	int	magic;

    /******************************************************************
    *                                            ����״̬��Ϣ
    *******************************************************************/
    mengine_status_t eg_status;
    
    /******************************************************************
     *                                            �м��״̬��Ϣ
    *******************************************************************/
    //����״̬��Ϣ
    mmm_music_status_t music_status;
    
    //�ļ���Ϣ
    mmm_music_file_info_t  file_info;
    
    //DRM��Ϣ
    mmm_license_info_t license_info;

    /******************************************************************
    *                                            ������Ϣ(��������)
    *******************************************************************/    
    mengine_config_t config;

}mengine_t;



/*************************************************************************
*                                                                       
*                     ȫ�ֺ���ԭ��                                      
*                                                                      
*************************************************************************/
bool mengine_init( void );
bool mengine_deinit( void );
bool mengine_update( void );

bool mengine_get_engine_info(const msg_apps_t *pmsg);
bool mengine_get_filepath(const msg_apps_t *pmsg);

bool mengine_set_eq(const msg_apps_t *pmsg);
bool mengine_set_playmode(const msg_apps_t *pmsg);
bool mengine_set_displaymode(const msg_apps_t *pmsg);
bool mengine_set_fade(const msg_apps_t *pmsg);
bool mengine_set_playspeed(const msg_apps_t *pmsg);
bool mengine_set_ab_auto(const msg_apps_t *pmsg);
bool mengine_set_ab_count(const msg_apps_t *pmsg);
bool mengine_set_ab_time(const msg_apps_t *pmsg);

bool mengine_start_ab(const msg_apps_t *pmsg);
bool mengine_clear_ab(const msg_apps_t *pmsg);
bool mengine_set_ab_next(const msg_apps_t *pmsg);
bool mengine_set_ab_prev(const msg_apps_t *pmsg);

bool mengine_play(const msg_apps_t *pmsg);
bool mengine_stop(const msg_apps_t *pmsg);
bool mengine_pause(const msg_apps_t *pmsg);
bool mengine_resume(const msg_apps_t *pmsg);
bool mengine_seek(const msg_apps_t *pmsg);

bool mengine_fast_forward(const msg_apps_t *pmsg);
bool mengine_fast_backward(const msg_apps_t *pmsg);
bool mengine_cancel_ffb(const msg_apps_t *pmsg);
bool mengine_set_ffb_step(const msg_apps_t *pmsg);
bool mengine_set_ffb_time(const msg_apps_t *pmsg);

bool mengine_set_filepath(const msg_apps_t *pmsg);
bool mengine_set_fileindex(const msg_apps_t *pmsg);
bool mengine_play_next(const msg_apps_t *pmsg);
bool mengine_play_prev(const msg_apps_t *pmsg);
bool mengine_delete_file(const msg_apps_t *pmsg);

bool  mengine_make_bookmark(const  msg_apps_t *pmsg);

bool  mengine_set_last_file(const  msg_apps_t *pmsg);

bool mengine_gsensor_shaked(const msg_apps_t *pmsg);


#endif

