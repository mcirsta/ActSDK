/*! \cond FM_API*/

/********************************************************************************
 *                              NOYA
 *                            Module: FM Drvier
 *                 Copyright(c) 2001-2007 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xkping     2008-07-03 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file   radio.h
 * \brief  describle the command to access the FM driver and the data struct used in the driver
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File
 *       1)Define some Commands which are used to interact with the driver
 *       2)Define some data type which will be used in driver and FM application
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/
#ifndef __RADIO_H__
#define __RADIO_H__

#define RADIO_GET_STEREO_MODE     0x1          // 1:stereo mode; 0 singled mode ��ȡ��̨Ƶ��ģʽ
#define RADIO_GET_INFO            0X2          //��ȡ��ǰradio��һЩ״̬��Ϣ
#define RADIO_GET_INTENSITY       0x3          //��ȡ��̨���ź�ǿ��
#define RADIO_GET_BAND_MODE       0x4          //��ȡ��̨����
#define RADIO_GET_FREQ            0x5          //��ȡ��̨Ƶ��
#define RADIO_GET_ANTENNA         0x6          //��ȡ�����Ƿ����
#define RADIO_SET_FREQ            0x31        //��������Ƶ��
#define RADIO_SET_THRESHOLD       0x32        //����������̨���ź�ǿ������ֵ
#define RADIO_SET_BAND_MODE       0x33        // 0 :US mode; 1:EURO mode 2��jpan mode
#define RADIO_SET_PLAY            0x34        //����FM�������� dac FM mute��FM IC MUTE��
#define RADIO_SET_STOP            0x35        //ֹͣFM�������ر� dac FM mute��FM IC MUTE
#define RADIO_SET_ENTER_STANDBY   0x36        //����FM ����standby
#define RADIO_SET_AUTO_SEEK       0x37        //����FM �Զ���̨
#define RADIO_SET_AUTO_SEEK_BREAK 0x38        //���������Զ���̨
#define RADIO_SET_FREQ_NO_CHECK   0X39

#define RADIO_CHECK_LINEIN        0x50        //���linein���Ƿ���룻

/*!
 *  ���� \n
 *  \li US :   ���޲���
 *  \li EURO:  ŷ�޲���
 *  \li JAPAN: �ձ�����
 *
 */
enum BAND_MODE
{
    US = 0,
    EURO,
    JAPAN
};

/*!
 *  �������� \n
 *  \li AUTO_ADD :   ��������
 *  \li AUTO_MINUS:  �Լ�����
 *
 */
enum SEEK_DIRECTION
{
    AUTO_MINUS = 0,
    AUTO_ADD
};

enum SEEK_STATUS
{
    FOUND_STATION = 0,
    FOUND_NOSTAION,
    INVALID_STATION,
    REACH_BANDLIMIT

};
enum CHECK_LINEIN_STATE
{
    LINEIN_IN = 0,
    LINEIN_OUT
};

//#define ADD_FM_PLAY_STANDBY         0           // ������FM��������������,��STANDBY
//#define SWITCH_LOW_OSC_SEARCH       0           // ������96MHz�����õ�Ƶ����

/*!
 * \brief
 *radio_info_t,��������ǰ״̬��Ϣ.
 */
struct radio_info_t
{
    /*!��ǰ��̨��Ƶ��*/
    unsigned int current_freq;

    /*!��ǰ��̨��������Ϣ*/
    unsigned int stereo_status;

    /*!��ǰ��̨���ź�ǿ�� */
    unsigned int intensity;

    /*!��ǰ��̨�Ĳ���ģʽ*/
    unsigned int band_mode;
    /*!����ģʽ�µ�����״̬*/
    unsigned int seek_status;

};

/*!
 * \brief
 *seek_info_t,������������.
 */
struct radio_seek_info_t
{
    /*!��ʼ����Ƶ�ʣ���λ��KHZ */
    unsigned int search_freq;

    /*!������������λ��KHZ */
    unsigned int search_step;

    /*!��������AUTO_ADD�����ӷ���AUTO_MINUS: �Լ�����*/
    unsigned int search_direction;

};
/*! \endcond*/
#endif //end of   '#ifndef FM_DRV_H'

