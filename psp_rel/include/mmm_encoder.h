/*! \cond MMM_ENCODE_API*/
/********************************************************************************
 *                              usdk1100
 *                            Module: act_decoder.c
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jszeng    2008-09-02 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_encoder.h
 * \brief    ������Ӧ�ò�Ľӿڼ�����
 * \author   ���������׿�
 * \version 1.0
 * \date  2008/09/02
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_ENCODER_H__
#define __MMM_ENCODER_H__

#include "fb.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \cond MMM_ENC_COMMON_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�õ�ENCODE��������
 */
typedef enum
{
    /*! ���ñ���·��*/
    MMM_SET_SAVE_PATH = 0x01,
    /*! �����ļ���ʽ*/
    MMM_SET_FILE_FMT,
    /*! ��ȡ������Ϣ*/
    MMM_GET_DISK_INFO
} mmm_enc_cmd_t;
/*!
 *  \brief
 *     ���ظ�Ӧ�ò�Ĵ�����Ϣ
 */
typedef struct
{
    /*! ����ʣ��ռ�*/
    unsigned int space_left;
    /*! ����¼����¼���˲���Ϊʣ���¼ʱ�䣬����dc�˲���Ϊ���յ���Ƭ*/
    unsigned int info;
} mmm_disk_info_t;
/*! \endcond*/

/*! \cond MMM_DV_RECORD_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�õ�DV��RECORD�Ĺ�������
 */
typedef enum
{
    /*! ������Ƶ����*/
    MMM_SET_AUDIO_PARAM = 0x21,
    /*! ��ȡ��������ǰ״̬*/
    MMM_GET_RECORDER_STATUS
} mmm_dv_record_cmd_t;
/*!
 *  \brief
 *     �м����������״̬
 */
typedef enum
{
    /*! ���ڱ���״̬*/
    MMM_RECORDER_RECORDING,
    /*! ����ֹͣ״̬*/
    MMM_RECORDER_STOPPED,
    /*! ������ͣ״̬*/
    MMM_RECORDER_PAUSED,
    /*! ���ڴ����ļ�״̬ */
    MMM_NWE_FILE_RECORDING,
    /*! ���ڳ���״̬״̬*/
    MMM_RECORDER_ERROR
} mmm_recorder_status_t;

/*!
 *  \brief
 *     ��Դ��ѡ�񣬵��ж��bitλΪ1ʱ����Ϊ��·����Ļ��
 */
typedef struct
{
    /*! �ڲ���˷�*/
    unsigned mic :1;
    /*! ������*/
    unsigned fm :1;
    /*! line in������*/
    unsigned line_in :1;
    /*! ��δʹ�ã���Ϊ0*/
    unsigned iis :1;
    /*! ��δʹ�ã���Ϊ0*/
    unsigned spdif :1;
    /*! ��δʹ�ã���Ϊ0*/
    unsigned tv :1;
    /*! ����λ*/
    unsigned reserved :26;
} mmm_audio_source_t;
/*!
 *  \brief
 *     ¼������Ҫ��
 */
typedef struct
{
    /*! �����������ѹ����mvΪ��λ*/
    unsigned int input_volt;
    /*! �����������ѹ����mvΪ��λ*/
    unsigned int output_volt;
} mmm_voice_volt_t;
/*!
 *  \brief
 *     ��Ƶ����Ĳ�������
 */
typedef struct
{
    /*! ¼����Դ��ѡ��*/
    mmm_audio_source_t source;
    /*! ¼������Ҫ��*/
    mmm_voice_volt_t voice_volt;
    /*! ��Ƶ������ľ��ȣ���8bit,16bit,24bit*/
    unsigned int bpp;
    /*! ��Ƶ������ͨ������:1 ��������2 ˫����*/
    unsigned int channel;
    /*! ����Ƶ����8000��16000��44100��*/
    unsigned int sample_rate;
    /*! ����ı�����*/
    unsigned int bitrate;
    /*! ��Ƶ�����ģʽ����ҪΪwav¼��ʱ��Ҫ���ֱ���ģʽΪadpcm����pcm��Ŀǰֻ֧��adpcm��Ҳ��Ĭ����0*/
    unsigned int encode_mode;
} mmm_audio_param_t;

