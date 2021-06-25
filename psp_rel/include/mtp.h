/*! \mainpage
 *
 * \section dest_sec �ĵ�Ŀ��
 * \li  MTP APIӦ�ýӿ��ĵ���

 * \section ref_sec �ο�����
 * \li [1] Media Transfer Protocol Enhanced,Revision 0.96, Microsoft Corp., August 31,2006 \n
 * \li [2] Photography-Electronic still picture imaging �C Picture Transfer Protocol (PTP) for Digital Still Photography Devices.Photographic and Imaging Manufacturers Association,Inc. \n
 * \li [3] Uiversal Serial Bus Still Image Capture Device Definition,Revision 1.0,USB Device Working Group,July 11,2000. \n
 * \li [4] PlaysForSure Version 2.20 Requirements Specification for portable Devices,version 2.20.00.070328, Microsoft Corp. \n

 * \section func_sec ���ܸ���
 * \li  ����һ����Ӧ�ó�����ʿ���MTP�Ľӿں�����
 *
 * \section auth_sec ����
 *  \li  Jerry He, hejf\@actions-semi.com
 *  \li Actions Semiconductor Corp.
 *
 * \section date_sec ʱ��
 *  \li 2009.07.16
 */

/*!
 * \file   mtp.h
 * \brief
 * \author Jerry He, hejf@actions-semi.com
 * \par GENERAL DESCRIPTION:
 *      MTP API�ӿں���
 *  \par Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2007/5/4
 *******************************************************************************/
#ifndef _MTP_H_3485_
#define _MTP_H_3485_

#include "ptp.h"

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_EXIT
 */
/*! MTP���������˳�״̬*/
#define   MTP_DAEMON_RUNNING_STATUS_EXIT                99

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND
 */
/*! MTP�����ڵȴ���������״̬*/
#define   MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND     100

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND
 */
/*! MTP������ִ����������״̬*/
#define   MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND            101

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT
 */
/*! ������MTP�����Ͷ�������״̬*/
#define   MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT                 102

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_GET_OBJECT
 */
/*! ������MTP�����ȡ��������״̬ */
#define   MTP_DAEMON_RUNNING_STATUS_GET_OBJECT                   103

/*!
 * \def  MTP_STORE_ID_NAND
 */
/*! Nand�洢�����*/
#define  MTP_STORE_ID_NAND        0x10001

/*!
 * \def  MTP_STORE_ID_CARD
 */
/*! Card�洢�����*/
#define  MTP_STORE_ID_CARD        0x20001

/*
 * ���º궨��Ϊ�ṩ������Playlistʱ����ʹ�á�����ͬ�����еĸĶ�������¼�����ݿ��mtpModifiedIdTable
 * �ñ������Ϊ��
 * CREATE TABLE IF NOT EXISTS mtpModifiedIdTable(id integer default 0, mode integer default 0);
 *
 * �ñ��id�ֶμ�¼�������޸ģ����ӻ�ɾ��������object id��
 * �ñ��mode�ֶμ�¼��Ӧid���޸����ͣ��磺MTP_ID_ADDED_STATE, MTP_ID_DELETED_STATE, MTP_ID_MODIFIED_STATE�ȡ�
 * ��Ӧ�ĺ궨��ֵ�����漸�С�
 *
 * ���⣬�������mtp���ӹ������и�ʽ��������mtpModifiedIdTable���л�����һ���¼��
 * id��-1, mode=MTP_ALL_FORMATED_STATE.
 * Playlist��ش���ɾݴ��ж��Ƿ��и�ʽ��������
 */
//#define   MTP_ID_NULL_STATE           0x0   /* nothing is done. */
#define   MTP_ID_ADDED_STATE          0x1   /* the object is new added. */
#define   MTP_ID_MODIFIED_STATE       0x2   /* the object is new modified. */
#define   MTP_ID_DELETED_STATE        0x4   /* the object is new deleted. */
#define   MTP_ALL_FORMATED_STATE      0x8   /* the whole disk is formated. */

/* below is the default value of mtp_vendor_info_t. */
#define MTP_VENDOR_ID_FEFAULT               0x10d6    /* Actions Semiconductor*/
#define MTP_PRODUCT_ID_FEFAULT              0x2300    /*default value*/
#define MTP_EXTERN_ID_FEFAULT               0x0000    /*default value*/

#define MTP_DEVICE_MANUFACTURER_DEFAULT     "Actions Semiconductor Co., Ltd."
#define MTP_DEVINFO_MODEL_DEFAULT           "H-MTP"
#define MTP_DEVINFO_VERSION_DEFAULT         "MTP-1.063-N-FRE"
#define MTP_DEVINFO_SERIAL_DEFAULT          "6FC89818B0F5C9AC5DAF09D860F78BFD"

#define MTP_STORE_INTERNAL_DESC_DEFAULT         "u1100"
#define MTP_STORE_PORTABLE_DESC_DEFAULT         "u1100 external"

#define MTP_STORE_EXT_DESC0_DEFAULT             "none"
#define MTP_STORE_EXT_DESC1_DEFAULT             "none"

#define MTP_MUSIC_ALBUM_OFFSET_VAL             -2

/*!
 * \brief
 * mtp_vendor_info_t    �ͻ��ɸ��ĵı���PC��MTP�豸��Ϣ��
 */
