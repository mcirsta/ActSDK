/*
 ********************************************************************************
 *                       uSDK130 -- play list
 *                (c) Copyright 2002-2007, Actions Co,Ld.
 *                        All Right Reserved
 *
 * FileName    : playlist.h
 * Description : play list module header file
 * Others      :
 * History     :
 *     <author>                     <date>                      <version>              <desc>
 *    fengyuanhua           17-09-2008                  1.0                       build the file
 ********************************************************************************
 */

/*!
 * \file  playlist.h
 * \brief
 * \author fengyuanhua
 * \par GENERAL DESCRIPTION:
 *           本模块实现了播放列表各个接口的功能。.
 * \par DECLARATIONS:
 *       (c) Copyright 2002-2007, Actions Co,Ld. All Right Reserved.
 *
 *  \version 1.0
 *  \date  17-09-2008
 */

#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

// only for debug.
// only for debug.
// only for debug.
// only for debug.
// only for debug.
// only for debug.

//#ifndef FSEL_HANDLE
////typedef void* FSEL_HANDLE;
//#endif

// only for debug.
// only for debug.
// only for debug.
// only for debug.
// only for debug.


/*
 ********************************************************************************
 *                         includes
 ********************************************************************************
 */
#include <types.h>
#include "fileselector.h"
//#include <libc/types.h>

/*
 ********************************************************************************
 *                         macros
 ********************************************************************************
 */

/*列表名称最大长度*/
#define MAXSTRLEN   200

/*最大可用字段数目*/
#define MAXFIELD     64

/*!
 *\brief 播放列表的类型
 *\li  MEDIALIB  媒体库列表
 *\li    //mtp列表类型不能创建，只能使用。
 MUSICMTP, //mtp列表
 VIDEOMTP,
 IMGMTP,
 TXTMTP,

 *\li  10音乐保留类型
 MUSICRESERVE1,
 MUSICRESERVE2,
 MUSICRESERVE3,
 MUSICRESERVE4,
 MUSICRESERVE5,
 MUSICRESERVE6,
 MUSICRESERVE7,
 MUSICRESERVE8,
 MUSICRESERVE9,
 MUSICRESERVE10,

 *\li  10个视频保留类型
 VIDEORESERVE1,
 VIDEORESERVE2,
 VIDEORESERVE3,
 VIDEORESERVE4,
 VIDEORESERVE5,
 VIDEORESERVE6,
 VIDEORESERVE7,
 VIDEORESERVE8,
 VIDEORESERVE9,
 VIDEORESERVE10,

 *\li 10个图片保留类型
 IMGRESERVE1,
 IMGRESERVE2,
 IMGRESERVE3,
 IMGRESERVE4,
 IMGRESERVE5,
 IMGRESERVE6,
 IMGRESERVE7,
 IMGRESERVE8,
 IMGRESERVE9,
 IMGRESERVE10,

 *\li 10个电子书保留类型
 TXTRESERVE1,
 TXTRESERVE2,
 TXTRESERVE3,
 TXTRESERVE4,
 TXTRESERVE5,
 TXTRESERVE6,
 TXTRESERVE7,
 TXTRESERVE8,
 TXTRESERVE9,
 TXTRESERVE10,

 *\li 10 个保留类型RESERVETYPE1对应的列表类型
 TYPE1REVER1,
 TYPE1REVER2,
 TYPE1REVER3,
 TYPE1REVER4,
 TYPE1REVER5,
 TYPE1REVER6,
 TYPE1REVER7,
 TYPE1REVER8,
 TYPE1REVER9,
 TYPE1REVER10,


 *\li 10个保留类型RESERVETYPE2对应的列表类型
 TYPE2REVER1,
 TYPE2REVER2,
 TYPE2REVER3,
 TYPE2REVER4,
 TYPE2REVER5,
 TYPE2REVER6,
 TYPE2REVER7,
 TYPE2REVER8,
 TYPE2REVER9,
 TYPE2REVER10
 */