/*! \endcond*/

/*! \cond MMM_DV_DC_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�õ�DV��DC�Ĺ�������
 */
typedef enum
{
    /*! ������Ƶ��ʾ���� */
    MMM_SET_CAPTURE_AREA = 0x41,
    /*! ����blending���򣬷��ؽ�����blending�����һ��int�͵ľ�� */
    MMM_CREATE_BLENDING_AREA,
    /*! ��ʾ���õ�blending���򣬲���Ϊ֮ǰcreate���صľ�� */
    MMM_SHOW_BLENDING_AREA,
    /*! �������õ�blending���򣬲���Ϊ֮ǰcreate���صľ�� */
    MMM_HIDE_BLENDING_AREA,
    /*! ���õ�blending��������� */
    MMM_SETATTR_BLENDING_AREA,
    /*! ����֮ǰ������blending���� */
    MMM_DESTROY_BLENDING_AREA,
    /*! ��ӡ��ǰͼƬ������ǰ��Ƶת��ΪRGB�����������Ļ�� */
    MMM_PRINT_CUR_SCREEN,
    /*! ���öԱȶ� */
    MMM_SET_CONTRAST,
    /*! ���ð�ƽ�� */
    MMM_SET_WHITE_BALANCE,
    /*! ��������Ч�� */
    MMM_SET_SPECIAL_EFFECT,
    /*! �����ع�ģʽ */
    MMM_SET_EXPOSURE_MODE,
    /*! ���÷ֱ��� */
    MMM_SET_RESOLUTION,
    /*! �����ļ��� */
    MMM_SET_FILE_NAME,
    /*! ��������佹���� */
    MMM_SET_ZOOM_LEVEL,
    /*! ��ʼȡ�� */
    MMM_START_CAPTURE,
    /*! ֹͣȡ�� */
    MMM_STOP_CAPTURE,
    /*! �������� */
    MMM_SET_BRIGHTNESS,
    /*! ���õ�ǰˢ��ģʽ */
    MMM_SET_REC_SCREEN_ROTATE
} mmm_dv_dc_cmd_t;

/*!
 *  \brief
 *    ȡ����ʾ���ڲ���
 */
typedef struct
{
    /*! ��ʾ�������Ͻǵ�x����*/
    int xpos;
    /*! ��ʾ�������Ͻǵ�y����*/
    int ypos;
    /*! ��ʾ���ڿ�*/
    unsigned int width;
    /*! ��ʾ���ڸ�*/
    unsigned int height;
} mmm_capture_area_t;

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
typedef fb_blending_param_t mmm_enc_blending_area_t;

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
typedef fb_blending_create_t mmm_enc_blending_create_t;

/*! \brief
 *     blending��������ԣ�����alphaֵ��colorkeyֵ
 *
 *  typedef struct
 *  {
 *      unsigned int color_key;
 *      unsigned int alpha;
 *  }fb_blending_attr_t;
 * */

typedef fb_blending_attr_t mmm_enc_blending_attr_t;
/*! \brief
 *      ����blendint���������ǵĲ������������������handle���Լ���Ҫ���õ�blending���������mmm_blending_attr_tֵ��
 *
 *  typedef struct
 *  {
 *      int handle;
 *      fb_blending_attr_t blending_attr;
 *  }fb_blending_setattr_t;
 * */
typedef fb_blending_setattr_t mmm_enc_blending_setattr_t;

/*!
 *  \brief
 *   ��ǰ��Ƶ��rgb����
 */
typedef struct
{
    /*! ��ǰ��Ƶ�����ϵ�x���� */
    int xpos;
    /*! ��ǰ��Ƶ�����ϵ�y���� */
    int ypos;
    /*! ��ǰ��Ƶ�Ŀ� */
    unsigned int width;
    /*! ��ǰ��Ƶ�ĸ� */
    unsigned int height;
    /*! ��ǰ��Ƶ���ݵ�ַ */
    char *video_frame;
} mmm_cur_frame_t;

/*!
 *  \brief
 *   �ֱ���
 */