typedef struct s_mtp_vendor_info
{
    /*! mtp device product id. */
    unsigned int mtp_pid;
    /*! mtp device vendor id. */
    unsigned int mtp_vid;
    /*! mtp device extend id. */
    unsigned int mtp_ext;

    /*! mtp device manufacturer description. */
    char dev_manufacturer[60];
    /*! mtp device mode. */
    char mtp_dev_model[60];
    /*! mtp device version. */
    char mtp_dev_version[60];
    /*! mtp device serial number. */
    char mtp_dev_serial[60];

    /*! mtp device's internal storage disk description. */
    char mtp_store_inner_desc[40];
    /*! mtp device's portable storage disk description (usually card). */
    char mtp_store_portable_desc[40];

    /*! mtp device's extend description. */
    char mtp_dev_ext_desc0[60];
    /*! mtp device's extend description. */
    char mtp_dev_ext_desc1[60];
} mtp_vendor_info_t;

/*!
 * \brief
 * mtp_modified_info_t    ��¼����ͬ�����޸���Ϣ
 */
typedef struct s_mtp_modified_info
{
    /*!
     * indicate the main mtpobjecttable whether be formated.
     * 0--not formated, 1--formated.
     */
    unsigned int is_formated;

    /*! buffer that store all the new added object ID in current connection. */
    unsigned int * added_objs;
    /*! count of the new added objects in current connection. */
    unsigned int added_cnt;

    /*! buffer that store all the modified object ID in current connection. */
    unsigned int * modified_objs;
    /*! count of the modified objects in current connection. */
    unsigned int modified_cnt;

    /*! buffer that store all the deteted object ID in current connection. */
    unsigned int * deleted_objs;
    /*! count of the deteted objects in current connection. */
    unsigned int deleted_cnt;

} mtp_modified_info_t;

/*!
 * \fn int mtp_init(void **ppMtp,char *dbName)
 * \brief  ��ʼ��MTP������.
 *
 * \par    Description
 *   mtp_init()��ʼ��һ��MTP�����ģ����������ȷ���أ�*ppDev������MTP�����ĵ�ַ��
 *����޷����һ����Ч�������ģ������������ĵ�MTP API�������޷������á�dbName�ṩ
 *��֧��MTPԪ���ݵ����ݿ�����
 * \param [out]  ppMtp
 *   MTP������ָ��
 * \param [in]  dbName
 *   MTP Ԫ���ݿ�·������
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 *
 * static  int MtpCallBack(void *pMtp,void *pPara,int type)
 * {
 *    int ret;
 *
 *    ret=0;
 *
 *    return ret;
 * }
 *
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0){
 *       goto out;
 *    }
 *    ret=mtp_run(pDev,"/dev/tunnel","/dev/mtpdrm","/dev/vram",MtpCallBack);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_init(void **ppMtp, char *dbName);

/*!
 * \fn int mtp_exit(void *pMtp)
 * \brief  �ͷ�MTP������.
 *
 * \par    Description
 *   mtp_exit()�ͷ�һ��MTP�����ģ�������mtp_init()��ʼ����MTP�����ģ���Ӧ�ڽ���
 * �˳�ǰ�ͷ���������������Դй©��
 *
 * \param [in]  pMtp
 *   MTP������
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 * static  int MtpCallBack(void *pMtp,void *pPara,int type)
 * {
 *    int ret;
 *
 *    ret=0;
 *
 *    return ret;
 * }
 *
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0){
 *       goto out;
 *    }
 *    ret=mtp_run(pDev,"/dev/tunnel","/dev/mtpdrm","/dev/vram",MtpCallBack);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_exit(void *pMtp);

/*!
 * \fn int mtp_run(void *pMtp,char *tunnel,char *drm, char *vram,int (*callback)(void *,void *,int))
 * \brief  ����MTP����.
 *
 * \par    Description
 *     ��MTP������������MTP����MTP���������ֻ������һ��MTP����mtp_run()һ�����У����Ƿ�������
 * �ָ���������������᷵�ء��ص�����callback���Կ���MTP�Ƿ���ֹ���С�mtp_run()��ʹ�õ�MTP�����ģ���
 * ���Ǿ���mtp_init()��ʼ���ģ������������ش���ֵ��
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  tunnel
 *   tunnel�豸����
 * \param [in]  drm
 *   drm�豸����
 * \param [in]  vram
 *   vram�豸����
 * \param [in]  callback
 *   �ص�����
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 * static  int MtpCallBack(void *pMtp,void *pPara,int type)
 * {
 *    int ret;
 *
 *    ret=0;
 *
 *    return ret;
 * }
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0){
 *       goto out;
 *    }
 *    ret=mtp_run(pDev,"/dev/tunnel","/dev/mtpdrm","/dev/vram",MtpCallBack);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_run(void *pMtp, char *tunnel, char *drm, char *vram, int(*callback)(
        void *, void *, int));

/*!
 * \fn int mtp_stop(void *pMtp)
 * \brief  ֹͣ�������е�MTP����
 *
 * \par    Description
 *     mtp_stop()����ֹͣһ������mtp_run()���е�MTP����
 *
 * \param [in]  pMtp
 *   MTP������
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 *
 * void *gpDev=NULL;
 *
 * static  int MtpCallBack(void *pMtp,void *pPara,int type)
 * {
 *    int ret;
 *
 *    ret=0;
 *
 *    return ret;
 * }
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    ret = mtp_init(&gpDev,"/udisk/MTP.db");
 *    if(ret<0){
 *       goto out;
 *    }
 *    ret=mtp_run(gpDev,"/dev/tunnel","/dev/mtpdrm","/dev/vram",MtpCallBack);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(gpDev);
 gpDev=NULL;
 *    return 0;
 * }
 *
 *  int service_stop(void)
 *  {
 *      int ret;
 *      ret = mtp_stop(gpDev);
 *      return ret;
 *  }
 * \endcode
 **/
int mtp_stop(void *pMtp);

