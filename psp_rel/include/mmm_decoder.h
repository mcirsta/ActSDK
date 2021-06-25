/*! \cond MMM_DECODE_API*/
/********************************************************************************
 *                              usdk1100
 *                            Module: act_decoder.c
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jszeng    2008-08-27 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_decoder.h
 * \brief    ������Ӧ�ò�Ľӿڼ�����
 * \author   �����������
 * \version 1.0
 * \date  2008/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_DECODER_H__
#define __MMM_DECODER_H__

#include "act_amvb.h"
#include "fb.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \cond MMM_AUDIO_VEDIO_API*/

#define VIDEO_TAG_SIZE
#define AUDIO_TAG_SIZE 64
/*!
 * \brief
 *      �ṩ��Ӧ�ò�Ĺ�����������
 */
typedef enum
{
    /*! ���� */
    MMM_PLAY = 1,
    /*! ֹͣ */
    MMM_STOP,
    /*! ��ͣ */
    MMM_PAUSE,
    /*! ���� */
    MMM_RESUME,
    /*! ��λ���ŵ� */
    MMM_SEEK,
    /*! ��� */
    MMM_FAST_FORWARD,
    /*! ���� */
    MMM_FAST_BACKWARD,
    /*! ȡ��������� */
    MMM_CANCEL_FFB,
    /*! ���ÿ�����˲��� */
    MMM_SET_FFB_STEP,
    /*! ���ò��ŵ��ļ��� */
    MMM_SET_FILE,
    /*! ����ǰ���ŵ�����Ϊ��ǩ */
    MMM_MAKE_MARK,
    /*! ��ȡ��ý���ļ�����Ϣ */
    MMM_GET_MEDIA_INFO,
    /*! ��ȡ�������ĵ�ǰ״̬ */
    MMM_GET_PLAYER_STATUS,
    /*! ����A�� */
    MMM_SET_A,
    /*! ����B�� */
    MMM_SET_B,
    /*! ȡ��AB���� */
    MMM_CLEAR_AB,
    /*! ����AB���ŵĴ��� */
    MMM_SET_AB_COUNT,
    /*! ��B�㷵�ص�A��ĵȴ�ʱ�䣬ֻ����Ƶ������Ч*/
    MMM_SET_AB_WAIT_TIME,
    /*! ��ȡDRM��Ϣ */
    MMM_GET_DRM_INFO,
    /*! ����video �Ļ���ģʽ*/
    MMM_VIDEO_MEM_MODE
} mmm_dec_cmd_t;

/*!
 * ����video����ķ���ģʽ
 */
typedef enum
{
    /*! ��������ģʽ*/
    MMM_MEM_NORMAL_MODE = 0x0,
    /*! ����ͼģʽ*/
    MMM_MEM_THUMBNAIL_MODE,
} mmm_video_mem_mode_t;
/*!
 *  \brief
 *      DRM����
 */
typedef enum
{
    /*! ����DRM�ļ� */
    MMM_NO_DRM = 0,
    /*! ACTIONS��DRM��Ƶ�ļ� */
    MMM_ACT_DRM,
    /*! ΢���DRM��Ƶ�ļ� */
    MMM_MS_DRM,
    /*! �������͵�DRM�ļ� */
    MMM_DRM_OTHER
} mmm_drm_type_t;
/*!
 *  \brief
 *      ��������Ĳ���״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_PLAYER_STOPPED,
    /*! ����״̬ */
    MMM_PLAYER_PLAYING,
    /*! ��ͣ״̬ */
    MMM_PLAYER_PAUSED,
    /*! �������״̬ */
    MMM_PLAYER_FFB,
    /*! ����״̬ */
    MMM_PLAYER_ERROR
} mmm_player_status_t;

/*!
 *  \brief
 *      AB����״̬
 */
typedef enum
{
    /*! A���Ѿ�����״̬ */
    MMM_A_IS_SET,
    /*! ����AB����״̬ */
    MMM_AB_PLAYING,
    /*! ��AB����״̬ */
    MMM_NORMAL_PLAYING
} mmm_ab_status_t;

/*! \endcond*/

/*! \cond MMM_IMAGE_VIDEO_API*/
/*!
 *  \brief
 *     ͼƬ��������Ƶ���빫������
 */
typedef enum
{
    /*! ����ˢ��ģʽ�������������� */
    MMM_SET_SCREEN_ROTATE = 0x21,
} mmm_image_video_cmd_t;

