/*
 ********************************************************************************
 *
 *			amvb structure
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : amvb_struct.h
 * by	 : AL/FW
 * Version: 1> v1.00     first version     01.09.2007
 ********************************************************************************
 */
#ifndef __ACT_AMVB_H__
#define	__ACT_AMVB_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      ��������amvb ��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ��Ŀ������ */
    int total_prog;
    /*! ��Ŀ���������� */
    int prog_language;
    /*! logo��־ */
    int has_logo;
    /*! logo�������� */
    int logo_language;
    /*! �������� */
    int total_track;
    /*! ��������������� */
    int track_language;
    /*! �����û���Ϣ������־ */
    int has_exif;
    /*! ������Ļ��Ϣ��־ */
    int lyric;
} amvb_info_t;

/*!
 * \brief
 *      ��������amvb �û�exif ������Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ����ʱ�� */
    unsigned short PublishTimes;
    /*! ������ */
    char Make[36];
    /*! ý������ */
    char Medium[16];
    /*! ģʽ */
    char Mode[24];
    /*! ������ */
    char Publish[32];
    /*! ����ʱ�� */
    char CreatData[32];
    /*! ����ʱ�� */
    char PublishData[32];
    /*! �״η��еص� */
    char location[40];
    /*! ��Ȩ������ */
    char Copyright[24];
    /*! ����֧�� */
    char Technical[56];
    /*! ������� */
    char Software[24];
    /*! ���� */
    char Comments[256];
} amvb_base_info_t;

/*!
 * \brief
 *      ��������amvb exif video��ʽ��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ��Ƶ֡��� */
    unsigned short dwWitdh;
    /*! ��Ƶ֡�߶� */
    unsigned short dwHeight;
    /*! ��ɫ��� */
    unsigned short dwBPP;
    /*! ��Ƶ�������� */
    unsigned int dwBitRate;
    /*! ѹ����ʽ */
    unsigned int dwCompression;
    /*! ����ʱ�� */
    unsigned short dwTime;
    /*! Ӱ�� */
    char MovieName[56];
    /*! ��Ҫ��Ա */
    char Actor[48];
    /*! �ؼ��� */
    char KeyWord[100];
} amvb_video_info_t;

/*!
 * \brief
 *      ��������amvb exif audio��ʽ��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ��Ƶ������Ϣ */
    unsigned short dwID;
    /*! ������ */
    unsigned int dwSamplerate;
    /*! �������� */
    unsigned short dwSamplebits;
    /*! ������ */
    unsigned int dwBitRate;
    /*! ����ʱ�� */
    unsigned int dwTime;
    /*! �ݳ��� */
    char Author[32];
    /*! ������ */
    char MusicName[64];
    /*! �ؼ��� */
    char KeyWord[80];
} amvb_audio_info_t;

/*!
 * \brief
 *      ��������amvb exif��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! amvb exif ������Ϣ */
    amvb_base_info_t base_info;
    /*! amvb exif ��Ƶ��Ϣ */
    amvb_video_info_t video_info;
    /*! amvb exif ��Ƶ��Ϣ */
    amvb_audio_info_t audio_info;
} amvb_exif_info;

/*!
 * \brief
 *      ��������amvb �����Խ�Ŀ��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ��Ŀ�������� */
    unsigned int prog_len;
    /*! ��Ŀ�������� */
    char *prog_context;
} amvb_siglang_prog_t;

/*!
 * \brief
 *      ��������amvb ����Ŀ��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ��Ӧ�Ľ�Ŀʱ��� */
    unsigned int prog_time;
    /*! �����Խ�Ŀ������������amvb_progtab_t�е�total_lang */
    amvb_siglang_prog_t *mutilang_prog;
} amvb_sig_prog_t;

/*!
 * \brief
 *      ��������amvb ��Ŀ��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! ���Ա�� */
    unsigned int lang_classify;
    /*! �������� */
    unsigned int total_lang;
    /*! ���Ŀ��Ϣ����Ŀ��getfileinfo�õ� */
    amvb_sig_prog_t *muti_prog;
} amvb_progtab_t;

/*!
 * \brief
 *      �����˲�ͬ��logo��ʽ���ͳ�������Щ��������ָ��logo��ʽ
 */
typedef enum
{
    /*! bmp24 */
    AMVB_LOGO_FMT_BMP24 = 0x101,
    /*! gif */
    AMVB_LOGO_FMT_GIF,
    /*! bmp565 */
    AMVB_LOGO_FMT_BMP565,
    /*! yuv420 */
    AMVB_LOGO_FMT_YUV420,
} amvb_logo_fmt_t;

/*!
 * \brief
 *      ������logo��Ϣ�йص����ݽṹ
 */
typedef struct
{
    /*! logo���½� */
    unsigned int logo_chapter;
    /*! logo��˳�����к� */
    unsigned int logo_serial_no;
    /*! ��Ч�������ֻһ����Ч [�ⲿ����] */
    unsigned int logo_language;
    /*! ��ǰlogo�ľ����ʽ */
    unsigned int logo_format;
    /*! logo�Ŀ� */
    unsigned int logo_width;
    /*! logo�ĸ� */
    unsigned int logo_height;
    /*! ����ʽ��Ϣ��ͼ����Ϣ [���] */
    char *logo_buf;
} amvb_logo_info_t;

#ifdef __cplusplus
}
#endif

#endif