/*!
 * \fn int mtp_get_running_status(void *pMtp)
 * \brief  ���MTP���������״̬
 *
 * \par    Description
 *     MTP������ʹ��mtp_run()������mtp_get_running_status()����������ѯ��ǰ��������������״̬��
 *
 * \param [in]  pMtp
 *   MTP������
 * \return
 * \li  MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND       �ȴ�����������������MTP����
 * \li  MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND       MTP�������
 * \li  MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT       �������豸���Ͷ���������
 * \li  MTP_DAEMON_RUNNING_STATUS_GET_OBJECT        �������豸��ȡ����������
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 *
 * void *gpDev=NULL;
 *
 * static  int MtpCallBack(void *pMtp,void *pPara,int type)
 * {
 *    int ret;
 *
 *    ret=0;
 *
 *    return ret;
 * }
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&gpDev,"/udisk/MTP.db");
 *    if(ret<0){
 *       goto out;
 *    }
 *    ret=mtp_run(gpDev,,"/dev/tunnel","/dev/mtpdrm","/dev/vram",MtpCallBack);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(gpDev);
 *    return 0;
 * }
 *
 *  int get_service_status(void)
 *  {
 *      int ret;
 *      ret = mtp_get_running_status(gpDev);
 *      return ret;
 *  }
 * \endcode
 **/
int mtp_get_running_status(void *pMtp);

