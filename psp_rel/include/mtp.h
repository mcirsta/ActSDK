/*! \mainpage
 *
 * \section dest_sec 文档目的
 * \li  MTP API应用接口文档。

 * \section ref_sec 参考文献
 * \li [1] Media Transfer Protocol Enhanced,Revision 0.96, Microsoft Corp., August 31,2006 \n
 * \li [2] Photography-Electronic still picture imaging – Picture Transfer Protocol (PTP) for Digital Still Photography Devices.Photographic and Imaging Manufacturers Association,Inc. \n
 * \li [3] Uiversal Serial Bus Still Image Capture Device Definition,Revision 1.0,USB Device Working Group,July 11,2000. \n
 * \li [4] PlaysForSure Version 2.20 Requirements Specification for portable Devices,version 2.20.00.070328, Microsoft Corp. \n

 * \section func_sec 功能概述
 * \li  定义一整套应用程序访问控制MTP的接口函数。
 *
 * \section auth_sec 作者
 *  \li  Jerry He, hejf\@actions-semi.com
 *  \li Actions Semiconductor Corp.
 *
 * \section date_sec 时间
 *  \li 2009.07.16
 */

/*!
 * \file   mtp.h
 * \brief
 * \author Jerry He, hejf@actions-semi.com
 * \par GENERAL DESCRIPTION:
 *      MTP API接口函数
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
/*! MTP服务处于已退出状态*/
#define   MTP_DAEMON_RUNNING_STATUS_EXIT                99

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND
 */
/*! MTP服务处于等待主机命令状态*/
#define   MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND     100

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND
 */
/*! MTP服务处于执行主机命令状态*/
#define   MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND            101

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT
 */
/*! 主机向MTP服务发送对象数据状态*/
#define   MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT                 102

/*!
 * \def  MTP_DAEMON_RUNNING_STATUS_GET_OBJECT
 */
/*! 主机从MTP服务获取对象数据状态 */
#define   MTP_DAEMON_RUNNING_STATUS_GET_OBJECT                   103

/*!
 * \def  MTP_STORE_ID_NAND
 */
/*! Nand存储区编号*/
#define  MTP_STORE_ID_NAND        0x10001

/*!
 * \def  MTP_STORE_ID_CARD
 */
/*! Card存储区编号*/
#define  MTP_STORE_ID_CARD        0x20001