typedef enum
{
    MEDIALIB = 1,
    MUSICMTP = 2,
    VIDEOMTP = 4,
    IMGMTP = 8,
    TXTMTP = 16,
    RESERVEMTP = 32,

    MUSICRESERVE1 = 1000,
    MUSICRESERVE2,
    MUSICRESERVE3,
    MUSICRESERVE4,
    MUSICRESERVE5,
    MUSICRESERVE6,
    MUSICRESERVE7,
    MUSICRESERVE8,
    MUSICRESERVE9,
    MUSICRESERVE10,

    VIDEORESERVE1,
    VIDEORESERVE2,
    VIDEORESERVE3,
    VIDEORESERVE4,
    VIDEORESERVE5,
    VIDEORESERVE6,
    VIDEORESERVE7,
    VIDEORESERVE8,
    VIDEORESERVE9,
    VIDEORESERVE10,

    IMGRESERVE1,
    IMGRESERVE2,
    IMGRESERVE3,
    IMGRESERVE4,
    IMGRESERVE5,
    IMGRESERVE6,
    IMGRESERVE7,
    IMGRESERVE8,
    IMGRESERVE9,
    IMGRESERVE10,

    TXTRESERVE1,
    TXTRESERVE2,
    TXTRESERVE3,
    TXTRESERVE4,
    TXTRESERVE5,
    TXTRESERVE6,
    TXTRESERVE7,
    TXTRESERVE8,
    TXTRESERVE9,
    TXTRESERVE10,

    TYPE1RESERVE1,
    TYPE1RESERVE2,
    TYPE1RESERVE3,
    TYPE1RESERVE4,
    TYPE1RESERVE5,
    TYPE1RESERVE6,
    TYPE1RESERVE7,
    TYPE1RESERVE8,
    TYPE1RESERVE9,
    TYPE1RESERVE10,

    TYPE2RESERVE1,
    TYPE2RESERVE2,
    TYPE2RESERVE3,
    TYPE2RESERVE4,
    TYPE2RESERVE5,
    TYPE2RESERVE6,
    TYPE2RESERVE7,
    TYPE2RESERVE8,
    TYPE2RESERVE9,
    TYPE2RESERVE10
} plist_type_t;

/*!
 *\brief 文件类型定义
 *\li  MUSIC  音乐
 *\li   VIDEO  视频
 *\li   IMAGE  图片
 *\li   TXT  电子书
 *\li   RESERVETYPE1 保留类型.可以用作一些不属于上面四类的文件。如有声读物。
 *\li   RESERVETYPE2 保留类型.
 */
typedef enum
{
    NOTYPE = 0,
    MUSIC = 1,
    VIDEO = 2,
    IMAGE = 4,
    TXT = 8,
    RESERVETYPE1 = 16,
    RESERVETYPE2 = 32
} plist_file_type_t;

/*!
 *\brief 类型-后缀名 映射结构。
 */
typedef struct
{
    /*!  file_type  文件类型*/
    plist_file_type_t file_type;

    /*! ext_name  file_type对应的文件名后缀。*/
    char ext_name[MAXSTRLEN];
    int ext_format[50];
} plist_map_type_ext_t;

/*!
 *\brief  列表文件名信息。

 */
typedef struct
{
    /*!  list_name  播放列表名称*/
    char list_name[MAXSTRLEN];
} plist_name_t;

/*!
 *\brief 播放列表信息结构体
 */
typedef struct
{
    /*!list_name  播放列表名称*/
    char list_name[MAXSTRLEN];

    /*!file_num   列表中的文件总数*/
    int file_num;

    /*! list_type   列表类型*/
    plist_type_t list_type;

    /*! is_mtp_list  是否是mtp列表*/
    int is_mtp_list;
} plist_listinfo_t;

/*!
 *\brief
 * 排序方式
 *\li  NOSORT  不排序
 *\li  PINYINSORT   拼音排序
 *\li  BINARYSORT  二进制排序
 */
typedef enum
{
    NOSORT,
    PINYINSORT,
    BINARYSORT
} plist_sortstyle_t;

/*!
 *\brief 专辑图片信息
 */
typedef struct
{
    /*!offset  内嵌专辑图片在文件中的偏移*/
    int offset;

    /*!  len   内嵌专辑图片在文件中的数据长度*/
    int len;

    /*!  exist_folderjpg 同路径下是否有folder.jpg图片*/
    int exist_folderjpg;

    /*! mtp_img_path  记录mtp下载专辑图片的位置文件全路径，为NULL则无图片。  */
    char* mtp_img_path;
} plist_album_img_t;

/*!
 *\brief 标签信息结构体
 */
typedef struct
{
    /*! time  标签时间点，单位是ms。*/
    unsigned int time;

    /*! mark_param_len   标签数据长度. 长度为0表示没有标签信息.*/
    unsigned int mark_param_len;

    /*!  mark_param 标签播放的数据参数*/
    unsigned char* mark_param;
} plist_mark_info_t;