/*!
 * \fn int mtp_get_device_model(void *pMtp,char *model,int size)
 * \brief ���MTP�豸model����.
 *
 * \par Description
 *    MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ����model����Ϣ����һ���֣�
 *mtp_get_device_model()���������豸�ϱ���model���ơ�
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [out]  model
 *   MTP �豸model���ơ� model����UTF-8���룬�����ԡ�\\0���������ַ�����
 * \param [in]  size
 *   model��������ĳ��ȡ�
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char model[256];
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_device_model(pDev,model,sizeof(model));
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_device_model(void *pDev, char *model, int size);

/*!
 * \fn int mtp_set_device_model(void *pMtp,char *model)
 * \brief ����MTP�豸model����.
 *
 * \par Description
 *    MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ����model����Ϣ����һ���֣�
 *mtp_set_device_model()���������豸�ϱ���model���ơ�
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  model
 *   MTP �豸model���ơ� model����UTF-8���룬�����ԡ�\\0���������ַ�����model�ַ����ĳ��������ƣ�
 *����ת��ΪUTF16��ʾ���ַ���ʱ���䳤��Ӧ������255��
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *     ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_set_device_model(pDev,"model 123");
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_device_model(void *pMtp, char *model);

/*!
 * \fn int mtp_get_device_manufacturer(void *pMtp,char *manufacturer,int size)
 * \brief ���MTP�豸manufacturer����.
 *
 * \par Description
 *    MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ����manufacturer����Ϣ����һ���֣�
 *mtp_get_device_manufacturer()���ڻ�ȡ�豸�ϱ���manufacturer���ơ�
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [out]  manufacturer
 *   MTP �豸manufacturer���ơ� manufacturer����UTF-8���룬�����ԡ�\\0���������ַ�����
 * \param [out]  size
 *   manufacturer�����С��
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char manufactuer[256];
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_device_manufactuer(pDev,manufactuer,sizeof(manufactuer);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_device_manufacturer(void *pDev, char *manufacturer, int size);

/*!
 * \fn int mtp_set_device_manufacturer(void *pMtp,char *manufacturer)
 * \brief  ����MTP�豸manufacturer����.
 *
 * \par Description
 *  MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ����Manufacturer����Ϣ��
 *��һ���֣�mtp_set_device_manufacturer ()���������豸�ϱ���manufactuer���ơ�
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  manufacturer
 *   MTP �豸manufacturer���ơ� manufacturer����UTF-8���룬�����ԡ�\\0���������ַ�����manufacturer�ַ����ĳ��������ƣ�
 *����ת��ΪUTF16��ʾ���ַ���ʱ���䳤��Ӧ������255��
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *     ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_set_device_manufacturer(pDev,"manufacture 456");
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_device_manufacturer(void *pMtp, char *manufacturer);

/*!
 * \fn int mtp_get_device_version(void *pMtp,char *version,int size)
 * \brief ���MTP�豸�汾������Ϣ.
 *
 * \par Description
 *    MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ�����豸�汾����Ϣ����һ���֣�
 *mtp_get_device_version()���ڻ���豸�ϱ����豸�汾��Ϣ��
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [out]  version
 *   MTP �豸�汾������Ϣ�� version����UTF-8���룬�����ԡ�\\0���������ַ�����
 * \param [in]  size
 *   version��������ĳ��ȡ�
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char version[256];
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_device_version(pDev,version,sizeof(version));
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_device_version(void *pDev, char *version, int size);

/*!
 * \fn int mtp_set_device_version(void *pMtp,char *version)
 * \brief ����MTP�豸�汾������Ϣ.
 *
 * \par Description
 *    MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ�����豸�汾������Ϣ����Ϣ����һ���֣�
 *mtp_set_device_version()���������豸�ϱ����豸�汾������Ϣ��
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  version
 *   MTP �豸�汾������Ϣ��version����UTF-8���룬�����ԡ�\\0���������ַ�����version�ַ����ĳ��������ƣ�
 *����ת��ΪUTF16��ʾ���ַ���ʱ���䳤��Ӧ������255��
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_set_device_version(pDev,"MTP-1.067-N-FRE");
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_device_version(void *pMtp, char *version);

/*!
 * \fn int mtp_get_device_serial(void *pMtp,char *serial,int size)
 * \brief ���MTP�豸manufacturer����.
 *
 * \par Description
 *    MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ����serial����Ϣ����һ���֣�
 *mtp_get_device_serial()���ڻ�ȡ�豸�ϱ���serial���ơ�
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [out]  serial
 *   MTP �豸serial���ơ� serial����UTF-8���룬�����ԡ�\\0���������ַ�����
 * \param [out]  size
 *   serial�����С��
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char serial[33];
 *
 *     ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_device_serial(pDev,serial,sizeof(serial));
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_device_serial(void *pDev, char *serial, int size);

/*!
 * \fn int mtp_set_device_serial(void *pMtp,char *serial)
 * \brief  ����MTP�豸�����к�
 *
 * \par Description
 *   MTPЭ��涨��������ͨ��GetDeviceInfo������ȡMTP�豸��Ϣ����Serial����Ϣ��
 * ��һ���֣�mtp_set_device_manufacturer ()���������豸�ϱ���serial���ơ�
 * \param [in] pMtp
 *   MTP������
 * \param [in]  serial
 *   MTP �豸serial����.
 *   serial����UTF-8���룬�����ԡ�\\0���������ַ�����serial�ַ����ĳ��������ƣ���������\\0���ַ����ڣ�
 *�32���ַ���
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_set_device_serial(pDev,"A80123456789012345678901234567890123456789");
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_device_serial(void *pMtp, char *serial);

/*!
 * \fn int mtp_get_store_name(void *pMtp,unsigned int storeID,char *name,int size)
 * \brief  ��ȡ�洢��������
 *
 * \par Description
 *  ��������ͨ��GetStorageInfo�����ȡ�洢�������ƣ�С���ϱ��Ĵ洢�����ƿ�������mtp_get_store_name()
 *��á��ṩ�Ļ���ռ�Ӧ�����Է����»�ȡ�����ơ������ǡ�\\0���ַ���β��UTF��8�����ַ�����
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  storeID
 *   �洢����ID
 * \param [out] name
 *   ���ֻ�������
 * \param [in]  size
 *   ���������С
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char name[256]= "";
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_store_name(pDev,MTP_STORE_ID_NAND,name,sizeof(name));
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_store_name(void *pMtp, unsigned int storeID, char *name, int size);

/*!
 * \fn int mtp_set_store_name(void *pMtp,unsigned int storeID,char *name)
 * \brief  ���ô洢������
 * \par Description
 *      ��������ͨ��GetStorageInfo�����ȡ�洢�������ƣ�С���ϱ��Ĵ洢�����ƿ�������
 *mtp_set_store_name()���á�name�ַ���Ӧ�����ԡ�\\0���ַ���β��UTF��8�����ַ�������
 *�Ͻ�β�ġ�\\0���ַ�������ת��ΪUTF16��ʾ���ַ���ʱ�򣬳��Ȳ�Ӧ�ó���255���ַ���
 * \param [in]  pMtp
 *   MTP������
 * \param [in]   storeID
 *   �洢�����
 * \param [in]   name
 *   �洢������
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char name[256]= "Mtp General Storage";
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *
 *    ret=mtp_set_store_name(pDev,MTP_STORE_ID_NAND,name);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_store_name(void *pMtp, unsigned int storeID, char *name);

/*!
 * \fn int mtp_get_store_root_path(void *pMtp,unsigned int storeID,char *name,int size)
 * \brief  ��ô洢���ĸ�Ŀ¼·�� .
 *
 * \par Description
 *   ÿһ���洢�����ļ�ϵͳ�϶���Ψһ��Ŀ¼����Ӧ�������������ڴ洢���Ķ������ݾ������ڶ�Ӧ��Ŀ¼�С�
 *mtp_get_store_root_path()���ڻ�ȡMTP�´洢���ĸ�Ŀ¼·����name�����ڷ�������ȡ�Ĵ洢�����ƣ�
 *��������޷����´洢���ĸ�·������ô����������һ������ֵ��*
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  storeID
 *   �洢�����
 * \param [out] name
 *  ��Ŀ¼���ƻ���
 * \param [in] size
 *  ���ƻ������Ĵ�С
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char path[512];
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_store_root_path(pDev,MTP_STORE_ID_NAND,path,sizeof(path));
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_store_root_path(void *pMtp, unsigned int storeID, char *name,
        int size);

/*!
 * \fn int mtp_set_store_root_path(void *pMtp,unsigned int storeID,char *path)
 * \brief  ��ô洢���ĸ�Ŀ¼·�� .
 *
 * \par Description
 *   ÿһ���洢�����ļ�ϵͳ�϶���Ψһ��Ŀ¼����Ӧ�������������ڴ洢���Ķ������ݾ������ڶ�Ӧ��Ŀ¼�С�
 * mtp_set_store_root_path()��������MTP�´洢���ĸ�Ŀ¼·����path�ַ���Ӧ�����ԡ�\\0���ַ���β��UTF��8
 * �����ַ��������Ͻ�β�ġ�\\0���ַ�������ת��ΪUTF16��ʾ���ַ���ʱ�򣬳��Ȳ�Ӧ�ó���255���ַ���
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  storeID
 *   �洢�����
 * \param [out] path
 *  ��Ŀ¼����
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char path[256]="/mnt/udisk";
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_set_store_root_path(pDev,MTP_STORE_ID_NAND,path);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_store_root_path(void *pMtp, unsigned int storeID, char *name);

/*!
 * \fn int mtp_get_store_dev(void *pMtp,unsigned int storeID,char *dev,int size)
 * \brief  ��ô洢����Ӧ���豸·�� .
 *
 * \par Description
 *   �ļ�ϵͳ�ҽ��ڴ洢�豸֮�ϡ��˽ӿ����ڻ�ȡ�洢����Ӧ�Ĵ洢�豸·����dev���ڷ�������ȡ���豸·����
 *���sizeָʾ�Ļ���̫С������������һ������ֵ��
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  storeID
 *   �洢�����
 * \param [out] dev
 *  �洢�豸���ƻ���
 * \param [in] size
 *  ���ƻ���Ĵ�С
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char path[512];
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_store_dev(pDev,MTP_STORE_ID_NAND,path,sizeof(path));
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_store_dev(void *pMtp, unsigned int storeID, char *dev, int size);

/*!
 * \fn int mtp_set_store_dev(void *pMtp,unsigned int storeID,char *dev)
 * \brief  ���ô洢�����ڵ��豸·�� .
 *
 * \par Description
 *   �ļ�ϵͳ�ҽ��ڴ洢�豸֮�ϡ��˽ӿ��������ô洢����Ӧ�Ĵ洢�豸·����path�ַ���Ӧ�����ԡ�\\0���ַ���β��UTF��8
 * �����ַ��������Ͻ�β�ġ�\\0���ַ���
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  storeID
 *   �洢�����
 * \param [in] dev
 *  �洢�豸����
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    char path[256]="/dev/udisk";
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_set_store_dev(pDev,MTP_STORE_ID_NAND,path);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_store_dev(void *pMtp, unsigned int storeID, char *dev);

/*!
 * \fn int mtp_create_object(void *pMtp, unsigned int storageID, unsigned short format, unsigned short protection,
 *        unsigned long long size,unsigned short thumbFormat,unsigned int thumbCompressedSize,
 *        unsigned int thumbPixWidth,unsigned int thumbPixHeight,unsigned int imagePixWidth,
 *        unsigned int imagePixHeight,unsigned int imageBitDepth,
 *        unsigned int parent, unsigned short associateType, unsigned int associateDesc,
 *       char *filename, char *dateCreated,char *dateModified, char *keywords, unsigned int *pHandle)
 * \brief  ����һ������
 *
 * \par Description
 *  ����һ��MTP�������MTP����ӳ�䵽�ļ�ϵͳ�ϵ��ļ�����Ŀ¼����ôʹ����Ӧ��ȷ���ļ�����Ŀ¼ȷʵ���ڡ�
 *δ�ڲ����г��ֵĶ������ԣ�����ͨ��mtp_set_object_prop_value()�������á�
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  storageID
 *   �������ڴ洢�����
 * \param [in] format
 *   �����ʽ
 * \param [in] protection
 *   ���ݶ���ı���״̬
 * \param [in] size
 *   �����С
 * \param [in] thumbFormat
 *   ����ͼ��ʽ
 * \param [in] thumbCompressedSize
 *   ����ͼ��С
 * \param [in] thumbPixWidth
 *   ����ͼ���
 * \param [in] thumbPixHeight
 *   ����ͼ�߶�
 * \param [in] imagePixWidth
 *   ͼƬ��ȣ������ؼƣ�
 * \param [in] imagePixHeight
 *   ͼƬ�߶ȣ������ؼƣ�
 * \param [in] imageBitDepth
 *   ͼƬ��ȣ������ؼƣ�
 * \param [in] parent
 *   ��������
 * \param [in] associateType
 *   ��������
 * \param [in] associateDesc
 *   ��������
 * \param [in] filename
 *   ����.
 * \param [in] dateCreated
 *   ��������
 * \param [in] dateModified
 *   ����޸�����
 * \param [in] keywords
 *   �ؼ���
 * \param [out] pHandle
 *   ������
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    unsigned int handle;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *   ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMV, 0,
 *            1000, 0, 0,0,0,0,0,0,
 *            0,0,0, "god.wma", "19901223", "20011230", "singer", &handle);
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_create_object(void *pDev, unsigned int storageID,
        unsigned short format, unsigned short protection,
        unsigned long long size, unsigned short thumbFormat,
        unsigned int thumbCompressedSize, unsigned int thumbPixWidth,
        unsigned int thumbPixHeight, unsigned int imagePixWidth,
        unsigned int imagePixHeight, unsigned int imageBitDepth,
        unsigned int parent, unsigned short associateType,
        unsigned int associateDesc, char *filename, char *dateCreated,
        char *dateModified, char *keywords, unsigned int *pHandle);

/*!
 * \fn int mtp_delete_object(void *pDev,unsigned int handle)
 * \brief  ɾ��һ������ .
 *
 * \par Description
 *   ����mtp_create_object()�����Ķ��󣬿�������mtp_delete_object()ɾ����
 * \param [in]  pDev
 *   MTP������
 * \param [in]  handle
 *   ������
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    unsigned int handle;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *   ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMV, 0,
 *            1000, 0, 0,0,0,0,0,0,
 *            0,0,0, "god.wma", "19901223", "20011230", "singer", &handle);
 *   if(ret<0){
 *       goto out;
 *   }
 *   ret = mtp_delete_object(pDev,handle);
 *   if(ret<0){
 *       goto out;
 *   }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_delete_object(void *pDev, unsigned int handle);

/*!
 * \fn int mtp_get_object_num(void *pDev,unsigned int storeID,unsigned int *pNum)
 * \brief  ��ô洢�����ϵĶ�����Ŀ
 *
 * \par Description
 *  ��ô洢�����ϵĶ�����Ŀ��
 *
 * \param [in]  pDev
 *       MTP������
 * \param [in] storeID
 *      �洢������
 * \param [out] pNum
 *      ������Ŀ�����ַ
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    void *pDev=NULL;
 *    unsigned int num;
 *
 *    ret = mtp_init(&pDev,MTP_SQLITE_DATABASE_PATH);
 *    if(ret<0) {
 *       goto out;
 *    }
 *    if(ret<0){
 *       goto out;
 *    }

 *    ret=mtp_get_object_num(pDev,MTP_STORE_ID_NAND,&num);
 *    if(ret<0){
 *       goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_object_num(void *pDev, unsigned int storeID, unsigned int *pNum);

/*!
 * \fn int mtp_get_object_handle (void *pDev,unsigned int storeID,
 *        unsigned int offset,unsigned int *pHandles, int num);
 *
 * \brief  ��ô洢�����ϵĶ�����
 *
 * \par Description
 *  �洢�����϶�����Ŀ���ܱȽ϶࣬��ʱ����ָ��ƫ��������ָ��λ��offset��ȡnum����������
 *
 * \param[in]  pDev
 *       MTP������
 * \param[in] storeID
 *       �洢������
 * \param[in] offset
 *       ƫ����
 * \param[in] pHandles
 *       �������ռ�
 * \param[in] num
 *       �����Ŀ
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret,i;
 *    UINT32 handle;
 *    UINT32 handles[100];
 *    char filename[128];
 *    void *pDev=NULL;

 *    ret = mtp_init(&pDev,MTP_SQLITE_DATABASE_PATH);
 *    if(ret<0) {
 *       goto out;
 *    }
 *   for (i = 0; i < 10; i++)
 *   {
 *       sprintf(filename, "god%d.wma\n", i);
 *       ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMV, 0,
 *           1000, 1, 2,
 *            3,4,5,
 *             6,7,
 *            0,8,9,
 *            filename, "19901223", "20011230", "hello", &handle);
 *   }
 *    ret = mtp_get_object_handle(pDev, MTP_STORE_ID_NAND, 0, handles, 2);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_object_handle(void *pDev, unsigned int storeID,
        unsigned int offset, unsigned int *pHandles, int num);

/*!
 * \fn int mtp_add_object_reference(void *pMtp,unsigned int parent,unsigned int child,unsigned int *pSeq)
 * \brief ���Ӷ��������.
 *
 * \par Description
 *  mtp_add_object_reference ()��������һ���������Ӷ�������á������ý������ڸ�����ԭ�����õ����
 *��������ʶ��ǰ�������Ƿ��Ѿ����Ӷ�������á��������������Ӷ��󲻴��ڣ��������ش���ֵ��*pSeq����
 *����������ţ�������������ָ�����ͬһ�Ӷ���Ķ�����á�
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  parent
 *    �����ߵľ��
 * \param [in]  child
 *    �������ߵľ��
 * \param [out]  pSeq
 *    �������
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    UINT32 handle0,handle1;
 *    UINT32 seq;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_ABSTRACT_AV_PLAYLIST, 0,
 *           0, 0, 0, 0,0,0, 0,0,0,0,0,"god.pla", "19901223", "20011230", "playlist", &handle0);
 *    if(ret<0){
 *         goto out;
 }

 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *           1000, 0, 0,0,0,0,0,0,0,0,0,"god is a girl.wma", "19901223", "20011230", "song 0", &handle1);
 *    if(ret<0){
 *        goto out;
 *     }
 *
 *    ret = mtp_add_object_reference(pDev, handle0, handle1, &seq);
 *    if(ret<0){
 *         goto out;
 }
 * out:
 *    mtp_exit(pDev);
 *    return ret;
 * }
 * \endcode
 **/
