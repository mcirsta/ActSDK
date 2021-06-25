/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_scene_playing.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_scene_playing.h
* \brief    radio application definitions
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_SCENE_PLAYING_H__
#define __RADIO_SCENE_PLAYING_H__

#include "include_case.h"
#include "radio_app.h"
#include "radio_playing_widget_option.h"
#include "radio_utils.h"

#define MAX_PATH_NAME 1024
/*
#define SYS_PATH "/mnt/disk0/system"
#define SYS_FM_PATH "/mnt/disk0/system/FM"
#define CHANNEL_LIST_FILE "/mnt/disk0/system/FM/default.ini"
*/
const char *sys_path;
const char *sys_fm_path;
char channel_list_file[MAX_PATH_NAME];
char userchannel_path[PATH_MAX];

#define FILE_HEAD_LENTH 4
#define FILE_FREQ_LENTH 6
#define FREQ_MIN_US 87500
#define FREQ_MAX_US 108000
#define FREQ_MIN_JP 76000
#define FREQ_MAX_JP 90000



/* ���Ž��� ״̬*/
typedef enum
{
    STATE_QUIT = -1,
    STATE_PLAYING,
    STATE_OPTION,
    STATE_VOLUMEBAR,

}scene_state_e;

/* ����״̬  */
typedef enum
{
    NORMAL_PLAY = 0,        /* �������� */
    HALF_AUTO_SEARCH_ADD,   /* ǰ����Զ�����(����̨��ͣ) */
    HALF_AUTO_SEARCH_MINUS, /* ������Զ�����(����̨��ͣ) */
    AUTO_SEARCH,            /* ȫ�Զ�������������ǰ�������е�̨����ౣ��20����*/

}search_state_e;

/*engine need info for auto seeking*/
typedef struct
{
    unsigned int min_freq;
    unsigned int max_freq;
    unsigned int step;
    search_state_e mode;
}en_seek_info_t;

/*result of auto seek*/
typedef struct{
    unsigned int channel_num;
    unsigned int channel_freq[MAX_CHANNEL_NUM];
}eg_seek_info_t;

/* ���Ž���ui��Դ */
typedef struct
{
    resgroup_resource_t     *resgroup_playing;  /* ���Ž�����Դ�� */

    picture_resource_t      *pic_foreground; /*ǰ��ͼ*/
    picture_resource_t      *pic_stereo;        /* ������ͼ�� */
    picture_resource_t      *pic_num_hundreds;  /* ��ǰƵ�ʰ�λ��ͼ�� */
    picture_resource_t      *pic_num_tens;      /* ��ǰƵ��ʮλ��ͼ�� */
    picture_resource_t      *pic_num_units;     /* ��ǰƵ�ʸ�λ��ͼ�� */
    picture_resource_t      *pic_num_dot;       /* ��ǰƵ��С����ͼ�� */
    picture_resource_t      *pic_num_tenth;     /* ��ǰƵ��ʮ��λ��ͼ�� */
    string_resource_t       *str_num_centi;     /* ��ǰƵ�ʰ�ʮ��λ�� */
    string_resource_t       *str_mhz;           /* Ƶ�ʵ�λ(MHz) */
    picregion_resource_t    *picreg_signal;     /* �ź�ǿ��ͼ��(��5��) */

    string_resource_t       *str_country;       /* ��ǰƵ���������� */
    string_resource_t       *str_channel;       /* ��ǰƵ������ */

    picture_resource_t      *pic_freq_bg;       /* Ƶ��ָʾ�Ʊ���ͼ */
    string_resource_t       *str_freq_start;    /* Ƶ��ָʾ����СƵ�� */
    string_resource_t       *str_freq_end;      /* Ƶ��ָʾ�����Ƶ�� */
    picture_resource_t      *pic_freq_notation[181]; /* ��ǰƵ��ָʾͼ�� */

}res_playing_t;

typedef struct
{
    unsigned int  channel_num;      /* user channel num [max = 20]*/
    unsigned int  channel_freq[MAX_CHANNEL_NUM];    /* user channel freq */
    char channel_name[MAX_CHANNEL_NUM][PATH_MAX];   /* user channel name */

} channel_info_t;