/*! \endcond*/

/*! \cond MMM_VIDEO_API*/

/*!
 *  \brief
 *      �ṩ��Ӧ�ò����Ƶ��������
 */
typedef enum
{
    /*! ������Ƶ����ʾ���� */
    MMM_SET_VIDEO_AREA = 0x31,
    /*! ����blending����*/
    MMM_BLENDING_AREA_CREATE,
    /*! ��ʾblending����*/
    MMM_BLENDING_AREA_SHOW,
    /*! ����blending����*/
    MMM_BLENDING_AREA_HIDE,
    /*! ����blending���������*/
    MMM_BLENDING_AREA_SETATTR,
    /*! ����blending����*/
    MMM_BLENDING_AREA_DESTROY,

    /*! ��ȡ��ǰ֡���� */
    MMM_PRINT_SCREEN,
    /*! ������ʾ�豸��LCD, TV��*/
    MMM_SET_DISPLAY_DEV,
    /*! ��ȡһ֡�µ�����ȥ��ʾ */
    MMM_SHOW_NEW_FRAME,
    /*! ������Ƶ�������*/
    MMM_SET_VIDEO_RATIO,
} mmm_video_cmd_t;

/*!
 *  \brief
 *      kshow��������
 */
typedef enum
{
    /*! �л����� */
    MMM_KSHOW_SWITCH_CHANNEL = 0x41,
    /*! ��ʼ���� */
    MMM_KSHOW_ENABLE_VOCALIZE,
    /*! ���������Ļ����� */
    MMM_KSHOW_SET_MIX_PROPORTION,
    /*! ֹͣ���� */
    MMM_KSHOW_DISABLE_VOCALIZE,
    /*! ��ʼ¼�� */
    MMM_KSHOW_START_RECORDING,
    /*! ֹͣ¼�� */
    MMM_KSHOW_STOP_RECORDING,
    /*! ���ø���ļ� */
    MMM_KSHOW_SET_TITLE_FILE,
    /*! ��ʾ��� */
    MMM_KSHOW_ENABLE_TITLE_DISPLAY
} mmm_kshow_cmd_t;

/*!
 *  \brief
 *     ¼��״̬
 */
typedef enum
{
    /*! Kshow¼�����ڹر�״̬ */
    MMM_KSHOW_RECORDER_OFF,
    /*! Kshow¼������¼��״̬ */
    MMM_KSHOW_RECORDER_ON,
    /*! Kshow¼��������ͣ״̬ */
    MMM_KSHOW_RECORDER_PAUSED,
    /*! Kshow¼�����ڴ���״̬ */
    MMM_KSHOW_RECORDER_ERROR
} mmm_k_record_status_t;

/*!
 *  \brief
 *     ����״̬
 */
typedef enum
{
    /*! Kshow�������ڹر�״̬ */
    MMM_KSHOW_VOCALIZER_OFF,
    /*! Kshow������������״̬ */
    MMM_KSHOW_VOCALIZER_ON,
    /*! Kshow����������ͣ״̬ */
    MMM_KSHOW_VOCALIZER_PAUSED,
    /*! Kshow�������ڴ���״̬ */
    MMM_KSHOW_VOCALIZER_ERROR
} mmm_vocalize_status_t;
/*!
 *  \brief
 *      ��ʾ�豸
 */
typedef enum
{
    /*! CPU����ʾ�豸*/
    MMM_CPU_LCD_DEV,
    /*! RGB����ʾ�豸*/
    MMM_RGB_LCD_DEV,
    /*! TV out��ʾ�豸, PAL mode*/
    MMM_TVOUT_PAL_DEV,
    /*! TV out��ʾ�豸, NTSC mode*/
    MMM_TVOUT_NTSC_DEV
} mmm_video_dev_t;

typedef enum
{
    /*! ����ȫ����ʾ */
    MMM_RATIO_FULL = 1,
    /*! ����ԭʼ�ߴ���ʾ */
    MMM_RATIO_ORG,
    /*! ����ȱ�����ʾ */
    MMM_RATIO_EQU,
} mmm_video_ratio_t;

/*!
 *  \brief
 *     blending�����λ�ú��������
 *
 *  typedef struct
 *  {
 *      unsigned int color_key;
 *      unsigned int alpha;
 *      fb_rect_area_t area;
 *  }fb_blending_param_t;
 *
 * */
