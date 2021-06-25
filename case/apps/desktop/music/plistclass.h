/********************************************************************************
 *                              USDK(1100)
 *                             Module: plistclass 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : plistclass.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-05-15          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     plistclass.h
* \brief    playlist class
* \author   songzhining
*               
* \version 1.0
* \date  2009/05/15
*******************************************************************************/

#ifndef __PLISTCLASS_H__
#define __PLISTCLASS_H__

//##########################//
//    INCLUDE FILES     
//##########################//

#include "include_case.h"



//##########################//
//    MACRO DEFINE    
//##########################// 

/*�����ü���Ҫ����ʾ�б�*/
#define NONE_LIST               -1
 
//##########################//
//    DATASTRUCT DESCRIPTION
//    OR  VALUEABLE DEFINE     
//##########################//
 
/*�б�ö�ٱ���*/
typedef enum
{
	PLIST_NOWPLAYING = 0,
	PLIST_SHOWLIST = 1,
	
}plistclass_type_e;
 
/*��һ���б�ö�ٱ���(�����ʾ��8/LEVEL_ONE_MAX���б�)*/
typedef enum
{
    /*for music*/
	LEVEL_ONE_NONE_LIST = NONE_LIST,
	LEVEL_ONE_MIN = 1,
	LEVEL_ONE_ALLMUSIC =LEVEL_ONE_MIN,
	LEVEL_ONE_ARTISTS=2,
	LEVEL_ONE_ALBUMS=3,
	LEVEL_ONE_GENRES=4,
	LEVEL_ONE_MY_PLAYLIST=5,
	LEVEL_ONE_MY_RECORDS=6,
	LEVEL_ONE_MY_RATING=7,
	LEVEL_ONE_MUSICTAGLIST=8,/*music book marklist*/

	LEVEL_ONE_MAX
	
}level_one_e;

/*��2���б�ö�ٱ���(�����ʾ��2/LEVEL_TWO_MAX���б�)*/
typedef enum
{
	LEVEL_TWO_NONE_LIST = NONE_LIST,
	LEVEL_TWO_MIN = 0,
	LEVEL_TWO_LIST = LEVEL_TWO_MIN,
	LEVEL_TWO_VOCREC = LEVEL_TWO_MIN,
	LEVEL_TWO_FMREC,
	LEVEL_TWO_MAX
	
}level_two_e;
 
/*��3���б�ö�ٱ���(�����ʾ��1/LEVEL_THREE_MAX���б�)*/
typedef enum
{
	LEVEL_THREE_NONE_LIST = NONE_LIST,
	LEVEL_THREE_MIN = 0,
	LEVEL_THREE_FILELIST = LEVEL_THREE_MIN,
	LEVEL_THREE_MAX
	
}level_three_e;

/*
�ϲ�֪ͨ��װ����ȡ�б�ľ�����Ϣ�ṹ��
��Ҫ��;��
1)�ϲ�֪ͨ��װ��ϣ����ȡ��һ������һ���б�
2)ϣ���õ����������ʽ
*/

typedef struct
{
	level_one_e level_one;
	level_two_e level_two;
	level_three_e level_three;
	plist_sortstyle_t sortstyle;//����ʽ
    
}plistclass_level_info_t;


/*
�û�ѡ������Ϣ�ṹ��
*/
typedef union 
{
    char itemstr[PATH_MAX];
    int  rating;
  
}plistclass_usrselcontent_u;


/*�б�״̬ö�ٱ���*/
typedef enum
{
	STATE_NONE = 0,
        
    /* level 1 */
	STATE_LEVEL_ONE = 1,
	STATE_ALLMUSIC,
	STATE_ARTIST,
	STATE_ALBUM,
	STATE_GENRE,
	STATE_MYLIST,
	STATE_RECLIST,
	STATE_MYRATING,
	STATE_TAGLIST,
	
    /* level 2 */
	STATE_LEVEL_TWO = 100,
	STATE_ALLMUSIC_FILE,
	STATE_ARTIST_LIST ,
	STATE_ALBUM_LIST ,
	STATE_GENRE_LIST ,
	STATE_MYLIST_LIST ,
	STATE_VOICEREC ,
	STATE_FMREC ,
	STATE_MYRATING_LIST ,
	STATE_TAGLIST_FILE ,
	
    /* level 3 */
	STATE_LEVEL_THREE = 200,
	STATE_ARTIST_FILE,
	STATE_ALBUM_FILE ,
	STATE_GENRE_FILE ,
	STATE_MYLIST_FILE ,
	STATE_VOICEREC_FILE ,
	STATE_FMREC_FILE ,
	STATE_MYRATING_FILE ,
	
	STATE_MAX,
}plistclass_state_e;
 