typedef enum
{
    /*! �ֱ���Ϊ160*120 */
    MMM_RES160X120 = 0x0001,
    /*! �ֱ���Ϊ176*144 */
    MMM_RES176X144,
    /*! �ֱ���Ϊ224*176 */
    MMM_RES224X176 = 0x0004,
    /*! �ֱ���Ϊ320*240 */
    MMM_RES320X240,
    /*! �ֱ���Ϊ640*480 */
    MMM_RES640X480 = 0x0007,
    /*! �ֱ���Ϊ1024*768 */
    MMM_RES1024X768 = 0x0009,
    /*! �ֱ���Ϊ1280*960 */
    MMM_RES1280X960,
    /*! �ֱ���Ϊ1280*1024 */
    MMM_RES1280X1024,
    /*! �ֱ���Ϊ1600*1200 */
    MMM_RES1600X1200 = 0x000d,
    /*! �ֱ���Ϊ2048*1536 */
    MMM_RES2048X1536 = 0x0011,
    /*! �ֱ���Ϊ2400*1800 */
    MMM_RES2400X1800,
    /*! �ֱ���Ϊ2560*1920 */
    MMM_RES2560X1920,
    /*! �ֱ���Ϊ2816*2112 */
    MMM_RES2816X2112,
    /*! �ֱ���Ϊ3264*2448 */
    MMM_RES3264X2448,
    /*! �ֱ���Ϊ3840*2880 */
    MMM_RES3840X2880,
    /*! �ֱ���Ϊ4416*3312 */
    MMM_RES4416X3312,
    /*! �ֱ���Ϊ4672*3504 */
    MMM_RES4672X3504
} mmm_res_constant_t;

/*!
 *  \brief
 *   ����佹��������ĸһ��Ϊ10�����Ӵ��ڵ��ڷ�ĸ������ֵ��Ϊ�佹����
 */
typedef struct
{
    /*! ���� */
    int numerator;
    /*! ��ĸ */
    int denominator;
} mmm_zoom_level_t;

/*!
 *  \brief
 *  ��Ļ����ת����0�ȣ�90�ȣ�180�ȣ�270��
 */
/*��Ļ����ת����*/
//typedef enum
//{
//    /*! ��Ļ��ת0�ȣ������ӽ�*/
//    ROTATION_0 = 0,
//    /*! ��Ļ������ת90��*/
//    ROTATION_90 = 90,
//    /*! ��Ļ������ת180��*/
//    ROTATION_180 = 180,
//    /*! ��Ļ������ת270��*/
//    ROTATION_270 = 270
//}rotation_e;

/*!
 *  \brief
 *     ��ƽ�����ò���
 */
typedef enum
{
    /*! �Զ� */
    MMM_WB_AUTO,
    /*! ���� */
    MMM_WB_DAYLIGHT,
    /*! ���� */
    MMM_WB_CLOUD,
    /*! �׳�� */
    MMM_WB_INCANDESCENCE,
    /*! ӫ���*/
    MMM_WB_FLUORESCENT
} mmm_white_balance_t;

/*!
 *  \brief
 *     ����Ч�����ò���
 */
typedef enum
{
    /*! ����Ч��*/
    MMM_NONE_SPECIAL,
    /*! �ڰ�Ч��*/
    MMM_WHITE_AND_BLACK,
    /*! ����Ч��*/
    MMM_SEPIA,
    /*! ��ƬЧ��*/
    MMM_SOLARIZE
} mmm_special_effect_t;

/*!
 *  \brief
 *     �ع�ģʽ���ò���
 */
typedef enum
{
    /*! �Զ��ع�ģʽ*/
    MMM_AUTO_EXPOSURE,
    /*! �����ع�ģʽ*/
    MMM_INDOOR_EXPOSURE,
    /*! �����ع�ģʽ*/
    MMM_OUTDOOR_EXPOSURE
} mmm_exposure_mode_t;

/*! \endcond*/
/*! \cond MMM_DV_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�õ�DV����
 */