int mtp_add_object_reference(void *pMtp, unsigned int parent,
        unsigned int child, unsigned int *pSeq);

/*!
 * \fn int mtp_delete_object_reference(void *pMtp,unsigned int parent,unsigned int child,unsigned int seq)
 * \brief  ɾ�����������.
 *
 * \par Description
 *  mtp_delete_object_reference ()����ɾ��һ���������Ӷ�������á�seq�������ָ�����ͬһ�Ӷ�����
 *������õ����Ρ��������������Ӷ��󲻴��ڣ�������ָ����ŵ�λ�ô���û�����������ã���ô��������
 *����ֵ��ɾ��ָ����Ŵ������ò���Ӱ�츸������������ü������
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  parent
 *   �����ߵľ��
 * \param [in]  child
 *   �������ߵľ��
 * \param [in]  seq
 *   �������
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    UINT32 handle0,handle1;
 *    UINT32 seq;
 *    void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_ABSTRACT_AV_PLAYLIST, 0,
 *           0, 0, 0, 0,0,0, 0,0,0,0,0,"god.pla", "19901223", "20011230", "playlist", &handle0);
 *    if(ret<0){
 *         goto out;
 }

 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *           1000, 0, 0,0,0,0,0,0,0,0,0,"god is a girl.wma", "19901223", "20011230", "song 0", &handle1);
 *    if(ret<0){
 *        goto out;
 *    }
 *
 *    ret = mtp_add_object_reference(pDev, handle0, handle1, &seq);
 *    if(ret<0){
 *        goto out;
 *    }
 *
 *    ret = mtp_del_object_reference(pMtp, handle0, handle1, seq);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return ret;
 * }
 * \endcode
 **/
