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
 *           ��ģ��ʵ���˲����б�����ӿڵĹ��ܡ�.
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

/*�б�������󳤶�*/
#define MAXSTRLEN   200

/*�������ֶ���Ŀ*/
#define MAXFIELD     64

/*!
 *\brief �����б������
 *\li  MEDIALIB  ý����б�
 *\li    //mtp�б����Ͳ��ܴ�����ֻ��ʹ�á�
 MUSICMTP, //mtp�б�
 VIDEOMTP,
 IMGMTP,
 TXTMTP,

 *\li  10���ֱ�������
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

 *\li  10����Ƶ��������
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

 *\li 10��ͼƬ��������
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

 *\li 10�������鱣������
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

 *\li 10 ����������RESERVETYPE1��Ӧ���б�����
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


 *\li 10����������RESERVETYPE2��Ӧ���б�����
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
 *\brief �ļ����Ͷ���
 *\li  MUSIC  ����
 *\li   VIDEO  ��Ƶ
 *\li   IMAGE  ͼƬ
 *\li   TXT  ������
 *\li   RESERVETYPE1 ��������.��������һЩ����������������ļ������������
 *\li   RESERVETYPE2 ��������.
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
 *\brief ����-��׺�� ӳ��ṹ��
 */
typedef struct
{
    /*!  file_type  �ļ�����*/
    plist_file_type_t file_type;

    /*! ext_name  file_type��Ӧ���ļ�����׺��*/
    char ext_name[MAXSTRLEN];
    int ext_format[50];
} plist_map_type_ext_t;

/*!
 *\brief  �б��ļ�����Ϣ��

 */
typedef struct
{
    /*!  list_name  �����б�����*/
    char list_name[MAXSTRLEN];
} plist_name_t;

/*!
 *\brief �����б���Ϣ�ṹ��
 */
typedef struct
{
    /*!list_name  �����б�����*/
    char list_name[MAXSTRLEN];

    /*!file_num   �б��е��ļ�����*/
    int file_num;

    /*! list_type   �б�����*/
    plist_type_t list_type;

    /*! is_mtp_list  �Ƿ���mtp�б�*/
    int is_mtp_list;
} plist_listinfo_t;

/*!
 *\brief
 * ����ʽ
 *\li  NOSORT  ������
 *\li  PINYINSORT   ƴ������
 *\li  BINARYSORT  ����������
 */
typedef enum
{
    NOSORT,
    PINYINSORT,
    BINARYSORT
} plist_sortstyle_t;

/*!
 *\brief ר��ͼƬ��Ϣ
 */
typedef struct
{
    /*!offset  ��Ƕר��ͼƬ���ļ��е�ƫ��*/
    int offset;

    /*!  len   ��Ƕר��ͼƬ���ļ��е����ݳ���*/
    int len;

    /*!  exist_folderjpg ͬ·�����Ƿ���folder.jpgͼƬ*/
    int exist_folderjpg;

    /*! mtp_img_path  ��¼mtp����ר��ͼƬ��λ���ļ�ȫ·����ΪNULL����ͼƬ��  */
    char* mtp_img_path;
} plist_album_img_t;

/*!
 *\brief ��ǩ��Ϣ�ṹ��
 */
typedef struct
{
    /*! time  ��ǩʱ��㣬��λ��ms��*/
    unsigned int time;

    /*! mark_param_len   ��ǩ���ݳ���. ����Ϊ0��ʾû�б�ǩ��Ϣ.*/
    unsigned int mark_param_len;

    /*!  mark_param ��ǩ���ŵ����ݲ���*/
    unsigned char* mark_param;
} plist_mark_info_t;

/*!
 *\brief ��ǩ��Ϣ�ṹ��
 * \li  BITRATE:  ������
 * \li  RATING :�Ǽ�
 * \li  DURATION : ������ʱ��
 * \li  CHANNEL : ����
 * \li  USECOUNT  :�Ѳ��Ŵ���
 * \li  SAMPLERATE :������
 * \li  FRAMEPS  :֡��(֡/ÿ��)
 * \li  WIDTH  :�����
 * \li  HEIGHT  :�����
 * \li  FRAMES  :��֡��
 * \li  FILESIZE  :�ļ���С
 * \li  TITLE  : ����
 * \li  ARTIST : �質��
 * \li  ALBUM  :ר��
 * \li  AGE  :���
 * \li  GENRE  :����
 * \li  TRACK  :����
 * \li  FULLPATH  :ȫ·��
 * \li  IMAGEINFO  :ר��ͼƬ��Ϣ
 * \li  MARK  : ��ǩ��Ϣ
 ���������ֶ�ֻ�������ڲ�ѯ������
 * \li  DIRECTORY : ��ȡָ���ļ����µ��ļ�
 * \li  EXT  :ָ����׺�������ļ�
 * \li  DEVICE  :  �豸����
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
 *\brief ��ǩ��Ϣ�ṹ��.
 */