typedef enum
{
    /*! ������ƵԴ */
    MMM_SET_VIDEO_SOURCE = 0x61,
    /*! ������Ƶ�����ʽ */
    MMM_SET_AUDIO_FMT,
    /*! ������Ƶ�����ʽ */
    MMM_SET_VIDEO_FMT,
    /*! ������Ƶ������� */
    MMM_SET_VIDEO_PARAM,
    /*! �Ƿ�ʹ�ܶ������ */
    MMM_ENABLE_VIDEO_MONITOR,
    /*! ��ʼ¼�� */
    MMM_START_VIDEO,
    /*! ֹͣ¼�� */
    MMM_STOP_VIDEO,
    /*! ��ͣ¼�� */
    MMM_PAUSE_VIDEO,
    /*! ����¼�� */
    MMM_RESUME_VIDEO,
    /*! �Ƿ���Ҫ����¼��ʱ�� */
    MMM_VIDEO_TIME
} mmm_dv_cmd_t;

/*!
 *  \brief
 *     ¼���������ݽṹ
 */
typedef struct
{
    /*! �Ƿ����¼������, 0: ��; 1: �� */
    int enc_date_flag;
    /*! ��*/
    unsigned int year;
    /*! ��*/
    unsigned int month;
    /*! ��*/
    unsigned int day;
} mmm_enc_date_t;

/*!
 *  \brief
 *     ��Ƶ���ʼ�����
 */
typedef enum
{
    /*! ��Ƶ���ʼ���0: 500kbps*/
    MMM_VIDDEO_CBR_LEVEL0,
    /*! ��Ƶ���ʼ���1: 800kbps*/
    MMM_VIDDEO_CBR_LEVEL1,
    /*! ��Ƶ���ʼ���2: 1000kbps*/
    MMM_VIDDEO_CBR_LEVEL2,
    /*! ��Ƶ���ʼ���3: 1200kbps*/
    MMM_VIDDEO_CBR_LEVEL3,
    /*! ��Ƶ���ʼ���4: 1500kbps*/
    MMM_VIDDEO_CBR_LEVEL4,
    /*! ���ʿɱ�*/
    MMM_VIDDEO_VBR
} mmm_video_bitrate_level_t;

/*!
 *  \brief
 *     ��Ƶ�������ݽṹ
 */
typedef struct
{
    /*! ��Ƶ����*/
    unsigned int frame_rate;
    /*! ��Ƶ���ʼ���*/
    mmm_video_bitrate_level_t bitrate_level;
} mmm_video_args_t;

/*!
 *  \brief
 *    ͼ�񶯾�״̬
 */
typedef enum
{
    /*! ͼ��ֹ */
    MMM_STILL,
    /*! ͼ���˶� */
    MMM_MOVING
} mmm_monitor_status_t;

/*!
 *  \brief
 *     ¼���м��״̬
 */
typedef struct
{
    /*! ��ǰ¼��ʱ�� */
    unsigned int time;
    /*! ��ǰ�м��״̬ */
    mmm_recorder_status_t status;
    /*! ��ǰͼ�񶯾�״̬ */
    mmm_monitor_status_t monitor_status;
    /*! �����(�м��״̬Ϊ����״̬ʱ��Ч)*/
    unsigned int err_no;
} mmm_dv_status_t;

void *mmm_ve_open(void *param);
int mmm_ve_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ve_close(void *handle, void *param);

/*! \endcond*/

/*! \cond MMM_DC_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�õ�DC����
 */
typedef enum
{
    /*! ������ֱ��� */
    MMM_GET_MAX_RESOLUTION = 0x81,
    /*! ��������ƹ��� */
    MMM_SET_FLASHLIGHT,
    /*! �Ƿ�����������⹦�� */
    MMM_ENABLE_FACE_DETECT,
    /*! �Ƿ�������ͷ������ */
    MMM_PHOTO_STICKER,
    /*! ����ͼƬ��exif��Ϣ */
    MMM_SET_EXIF_INFO,
    /*! �Ƿ����Ƭ�������� */
    MMM_PHOTO_DATE,
    /*! ���� */
    MMM_SNAPSHOT
} mmm_dc_cmd_t;

/*!
 *  \brief
 *   ��������Ʋ���
 */
typedef enum
{
    /*! �ر� */
    MMM_FLASHLIGHT_OFF,
    /*! �� */
    MMM_FLASHLIGHT_ON,
    /*! �Զ� */
    MMM_FLASHLIGHT_AUTO
} mmm_flashlight_param;