/*
 * 以下宏定义为提供给生成Playlist时候所使用。本次同步所有的改动项均会记录在数据库表：mtpModifiedIdTable
 * 该表创建语句为：
 * CREATE TABLE IF NOT EXISTS mtpModifiedIdTable(id integer default 0, mode integer default 0);
 *
 * 该表的id字段记录所有有修改，增加或删除动作的object id。
 * 该表的mode字段记录对应id的修改类型，如：MTP_ID_ADDED_STATE, MTP_ID_DELETED_STATE, MTP_ID_MODIFIED_STATE等。
 * 对应的宏定义值见下面几行。
 *
 * 另外，如果本次mtp连接过程中有格式化动作，mtpModifiedIdTable表中会增加一项记录：
 * id＝-1, mode=MTP_ALL_FORMATED_STATE.
 * Playlist相关代码可据此判断是否有格式化动作。
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
 * mtp_vendor_info_t    客户可更改的报给PC的MTP设备信息。
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
 * mtp_modified_info_t    记录本次同步的修改信息
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
 * \brief  初始化MTP上下文.
 *
 * \par    Description
 *   mtp_init()初始化一个MTP上下文，如果函数正确返回，*ppDev将放置MTP上下文地址。
 *如果无法获得一个有效的上下文，则依赖上下文的MTP API函数将无法被调用。dbName提供
 *了支撑MTP元数据的数据库名称
 * \param [out]  ppMtp
 *   MTP上下文指针
 * \param [in]  dbName
 *   MTP 元数据库路径名称
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  释放MTP上下文.
 *
 * \par    Description
 *   mtp_exit()释放一个MTP上下文，凡经由mtp_init()初始化的MTP上下文，均应在进程
 * 退出前释放它，否则会产生资源泄漏。
 *
 * \param [in]  pMtp
 *   MTP上下文
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  运行MTP服务.
 *
 * \par    Description
 *     在MTP上下文中启动MTP服务。MTP上下文最多只能运行一个MTP服务。mtp_run()一旦运行，除非发生不可
 * 恢复的情况，它将不会返回。回调函数callback可以控制MTP是否终止运行。mtp_run()所使用的MTP上下文，必
 * 须是经过mtp_init()初始化的，否则它将返回错误值。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  tunnel
 *   tunnel设备名称
 * \param [in]  drm
 *   drm设备名称
 * \param [in]  vram
 *   vram设备名称
 * \param [in]  callback
 *   回调函数
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  停止正在运行的MTP服务。
 *
 * \par    Description
 *     mtp_stop()用于停止一个经由mtp_run()运行的MTP服务。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  获得MTP服务的运行状态
 *
 * \par    Description
 *     MTP服务在使用mtp_run()启动后，mtp_get_running_status()可以用来查询当前服务所处的运行状态。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \return
 * \li  MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND       等待接收来自于主机的MTP命令
 * \li  MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND       MTP命令处理中
 * \li  MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT       主机向设备发送对象数据中
 * \li  MTP_DAEMON_RUNNING_STATUS_GET_OBJECT        主机从设备获取对象数据中
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief 获得MTP设备model名称.
 *
 * \par Description
 *    MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。model是信息集的一部分，
 *mtp_get_device_model()用于设置设备上报的model名称。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [out]  model
 *   MTP 设备model名称。 model采用UTF-8编码，它是以‘\\0’结束的字符串。
 * \param [in]  size
 *   model缓存区域的长度。
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief 设置MTP设备model名称.
 *
 * \par Description
 *    MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。model是信息集的一部分，
 *mtp_set_device_model()用于设置设备上报的model名称。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  model
 *   MTP 设备model名称。 model采用UTF-8编码，它是以‘\\0’结束的字符串。model字符串的长度有限制，
 *当它转化为UTF16表示的字符串时，其长度应不超过255。
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief 获得MTP设备manufacturer名称.
 *
 * \par Description
 *    MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。manufacturer是信息集的一部分，
 *mtp_get_device_manufacturer()用于获取设备上报的manufacturer名称。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [out]  manufacturer
 *   MTP 设备manufacturer名称。 manufacturer采用UTF-8编码，它是以‘\\0’结束的字符串。
 * \param [out]  size
 *   manufacturer缓存大小。
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  设置MTP设备manufacturer名称.
 *
 * \par Description
 *  MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。Manufacturer是信息集
 *的一部分，mtp_set_device_manufacturer ()用于设置设备上报的manufactuer名称。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  manufacturer
 *   MTP 设备manufacturer名称。 manufacturer采用UTF-8编码，它是以‘\\0’结束的字符串。manufacturer字符串的长度有限制，
 *当它转化为UTF16表示的字符串时，其长度应不超过255。
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief 获得MTP设备版本描述信息.
 *
 * \par Description
 *    MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。设备版本是信息集的一部分，
 *mtp_get_device_version()用于获得设备上报的设备版本信息。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [out]  version
 *   MTP 设备版本描述信息。 version采用UTF-8编码，它是以‘\\0’结束的字符串。
 * \param [in]  size
 *   version缓存区域的长度。
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief 设置MTP设备版本描述信息.
 *
 * \par Description
 *    MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。设备版本描述信息是信息集的一部分，
 *mtp_set_device_version()用于设置设备上报的设备版本描述信息。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  version
 *   MTP 设备版本描述信息。version采用UTF-8编码，它是以‘\\0’结束的字符串。version字符串的长度有限制，
 *当它转化为UTF16表示的字符串时，其长度应不超过255。
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief 获得MTP设备manufacturer名称.
 *
 * \par Description
 *    MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。serial是信息集的一部分，
 *mtp_get_device_serial()用于获取设备上报的serial名称。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [out]  serial
 *   MTP 设备serial名称。 serial采用UTF-8编码，它是以‘\\0’结束的字符串。
 * \param [out]  size
 *   serial缓存大小。
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  设置MTP设备的序列号
 *
 * \par Description
 *   MTP协议规定主机可以通过GetDeviceInfo操作获取MTP设备信息集。Serial是信息集
 * 的一部分，mtp_set_device_manufacturer ()用于设置设备上报的serial名称。
 * \param [in] pMtp
 *   MTP上下文
 * \param [in]  serial
 *   MTP 设备serial名称.
 *   serial采用UTF-8编码，它是以‘\\0’结束的字符串。serial字符串的长度有限制，不包括‘\\0’字符在内，
 *最长32个字符。
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  读取存储区的名称
 *
 * \par Description
 *  主机可以通过GetStorageInfo命令获取存储区的名称，小机上报的存储区名称可以利用mtp_get_store_name()
 *获得。提供的缓存空间应当足以放置下获取的名称。名称是‘\\0’字符结尾的UTF－8编码字符串。
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  storeID
 *   存储区域ID
 * \param [out] name
 *   名字缓存区域
 * \param [in]  size
 *   缓存区域大小
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  设置存储区名称
 * \par Description
 *      主机可以通过GetStorageInfo命令获取存储区的名称，小机上报的存储区名称可以利用
 *mtp_set_store_name()设置。name字符串应当是以‘\\0’字符结尾的UTF－8编码字符串，加
 *上结尾的’\\0’字符，当它转化为UTF16表示的字符串时候，长度不应该超过255个字符。
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]   storeID
 *   存储区编号
 * \param [in]   name
 *   存储区名称
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获得存储区的根目录路径 .
 *
 * \par Description
 *   每一个存储区在文件系统上都有唯一根目录来对应它，主机放置于存储区的对象数据均放置于对应的目录中。
 *mtp_get_store_root_path()用于获取MTP下存储区的根目录路径。name将用于放置所获取的存储区名称，
 *如果缓存无法放下存储区的根路径，那么函数将返回一个错误值。*
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  storeID
 *   存储区编号
 * \param [out] name
 *  根目录名称缓存
 * \param [in] size
 *  名称缓存区的大小
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获得存储区的根目录路径 .
 *
 * \par Description
 *   每一个存储区在文件系统上都有唯一根目录来对应它，主机放置于存储区的对象数据均放置于对应的目录中。
 * mtp_set_store_root_path()用于设置MTP下存储区的根目录路径。path字符串应当是以‘\\0’字符结尾的UTF－8
 * 编码字符串，加上结尾的’\\0’字符，当它转化为UTF16表示的字符串时候，长度不应该超过255个字符。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  storeID
 *   存储区编号
 * \param [out] path
 *  根目录名称
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获得存储区对应的设备路径 .
 *
 * \par Description
 *   文件系统挂接在存储设备之上。此接口用于获取存储区对应的存储设备路径。dev用于放置所获取的设备路径，
 *如果size指示的缓存太小，函数将返回一个错误值。
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  storeID
 *   存储区编号
 * \param [out] dev
 *  存储设备名称缓存
 * \param [in] size
 *  名称缓存的大小
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  设置存储区所在的设备路径 .
 *
 * \par Description
 *   文件系统挂接在存储设备之上。此接口用于设置存储区对应的存储设备路径。path字符串应当是以‘\\0’字符结尾的UTF－8
 * 编码字符串，加上结尾的’\\0’字符。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  storeID
 *   存储区编号
 * \param [in] dev
 *  存储设备名称
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  创建一个对象
 *
 * \par Description
 *  创建一个MTP对象。如果MTP对象映射到文件系统上的文件或者目录，那么使用者应当确保文件或者目录确实存在。
 *未在参数中出现的对象属性，可以通过mtp_set_object_prop_value()进行设置。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  storageID
 *   对象所在存储区编号
 * \param [in] format
 *   对象格式
 * \param [in] protection
 *   数据对象的保护状态
 * \param [in] size
 *   对象大小
 * \param [in] thumbFormat
 *   缩略图格式
 * \param [in] thumbCompressedSize
 *   缩略图大小
 * \param [in] thumbPixWidth
 *   缩略图宽度
 * \param [in] thumbPixHeight
 *   缩略图高度
 * \param [in] imagePixWidth
 *   图片宽度（以象素计）
 * \param [in] imagePixHeight
 *   图片高度（以象素计）
 * \param [in] imageBitDepth
 *   图片深度（以象素计）
 * \param [in] parent
 *   父对象句柄
 * \param [in] associateType
 *   关联类型
 * \param [in] associateDesc
 *   关联描述
 * \param [in] filename
 *   名称.
 * \param [in] dateCreated
 *   创建日期
 * \param [in] dateModified
 *   最近修改日期
 * \param [in] keywords
 *   关键字
 * \param [out] pHandle
 *   对象句柄
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  删除一个对象 .
 *
 * \par Description
 *   利用mtp_create_object()创建的对象，可以利用mtp_delete_object()删除。
 * \param [in]  pDev
 *   MTP上下文
 * \param [in]  handle
 *   对象句柄
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获得存储区域上的对象数目
 *
 * \par Description
 *  获得存储区域上的对象数目。
 *
 * \param [in]  pDev
 *       MTP上下文
 * \param [in] storeID
 *      存储区域编号
 * \param [out] pNum
 *      对象数目缓存地址
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
 * \brief  获得存储区域上的对象句柄
 *
 * \par Description
 *  存储区域上对象数目可能比较多，这时可以指定偏移量，从指定位置offset出取num个对象句柄。
 *
 * \param[in]  pDev
 *       MTP上下文
 * \param[in] storeID
 *       存储区域编号
 * \param[in] offset
 *       偏移量
 * \param[in] pHandles
 *       句柄缓存空间
 * \param[in] num
 *       句柄数目
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
 * \brief 增加对象的引用.
 *
 * \par Description
 *  mtp_add_object_reference ()用于增加一个父对象到子对象的引用。该引用将放置于父对象原有引用的最后，
 *它并不辨识先前父对象是否已经有子对象的引用。如果父对象或者子对象不存在，函数返回错误值。*pSeq用于
 *放置引用序号，该序号用于区分父对象到同一子对象的多次引用。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  parent
 *    引用者的句柄
 * \param [in]  child
 *    被引用者的句柄
 * \param [out]  pSeq
 *    引用序号
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  删除对象的引用.
 *
 * \par Description
 *  mtp_delete_object_reference ()用于删除一个父对象到子对象的引用。seq用于区分父对象到同一子对象有
 *多个引用的情形。如果父对象或者子对象不存在，或者在指定序号的位置处并没有期望的引用，那么函数返回
 *错误值。删除指定序号处的引用并不影响父对象的其它引用及其序号
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  parent
 *   引用者的句柄
 * \param [in]  child
 *   被引用者的句柄
 * \param [in]  seq
 *   引用序号
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获取对象引用数目.
 *
 * \par Description
 *  获取对象引用数目。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  parent
 *   父对象句柄
 * \param [out]  pNum
 *   对象引用数目.
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获取对象引用数目.
 *
 * \par Description
 *  获取对象引用数目。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  parent
 *   父对象句柄
 * \param [out]  pReference
 *   放置对象引用的数组.
 * \param [out]  pSeq
 *   放置对象引用序号的数组
 * \param [in] num
 *   数组大小
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获得对象的路径.
 *
 * \par Description
 *   MTP中的一些对象具有文件系统上的对应物，mtp_get_object_path用于获取对象在文件系统上对应的目录
 *或则文件名称。如果给定句柄的对象不存在，那么函数将返回错误值。*
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  handle
 *   对象句柄
 * \param [out]  path
 *   路径缓存
 * \param [in]  size
 *   缓存大小
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  利用对象的文件路径名搜索对象的句柄.
 *
 * \par Description
 *  如果对象有文件或者目录支撑，则可以使用mtp_search_object_using_path()获得对象句柄。
 *
 * \param[in]  pMtp
 *      MTP 上下文
 * \param[in] path
 *      文件或者目录路径
 * \param[out] pHandle
 *       对象句柄
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  设置对象的属性
 *
 * \par Description
 *      一个MTP对象支持多种属性，mtp_set_object_prop_value()用于设置对象属性值。如果对象不存在，
 *或者对象不支持该属性，则函数返回错误值。
 *
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  handle
 *   对象句柄
 * \param [in]  propCode
 *   属性编码
 * \param [in]  data
 *   属性值
 * \param [in]  size
 *   属性占据的空间大小
 *
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par
 * \par      例程
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
 * \brief  获取对象属性值
 *
 * \par Description
 *       一个MTP对象支持多种属性，mtp_get_object_prop_value()用于获取对象属性值。如果对象不存在，
 *或者对象不支持该属性，则函数返回错误值。
 * \param [in]  pMtp
 *   MTP上下文
 * \param [in]  handle
 *   对象句柄
 * \param [in]  propCode
 *   对象属性编码
 * \param [out]  data
 *   属性值缓存
 * \param [in]  size
 *   属性值缓存空间大小
 *
 * \return
 * \li  0        Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  获取当前数据库更改了的OBJECT的个数。
 *
 * \par Description
 *  获取当前数据库更改了的OBJECT的个数，保存在pObjCnt所指向的数据里头。
 *
 * \param[in]  pDev
 *      MTP 上下文
 * \param[out] pObjCnt
 *      被更改的数据库记录次数
 *
 * \return
 * \li  0      Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  获取本次MTP同步所有的改动信息。
 *
 * \par Description
 * \brief  获取本次MTP同步所有的改动信息，包括是否已经格式化过，
 * 有哪些Object ID是新增的，哪些是被删除的，哪些是修改过的�
 *
 * \param[in]  pDev
 *      MTP 上下文
 * \param[out] pInf
 *      结构体指针，指向记录被更改的信息的结构体。
 *
 * \return
 * \li  0      Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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
 * \brief  释放上次获取本次MTP同步所有的改动信息时候所申请的系统内存等资源。
 *
 * \par Description
 * \brief  释放上次获取本次MTP同步所有的改动信息时候所申请的系统内存等资源。
 *
 * \param[in]  pDev
 *      MTP 上下文
 * \param[out] pInf
 *      结构体指针，指向记录被更改的信息的结构体。
 *
 * \return
 * \li  0      Success
 * \li <0      Failure
 * \ingroup   mtp
 *
 * \par      例程
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