//##########################//
//    FUNCTIONS DESCRIPTION     
//##########################//
 
/*******************************************************************
Function : plistclass_init
Brief    : ��ʼ����װ��
Input    : 
           1)  plist_file_type_t filetype
           Ӧ������
Output   : bool
           0,      fail
           1,       success
ע��     ��
           1)   filetypeĿǰ�ɶ����У�
            MUSIC ���� 
						VIDEO ��Ƶ 
						IMAGE ͼƬ 
						TXT ������ 
						FLASH(REVERSETYPE1) ��������1(��������FLASH)
						REVERSETYPE2        ��������2 
********************************************************************/
bool plistclass_init(plist_file_type_t filetype);

/*******************************************************************
Function : plistclass_exit
Brief    : ���ٷ�װ��
Input    : 
           1)  plist_file_type_t filetype
           Ӧ������
Output   : bool
           0,      fail
           1,       success
ע��     ��
           1)   filetypeĿǰ�ɶ����У�
            MUSIC ���� 
						VIDEO ��Ƶ 
						IMAGE ͼƬ 
						TXT ������ 
						FLASH(REVERSETYPE1) ��������1(��������FLASH)
						REVERSETYPE2        ��������2 
					 2)   ����ʱ�������������Ӧ�ò��Ǳ���ģ��ڴ˱����ò���
					  ��ϣ��ʵ����ƽ����û���õ���������ģ��ﵽ���������
					  ʹ�øĽӿ�
********************************************************************/
bool plistclass_exit(plist_file_type_t filetype);

/*******************************************************************
Function : plistclass_get_item_num
Brief    : ��ȡָ���б����Ŀ����
Input    : 
           1)  plistclass_type_e type
           Ŀ���б�
Output   : int
           -1,      fail
           other,   ʵ��ͳ�ƻ�õ���Ŀ����
ע��     ��
********************************************************************/
int plistclass_get_item_num(plistclass_type_e type);

/*******************************************************************
Function : plistclass_get_album_file_num
Brief    : ��ȡָ��album�ļ�����
Input    : 
           1)  album
           ר������
Output   : int
           -1,      fail
           other,   ʵ��ͳ�ƻ�õ���Ŀ����
ע��     ��
********************************************************************/
int plistclass_get_album_file_num( char *album );

/*******************************************************************
Function : plistclass_get_files
Brief    : ����ʾ�б��ȡ��Ŀ������֧������
Input    : 
           1)  PlistClass_level_info_t *level_info
           ����UI�㽫Ҫ��ȡ���б��������״̬���Լ�
           �����������
           2)  int start
           ��ȡ��Ŀ�Ŀ�ʼ���
           3)  int num
           ��ȡ����Ŀ����
           4)  plist_detail_info_t **pid3_info
           ���ؼ�¼ָ��
Output   : int
           -1,    fail
           other, ʵ�ʻ�ȡ������Ŀ��
ע��     ��
           1)������start = -1ʱ����ʾϣ����ȡ���з�����������Ŀ��
             ��ʱ����num��Ч
           2)�������ݻ�������Ӧ�ò㿪�ٺ����ٿռ䣬�˴�ֻ���ݸ�
             ��װ��ʹ��
           3)���ݻ�����Ӧ�ϸ����Ҫ�󣬶ž������ڴ�й©
             �ݶ�������ĿӦ����1KB�Ļ�����
********************************************************************/
int plistclass_get_files(plistclass_level_info_t *level_info, int start, int num, plist_detail_info_t **pid3_info);

