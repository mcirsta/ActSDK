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

/*表明该级不要求显示列表*/
#define NONE_LIST               -1
 
//##########################//
//    DATASTRUCT DESCRIPTION
//    OR  VALUEABLE DEFINE     
//##########################//
 
/*列表枚举变量*/
typedef enum
{
	PLIST_NOWPLAYING = 0,
	PLIST_SHOWLIST = 1,
	
}plistclass_type_e;
 
/*第一级列表枚举变量(最多显示有8/LEVEL_ONE_MAX个列表)*/
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

/*第2级列表枚举变量(最多显示有2/LEVEL_TWO_MAX个列表)*/
typedef enum
{
	LEVEL_TWO_NONE_LIST = NONE_LIST,
	LEVEL_TWO_MIN = 0,
	LEVEL_TWO_LIST = LEVEL_TWO_MIN,
	LEVEL_TWO_VOCREC = LEVEL_TWO_MIN,
	LEVEL_TWO_FMREC,
	LEVEL_TWO_MAX
	
}level_two_e;
 
/*第3级列表枚举变量(最多显示有1/LEVEL_THREE_MAX个列表)*/
typedef enum
{
	LEVEL_THREE_NONE_LIST = NONE_LIST,
	LEVEL_THREE_MIN = 0,
	LEVEL_THREE_FILELIST = LEVEL_THREE_MIN,
	LEVEL_THREE_MAX
	
}level_three_e;

/*
上层通知封装层提取列表的具体信息结构体
主要用途：
1)上层通知封装层希望提取哪一级的哪一个列表
2)希望得到结果的排序方式
*/

typedef struct
{
	level_one_e level_one;
	level_two_e level_two;
	level_three_e level_three;
	plist_sortstyle_t sortstyle;//排序方式
    
}plistclass_level_info_t;


/*
用户选中项信息结构体
*/
typedef union 
{
    char itemstr[PATH_MAX];
    int  rating;
  
}plistclass_usrselcontent_u;


/*列表状态枚举变量*/
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
Brief    : 初始化封装层
Input    : 
           1)  plist_file_type_t filetype
           应用类型
Output   : bool
           0,      fail
           1,       success
注意     ：
           1)   filetype目前可定的有：
            MUSIC 音乐 
						VIDEO 视频 
						IMAGE 图片 
						TXT 电子书 
						FLASH(REVERSETYPE1) 保留类型1(该项用于FLASH)
						REVERSETYPE2        保留类型2 
********************************************************************/
bool plistclass_init(plist_file_type_t filetype);

/*******************************************************************
Function : plistclass_exit
Brief    : 销毁封装层
Input    : 
           1)  plist_file_type_t filetype
           应用类型
Output   : bool
           0,      fail
           1,       success
注意     ：
           1)   filetype目前可定的有：
            MUSIC 音乐 
						VIDEO 视频 
						IMAGE 图片 
						TXT 电子书 
						FLASH(REVERSETYPE1) 保留类型1(该项用于FLASH)
						REVERSETYPE2        保留类型2 
					 2)   销毁时按道理输入参数应该不是必须的，在此保留该参数
					  但希望实际设计结果是没有用到输入参数的，达到方便调用者
					  使用改接口
********************************************************************/
bool plistclass_exit(plist_file_type_t filetype);

/*******************************************************************
Function : plistclass_get_item_num
Brief    : 获取指定列表的条目总数
Input    : 
           1)  plistclass_type_e type
           目标列表
Output   : int
           -1,      fail
           other,   实际统计获得的条目总数
注意     ：
********************************************************************/
int plistclass_get_item_num(plistclass_type_e type);

/*******************************************************************
Function : plistclass_get_album_file_num
Brief    : 获取指定album文件总数
Input    : 
           1)  album
           专辑名称
Output   : int
           -1,      fail
           other,   实际统计获得的条目总数
注意     ：
********************************************************************/
int plistclass_get_album_file_num( char *album );