typedef fb_blending_param_t mmm_blending_area_t;

/*!
 *  \brief
 *     blending����, create blending����Ĳ���
 *
 *  typedef struct
 *  {
 *      int handle;
 *      fb_blending_param_t blend_param;
 *  }fb_blending_create_t;
 * */
typedef fb_blending_create_t mmm_blending_create_t;
/*! \brief
 *     blending��������ԣ�����alphaֵ��colorkeyֵ
 *
 *  typedef struct
 *  {
 *      unsigned int color_key;
 *      unsigned int alpha;
 *  }fb_blending_attr_t;
 * */

typedef fb_blending_attr_t mmm_blending_attr_t;
/*! \brief
 *      ����blendint���������ǵĲ������������������handle���Լ���Ҫ���õ�blending���������mmm_blending_attr_tֵ��
 *
 *  typedef struct
 *  {
 *      int handle;
 *      fb_blending_attr_t blending_attr;
 *  }fb_blending_setattr_t;
 * */
typedef fb_blending_setattr_t mmm_blending_setattr_t;

/*!
 *  \brief
 *     DRMʱ����Ϣ
 */
typedef struct
{
    /*! �� */
    short year;
    /*! �� */
    char month;
    /*! �� */
    char day;
    /*! ʱ */
    char hour;
    /*! �� */
    char minute;
    /*! �� */
    char second;
    /*! ΢�� */
    char minisecond;
} mmm_drm_date_t;
/*!
 *  \brief
 *     DRM��Ϣѡ���־
 */
typedef struct
{
    /*! ��ʶremain_play_cnt�ֶ���Ч */
    char remain_cnt :1;
    /*! ��ʶremain_play_hours�ֶ���Ч */
    char remain_play_hours :1;
    /*! ��ʶbegin_time�ֶ���Ч */
    char begin_time :1;
    /*! ��ʶexpire_time�ֶ���Ч */
    char expire_time :1;
    /*! ��ʶrollback�ֶ���Ч */
    char rollback :1;
    /*! ��ʶremain_store_hours�ֶ���Ч */
    char remain_store_hours :1;
    /*! ��ʶorigin_play_cnt�ֶ���Ч */
    char origin_cnt :1;
} mmm_switch_flag_t;
/*!
 *  \brief
 *     DRM��Ϣ
 */
typedef struct
{
    /*! �Ƿ���DRM��Ϣ��1Ϊ��DRM��Ϣ��0��ʾ��DRM��Ϣ */
    char license_flag;
    /*! DRM��Ϣѡ���־ */
    mmm_switch_flag_t switch_flag;
    /*!  */
    char rollback;
    /*!  */
    short remain_play_cnt;
    /*!  */
    short origin_play_cnt;
    /*!  */
    short remain_play_hours;
    /*!  */
    short remain_store_hours;
    /*!  */
    mmm_drm_date_t begin_time;
    /*!  */
    mmm_drm_date_t expire_time;
} mmm_license_info_t;
/*!
 *  \brief
 *     ��Ƶ�ļ�����Ϣ
 */
typedef struct
{
    /*! �ļ�����ʱ�䣨ms�ƣ� */
    unsigned int total_time;
    /*! ��ý���ļ���ʵ�ʸ�ʽ */
    char format[8];
    /*! �ļ��е���Ƶ����ʽ����"ogg" */
    char audio[8];
    /*! �ļ��е���Ƶ����ʽ���� "wmv" */
    char video[8];
    /*! ��Ƶ�ļ���֡�ʣ�fps�� */
    unsigned int frame_rate;
    /*! ��Ƶ�Ŀ�pixels�� */
    unsigned int width;
    /*! ��Ƶ�ĸߣ�pixels�� */
    unsigned int height;
    /*! �ļ��е���Ƶ�������� */
    unsigned int video_bitrate;
    /*! ��Ƶ������ */
    unsigned int sample_rate;
    /*! �ļ��е���Ƶ�������� */
    unsigned int audio_bitrate;
    /*! ��Ƶ���������� */
    unsigned int channels;
    /*! ��Ƶ�ļ���drm��Ϣ */
    mmm_drm_type_t drm_type;
    /*! amvb��Ϣ */
    amvb_info_t amvb_info;
} mmm_video_file_info_t;
/*!
 *  \brief
 *     ��Ƶ�����״̬��Ϣ
 */