/*******************************************************************
Function : plistclass_set_filter
Brief    : ���ù����ַ���
Input    : 
           1)  filter_str
           �����ַ���
Output   : bool
           false,      fail
           true,    success
ע��     ��
********************************************************************/
bool plistclass_set_filter( char *filter_str );

/**************************************************************************************
*
*                             Now Playing Interface
*
***************************************************************************************/

/*******************************************************************
Function : plistclass_get_id3_by_index
Brief    : ��ȡ���ڲ����б���ĳ��Ŀ��ID3��Ϣ
Input    : 
           1)  int index
           ϣ����ȡID3��Ϣ����Ŀ���
Output   : plist_detail_info_t *
           NULL,    fail
           other,   ID3���ݵ��׵�ַ
ע��     ��
           1)���ص��ǵ�ַ�����ڴ�ռ����ɷ�װ�㿪�ٺ����٣�������Ӧ
             �ϸ����ع涨����Ҫ��ͼȥ���Ĳ������Լ�������ڴ�ռ�
********************************************************************/
plist_detail_info_t * plistclass_get_id3_by_index(int cur_index);

/*******************************************************************
Function : plistclass_get_id3_by_fullpath
Brief    : ��ȡ���ڲ����б���ĳ��Ŀ��ID3��Ϣ
Input    : 
           1)  fullpath: �ļ�ȫ·��
           ϣ����ȡID3��Ϣ����Ŀ���
Output   : plist_detail_info_t *
           NULL,    fail
           other,   ID3���ݵ��׵�ַ
ע��     ��
           1)���ص��ǵ�ַ. 
********************************************************************/
plist_detail_info_t * plistclass_get_id3_by_fullpath(const char* fullpath);

/*******************************************************************
Function : plistclass_add_item_to_np
Brief    : ����ʾ�б���Ŀ��ӵ����ڲ����б�
Input    : 
           1)  index
           �����Ŀ
Output   : bool
           0,      fail
           1,       success
********************************************************************/
bool plistclass_add_showlist_item_to_np( int cur_index );

/*******************************************************************
Function : plistclass_add_showlist_to_np
Brief    : ����ʾ�б���Ŀ��ӵ����ڲ����б�
Input    : 
           1)  int start
           �����Ŀ�Ŀ�ʼ���
           2)  int num
           ��ӵ���Ŀ����
Output   : int
           -1,    fail
           other, ʵ�ʻ�ȡ������Ŀ��
ע��     ��
           1)������start = -1ʱ����ʾҪ�����������ʾ�б����ڲ���
             �б���ʱ����num��Ч
********************************************************************/
int plistclass_add_showlist_to_np( void );

/*******************************************************************
Function : plistclass_del_items_from_showlist
Brief    : ɾ����ʾ�б���Ŀ
Input    : 
           1)  int start
           ��ʼ���
           2)  int num
           ��Ҫ�����Ŀ������
Output   : int
           -1,      fail
           other,   ʵ��ɾ������Ŀ��
ע��     ��
           1)��start = -1ʱ����ʾϣ�����������ʾ�б���ʱnum����
             ��Ч
********************************************************************/
int plistclass_del_items_from_showlist(int start,int num);

/*******************************************************************
Function : plistclass_add_fullpath_to_np
Brief    : ��Ŀ¼�б���Ŀ��ӵ����ڲ����б�
Input    : 
           1)  int start
           �����Ŀ�Ŀ�ʼ���
           2)  int num
           ��ӵ���Ŀ����
Output   : int
           -1,    fail
           other, ʵ�ʻ�ȡ������Ŀ��
ע��     ��
           1)������start = -1ʱ����ʾҪ�����������ʾ�б����ڲ���
             �б���ʱ����num��Ч
********************************************************************/
int plistclass_add_fullpath_to_np( char *fullpath );

/*******************************************************************
Function : plistclass_add_dirlist_to_np
Brief    : ���DirList���ݵ����ڲ����б�
Input    : 
           1)  void *handle
           DirList�ľ��
           2)  int start
           ��ʼ���
           3)  int num
           ��Ҫ��ӵ�����
Output   : int
           -1,      fail
           other,   ʵ����ӵ���Ŀ��
ע��     ��
           1)��start = -1ʱ����ʾϣ���������DirList�����ڲ����б�
             ��ʱnum������Ч
********************************************************************/
int plistclass_add_dirlist_to_np(void *handle,int start,int num);