/*!
 *  \brief
 *   �Ƿ�������ͷ������
 */
typedef struct
{
    /*! �Ƿ�������ͷ������, 0: ��;1: �� */
    int photo_sticker_flag;
    /*! ��ͷ������ͼƬ��Ϣ */
    unsigned char *buf;
    /*! ��ͷ������ͼƬ�� */
    unsigned int width;
    /*! ��ͷ������ͼƬ�� */
    unsigned int height;
} mmm_photo_sticker_t;

/*!
 *  \brief
 *   exif time��Ϣ
 */
typedef struct
{
    /*! �� */
    int year;
    /*! �� */
    int month;
    /*! �� */
    int day;
    /*! ʱ */
    int hour;
    /*! �� */
    int minute;
    /*! �� */
    int second;
} mmm_camera_exif_time_t;

/*!
 *  \brief
 *   exif������Ϣ,д���ļ�ͷ
 */
typedef struct
{
    /*! ��������� */
    char *machine;
    /*! ��Ƭ������ */
    char *owner;
    /*! ����ʱ����Ϣ */
    mmm_camera_exif_time_t time;
} mmm_camera_exif_t;

/*!
 *  \brief
 *   ����ʱ����Ϣ(����)
 */
typedef struct
{
    /*! �Ƿ��������������Ϣ, 0: ��; 1: �� */
    int photo_data_flag;
    /*! ʱ����Ϣ����buf��ַ */
    unsigned char *buf;
    /*! ʱ����Ϣ����� */
    unsigned int width;
    /*! ʱ����Ϣ����� */
    unsigned int height;
} mmm_photo_data_t;

void *mmm_ie_open(void *param);
int mmm_ie_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ie_close(void *handle, void *param);

/*! \endcond*/

/*! \cond MMM_RECORD_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�õ�¼������
 */
typedef enum
{
    /*! ��ʼ����*/
    MMM_START_MONITOR = 0xa1,
    /*! ֹͣ����*/
    MMM_STOP_MONITOR,
    /*! ����VOR����������ʱ��������С*/
    MMM_SET_VOR_PARAM,
    /*! ʹ�ܾ������*/
    MMM_SILENCE_DECTECT_ON,
    /*! �رվ������*/
    MMM_SILENCE_DECTECT_OFF,
    /*! ��ʼ¼��*/
    MMM_START_RECORDING,
    /*! ֹͣ¼��*/
    MMM_STOP_RECORDING,
    /*! ��ͣ¼��*/
    MMM_PAUSE_RECORDING,
    /*! ��ͣ�����¼��*/
    MMM_RESUME_RECORDING,
    /*! ��������*/
    MMM_SET_ENERGY_LEVEL
} mmm_record_cmd_t;
/*!
 *  \brief
 *     ����ģʽ
 */
typedef enum
{
    /*! ģ�����*/
    MMM_ANALOGE_MODE,
    /*! DSP����*/
    MMM_DIGITAL_MODE
} mmm_monitor_mode_t;
/*!
 *  \brief
 *     vor����
 */
typedef struct
{
    /*! ����ʱ��*/
    unsigned int vor_time;
    /*! ��������ֵ*/
    unsigned int vor_gain;
} mmm_vor_t;
/*!
 *  \brief
 *     �м����������ʵʱ��Ϣ
 */
typedef struct
{
    /*! ��ǰ��¼��ʱ��*/
    unsigned int time;
    /*! ��ǰcodec��״̬*/
    mmm_recorder_status_t status;
    /*! �����ǰʵʱ��������*/
    unsigned char energy_level;
    /*! ��ǰ�Ƿ��Ǿ������ھ�����⹦�ܿ���ʱ��Ч*/
    unsigned char silence_flag;
    /*! �ڳ���״̬ʱ���صĴ����*/
    unsigned int err_no;
} mmm_record_status_t;

void *mmm_ae_open(void *param);
int mmm_ae_cmd(void *handle, unsigned int cmd, unsigned int param);
int mmm_ae_close(void *handle, void *param);

#ifdef __cplusplus
}
#endif

#endif