typedef struct
{
    /*! ��ǰ�Ĳ���ʱ�䣬��λΪms */
    unsigned int cur_time;
    /*! ��Ƶ���������״̬ */
    mmm_player_status_t player_status;
    /*! �м��KSHOW¼��״̬ */
    mmm_k_record_status_t record_status;
    /*! �м��KSHOW����״̬ */
    mmm_vocalize_status_t vocalize_status;
    /*! �м���������� */
    unsigned int err_no;
    /*! ��ǰ��ab����״̬ */
    mmm_ab_status_t ab_flag;
    /*! ��ǰ���̵�ʣ��ռ� */
    unsigned int space_left;
} mmm_video_status_t;
/*!
 *  \brief
 *     ��Ƶ¼������Դ
 */
typedef struct
{
    /*! �ڲ���˷� */
    unsigned int mic :1;
    /*! FM */
    unsigned int fm :1;
    /*! ��line-in���ӵ��������� */
    unsigned int line_in :1;
    /*! ��δ�ṩ */
    unsigned int iis :1;
    /*! ��δ�ṩ */
    unsigned int spdif :1;
    /*! ��δ�ṩ */
    unsigned int tv :1;
    /*! ����λ */
    unsigned int reserved :26;
} mmm_kaudio_source_t;
/*!
 *  \brief
 *     ¼������Ҫ��
 */
typedef struct
{
    /*! �������������棬��mvΪ��λ*/
    unsigned int input_gain;
    /*! ������������棬��mvΪ��λ*/
    unsigned int output_gain;
} mmm_kvoice_gain_t;
/*!
 *  \brief
 *     �����Ĳ���
 */
typedef struct
{
    /*! ��Դ */
    mmm_kaudio_source_t ai_source;
    /*! ¼������Ҫ��*/
    mmm_kvoice_gain_t voice_gain;
    /*! ��Ƶ�������ȣ�Ŀǰֻ֧��16bits */
    char bpp;
    /*! ��������Ŀǰ֧��˫���� */
    char channel;
    /*! �����ʣ�Ŀǰֻ�����ó�32K */
    unsigned int sample_rate;
    /*! ������*/
    unsigned int bitrate;
    /*! ��Ƶ�����ģʽ����ҪΪwav¼��ʱ��Ҫ���ֱ���ģʽΪadpcm����pcm��Ŀǰֻ֧��adpcm��Ҳ��Ĭ����0*/
    unsigned int encode_mode;
    /*! �����Ļ����� */
    int mix_propotion;
} mmm_vocalize_param_t;

/*!
 *  \brief
 *     ��Ƶ�ı�ǩ����
 */
typedef struct
{
    /*! ��ǩ��Ӧ����Ƶʱ��� XXms*/
    unsigned int time;
    /*! ������ǩ��Ϣ���ɽ�����ȷ�� */
    unsigned char* buf;
} mmm_video_tag_t;
/*!
 *  \brief
 *     ��Ƶ��ʾ������
 */
typedef struct
{
    /*! ��Ƶ��������Ļx�����ϵ�ƫ�� */
    unsigned int xpos;
    /*! ��Ƶ��������Ļy�����ϵ�ƫ�� */
    unsigned int ypos;
    /*! ��Ƶ����Ŀ� */
    unsigned int width;
    /*! ��Ƶ����ĸ� */
    unsigned int height;
} mmm_video_area_t;
/*!
 *  \brief
 *     ��ǰ��ʾ����Ƶ֡������
 */
typedef struct
{
    /*! ��ǰ��Ƶ������ʾ��x����ƫ�� */
    unsigned int xpos;
    /*! ��ǰ��Ƶ������ʾ��y����ƫ�� */
    unsigned int ypos;
    /*! ��ǰ��Ƶ�Ŀ� */
    unsigned int width;
    /*! ��ǰ��Ƶ�ĸ� */
    unsigned int height;
    /*! ת�������Ƶ֡���� */
    char *buf;
} mmm_cur_video_t;

void * mmm_vd_open(void *param);
int mmm_vd_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_vd_close(void *handle, void *param);
/*! \endcond*/

/*! \cond MMM_MUSIC_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�ò����Ƶ��������
 */