/*!
 *\brief 标签信息结构体
 * \li  BITRATE:  比特率
 * \li  RATING :星级
 * \li  DURATION : 播放总时间
 * \li  CHANNEL : 声道
 * \li  USECOUNT  :已播放次数
 * \li  SAMPLERATE :采样率
 * \li  FRAMEPS  :帧率(帧/每秒)
 * \li  WIDTH  :画面宽
 * \li  HEIGHT  :画面高
 * \li  FRAMES  :总帧数
 * \li  FILESIZE  :文件大小
 * \li  TITLE  : 标题
 * \li  ARTIST : 歌唱家
 * \li  ALBUM  :专辑
 * \li  AGE  :年代
 * \li  GENRE  :流派
 * \li  TRACK  :音轨
 * \li  FULLPATH  :全路径
 * \li  IMAGEINFO  :专辑图片信息
 * \li  MARK  : 标签信息
 下面三个字段只可以用在查询条件中
 * \li  DIRECTORY : 提取指定文件夹下的文件
 * \li  EXT  :指定后缀名过滤文件
 * \li  DEVICE  :  设备类型
 */
#if 0
typedef enum
{
    BITRATE,RATING,DURATION,CHANNEL,
    USECOUNT,SAMPLERATE,FRAMEPS,WIDTH,
    HEIGHT,FRAMES,FILESIZE,

    TITLE,ARTIST, ALBUM, AGE,
    GENRE,TRACK, FULLPATH,

    IMAGEINFO,MARK,

    DIRECTORY,
    EXT,
    DEVICE
}plist_detail_mask_t;

#endif

typedef enum
{
    RATING,
    CHANNEL,
    TITLE,
    ARTIST,
    ALBUM,
    AGE,
    GENRE,
    TRACK/*add by xgk 20090807*/,
    FULLPATH,

    IMAGEINFO,
    MARK,
    FILENAME,
    DIRECTORY,
    EXT,
    DEVICE
} plist_detail_mask_t;

#if  0
/*!
 *\brief 标签信息结构体.
 */
typedef struct
{
    /*!  bitrate  文件比特率*/
    int bitrate;

    /*!   rating 星级*/
    int rating;

    /*!  duration  播放时长，以ms为单位.*/
    int duration;

    /*!   channels 声道*/
    int channels;

    /*!   usecount  播放次数*/
    int usecount;

    /*!  samplerate  采样率*/
    int samplerate;

    /*!   frame_1000second  1000倍帧率*/
    int frame_1000second;

    /*!   img_width  画面宽度 */
    int img_width;

    /*!   img_height  画面高度*/
    int img_height;

    /*!   frame_num  总帧数*/
    int frame_num;

    /*!  file_size  文件大小，以k字节为单位 */
    int file_size;

    /*!   title   标题    */
    char* title;

    /*!   artist  歌唱者、演说者等*/
    char* artist;

    /*!   album  专辑 */
    char* album;

    /*!  age   年代*/
    char* age;

    /*!   genre  流派*/
    char* genre;

    /*!   track   音轨 */
    char* track;

    /*! 文件全路径*/
    char* fullpath;
    /*! 文件全*/
    char* filename;

    /*!   imageinfo   专辑图片*/
    plist_album_img_t imageinfo;

    /*!   markinfo    标签信息*/
    plist_mark_info_t markinfo;
}plist_detail_info_t;

#endif

typedef struct
{
    /*!   rating 星级*/
    int rating;

    /*!   channels 声道*/
    int channels;

    /*!   title   标题    */
    char* title;

    /*!   artist  歌唱者、演说者等*/
    char* artist;

    /*!   album  专辑 */
    char* album;

    /*!  age   年代*/
    char* age;

    /*!   genre  流派*/
    char* genre;

    /*!   track   音轨 */
    char* track; //add by xgk 20090807

    /*! 文件全路径*/
    char* fullpath;

    /*! 文件名*/
    char* filename;

    /*!   imageinfo   专辑图片*/
    plist_album_img_t imageinfo;

    /*!   markinfo    标签信息*/
    plist_mark_info_t markinfo;
} plist_detail_info_t;

/*!
 *\brief 查询时的提取字段描述和排序字段描述。
 */
typedef struct
{
    /*! nfield  字段掩码*/
    plist_detail_mask_t nfield[MAXFIELD];

    /*! num   要提取的字段数目*/
    int num;
} plist_trivefield_t, plist_modifyfield_t;

/*!
 *\brief 查询时的提取字段描述和排序字段描述。
 */
typedef struct
{
    /*!  nfield  字段掩码*/
    plist_detail_mask_t nfield[MAXFIELD];
    /*!  nstyle   排序方式*/
    plist_sortstyle_t nstyle[MAXFIELD];

    /*排序方向，升降序*/
    plist_sortstyle_t nsortdirection[MAXFIELD];

    /*! num   要提取的字段数目*/
    int num;
} plist_sortfield_t, plist_groupfield_t;

/*!
 *\brief  表示字段值的联合体。
 */
typedef union
{
    /*!  pval  此值是一个指针。当字段值是字符串时，使用这个域。*/
    char* pval;
    /*!  nval  此值是一个整数。当字段值是整数时，使用这个域。*/
    int nval;
} plist_field_val;