/*******************************************************************
Function : plistclass_get_files
Brief    : 从显示列表获取条目，并且支持排序
Input    : 
           1)  PlistClass_level_info_t *level_info
           传递UI层将要获取的列表所处层次状态，以及
           结果的排序风格
           2)  int start
           获取条目的开始序号
           3)  int num
           获取的条目数量
           4)  plist_detail_info_t **pid3_info
           返回记录指针
Output   : int
           -1,    fail
           other, 实际获取到的条目数
注意     ：
           1)当参数start = -1时，表示希望获取所有符合条件的条目，
             此时参数num无效
           2)返回内容缓冲区由应用层开辟和销毁空间，此处只传递给
             封装层使用
           3)内容缓冲区应严格符合要求，杜绝出现内存泄漏
             暂定单个条目应开辟1KB的缓冲区
********************************************************************/
int plistclass_get_files(plistclass_level_info_t *level_info, int start, int num, plist_detail_info_t **pid3_info);

/*******************************************************************
Function : plistclass_set_filter
Brief    : 设置过滤字符串
Input    : 
           1)  filter_str
           过滤字符串
Output   : bool
           false,      fail
           true,    success
注意     ：
********************************************************************/
bool plistclass_set_filter( char *filter_str );

/**************************************************************************************
*
*                             Now Playing Interface
*
***************************************************************************************/

/*******************************************************************
Function : plistclass_get_id3_by_index
Brief    : 获取正在播放列表中某条目的ID3信息
Input    : 
           1)  int index
           希望获取ID3信息的条目序号
Output   : plist_detail_info_t *
           NULL,    fail
           other,   ID3内容的首地址
注意     ：
           1)返回的是地址，该内存空间是由封装层开辟和销毁，调用者应
             严格遵守规定，不要试图去更改不属于自己管理的内存空间
********************************************************************/
plist_detail_info_t * plistclass_get_id3_by_index(int cur_index);

/*******************************************************************
Function : plistclass_get_id3_by_fullpath
Brief    : 获取正在播放列表中某条目的ID3信息
Input    : 
           1)  fullpath: 文件全路径
           希望获取ID3信息的条目序号
Output   : plist_detail_info_t *
           NULL,    fail
           other,   ID3内容的首地址
注意     ：
           1)返回的是地址. 
********************************************************************/
plist_detail_info_t * plistclass_get_id3_by_fullpath(const char* fullpath);

/*******************************************************************
Function : plistclass_add_item_to_np
Brief    : 将显示列表条目添加到正在播放列表
Input    : 
           1)  index
           添加条目
Output   : bool
           0,      fail
           1,       success
********************************************************************/
bool plistclass_add_showlist_item_to_np( int cur_index );

/*******************************************************************
Function : plistclass_add_showlist_to_np
Brief    : 将显示列表条目添加到正在播放列表
Input    : 
           1)  int start
           添加条目的开始序号
           2)  int num
           添加的条目数量
Output   : int
           -1,    fail
           other, 实际获取到的条目数
注意     ：
           1)当参数start = -1时，表示要求添加整个显示列表到正在播放
             列表，此时参数num无效
********************************************************************/
int plistclass_add_showlist_to_np( void );

/*******************************************************************
Function : plistclass_del_items_from_showlist
Brief    : 删除显示列表条目
Input    : 
           1)  int start
           开始序号
           2)  int num
           需要清除条目的数量
Output   : int
           -1,      fail
           other,   实际删除的条目数
注意     ：
           1)当start = -1时，表示希望清空整个显示列表，此时num参数
             无效
********************************************************************/
int plistclass_del_items_from_showlist(int start,int num);

/*******************************************************************
Function : plistclass_add_fullpath_to_np
Brief    : 将目录列表条目添加到正在播放列表
Input    : 
           1)  int start
           添加条目的开始序号
           2)  int num
           添加的条目数量
Output   : int
           -1,    fail
           other, 实际获取到的条目数
注意     ：
           1)当参数start = -1时，表示要求添加整个显示列表到正在播放
             列表，此时参数num无效
********************************************************************/
int plistclass_add_fullpath_to_np( char *fullpath );