typedef enum
{
    /*! ����*/
    MMM_FADE_IN = 0x61,
    /*! ����*/
    MMM_FADE_OUT,
    /*! ����EQ*/
    MMM_SET_EQ,
    /*! ������������ֵ*/
    MMM_SET_ENERGY_DOOR,
    /*! ������������*/
    MMM_SET_ENERGY_STEP,
    /*! ʹ��Ƶ����ʾ*/
    MMM_ENABLE_SPECTRUM_SHOW,
    /*! �ر�Ƶ����ʾ*/
    MMM_DISABLE_SPECTRUM_SHOW,
    /*! �������ŵ��ٶ�*/
    MMM_ALTER_PLAY_SPEED,
    /*! ���ÿ������֮����������*/
    MMM_SET_FFB_PLAY_TIME,
    /*! ��������1*/
    MMM_RESERVED1,
    /*! ��������2*/
    MMM_RESERVED2
} mmm_music_decoder_cmd_t;
/*!
 *  \brief
 *     �����ļ���Ϣ
 */
typedef struct
{
    /*! �ļ���ʽ*/
    char file_format[8];
    /*! ������ʱ��,���루s��Ϊ��λ*/
    unsigned int total_time;
    /*! ���ֱ����ʣ���KbpsΪ��λ*/
    unsigned int bitrate;
    /*! ���ֲ����ʣ���KHzΪ��λ*/
    unsigned int sample_rate;
    /*! ����������*/
    unsigned int channels;
    /*! ��������*/
    unsigned int bpp;
    /*! �Ƿ���drm����*/
    mmm_drm_type_t drm_type;
} mmm_music_file_info_t;
/*!
 *  \brief
 *     �ϵ㡢��ǩ��Ϣ�ṹ
 */
typedef struct
{
    /*! �ϵ�ʱ�� xxms*/
    unsigned int time;
    /*! buf��Ϣ*/
    char *buf;
} mmm_audio_tag_t;
/*!
 *  \brief
 *     EQģʽ
 */
typedef enum
{
    /*! ��ͨģʽ*/
    MMM_NORMAL = 0,
    /*! ҡ��ģʽ*/
    MMM_ROCK,
    /*! ����ģʽ*/
    MMM_POP,
    /*! �ŵ�ģʽ*/
    MMM_CLASSIC,
    /*! ���ģʽ*/
    MMM_SOFT,
    /*! ��ʿģʽ*/
    MMM_JAZZ,
    /*! �ص���ģʽ*/
    MMM_DBB,
    /*! �û�ģʽ*/
    MMM_EQ_USR_MODE,
    /*! SRSģʽ*/
    MMM_SRS = 0x40,
    /*! SRS�µ�WOWģʽ*/
    MMM_WOW = 0x41,
    /*! TRUE_BASSģʽ*/
    MMM_TRUE_BASS = 0x42,
    /*! SRS�û�ģʽ*/
    MMM_SRS_USR_MODE = 0x4f
} mmm_eq_mode_t;
/*!
 *  \brief
 *     ����eqʱ�Ĳ���
 */
typedef struct
{
    /*!EQģʽ*/
    mmm_eq_mode_t eq_mode;
    /*! ���û�ģʽʱ�û���Ҫ���õĲ���*/
    int param[10];
} mmm_eq_set_t;
/*!
 *  \brief
 *     ���ֵ�ǰ�Ƿ�������״̬��־
 */
typedef struct
{
    /*!��ǰ�Ƿ��ڵ���״̬*/
    unsigned fade_out_flag :1;
    /*!��ǰ�Ƿ���״̬*/
    unsigned silence_flag :1;
    /*!��ǰ�Ƿ���ab����״̬*/
    unsigned AB_play_flag :1;
    /*!��ǰ�Ƿ��ѵ���B��*/
    unsigned reach_B_flag :1;
    /*!��ǰ�Ƿ���B���ĵȴ�ʱ���ѵ���Ҫ���ص�A��*/
    unsigned return_A_flag :1;
} mmm_status_flag_t;
/*!
 *  \brief
 *     ����״̬
 */
typedef struct
{
    /*!������ǰ�Ĳ���ʱ��,xxms*/
    int cur_time;
    /*!�м����Ƶ����Ŀǰ������״̬*/
    mmm_player_status_t status;
    /*!�����*/
    unsigned int err_no;
    /*!���ֵ�ǰ�Ƿ�������״̬��־*/
    mmm_status_flag_t flag;
    /*!��ǰ�����ı�������Ϣ*/
    unsigned int cur_bps;
    /*!��ǰ���������ֵ*/
    unsigned int cur_power;
    /*!Ƶ��,18��Ƶ�ʵ�.*/
    unsigned char spectrum[18];
} mmm_music_status_t;