int mtp_delete_object_reference(void *pMtp, unsigned int parent,
        unsigned int child, unsigned int seq);

/*!
 * \fn int mtp_get_object_reference_num(void *pMtp,unsigned int parent,unsigned int *pNum)
 * \brief  ��ȡ����������Ŀ.
 *
 * \par Description
 *  ��ȡ����������Ŀ��
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  parent
 *   ��������
 * \param [out]  pNum
 *   ����������Ŀ.
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *   int ret;
 *   UINT32 handle0;
 *   UINT32 num;
 *  void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *
 *    parent = 0;
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_ABSTRACT_AV_PLAYLIST, 0,
 *           0, 0, 0,0,0,0,0,0,0,0,0,
 *           "god.pla", "19901223", "20011230", "playlist", &handle0);
 *    if(ret<0){
 *       goto out;
 *    }
 *
 *    ret = mtp_get_object_reference_num(pDev, handle0, &num);
 *    if(ret<0){
 *       goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return ret;
 * }
 * \endcode
 **/
int mtp_get_object_reference_num(void *pMtp, unsigned int parent,
        unsigned int *pNum);

/*!
 * \fn int mtp_get_object_reference(void *pMtp, unsigned int parent, unsigned int *pReference, unsigned int *pSeq, int num);
 * \brief  ��ȡ����������Ŀ.
 *
 * \par Description
 *  ��ȡ����������Ŀ��
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  parent
 *   ��������
 * \param [out]  pReference
 *   ���ö������õ�����.
 * \param [out]  pSeq
 *   ���ö���������ŵ�����
 * \param [in] num
 *   �����С
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *   int ret;
 *   UINT32 handle0,handle1,seq0;
 *   UINT32 num;
 *  void *pDev=NULL;
 *
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *
 *    parent = 0;
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_ABSTRACT_AV_PLAYLIST, 0,
 *           0, 0, 0,0,0,0,0,0,0,0,0,
 *           "god.pla", "19901223", "20011230", "playlist", &handle0);
 *    if(ret<0){
 *         goto out;
 *    }
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *            1000, 1, 2,
 *            3,4,5,
 *             6,7,
 *            0,8,9,
 *            "god.wma", "19901223", "20011230", "song 0", &handle1);
 *    ret = mtp_add_object_reference(pDev, handle0, handle1, &seq0);
 *    if(ret<0){
 *        goto out;
 *    }
 *    ret = mtp_get_object_reference_num(pDev, handle0, &num);
 *    if(ret<0) {
 *        goto out;
 *    }
 *    ret = mtp_get_object_reference(pDev, handle0, references, seqs, num);
 * out:
 *    mtp_exit(pDev);
 *    return ret;
 * }
 * \endcode
 **/
