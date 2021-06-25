/*******************************************************************************
 *                              5003
 *                            Module: sound
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-05 15:00     1.0             build this file 
*******************************************************************************/
/*!
 * \file     sound.h
 * \brief    ����Ƶ����豸�Ĳ���
 * \author   kkli
 * \version 1.0
 * \date  2008/09/05
*******************************************************************************/
#ifndef __SOUND_H__
#define __SOUND_H__
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief  
 *      �������������ģʽ�����Ƿ�����豸�Լ�����豸�������й�
 *      ���Ź�����������Ч
 */
typedef enum
{
    /*! I2S˫���� */
    AO_MODE_I2S_2 = 0x1,
    /*! I2S 5.1������3wire */
    AO_MODE_I2S_51_3WDOUT,
    /*! I2S 5.1������TDM */
    AO_MODE_I2S_51_TDM,
    /*! SPDIF */
    AO_MODE_SPDIF,
    /*! HDMI */
    AO_MODE_HDMI,
    /*! On-chip Sigma-Delta */
    AO_MODE_CHIP
} ao_mode_t;
/*!
 * \brief  
 *      ������������ĵ��Է�ʽ
 *      ���Ź�����������Ч 
 */
typedef enum
{
    /*! ֻ�ǲ��� */
    AO_DEBUG_PLAY = 0x1,
    /*! д�ļ���ͬʱ���ţ��̶�λ���ļ����� */
    AO_DEBUG_FILE_PLAY,
    /*! ֻд�ļ����̶�λ���ļ����� */
    AO_DEBUG_FILE,
    /*! ����� */    
    AO_DEBUG_NONE
} ao_debug_t;
/*!
 * \brief  
 *      ����PCM����ķ�ʽ
 */
typedef enum
{
    /*! �������ż�ֹͣ*/
    AO_PCM_MODE_NORMAL = 0x1,
    /*! ѭ������ */
    AO_PCM_MODE_LOOP
} ao_pcm_mode_t;

/* magic word: 0x414f */
typedef enum
{
    AO_CMD_INIT = 0x414f00,
    AO_CMD_PLAY,
    AO_CMD_STOP,
    AO_CMD_PAUSE,
    AO_CMD_RESUME,
    /*! ���ģʽ */
    AO_CMD_MODE,
	AO_CMD_SET_DEBUG,
    AO_CMD_SET_VOLUME,
    AO_CMD_GET_VOLUME,    
    /*! ����PCM�ļ���ȫ·���� */
    AO_CMD_PCM_FILE,
    /*! ����PCM����ģʽ */
    AO_CMD_PCM_MODE,
    /*! ��ʼ����PCM */
    AO_CMD_PCM_ON,
    /*! ֹͣ����PCM */
    AO_CMD_PCM_OFF,    
} ao_cmd_t;

typedef enum
{
    AO_TERMINATED,
    AO_PLAYING,     
    AO_STOPPED,     
    AO_PAUSED,      
} ao_status_t;

typedef struct
{
    int channels;		/* ����������� */
    int sample_rate;    /* ��������� */
} audiout_init_t;

void *audiout_open(void);
int audiout_cmd(void *, ao_cmd_t, unsigned long);
void audiout_close(void *);

#define AI_SOURCE_MIC       0
#define AI_SOURCE_FM        1
#define AI_SOURCE_LINEIN    2

/*!
 * \brief  
 *      ������������ɼ�ģ���������
 */
typedef enum
{
    /*! �����������Ϊaudioin_init_t */
    AI_CMD_INIT = 0x414900,
    AI_CMD_START,
    AI_CMD_STOP,
    AI_CMD_PAUSE,
    AI_CMD_RESUME,
    /*! �����������Ϊaudioin_source_t */
    AI_CMD_SET_SOURCE,
    AI_CMD_SET_DEBUG
} ai_cmd_t;
/*!
 * \brief  
 *      ���������ɼ�ģ��ĳ�ʼ������
 */
typedef struct
{
    /*! һ�βɼ���sample�� */
    int frame_samples; 
    /*! ������ */
    int sample_rate;
    /*! ����������� */
    int channels;
} audioin_init_t;
/*!
 * \brief  
 *      �����¼��Դ������
 */
typedef struct
{
    /*! Դ���� */
    int type;
    /*! ����Դ�ķ��ȣ���λ���� */
    int source_amplitude;
    /* ��������ķ��ȣ���λ���� */
    int target_amplitude;
} audioin_source_t;

void *audioin_open(void);
int audioin_cmd(void *, ai_cmd_t, unsigned long);
void audioin_close(void *);

#ifdef __cplusplus
}
#endif
#endif // __SOUND_H__