void *mmm_ad_open(void *param);
int mmm_ad_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ad_close(void *handle, void *param);
/*! \endcond*/

/*! \cond MMM_IMAGE_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�ò��ͼƬ��������
 */
typedef enum
{
    /*! ����ר��ͼƬ��Ϣ*/
    MMM_SET_ALBUM_INFO = 0x91,
    /*! ����BUFͼƬ��Ϣ*/
    MMM_SET_MEDIA_BUFFER,
    /*! ��ȡ������Ϣ*/
    MMM_GET_DECODER_STATUS,
    /*! ͼƬ����*/
    MMM_IMG_DECODE,
    /*! ͼƬ����ͼ����*/
    MMM_IMG_BROWSE,
    /*! ȡ��ͼƬ����*/
    MMM_ABORT_DECODE,
    /*! ���¶�λ����ͼƬ��С*/
    MMM_RESIZE,
    /*! ͼƬ�ƶ�*/
    MMM_MOVE,
    /*! ͼƬ����*/
    MMM_ZOOM,
    /*! ͼƬ��ת*/
    MMM_ROTATE,
    /*! ͼƬ����ԳƱ任*/
    MMM_MIRROR,
    /*! ���ý���Ļص�����*/
    MMM_SET_CALLBACK,
    /*! ���ý���ʱ��Ҫ��ȡalpha���ݣ�
     ����MMM_IMG_DECODE���ݵ�buf���Ӵ�Сwidth*height�����ڴ洢alpha����*/
    MMM_SET_ALPHA_DEC
} mmm_image_decoder_cmd_t;
/*!
 *  \brief
 *     ��ɫ��ʽ
 */
typedef enum
{
    /*! YUV��ʽ*/
    MMM_YUV,
    /*! RGB��ʽ*/
    MMM_RGB
} mmm_color_format_t;
/*!
 *  \brief
 *     �м��ͼƬ����״̬
 */
typedef enum
{
    /*! ����״̬*/
    MMM_IMAGE_DECODER_BUSY,
    /*! ���ڿ���״̬*/
    MMM_IMAGE_DECODER_FREE,
    /*! ���ڴ���״̬*/
    MMM_IMAGE_DECODER_ERROR
} mmm_imgdec_status_t;

/*!
 * \brief
 *      ͼƬ�������
 */
typedef struct
{
    /*! ����״̬*/
    mmm_imgdec_status_t status;
    /*! �����*/
    unsigned int err_no;
    /*! �ļ��Ƿ�ﵽβ���ı�־*/
    unsigned int end_of_file;
} mmm_image_status_t;

/*!
 *  \brief
 *      ͼƬ����ģʽ
 */
typedef enum
{
    /*! �Ŵ�*/
    MMM_IMG_ZOOMIN,
    /*! ��С*/
    MMM_IMG_ZOOMOUT
} mmm_image_zoom_t;

/*!
 *  \brief
 *     ��ת����
 */
enum mmm_image_rotation_t
{
    /*! ˳ʱ����ת90��*/
    MMM_IMG_LEFT90,
    /*! ��ʱ����ת90��*/
    MMM_IMG_RIGHT90,
    /*! ��ת180��*/
    MMM_IMG_R180
};

/*!
 *  \brief
 *    ����ԳƱ任
 */
enum mmm_image_mirror_t
{
    /*! ���µߵ�*/
    MMM_IMG_UP_DOWN,
    /*! ���ҵߵ�������*/
    MMM_IMG_LEFT_RIGHT
};

/*!
 *  \brief
 *     �ƶ�ͼƬ
 */
enum mmm_image_move_direction_t
{
    /*! ����*/
    MMM_IMG_UP,
    /*! ����*/
    MMM_IMG_DOWN,
    /*! ����*/
    MMM_IMG_LEFT,
    /*! ����*/
    MMM_IMG_RIGHT
};

/*!
 * \brief
 *      ͼƬ����
 */
typedef struct
{
    /*! ��*/
    unsigned int year;
    /*! ��*/
    unsigned int month;
    /*! ��*/
    unsigned int day;
} mmm_img_date_t;

