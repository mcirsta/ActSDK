/********************************************************************************
 *                              usdk130
 *                            Module: UPGRADE
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       huanghe     2008-08-31      1.0                    build this file
 *       huanghe     2009-01-04      2.0                    sdk��������
 ********************************************************************************/
/*!
 * \file			upgrade.h
 * \brief		upgrade�����ⲿ�ı������ͺ��ṩ�Ķ���ӿ�
 * \author		�ƺ�
 * \par Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version		2.0
 * \date			2009/1/4
 *******************************************************************************/

#ifndef  __UPGRADE_H__
#define  __UPGRADE_H__

/*! \cond USB_UPGRADE_API*/

/*!	��������״̬
 *  \li UPG_STATE_DEFAULT����ʼ֮ǰ��Ĭ��״̬
 *  \li UPG_STATE_INIT����ʼ״̬
 *  \li UPG_STATE_CHECK_FW�����fw��ȷ��
 *  \li UPG_STATE_CHECK_OK��fw������
 *  \li UPG_STATE_WRITE_FW��д��fw
 *  \li UPG_STATE_WRITE_OK��fwд�����
 *  \li UPG_STATE_EXIT���˳����˲���
 * 	\li UPG_STATE_ERROR������
 */
enum upg_state_t
{
    UPG_STATE_DEFAULT = 0,
    UPG_STATE_INIT,
    UPG_STATE_CHECK_FW,
    UPG_STATE_CHECK_OK,
    UPG_STATE_WRITE_FW,
    UPG_STATE_WRITE_OK,
    UPG_STATE_EXIT,
    UPG_STATE_ERROR
};

/*! �޴� */
#define NO_ERR                  (0)

/*! ���� */
#define BASE_ERR                (100)

/*! �̼������� */
#define FW_NOT_EXIST_ERR        (BASE_ERR+1)

/*! �ڴ治�� */
#define BUF_MALLOC_ERR          (BASE_ERR+2)

/*! �̼�ͷ�ı�־���� */
#define FW_HEAD_FLAG_ERR        (BASE_ERR+3)

/*! dsp��ʼ������ */
#define DSP_INIT_ERR            (BASE_ERR+4)

/*! �̼�ͷ���ݽ��ܳ��� */
#define FW_HEAD_DATA_ERR        (BASE_ERR+5)

/*! �̼����Ȳ��� */
#define FW_LENGTH_ERR           (BASE_ERR+6)

/*! �̼�β����־���� */
#define FW_TAIL_FLAG_ERR        (BASE_ERR+7)

/*! �̼�β��У��ʹ��� */
#define FW_TAIL_CHECKSUM_ERR    (BASE_ERR+8)

/*! �̼���У��ʹ��� */
#define FW_CHECKSUM_ERR         (BASE_ERR+9)

/*! afiͷ�ı�־Ϊ���� */
#define AFI_HEAD_FLAG_ERR       (BASE_ERR+10)

/*! afiͷ��Ŀ¼����� */
#define AFI_HEAD_DIR_ERR        (BASE_ERR+11)

/*! afuͷ�Ĵ��� */
#define AFI_HEAD_CHECKSUM_ERR   (BASE_ERR+12)

/*! lfiͷ�ı�־λ���� */
#define LFI_HEAD_FLAG_ERR       (BASE_ERR+13)

/*! lfiͷ��У��ʹ��� */
#define LFI_HEAD_CHECKSUM_ERR   (BASE_ERR+14)

/*! �¹̼�д��ʱ�����Ǿɹ̼���u���� */
#define UDISK_OVERLAP_ERR       (BASE_ERR+15)

/*! mbrc�ı�־λ���� */
#define MBRC_FLAG_ERR           (BASE_ERR+16)

/*! mbrc1��У��ʹ��� */
#define MBRC_1st_CHECKSUM_ERR   (BASE_ERR+17)

/*! mbrc2��У��ʹ��� */
#define MBRC_2nd_CHECKSUM_ERR   (BASE_ERR+18)

/*! brec�ı�־λ���� */
#define BREC_FLAG_ERR           (BASE_ERR+19)

/*! brec��У��ʹ��� */
#define BREC_CHECKSUM_ERR       (BASE_ERR+20)

/*! storage�߼�д�� */
#define STORAGE_LOG_WRITE_ERR   (BASE_ERR+21)

/*! storage�߼����� */
#define STORAGE_LOG_READ_ERR    (BASE_ERR+22)

/*! storage����д�� */
#define STORAGE_PHY_WRITE_ERR   (BASE_ERR+23)

/*! storage������� */
#define STORAGE_PHY_READ_ERR	(BASE_ERR+24)

/*! strage���ʳ��� */
#define STORAGE_OTHER_ERR       (BASE_ERR+25)

/*! �̼����� */
#define FW_READ_ERR             (BASE_ERR+26)

/*! �̼���λ�� */
#define FW_SEEK_ERR             (BASE_ERR+27)

/*! ��ʱ */
#define TIMEOUT_ERR             (BASE_ERR+28)

/*! ״̬���� */
#define STATE_ERR               (BASE_ERR+29)

/*! �������̼������ */
#define CREATE_CHECK_TASK_ERR   (BASE_ERR+30)

/*! ����д�̼������ */
#define CREATE_WRITE_TASK_ERR   (BASE_ERR+31)

/*! �ں˵������Ŵ��� */
#define KERNEL_SYM_ERR          (BASE_ERR+32)

/*! δ֪���� */
#define UNKNOWN_ERR             (BASE_ERR+100)

/*!
 * \brief	��������״̬�Ľṹ�壬����Ӧ�ú������䴫��״̬
 */
struct upg_status_t
{
    /*! ��������״̬ */
    enum upg_state_t e_state;

    /*! ��������ֵ��0��100��������д�� */
    unsigned int d_prg;

    /*! ������������ */
    int d_errtype;
};

/*! this enumeration is defined for ioctrl command of upgrade driver
 *  \li UPG_CMD_INIT : ��ʼ��
 *  \li UPG_CMD_CHECK_FW : ���̼��Ƿ���ȷ
 *  \li UPGRADE_WRITE_FW : д��̼�
 *  \li UPG_CMD_EXIT : �˳�
 *  \li UPGRADE_READ_STATUS : ��ȡ����״̬
 *  \li UPG_CMD_MODIFY_PARAM : �޸���������
 */
enum upg_ioctl_cmd_t
{
    UPG_CMD_INIT,
    UPG_CMD_CHECK_FW,
    UPG_CMD_WRITE_FW,
    UPG_CMD_EXIT,
    UPG_CMD_READ_STATUS,
    UPG_CMD_MODIFY_PARAM
};

/*!
 * \brief	Ӧ�ô��������Ĳ���
 */
struct upg_param_t
{
    /*! �Զ����������ڲ���������ȼ� */
    int task_priority;

    /*! �̼�·�� */
    unsigned char *pb_fwpath;

    /*! �Ƿ��齫Ҫд�����lba���Է�ֹ���Ǵ������Ĺ̼�����fs */
    unsigned char b_is_check_maxlba;
};

/*! \endcond*/

#endif /*__UPGRADE_H__ */