int mtp_get_object_reference(void *pMtp, unsigned int parent,
        unsigned int *pReference, unsigned int *pSeq, int num);

/*!
 * \fn int mtp_get_object_path(void *pMtp,unsigned int handle,char *path,int size)
 * \brief  ��ö����·��.
 *
 * \par Description
 *   MTP�е�һЩ��������ļ�ϵͳ�ϵĶ�Ӧ�mtp_get_object_path���ڻ�ȡ�������ļ�ϵͳ�϶�Ӧ��Ŀ¼
 *�����ļ����ơ������������Ķ��󲻴��ڣ���ô���������ش���ֵ��*
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  handle
 *   ������
 * \param [out]  path
 *   ·������
 * \param [in]  size
 *   �����С
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    unsigned int handle;
 *    char path[512];
 *    void *pDev=NULL;
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *            1000, 0, 0, 0,0,0,0,0,0,0,0,
 *            "god.wma", "19901223", "20011230", "song 0", &handle1);
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_object_path(pDev,handle1,path,sizeof(path));
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_object_path(void *pMtp, unsigned int handle, char *path, int size);

/*!
 * \fn int mtp_search_object_using_path(void *pMtp, char *path, unsigned int *pHandle);
 * \brief  ���ö�����ļ�·������������ľ��.
 *
 * \par Description
 *  ����������ļ�����Ŀ¼֧�ţ������ʹ��mtp_search_object_using_path()��ö�������
 *
 * \param[in]  pMtp
 *      MTP ������
 * \param[in] path
 *      �ļ�����Ŀ¼·��
 * \param[out] pHandle
 *       ������
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    unsigned int handle,handle1;
 *    char path[512];
 *    void *pDev=NULL;
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *            1000, 0, 0, 0,0,0,0,0,0,0,0,
 *            "god.wma", "19901223", "20011230", "song 0", &handle1);
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_object_path(pDev,handle1,path,sizeof(path));
 *    if(ret<0){
 *        goto out;
 *    }
 *    ret = mtp_search_object_using_path(pDev,path,&handle);
 *    if(ret<0){
 *      goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_search_object_using_path(void *pMtp, char *path, unsigned int *pHandle);

/*!
 * \fn int mtp_set_object_prop_value(void *pMtp,unsigned int handle, unsigned short propCode, void *data, int size)
 * \brief  ���ö��������
 *
 * \par Description
 *      һ��MTP����֧�ֶ������ԣ�mtp_set_object_prop_value()�������ö�������ֵ��������󲻴��ڣ�
 *���߶���֧�ָ����ԣ��������ش���ֵ��
 *
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  handle
 *   ������
 * \param [in]  propCode
 *   ���Ա���
 * \param [in]  data
 *   ����ֵ
 * \param [in]  size
 *   ����ռ�ݵĿռ��С
 *
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    unsigned int handle;
 *    char value[]="hello world\n";
 *    void *pDev=NULL;
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *   ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *            1000, 0, 0, 0,0,0,0,0,0,0,0,
 *            "god.wma", "19901223", "20011230", "song 0", &handle);
 *   if(ret<0){
 *      goto out;
 *   }
 *
 *   ret = mtp_set_object_prop_value(pDev, handle, MTP_OBJ_PROPERTYCODE_NAME, value, sizeof(value));
 *   if(ret<0){
 *      goto out;
 *   }
 *
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_set_object_prop_value(void *pMtp, unsigned int handle,
        unsigned short propCode, void *data, int size);

/*!
 * \fn int mtp_get_object_prop_value(void *pMtp,unsigned int handle,
 unsigned short propCode, void *data,int size);
 * \brief  ��ȡ��������ֵ
 *
 * \par Description
 *       һ��MTP����֧�ֶ������ԣ�mtp_get_object_prop_value()���ڻ�ȡ��������ֵ��������󲻴��ڣ�
 *���߶���֧�ָ����ԣ��������ش���ֵ��
 * \param [in]  pMtp
 *   MTP������
 * \param [in]  handle
 *   ������
 * \param [in]  propCode
 *   �������Ա���
 * \param [out]  data
 *   ����ֵ����
 * \param [in]  size
 *   ����ֵ����ռ��С
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    unsigned int handle,storaeID;
 *    char value[]="hello world\n";
 *    void *pDev=NULL;
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *            1000, 0, 0, 0,0,0,0,0,0,0,0,
 *            "god.wma", "19901223", "20011230", "song 0", &handle);
 *    if(ret<0){
 *       goto out;
 *    }
 *   ret = mtp_get_object_prop_value(pDev, handle, MTP_OBJ_PROPERTYCODE_STORAGEID, &storageID, sizeof(UINT32));
 *   if(ret<0){
 *       goto out;
 }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_object_prop_value(void *pMtp, unsigned int handle,
        unsigned short propCode, void *data, int size);

/*!
 * \fn int mtp_get_obj_modify_cnt(void *pDev, unsigned int *pObjCnt);
 * \brief  ��ȡ��ǰ���ݿ�����˵�OBJECT�ĸ�����
 *
 * \par Description
 *  ��ȡ��ǰ���ݿ�����˵�OBJECT�ĸ�����������pObjCnt��ָ���������ͷ��
 *
 * \param[in]  pDev
 *      MTP ������
 * \param[out] pObjCnt
 *      �����ĵ����ݿ��¼����
 *
 * \return
 * \li  0      Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    unsigned int handle,handle1;
 *    unsigned int times;
 *    char path[512];
 *    void *pDev=NULL;
 *    ret = mtp_init(&pDev,"/udisk/MTP.db");
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret = mtp_create_object(pDev, MTP_STORE_ID_NAND, MTP_FORMATCODE_WMA, 0,
 *            1000, 0, 0, 0,0,0,0,0,0,0,0,
 *            "god.wma", "19901223", "20011230", "song 0", &handle1);
 *    if(ret<0) {
 *       goto out;
 *    }
 *    ret=mtp_get_obj_modify_cnt(pDev, &times);
 *    if(ret<0){
 *        goto out;
 *    }
 * out:
 *    mtp_exit(pDev);
 *    return 0;
 * }
 * \endcode
 **/