/*!
 *\brief  描述字段属性的结构体。
 */
typedef struct
{
    /*!  content  字段内容。*/
    plist_field_val content;
    /*!  fieldno  字段编号。*/
    plist_detail_mask_t fieldno;
} plist_field_t;

/*!
 *\brief 查询时的提取字段描述和排序字段描述。
 */
typedef struct
{
    /*!  filter_field  要查询的字段掩码*/
    plist_field_t filter_field[MAXFIELD];
    /*! field_num   要查询的字段数目。*/
    int field_num;
    /*! is_pattern_search   是否有模糊查询属性，如果有，最后一个Field_t作为模糊查询条件。*/
    int is_pattern_search;
} plist_filterfield_t;

/*!
 *\brief  文件所在设备类型。
 *\li  SYSTEMDISK  系统自带flash.
 *\li  CARD  外部卡设备。
 *\li  MTPFILE 通过mtp下载的文件.
 */
typedef enum
{
    SYSTEMDISK = 0x10001,
    CARD = 0x20001,
    MTPFILE = 0x30001
} plist_device_t;

typedef enum
{
    SYNC_TO_MTP,
    SYNC_FROM_MTP
} plist_sync_type_t;

extern int errno;

//函数列表
int plist_init(void);
int plist_quit(void);
void* plist_open(const char*, plist_type_t, plist_trivefield_t *);
void plist_close(void* hplist);
int plist_clear(void* hplist);
int plist_delete_by_handle(void* hplist);
int plist_delete_by_type(int list_type);
int plist_delete_by_tblname(const char* table_name);
int plist_sync_mtp(plist_sync_type_t sync_type, const char* rootdir);
int plist_get_listnum_by_type(plist_type_t type);
int plist_get_listname_by_type(plist_type_t, plist_name_t*, int, int);
int plist_get_listinfo_by_handle(void* hplist, plist_listinfo_t* buf);
int plist_verify_list(void* hplist);
int plist_modify_name(void* hplist, const char* new_name);
int plist_add_query(void* srchplist, void* desthplist,
        plist_filterfield_t * pfilter_field, plist_sortfield_t*,
        plist_groupfield_t* pgroup_field);
int plist_sort_items(void* hplist, void* hpresult,
        plist_sortfield_t* sort_field);
int plist_get_items_to_mallocbuf(void* hplist,
        plist_detail_info_t* xx_detail_info, int start, int num, int reverse);
void plist_free_buf(plist_detail_info_t* xx_detail_info, int num);
int plist_get_item_num(void* hplist);

//有条件的取记录总数 by xgk 20091029
int plist_get_item_num_filter(void* hplist, plist_filterfield_t* pFilter);
int plist_get_items_to_mallocbuf_filter(void* hplist,
        plist_filterfield_t* pFilter, plist_detail_info_t* xx_detail_info,
        int start, int num, int reverse);

int plist_del_items(void* hplist, int start, int num);
int plist_filter_del_item(void* hplist, plist_filterfield_t* filter_field);
int plist_add_file(void* hplist, const char* file_path);

int plist_add_dir(void* hplist, const char* dir_path);
int plist_add_dirlist(void* hplist, FSEL_HANDLE hdir, int);
int plist_add_item(void* hplist, plist_detail_info_t* xx_detail_info);
int plist_transfer_items(void* srchplist, void* desthplist, int nSortField,
        int start, int num);
int plist_modify_item(void* hplist, int index, plist_detail_info_t * field_buf,
        plist_modifyfield_t * modify_field);
int plist_name_to_index(void* hplist, const char* filepath, int start);
int plist_get_ID3_by_index(void* hplist, plist_detail_info_t* xx_detail_info,
        int index);
int plist_refresh_ID3(const char* dir);
int plist_set_filetype_ext(plist_file_type_t filetype, const char* ext);
int plist_get_change_sign(void* hplist);
void plist_init_detail_info_struct(plist_detail_info_t*, int);
void plist_set_rootdir(const char*);
int plist_generate_medialib(const char *dir, plist_device_t device,
        plist_file_type_t file_type, int(*func)(int));
int plist_table_exist(const char * pTableName); //查看表是否存在
int plist_set_table_invalid(void* hplist); //设置表状态为无效
int plist_create_index(const char *pTbName, plist_filterfield_t * pIdxField,
        int nCollate); //创建索引
int plist_erase_invalid_data(const char * pTbname, const char * pInvaliddir); //删除应用的某个表不需要的数据
int plist_get_ID3_by_name(const char* filepath, plist_detail_info_t* pinfo_buf);

#endif