/**************************************************************************************
*
*                             Bookmarks Interface
*
***************************************************************************************/

/*******************************************************************
Function : plistclass_add_item_to_bmlist
Brief    : �����Ŀ����ǩ�б�
Input    : 
           1)  const char *fullpath
           ��ǩ�ļ���ȫ·��
           2)  const plist_mark_info_t *markinfo
           ��ǩ��Ϣ
Output   : bool
           0,      fail
           1,       success
ע��     ��
           1)��װ�㲻Ҫ�����޸ĵ����ߴ��ݹ�ȥ���ڴ�ռ�
********************************************************************/
bool plistclass_add_item_to_bmlist(plist_detail_info_t *item);

/**************************************************************************************
*
*                             My Playlist Interface
*
***************************************************************************************/

/*******************************************************************
Function : plistclass_add_item_to_mylist
Brief    : �����Ŀ���ҵĲ����б�
Input    : 
           1) int mylist_no
           �б����, 1-5 
           2)  const char *fullpath
           �ļ���ȫ·��
Output   : bool
           0,      fail
           1,       success
ע��     ��
           1)��װ�㲻Ҫ�����޸ĵ����ߴ��ݹ�ȥ���ڴ�ռ�
********************************************************************/
bool plistclass_add_item_to_mylist(int mylist_no, plist_detail_info_t *item);


/********************************************************************************
����˵�� : �ı�ָ��λ���ļ����Ǽ�

Input    : 
           1)  const char *fullpath
           �ļ���ȫ·��
           2) int rating
           �ļ����Ǽ�, 0-5 , 0Ϊδ�ּ�
Output   : bool
           0,      fail
           1,       success        
ע��:
********************************************************************************/
bool plistclass_change_rating(const char *fullpath, int rating);

/*******************************************************************
Function : plistclass_store_usrselcontent
Brief    : �����û���ǰѡ����Ŀ�����ݡ�
Input    : 
           1)  mask           ��ǰ�������ֶ�����
          2) content         ��ѡ���ֶ�ֵ
Output   : bool
           0,      fail
           1,       success
ע��     ��        
********************************************************************/
bool plistclass_store_usrselcontent(plist_detail_mask_t mask,plistclass_usrselcontent_u *content);

/*******************************************************************
Function : plistclass_sync_to_fs
Brief    : ͬ�����в����б��Ƿ����bookmarkList��װ����ʵ�ʵ�����
Input    : 
           1)  const char *fullpath
           ��ɾ���ļ�ȫ·��
Output   : bool
           0,      fail
           1,       success
ע��     ��
           1) �ýӿ�����Ӧ�ò�ֱ��ʹ���ļ�ϵͳ�ӿ�ɾ���ļ���Ӧ
              ���̵��ñ��ӿ�֪ͨ��װ��ͬ�����Ͻ�µ����һϵ�в���
              �б�
           2) ͨ���˽ӿ��Ƿ�ͬʱͬ����ǩ�б�δ����������ɷ�װ����
              ʵ�����������
           3) ��װ����Ƹ�д���������fullpath�����ڴ�ռ�����ݡ�
********************************************************************/
bool plistclass_sync_to_fs(char *fullpath);



/*����levelinfo��Ϣ��ȡ��ʾ�б�״̬�����ں�����ȡ�б��¼��*/
/*ֻ������,��Ƶ ����Ҫ��ȡ����������-1--fail,other-�б�ǰ״̬��*/
/*�������ص�״ֵ̬����ȡ����������*/
plistclass_state_e plistclass_get_state(plistclass_level_info_t* levelinfo);

bool plistclass_init_level(plistclass_level_info_t* levelinfo);
bool plistclass_next_level(plistclass_level_info_t* levelinfo, int current, int next);
bool plistclass_prev_level(plistclass_level_info_t* levelinfo);

int plistclass_get_mtplist_num( void );
int plistclass_get_mtplist_name( plist_name_t *buf, int start, int num );

#endif