channel_info_t channel_info;

/*band*/
typedef enum
{
    BAND_CHINA = 0,
    BAND_USA,
    BAND_JAPAN,
    BAND_EUROPE,
}option_band_item_e;


/* scene playing data */
typedef struct
{
    /* style resource */
    scene_t                 *scene;

    /* common scene */
    scene_t *common_scene;

    /* default ini�ļ����*/
    int fd; 

    /* fm�������*/
    int fm_fd;
    
    /*���ش洢��״̬*/
    bool local_disk_status;
    /*fsel���*/
    FSEL_HANDLE fp;

    int fontdes;

    picture_resource_t      *pic_bg;        /* ��������ͼ */
    gui_bitmap_info_t  * bg_bitmap_info;
    picture_resource_t      *pic_select;    /*optionѡ��ͼ��*/
    picture_resource_t      *pic_logo;      /* headbar����ʾͼ�� */
    string_resource_t        *str_logo;     /* headbar����ʾ�ַ���*/
    picregion_resource_t    *picreg_num;    /* ����ͼƬ�� */
    string_resource_t        *str_stoprec;  /* adjust����ʾ�ַ���*/
    string_resource_t        *str_fmterr;   /*�ļ���ʽ��֧��ʱdialog����ʾ�ַ���*/
    string_resource_t        *str_earinsert; /*����δ����ʱdialog����ʾ�ַ���*/
    string_resource_t        *str_linein; 
    string_resource_t        *str_nodisk;
    string_resource_t        *str_diskins;

    res_playing_t           res_playing;

    /* window resource */
    int                     win_id;

    /*fm¼����Ϣ*/
    int rec_quality;
    int rec_gain;
    int rec_path;

    /* ��ʱ��Ϣ */
    bool                    use_custum_file;    /* �Ƿ�ʹ���û��ļ����硰�ǡ����򲨶�id��Ч */
    char                    cur_file_path[PATH_MAX]; /* �û��ļ�·�� */
    int                     timer_id;           /* ��ʱ��id */
    bool                    mute;               /* �Ƿ��� */
    int                     cur_intensity;      /* ��ʱ�ź�ǿ�� */
    bool                    cur_stereo;         /* ��ǰƵ���Ƿ�Ϊ������ */
    search_state_e          searching_flag;     /* ��̨��־��0-�������ţ�1-���Զ���������̨��ֹͣ��, 2-ȫ�Զ� */
  
    bool                    auto_search_start;  /*��־�ѿ�ʼ��̨��0δ��ʼ��1�ѿ�ʼ��̨*/


    bool load_config_flag;/*���������ñ�־��true������false*/
    bool parse_default_file_flag;/*��ȷ����default.ini�ļ����ñ�־��true��δ��������δ��ȷ������false*/

    bool is_userchannel_mode;
    /* ��Ҫ���浽config�������Ϣ */
    bool                    use_preset;             /* �Ƿ�ʹ��Ԥ��Ƶ�� */
    int                     cur_preset_channel_id;  /* Ԥ��Ƶ��id */
    int                     cur_band;               /* ��ǰ����  */
    unsigned int            cur_frequency;          /* ��ǰƵ�� */
    int                     cur_threthod;           /* ��ǰ�ż�ֵ */
   
    channel_info_t channel_list_info;

    /* �ؼ� */
    headbar_t               *headbar;
    volumebar_t             *volumebar;
    menuview_t              *option_menu;
    adjust_t                  *adjust;
    dialog_t                  *dialog;
    dialog_t                  *dialog_linein;
    msgbox_t               *msgbox;
    /*�Ƿ���Ҫ���LINE_IN*/
    int line_in_check;

    app_scene_t             next_scene;

}radio_scene_playing_t;

extern radio_scene_playing_t* g_scene_playing;

/* scene entry */
int radio_scene_playing( fusion_enter_e fenter );
bool parse_ini_file(channel_info_t *channel_info, char *file_buf, unsigned int file_buf_lenth);
bool check_freq(option_band_item_e band, int freq);
bool save_default_file(radio_scene_playing_t *playing);


#endif  /* __RADIO_SCENE_PLAYING_H__ */