#define MAX_ASCII_LEN 120
/*!
 * \brief
 *      ͼƬ������Ϣ
 */
typedef struct
{
    /*! ����������*/
    char make[MAX_ASCII_LEN];
    /*! �豸ģʽ*/
    char model[MAX_ASCII_LEN];
    /*! ���˵��*/
    char software[MAX_ASCII_LEN];
    /*! ͼƬ��������*/
    char datetime[20];
    /*! ͼƬ������*/
    char artist[MAX_ASCII_LEN];
    /*! �����Ȩ˵��*/
    char copyright[MAX_ASCII_LEN];
    /*! ͼ������*/
    char imageDescription[MAX_ASCII_LEN];
    /*! ������˵��*/
    char makernote[MAX_ASCII_LEN];
    /*! ����ʱ��*/
    char exposuretime[MAX_ASCII_LEN];
    /*! �������Ȧ*/
    char F_number[MAX_ASCII_LEN];
    /*! ������Ҫ*/
    char exposureProg[MAX_ASCII_LEN];
    /*! ͼƬ�ļ���������֡������gif����֡����*/
    short iso;
    /*! Ԫ������*/
    char componentConfig[5];
    /*! �����ٶ�*/
    char shutterSpeed[MAX_ASCII_LEN];
    /*! ����*/
    char meterModel[MAX_ASCII_LEN];
    /*! ��Դ*/
    char lightSource[MAX_ASCII_LEN];
    /*! ����ʱ��flash���˵��*/
    char flash[MAX_ASCII_LEN];
    /*! ����*/
    char focalLength[MAX_ASCII_LEN];
    /*! ͼ����*/
    long width;
    /*! ͼ��߶�*/
    long height;
    /*! Exif�汾*/
    char exifVersion[4];
    /*! GPS �汾*/
    char GPSVersion[4];
    /*! GPSγ�Ȳο�*/
    char GPSLatitudeRef[4];
    /*! GPSγ��*/
    char GPSLatitude[MAX_ASCII_LEN];
    /*! GPS���Ȳο�*/
    char GPSLongtitudeRef[4];
    /*! GPS����*/
    char GPSLongtitude[MAX_ASCII_LEN];
} mmm_image_exif_t;

/*!
 * \brief
 *      ͼƬ��Ϣ
 */
typedef struct
{
    /*! ͼƬ��ʽ*/
    char file_format[8];
    /*! ͼƬ���*/
    unsigned int width;
    /*! ͼƬ�߶�*/
    unsigned int height;
    /*! ͼƬ����*/
    mmm_img_date_t date;
    /*! ͼƬ������Ϣ*/
    mmm_image_exif_t *exif;
} mmm_img_file_info_t;

/*!
 * \brief
 *      ͼƬ�������
 */
typedef struct
{
    /*! ͼƬ���ȣ���ÿһ���ص��Ӧ��byte������2��3*/
    unsigned int bpp;
    /*! ͼƬ�����ʽ��YUVorRGB��*/
    unsigned int formate;
    /*! Ҫ����������ͼƬ�Ŀ�*/
    unsigned int rect_width;
    /*! Ҫ����������ͼƬ�ĸ�*/
    unsigned int rect_height;
    /*! ʵ�ʽ��������ͼƬ�Ŀ�*/
    unsigned int img_width;
    /*! Ҫ����������ͼƬ�ĸ�*/
    unsigned int img_height;
    /*! Ҫ����������ͼƬ���ݴ洢��buf��ַ*/
    unsigned char *buf;
    /*! buffer��ַ�ĳ���*/
    unsigned int len;
} mmm_img_info_t;
/*!
 * \brief
 *      �洢��bufͼƬ����Ϣ�ṹ
 */
typedef struct
{
    /*! buffer��ַ*/
    char *media_buf;
    /*! buffer����*/
    int media_len;
} mmm_media_buf_t;
/*!
 * \brief
 *      ר��ͼƬ��Ϣ�ṹ
 */
typedef struct
{
    /*! album��Ϣ���ڵ��ļ�*/
    char *file_name;
    /*! album��Ϣ���ļ��е�ƫ��*/
    int data_offset;
    /*! album��Ϣ�ĳ���*/
    int data_len;
} mmm_album_info_t;

void *mmm_id_open(void *param);
int mmm_id_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_id_close(void *handle, void *param);

/*! \endcond*/

#ifdef __cplusplus
}
#endif

#endif