int mtp_get_obj_modify_cnt(void *pDev, unsigned int *pObjCnt);

/*!
 * \fn int mtp_get_cur_modified_info(void *pDev, mtp_modified_info_t *pInf);
 * \brief  ��ȡ����MTPͬ�����еĸĶ���Ϣ��
 *
 * \par Description
 * \brief  ��ȡ����MTPͬ�����еĸĶ���Ϣ�������Ƿ��Ѿ���ʽ������
 * ����ЩObject ID�������ģ���Щ�Ǳ�ɾ���ģ���Щ���޸Ĺ��Ą1�7
 *
 * \param[in]  pDev
 *      MTP ������
 * \param[out] pInf
 *      �ṹ��ָ�룬ָ���¼�����ĵ���Ϣ�Ľṹ�塣
 *
 * \return
 * \li  0      Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 *
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    mtp_modified_info_t db_modif_info;
 *
 *    ret = mtp_init(&gpDev,"/udisk/MTP.db");
 *    if(ret<0){
 *       goto out;
 *    }
 *    ret=mtp_run(gpDev,"/dev/tunnel","/dev/mtpdrm","/dev/vram",MtpCallBack);
 *    if(ret<0){
 *        goto out;
 *    }
 *    mtp_get_cur_modified_info(gpDev, &db_modif_info);
 *
 *    * call playlist interface here. *
 *
 *    mtp_free_cur_modified_info(gpDev, &db_modif_info);
 * out:
 *    mtp_exit(gpDev);
 gpDev=NULL;
 *    return 0;
 * }
 *
 * \endcode
 **/
int mtp_get_cur_modified_info(void *pDev, mtp_modified_info_t *pInf);

/*!
 * \fn int mtp_free_cur_modified_info(void *pDev, mtp_modified_info_t *pInf);
 * \brief  �ͷ��ϴλ�ȡ����MTPͬ�����еĸĶ���Ϣʱ���������ϵͳ�ڴ����Դ��
 *
 * \par Description
 * \brief  �ͷ��ϴλ�ȡ����MTPͬ�����еĸĶ���Ϣʱ���������ϵͳ�ڴ����Դ��
 *
 * \param[in]  pDev
 *      MTP ������
 * \param[out] pInf
 *      �ṹ��ָ�룬ָ���¼�����ĵ���Ϣ�Ľṹ�塣
 *
 * \return
 * \li  0      Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      ����
 * \code
 *
 * int main(int argc,char **argv)
 * {
 *    int ret;
 *    mtp_modified_info_t db_modif_info;
 *
 *    ret = mtp_init(&gpDev,"/udisk/MTP.db");
 *    if(ret<0){
 *       goto out;
 *    }
 *    ret=mtp_run(gpDev,"/dev/tunnel","/dev/mtpdrm","/dev/vram",MtpCallBack);
 *    if(ret<0){
 *        goto out;
 *    }
 *    mtp_get_cur_modified_info(gpDev, &db_modif_info);
 *
 *    * call playlist interface here. *
 *
 *    mtp_free_cur_modified_info(gpDev, &db_modif_info);
 * out:
 *    mtp_exit(gpDev);
 gpDev=NULL;
 *    return 0;
 * }
 *
 * \endcode
 **/
int mtp_free_cur_modified_info(void *pDev, mtp_modified_info_t *pInf);

#endif