/*******************************************************************
Function : plistclass_add_dirlist_to_np
Brief    : 添加DirList内容到正在播放列表
Input    : 
           1)  void *handle
           DirList的句柄
           2)  int start
           开始序号
           3)  int num
           需要添加的数量
Output   : int
           -1,      fail
           other,   实际添加的条目数
注意     ：
           1)当start = -1时，表示希望添加整个DirList到正在播放列表，
             此时num参数无效
********************************************************************/
int plistclass_add_dirlist_to_np(void *handle,int start,int num);

/**************************************************************************************
*
*                             Bookmarks Interface
*
***************************************************************************************/

/*******************************************************************
Function : plistclass_add_item_to_bmlist
Brief    : 添加条目到书签列表
Input    : 
           1)  const char *fullpath
           书签文件的全路径
           2)  const plist_mark_info_t *markinfo
           书签信息
Output   : bool
           0,      fail
           1,       success
注意     ：
           1)封装层不要随意修改调用者传递过去的内存空间
********************************************************************/
bool plistclass_add_item_to_bmlist(plist_detail_info_t *item);

/**************************************************************************************
*
*                             My Playlist Interface
*
***************************************************************************************/

/*******************************************************************
Function : plistclass_add_item_to_mylist
Brief    : 添加条目到我的播放列表
Input    : 
           1) int mylist_no
           列表序号, 1-5 
           2)  const char *fullpath
           文件的全路径
Output   : bool
           0,      fail
           1,       success
注意     ：
           1)封装层不要随意修改调用者传递过去的内存空间
********************************************************************/
bool plistclass_add_item_to_mylist(int mylist_no, plist_detail_info_t *item);


/********************************************************************************
函数说明 : 改变指定位置文件的星级

Input    : 
           1)  const char *fullpath
           文件的全路径
           2) int rating
           文件的星级, 0-5 , 0为未分级
Output   : bool
           0,      fail
           1,       success        
注意:
********************************************************************************/
bool plistclass_change_rating(const char *fullpath, int rating);

/*******************************************************************
Function : plistclass_store_usrselcontent
Brief    : 保存用户当前选中条目的内容。
Input    : 
           1)  mask           当前操作的字段名称
          2) content         被选中字段值
Output   : bool
           0,      fail
           1,       success
注意     ：        
********************************************************************/
bool plistclass_store_usrselcontent(plist_detail_mask_t mask,plistclass_usrselcontent_u *content);

/*******************************************************************
Function : plistclass_sync_to_fs
Brief    : 同步所有播放列表（是否包括bookmarkList封装层视实际调整）
Input    : 
           1)  const char *fullpath
           被删除文件全路径
Output   : bool
           0,      fail
           1,       success
注意     ：
           1) 该接口用于应用层直接使用文件系统接口删除文件后，应
              立刻调用本接口通知封装层同步其管辖下的相关一系列播放
              列表。
           2) 通过此接口是否同时同步书签列表未定。具体可由封装层视
              实际情况决定。
           3) 封装层进制改写传入参数（fullpath）的内存空间的数据。
********************************************************************/
bool plistclass_sync_to_fs(char *fullpath);



/*根据levelinfo信息提取显示列表状态，用于后续提取列表记录。*/
/*只有音乐,视频 才需要提取过滤条件。-1--fail,other-列表当前状态。*/
/*根绝返回的状态值，提取过滤条件。*/
plistclass_state_e plistclass_get_state(plistclass_level_info_t* levelinfo);

bool plistclass_init_level(plistclass_level_info_t* levelinfo);
bool plistclass_next_level(plistclass_level_info_t* levelinfo, int current, int next);
bool plistclass_prev_level(plistclass_level_info_t* levelinfo);

int plistclass_get_mtplist_num( void );
int plistclass_get_mtplist_name( plist_name_t *buf, int start, int num );

#endif