typedef struct
{
    /*!  bitrate  �ļ�������*/
    int bitrate;

    /*!   rating �Ǽ�*/
    int rating;

    /*!  duration  ����ʱ������msΪ��λ.*/
    int duration;

    /*!   channels ����*/
    int channels;

    /*!   usecount  ���Ŵ���*/
    int usecount;

    /*!  samplerate  ������*/
    int samplerate;

    /*!   frame_1000second  1000��֡��*/
    int frame_1000second;

    /*!   img_width  ������ */
    int img_width;

    /*!   img_height  ����߶�*/
    int img_height;

    /*!   frame_num  ��֡��*/
    int frame_num;

    /*!  file_size  �ļ���С����k�ֽ�Ϊ��λ */
    int file_size;

    /*!   title   ����    */
    char* title;

    /*!   artist  �質�ߡ���˵�ߵ�*/
    char* artist;

    /*!   album  ר�� */
    char* album;

    /*!  age   ���*/
    char* age;

    /*!   genre  ����*/
    char* genre;

    /*!   track   ���� */
    char* track;

    /*! �ļ�ȫ·��*/
    char* fullpath;
    /*! �ļ�ȫ*/
    char* filename;

    /*!   imageinfo   ר��ͼƬ*/
    plist_album_img_t imageinfo;

    /*!   markinfo    ��ǩ��Ϣ*/
    plist_mark_info_t markinfo;
}plist_detail_info_t;

#endif

typedef struct
{
    /*!   rating �Ǽ�*/
    int rating;

    /*!   channels ����*/
    int channels;

    /*!   title   ����    */
    char* title;

    /*!   artist  �質�ߡ���˵�ߵ�*/
    char* artist;

    /*!   album  ר�� */
    char* album;

    /*!  age   ���*/
    char* age;

    /*!   genre  ����*/
    char* genre;

    /*!   track   ���� */
    char* track; //add by xgk 20090807

    /*! �ļ�ȫ·��*/
    char* fullpath;

    /*! �ļ���*/
    char* filename;

    /*!   imageinfo   ר��ͼƬ*/
    plist_album_img_t imageinfo;

    /*!   markinfo    ��ǩ��Ϣ*/
    plist_mark_info_t markinfo;
} plist_detail_info_t;

/*!
 *\brief ��ѯʱ����ȡ�ֶ������������ֶ�������
 */
typedef struct
{
    /*! nfield  �ֶ�����*/
    plist_detail_mask_t nfield[MAXFIELD];

    /*! num   Ҫ��ȡ���ֶ���Ŀ*/
    int num;
} plist_trivefield_t, plist_modifyfield_t;

/*!
 *\brief ��ѯʱ����ȡ�ֶ������������ֶ�������
 */
typedef struct
{
    /*!  nfield  �ֶ�����*/
    plist_detail_mask_t nfield[MAXFIELD];
    /*!  nstyle   ����ʽ*/
    plist_sortstyle_t nstyle[MAXFIELD];

    /*������������*/
    plist_sortstyle_t nsortdirection[MAXFIELD];

    /*! num   Ҫ��ȡ���ֶ���Ŀ*/
    int num;
} plist_sortfield_t, plist_groupfield_t;

/*!
 *\brief  ��ʾ�ֶ�ֵ�������塣
 */
typedef union
{
    /*!  pval  ��ֵ��һ��ָ�롣���ֶ�ֵ���ַ���ʱ��ʹ�������*/
    char* pval;
    /*!  nval  ��ֵ��һ�����������ֶ�ֵ������ʱ��ʹ�������*/
    int nval;
} plist_field_val;

/*!
 *\brief  �����ֶ����ԵĽṹ�塣
 */
typedef struct
{
    /*!  content  �ֶ����ݡ�*/
    plist_field_val content;
    /*!  fieldno  �ֶα�š�*/
    plist_detail_mask_t fieldno;
} plist_field_t;

/*!
 *\brief ��ѯʱ����ȡ�ֶ������������ֶ�������
 */
typedef struct
{
    /*!  filter_field  Ҫ��ѯ���ֶ�����*/
    plist_field_t filter_field[MAXFIELD];
    /*! field_num   Ҫ��ѯ���ֶ���Ŀ��*/
    int field_num;
    /*! is_pattern_search   �Ƿ���ģ����ѯ���ԣ�����У����һ��Field_t��Ϊģ����ѯ������*/
    int is_pattern_search;
} plist_filterfield_t;

/*!
 *\brief  �ļ������豸���͡�
 *\li  SYSTEMDISK  ϵͳ�Դ�flash.
 *\li  CARD  �ⲿ���豸��
 *\li  MTPFILE ͨ��mtp���ص��ļ�.
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

//�����б�
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

//��������ȡ��¼���� by xgk 20091029
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
int plist_table_exist(const char * pTableName); //�鿴���Ƿ����
int plist_set_table_invalid(void* hplist); //���ñ�״̬Ϊ��Ч
int plist_create_index(const char *pTbName, plist_filterfield_t * pIdxField,
        int nCollate); //��������
int plist_erase_invalid_data(const char * pTbname, const char * pInvaliddir); //ɾ��Ӧ�õ�ĳ������Ҫ������
int plist_get_ID3_by_name(const char* filepath, plist_detail_info_t* pinfo_buf);

#endif

