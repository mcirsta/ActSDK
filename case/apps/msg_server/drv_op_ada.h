/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             �������ļ���
*******************************************************************************
*/ 

#ifndef __DRV_OP_ADA_H__
#define __DRV_OP_ADA_H__


#include "include_case.h"
#include "include_msg_server.h"


/* wav�������ļ��� �ļ�ͷ���Ⱥ궨��. �м�����ʽ, 44Ϊ�����һ�� */
#define WAV_FILE_HEAD_LENGTH    (44)



/*!
*\brief
    �����İ������ļ���Ϣ�ṹ��, �������ø�ADA���� */
typedef struct
{
    /*! key tone ��������, �ֵ�������������. һ�㰴�����õ���������, ���������Լ���һ�� */
    enum ADA_VOICE_CHANNEL channle;
    /*! key tone �����ʲ���. �������������ʵ�����¾�����, �Լ��������� */
    int sample_rate;
}key_tone_head_t;



/*!
*\brief
    ��������Ϣ�ṹ��, ���еİ�����������״̬�������ڴ˽ṹ���� */
typedef struct
{
    /*! key tone �Ƿ���Ч, ����setting�򿪹رհ�����, Ҳ�����ڲ���ʼ���Ƿ�ɹ� ������ */
    int on;
    
    /*! key tone�Ƿ�����, ��ֹ��audioӦ�ó�ͻ, ������audioʱ����0, �˳�������1 */
    int enable;
    
    /*! key tone file handle */
    int fd;
    
    /*! key tone file fullpath */
    char fullpath[PATH_MAX];
    
    /*! key tone file data len, unit is byte */
    unsigned int len;
    
    /*! key tone data pointer */
    char *data;
    
    key_tone_head_t head;
}key_tone_param_t;



/************************ extern variable, �������� ***************************/
extern key_tone_param_t key_tone_param;



/************************ extern function, �������� ***************************/
extern void ada_para_init( void );
extern void ada_device_open( void );
extern void ada_device_close( void );

extern bool ada_set_key_tone_param( void );
extern bool ada_set_key_tone_on( int flag_on );
extern bool ada_set_key_tone_enable( int flag_enable );
extern bool ada_set_key_tone_play( void );
extern bool ada_set_volume( int volume );
extern bool ada_get_volume( int *volume );


#endif /* __DRV_OP_ADA_H__ */

