/********************************************************************************
 *                              USDK(1100)
 *                             Module: plistclass
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : plistclass.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-05-15          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     plistclass.c
 * \brief   playlist class
 * \author   songzhining
 *
 * \version 1.0
 * \date  2009/05/15
 *******************************************************************************/

//##########################//
//    INCLUDE FILES
//##########################//
#include "plistclass.h"
#include "music_config.h"

//##########################//
//    MACRO DEFINE
//##########################//

//TODO,ADD YOUR CODE HERE

//##########################//

/*暂存id3信息的数组大小*/
#define  ID3INFOSIZE  20

/*书签记录的大小*/
#define  BOOKMARKSIZE  32

/*我的列表类型*/
#define MAX_MYLIST    5

/*星级列表类型*/
#define MAX_MYRATE    5

//基本表: 包括小媒体库表,正在播放列表, 浏览用的显示列表, 书签列表, 我的列表.
//对于小媒体库，只有音乐才有。包括album,artist,genre,path,rating字段，title也应该加上，这样所有的id3信息都在这里了。
//对于正在播放列表（具有path,bookmark字段）,音乐有一个,视频有一个，图片有一个，电子书有一个。
//对于浏览显示列表，只含有全路径字段，不同类型的文件公用一个.
//对于书签列表（具有path,bookmark字段）,音乐有一个，视频有一个，电子书有一个。
//书签和正在播放列表每个应用各有一个,是因为这些列表示需要永久保存记录的.
//对于我的列表（具有path,rating字段）, 音乐有一个

/*音乐媒体库(本地)列表类型*/
#define MUSICLIB      MUSICRESERVE1

/*音乐媒体库(外部)列表类型*/
#define EXTRAMUSICLIB MUSICRESERVE1

/*正在播放列表列表类型*/
#define MUSICNPLIST   MUSICRESERVE2

/*临时文件列表类型*/
#define MUSICTMPLIST  MUSICRESERVE3

/*书签列表列表类型*/
#define MUSICBLLIST   MUSICRESERVE4

/*我的列表类型*/
static int s_musicmylist[MAX_MYLIST] =
{ MUSICRESERVE5, MUSICRESERVE5, MUSICRESERVE5, MUSICRESERVE5, MUSICRESERVE5, };

/*星级列表类型*/
static int s_musicmyrate[MAX_MYRATE] =
{ MUSICRESERVE6, MUSICRESERVE6, MUSICRESERVE6, MUSICRESERVE6, MUSICRESERVE6, };

/*Artist列表类型*/
#define ARTISTLIST   MUSICRESERVE7

/*Album列表类型*/
#define ALBUMLIST   MUSICRESERVE8

/*Genre列表类型*/
#define GENRELIST   MUSICRESERVE9

/*搜索列表类型*/
#define SEARCHLIST   MUSICRESERVE10

//##########################//

static bool plist_inited = false;
    
/*当前显示句柄*/
static void* s_hshowlist = NULL;

/*媒体库*/
static void* s_hmedia = NULL;

/*音乐子媒体库(本地)*/
static void* s_hsubmusiclib = NULL;
static char* s_szsubmusictbl = "submusiclib";
static plist_trivefield_t s_musiclib_field;

/*音乐子媒体库(外部)*/
static void* s_hextramusiclib = NULL;
static char* s_szextramusictbl = "extramusiclib";
static plist_trivefield_t s_extramusiclib_field;

/*音乐正在播放列表*/
static void* s_hnpmusic = NULL;
static char* s_sznpmusictbl = "npmusiclist";
static plist_trivefield_t s_nplist_field;

/*临时列表 的句柄，列表名称，列表结构*/
static void* s_htmplist = NULL;
static char* s_sztmptbl = "plisttmplist";
static plist_trivefield_t s_tmplist_field;

/*音乐书签列表句柄*/
static void* s_hblmusic = NULL;
static char* s_szblmusictbl = "blmusiclist";
static plist_trivefield_t s_bllist_field;

/*我的列表句柄*/
static void* s_hmymusic[MAX_MYLIST] =
{ NULL };
static char* s_szmymusictbl[MAX_MYLIST] =
{ "mymusiclist1", "mymusiclist2", "mymusiclist3", "mymusiclist4", "mymusiclist5", };
static plist_trivefield_t s_mylist_field;

/*星级列表类型*/
static void* s_hrate[MAX_MYRATE] =
{ NULL };
static char* s_szratetbl[MAX_MYRATE] =
{ "level1", "level2", "level3", "level4", "level5", };
static plist_trivefield_t s_rate_field;

/*Artist列表类型*/
static void* s_hartist = NULL;
static char* s_szartisttbl = "artistlist";
static plist_trivefield_t s_artist_field;

/*Album列表类型*/
static void* s_halbum = NULL;
static char* s_szalbumtbl = "albumlist";
static plist_trivefield_t s_album_field;

/*Genre列表类型*/
static void* s_hgenre = NULL;
static char* s_szgenretbl = "genrelist";
static plist_trivefield_t s_genre_field;

/*搜索列表类型*/
static void* s_hsearch = NULL;
static char* s_szsearchtbl = "searchlist";
static plist_trivefield_t s_search_field;

/*MTP列表类型*/
static void* s_hmtplist = NULL;
static plist_name_t s_szmtplist;

//##########################//

/*当前的应用类型。 音乐，视频，图片，电子书*/
static plist_file_type_t s_curfiletype;

/*显示列表状态信息*/
static plistclass_level_info_t s_tmplist_levelinfo;

/*保存在浏览过程中用户选中项的内容*/
static plistclass_usrselcontent_u s_album_content;
static plistclass_usrselcontent_u s_artist_content;
static plistclass_usrselcontent_u s_genre_content;
static plistclass_usrselcontent_u s_path_content;
static plistclass_usrselcontent_u s_rating_content;

/*显示列表的提取条件*/
static plist_filterfield_t s_tmplistfilter;

/*显示列表的排序字段*/
static plist_sortfield_t s_tmplistsort;

/*显示列表的分组字段*/
static plist_groupfield_t s_tmplistgroup;

/*暂存显示列表中记录的buffer*/
/*一次可以取20个ID3INFOSIZE个id3信息*/
static plist_detail_info_t s_id3info[ID3INFOSIZE];

/* 字符串过滤 */
static char s_filter[PATH_MAX];
static bool s_filter_flag = false;

static int _curlevel = 1;

//##########################//
//    INTERNAL FUNCTIONS
//##########################//

/*初始化列表结构变量*/
static void _plistclass_init_list_struct(plist_file_type_t type)
{
    /*定义音乐子媒体库的结构*/
    s_musiclib_field.num = 10;
    s_musiclib_field.nfield[0] = FULLPATH;
    s_musiclib_field.nfield[1] = MARK;
    s_musiclib_field.nfield[2] = TITLE;
    s_musiclib_field.nfield[3] = ARTIST;
    s_musiclib_field.nfield[4] = ALBUM;
    s_musiclib_field.nfield[5] = GENRE;
    s_musiclib_field.nfield[6] = IMAGEINFO;
    s_musiclib_field.nfield[7] = RATING;
    s_musiclib_field.nfield[8] = AGE;
    s_musiclib_field.nfield[9] = TRACK;

    /*定义音乐子媒体库(外部)的结构*/
    s_extramusiclib_field = s_musiclib_field;
    
    /*正在播放列表的结构对于不同类型的文件来说是相同的，即只有全路径字段和书签信息字段。*/
    s_nplist_field.num = 2;
    s_nplist_field.nfield[0] = FULLPATH;
    s_nplist_field.nfield[1] = MARK;

    /*临时列表的结构对于不同类型的文件来说是相同的，即只有全路径字段。*/
    s_tmplist_field.num = 8;
    s_tmplist_field.nfield[0] = FULLPATH;
    s_tmplist_field.nfield[1] = MARK;
    s_tmplist_field.nfield[2] = TITLE;
    s_tmplist_field.nfield[3] = ARTIST;
    s_tmplist_field.nfield[4] = ALBUM;
    s_tmplist_field.nfield[5] = GENRE;
    s_tmplist_field.nfield[6] = IMAGEINFO;
    s_tmplist_field.nfield[7] = TRACK;

    /*书签列表结构对于不同类型的文件来说是相同的，即只有全路径字段和书签信息字段。*/
    s_bllist_field = s_tmplist_field;

    /*我的列表结构对于不同类型的文件来说是相同的，即只有全路径字段和书签信息字段。*/
    s_mylist_field = s_tmplist_field;

    /*星级列表类型*/
    s_rate_field = s_tmplist_field;

    /*Artist列表类型*/
    s_artist_field = s_tmplist_field;

    /*Album列表类型*/
    s_album_field = s_tmplist_field;

    /*Genre列表类型*/
    s_genre_field = s_tmplist_field;

    /*搜索列表类型*/
    s_search_field = s_tmplist_field;
}
;

static void _plistclass_init_userselcontent( void )
{
    memset(&s_album_content, 0, sizeof(s_album_content));
    memset(&s_artist_content, 0, sizeof(s_artist_content));
    memset(&s_genre_content, 0, sizeof(s_genre_content));
    memset(&s_path_content, 0, sizeof(s_path_content));
    memset(&s_rating_content, 0, sizeof(s_rating_content));
}

static void _plistclass_init_filetype_ext(plist_file_type_t type)
{
    int result;
    char ext_music[PATH_MAX];
    
    switch (type)
    {
        case MUSIC:
        /* read ext config */
        result = get_config( CFG_APP_SUPPORT_EXT_MUSIC, (char*)ext_music, sizeof(ext_music) );
        if( result < 0 )
        {
            print_err("get music ext failed!");
            return;
        }
    
        plist_set_filetype_ext(MUSIC, ext_music);
        break;

        case VIDEO:
        break;

        case IMAGE:
        break;

        case TXT:
        break;

        default:
        break;
    }

}

/*建立数据表。0---出错， 1--成功。*/
static bool _plistclass_init_table( void **pplist )
{
    int idx;
    int result = -1;
    
    /* 指针检查*/
    if ( pplist == NULL )
    {
        return false;
    }
        
    /* 内容检查*/
    if ( (*pplist) != NULL )
    {
        return true;
    }
    
    /*打开媒体库*/
    if ( pplist == &s_hmedia )
    {
        s_hmedia = plist_open(NULL, MEDIALIB, NULL);
        return ( s_hmedia != NULL );
    }
    
    /*打开音乐媒体库*/
    if ( pplist == &s_hsubmusiclib )
    {
        s_hsubmusiclib = plist_open(s_szsubmusictbl, MUSICLIB, &s_musiclib_field);
        return ( s_hsubmusiclib != NULL );
    }

    /*打开扩展音乐媒体库*/
    if ( pplist == &s_hextramusiclib )
    {
        s_hextramusiclib = plist_open(s_szextramusictbl, EXTRAMUSICLIB, &s_extramusiclib_field);
        return ( s_hextramusiclib != NULL );
    }
        
    /*打开正在播放列表*/
    if ( pplist == &s_hnpmusic )
    {
        s_hnpmusic = plist_open(s_sznpmusictbl, MUSICNPLIST, &s_nplist_field);
        return ( s_hnpmusic != NULL );
    }

    /*打开临时列表*/
    if ( pplist == &s_htmplist )
    {
        s_htmplist = plist_open(s_sztmptbl, MUSICTMPLIST, &s_tmplist_field);
        return ( s_htmplist != NULL );
    }
    
    /*打开书签列表*/
    if ( pplist == &s_hblmusic )
    {
        s_hblmusic = plist_open(s_szblmusictbl, MUSICBLLIST, &s_bllist_field);
        return ( s_hblmusic != NULL );
    }

    /*打开我的列表*/
    for (idx = 0; idx < MAX_MYLIST; idx++)
    {
        if ( pplist == &s_hmymusic[idx] )
        {
            s_hmymusic[idx] = plist_open(s_szmymusictbl[idx], s_musicmylist[idx], &s_mylist_field);
            return ( s_hmymusic[idx] != NULL );
        }
    }

    /*打开星级列表*/
    for (idx = 0; idx < MAX_MYRATE; idx++)
    {
        if ( pplist == &s_hrate[idx] )
        {
            result = plist_table_exist(s_szratetbl[idx]);
            
            s_hrate[idx] = plist_open(s_szratetbl[idx], s_musicmyrate[idx], &s_rate_field);
            
            if (0 == result)
            {
                plist_set_table_invalid(s_hrate[idx]);
            }
            return ( s_hrate[idx] != NULL );
        }
    }

    /*打开Artist列表*/
    if ( pplist == &s_hartist )
    {
        result = plist_table_exist(s_szartisttbl);
        
        s_hartist = plist_open(s_szartisttbl, ARTISTLIST, &s_artist_field);   
        
        if (0 == result)
        {
            plist_set_table_invalid(s_hartist);
        }
        return ( s_hartist != NULL );
    }

    /*打开Album列表*/
    if ( pplist == &s_halbum )
    {
        result = plist_table_exist(s_szalbumtbl);
        
        s_halbum = plist_open(s_szalbumtbl, ALBUMLIST, &s_album_field);  
        
        if (0 == result)
        {
            plist_set_table_invalid(s_halbum);
        }
        return ( s_halbum != NULL );
    }

    /*打开Genre列表*/
    if ( pplist == &s_hgenre )
    {
        result = plist_table_exist(s_szgenretbl);
        
        s_hgenre = plist_open(s_szgenretbl, GENRELIST, &s_genre_field); 
        
        if (0 == result)
        {
            plist_set_table_invalid(s_hgenre);
        }
        return ( s_hgenre != NULL );
    }
    
    /*搜索列表类型*/
    if ( pplist == &s_hsearch )
    {
        s_hsearch = plist_open(s_szsearchtbl, SEARCHLIST, &s_search_field);
        return ( s_hsearch != NULL );
    }
    
    /*MTP列表类型*/
    if ( pplist == &s_hmtplist )
    {        
        //open new table
        s_hmtplist = plist_open(s_szmtplist.list_name, MUSICMTP, NULL);        
        return ( s_hmtplist != NULL );
    }
    
    return false;
}

/*清空数据表。0---出错， 1--成功。*/
static bool _plistclass_clear_table( void **pplist )
{
    bool bret = false;
    
    /* 指针检查*/
    if ( pplist == NULL )
    {
        return false;
    }
        
    /* 内容检查*/
    if ( (*pplist) != NULL )
    {
        /* 删除数据表 */
        plist_delete_by_handle( *pplist );  //delete
        plist_close( *pplist );
        (*pplist) = NULL;
        
        /* 创建数据表 */
        bret = _plistclass_init_table( pplist );
    }

    return bret;
}

static int _plistclass_init_direction( void )
{
    _curlevel = 1;

    return true;
}

static int _plistclass_get_next_direction(plistclass_level_info_t* level_info)
{
    int result = -1;

    if (1 == _curlevel)
    {
        result = (int) (level_info->level_one);
        _curlevel++;
    }
    else if (2 == _curlevel)
    {
        result = (int) (level_info->level_two);
        _curlevel++;
    }
    else if (3 == _curlevel)
    {
        result = (int) (level_info->level_three);
        _curlevel++;
    }
    else
    {
    }

    return result;
}

/*比较两个levelinfo是否相同.
 0-same.
 -1--different.*/
static int _plistclass_cmplevelinfo(plistclass_level_info_t* info1, plistclass_level_info_t* info2)
{
    if (info1->sortstyle != info2->sortstyle)
    {
        return -1;
    }

    if (info1->level_one != info2->level_one)
    {
        return -1;
    }

    if (info1->level_two != info2->level_two)
    {
        return -1;
    }
    if (-1 == info1->level_two)
    {
        return 0;
    }

    if (info1->level_three != info2->level_three)
    {
        return -1;
    }
    if (-1 == info1->level_three)
    {
        return 0;
    }

    return 0;
}

/*
 根据状态填充显示列表
 0--ok, -1--fail.
 此函数成功执行完以后,显示列表中已经有数据了.
 */
static int _plistclass_fill_showlist(plistclass_level_info_t *level_info)
{
    int result = -1;
    int cur_index;
    plist_detail_mask_t mask;
    plistclass_state_e state;
    bool need_query = false;
    
    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hsubmusiclib);  

    state = plistclass_get_state(level_info);

    /*根据状态设置排序字段*/
    s_tmplistsort.num = 1;
    s_tmplistsort.nfield[0] = TITLE;//FULLPATH;
    s_tmplistsort.nstyle[0] = level_info->sortstyle;

    /*根据状态设置过滤条件*/
    s_tmplistfilter.field_num = 0;
    s_tmplistfilter.is_pattern_search = 0;
    mask = TITLE;

    /*设置分组条件*/
    s_tmplistgroup.num = 0;
    s_tmplistgroup.nstyle[0] = level_info->sortstyle;
    s_tmplistgroup.nsortdirection[0] = 0;

    switch (state)
    {
        case STATE_ALLMUSIC_FILE:/*music song list*/
        s_hshowlist = s_hsubmusiclib;
        break;

        case STATE_ALBUM_LIST:
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_halbum);
        if (1 == plist_get_change_sign(s_halbum))
        {
            //table清空
            _plistclass_clear_table(&s_halbum);

            plist_verify_list(s_halbum);
            need_query = true;
        }

        /*sort by album*/
        s_tmplistsort.nfield[0] = ALBUM;

        /*设置分组条件*/
        cur_index = s_tmplistgroup.num;
        s_tmplistgroup.num++;
        s_tmplistgroup.nfield[cur_index] = ALBUM;

        /* filter by album*/
        mask = ALBUM;

        s_hshowlist = s_halbum;
        break;

        case STATE_ARTIST_LIST:
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hartist);
        if (1 == plist_get_change_sign(s_hartist))
        {
            //table清空
            _plistclass_clear_table(&s_hartist);

            plist_verify_list(s_hartist);
            need_query = true;
        }

        /*sort by artist*/
        s_tmplistsort.nfield[0] = ARTIST;

        /*设置分组条件*/
        cur_index = s_tmplistgroup.num;
        s_tmplistgroup.num++;
        s_tmplistgroup.nfield[cur_index] = ARTIST;

        /* filter by artist*/
        mask = ARTIST;

        s_hshowlist = s_hartist;
        break;

        case STATE_GENRE_LIST:
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hgenre);
        if (1 == plist_get_change_sign(s_hgenre))
        {
            //table清空
            _plistclass_clear_table(&s_hgenre);

            plist_verify_list(s_hgenre);
            need_query = true;
        }

        /*sort by genre*/
        s_tmplistsort.nfield[0] = GENRE;

        /*设置分组条件*/
        cur_index = s_tmplistgroup.num;
        s_tmplistgroup.num++;
        s_tmplistgroup.nfield[cur_index] = GENRE;

        /* filter by genre*/
        mask = GENRE;

        s_hshowlist = s_hgenre;
        break;

        case STATE_MYRATING_LIST:
        break;

        case STATE_TAGLIST_FILE:
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hblmusic);
        s_hshowlist = s_hblmusic;
        break;

        case STATE_MYLIST_LIST:
        break;

        case STATE_VOICEREC:/*读取录音文件*/
        break;

        case STATE_FMREC:/*读取录音文件*/
        break;

        case STATE_ALBUM_FILE:
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_htmplist);
        
        //table清空
        _plistclass_clear_table(&s_htmplist);

        /*filter condition is album */
        cur_index = s_tmplistfilter.field_num;
        s_tmplistfilter.field_num++;
        s_tmplistfilter.is_pattern_search = 0;
        s_tmplistfilter.filter_field[cur_index].fieldno = ALBUM;
        s_tmplistfilter.filter_field[cur_index].content.pval = s_album_content.itemstr;

        /*sort by track num*/
        s_tmplistsort.nfield[0] = TRACK;

        s_hshowlist = s_htmplist;
        need_query = true;
        break;

        case STATE_ARTIST_FILE:/*提取某个歌手的所有歌曲*/
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_htmplist);
        
        //table清空
        _plistclass_clear_table(&s_htmplist);
        
        /*filter condition is album */
        cur_index = s_tmplistfilter.field_num;
        s_tmplistfilter.field_num++;
        s_tmplistfilter.is_pattern_search = 0;
        s_tmplistfilter.filter_field[cur_index].fieldno = ARTIST;
        s_tmplistfilter.filter_field[cur_index].content.pval = s_artist_content.itemstr;

        s_hshowlist = s_htmplist;
        need_query = true;
        break;

        case STATE_GENRE_FILE:/*提取某个流派的所有歌曲*/
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_htmplist);
        
        //table清空
        _plistclass_clear_table(&s_htmplist);

        /*filter condition is genre */
        cur_index = s_tmplistfilter.field_num;
        s_tmplistfilter.field_num++;
        s_tmplistfilter.is_pattern_search = 0;
        s_tmplistfilter.filter_field[cur_index].fieldno = GENRE;
        s_tmplistfilter.filter_field[cur_index].content.pval = s_genre_content.itemstr;

        s_hshowlist = s_htmplist;
        need_query = true;
        break;

        case STATE_MYRATING_FILE:/*提取某个星级的所有歌曲*/
        cur_index = s_rating_content.rating - 1;
        
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hrate[cur_index]);        

        if (1 == plist_get_change_sign(s_hrate[cur_index]))
        {
            //table清空
            _plistclass_clear_table(&s_hrate[cur_index]);
        
            plist_verify_list(s_hrate[cur_index]);
            need_query = true;
        }

        /*filter condition is genre */
        cur_index = s_tmplistfilter.field_num;
        s_tmplistfilter.field_num++;
        s_tmplistfilter.is_pattern_search = 0;
        s_tmplistfilter.filter_field[cur_index].fieldno = RATING;
        s_tmplistfilter.filter_field[cur_index].content.nval = s_rating_content.rating;

        s_hshowlist = s_hrate[s_rating_content.rating - 1];
        break;

        case STATE_VOICEREC_FILE:
        break;

        case STATE_FMREC_FILE:
        break;

        case STATE_MYLIST_FILE:
        cur_index = s_rating_content.rating - 1;

        if (cur_index < MAX_MYLIST)
        {            
            //table初始化( 内部只做1次 )
            _plistclass_init_table(&s_hmymusic[cur_index]);        
            s_hshowlist = s_hmymusic[cur_index];
        }
        else
        {
            cur_index = cur_index - MAX_MYLIST;

            //get table name
            plist_get_listname_by_type(MUSICMTP, &s_szmtplist, cur_index, 1);

            //close old table
            if (NULL != s_hmtplist)
            {
                plist_close(s_hmtplist);
                s_hmtplist = NULL;
            }

            //table初始化( 内部只做1次 )
            _plistclass_init_table(&s_hmtplist);  

            //table初始化( 内部只做1次 )
            _plistclass_init_table(&s_htmplist);   

            //table清空
            _plistclass_clear_table(&s_htmplist);
        
            //query: mtplist -> tmplist
            result = plist_add_query(s_hmtplist, s_htmplist, &s_tmplistfilter, &s_tmplistsort, NULL);
            if (result < 0)
            {
                print_err();
            }
        
            s_hshowlist = s_htmplist;
        }
        break;

        default:
        print_err("error occur. state is not correct!");
        return -1;
    }

    /*根据状态设置过滤条件*/
    if (s_filter[0] != '\0')
    {
        cur_index = s_tmplistfilter.field_num;
        s_tmplistfilter.field_num++;
        s_tmplistfilter.is_pattern_search = 1;
        s_tmplistfilter.filter_field[cur_index].fieldno = mask;
        s_tmplistfilter.filter_field[cur_index].content.pval = s_filter;
        
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hsearch);  
        
        //table清空
        _plistclass_clear_table(&s_hsearch);

        result = plist_add_query(s_hsubmusiclib, s_hsearch, &s_tmplistfilter, &s_tmplistsort, &s_tmplistgroup);

        s_hshowlist = s_hsearch;
    }
    else if (true == need_query)
    {        
        result = plist_add_query(s_hsubmusiclib, s_hshowlist, &s_tmplistfilter, &s_tmplistsort, &s_tmplistgroup);
    }
    else
    {
    }

    return result;
}

char * dirname(char *path)
{
    static const char dot[] = ".";
    char *last_slash;

    /* Find last '/'.  */
    last_slash =( (path != NULL) ? strrchr(path, '/') : NULL );

    if ((last_slash != NULL) &&  (last_slash == path ))
    {
        /* The last slash is the first character in the string.  We have to
         return "/".  */
        last_slash++;
    }
    else if ((last_slash != NULL) && (last_slash[1] == '\0'))
    {
        /* The '/' is the last character, we have to look further.  */
        last_slash = memchr(path, last_slash - path, '/');
    }
    else
    {
        print_dbg();
    }

    if (last_slash != NULL)
    {
        /* Terminate the path.  */
        last_slash[0] = '\0';
    }
    else
    {
        /* This assignment is ill-designed but the XPG specs require to
         return a string containing "." in any case no directory part is
         found and so a static and constant string is required.  */
        path = (char *) dot;
    }
    return path;
}

//##########################//
//    EXTERNAL FUNCTIONS
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
bool plistclass_init(plist_file_type_t filetype)
{
    //check init
    if ( plist_inited == true )
    {
        return true;
    }
    
    s_curfiletype = filetype;

    plist_init_detail_info_struct(s_id3info, ID3INFOSIZE);

    memset(&s_tmplist_levelinfo, -1, sizeof(s_tmplist_levelinfo));

    /*设置数据库路径*/
    plist_set_rootdir(sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST));

    if (-1 == plist_init())
    {
        print_err("test playlist--init fail---- ");
        return false;
    }
    else
    {
        plist_inited = true;
    }
   
    /*初始化列表结构*/
    _plistclass_init_list_struct(filetype);

    /*初始化文件类型对因的后缀名*/
    _plistclass_init_filetype_ext(filetype);

    /*初始化记录用户选中条目内容的联合体*/
    _plistclass_init_userselcontent();

    print_dbg("plistclass init ok ");

    return true;

    //TODO,ADD YOUR CODE HERE
}

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
bool plistclass_exit(plist_file_type_t filetype)
{
    int num;

    //check init
    if ( plist_inited == false )
    {
        return true;
    }
    else
    {
        plist_inited = false;
    }
    
    plist_free_buf(s_id3info, ID3INFOSIZE);

    if (NULL != s_hmedia)
    {
        plist_close(s_hmedia);
        s_hmedia = NULL;
    }

    if (NULL != s_hsubmusiclib)
    {
        plist_close(s_hsubmusiclib);
        s_hsubmusiclib = NULL;
    }

    if (NULL != s_hextramusiclib)
    {
        num = plist_get_item_num(s_hextramusiclib);
        if ( num > 0 )
        {
            plist_delete_by_handle(s_hextramusiclib);   //delete
        }
        plist_close(s_hextramusiclib);
        s_hextramusiclib = NULL;
    }

    if (NULL != s_hnpmusic)
    {
        plist_close(s_hnpmusic);
        s_hnpmusic = NULL;
    }

    if (NULL != s_htmplist)
    {
        num = plist_get_item_num(s_htmplist);
        if ( num > 0 )
        {
            plist_delete_by_handle(s_htmplist);   //delete
        }
        plist_close(s_htmplist);
        s_htmplist = NULL;
    }

    if (NULL != s_hblmusic)
    {
        plist_close(s_hblmusic);
        s_hblmusic = NULL;
    }

    /*关闭我的列表*/
    for (num = 0; num < MAX_MYLIST; num++)
    {
        if (NULL != s_hmymusic[num])
        {
            plist_close(s_hmymusic[num]);
            s_hmymusic[num] = NULL;
        }
    }

    for (num = 0; num < MAX_MYRATE; num++)
    {
        if (NULL != s_hrate[num])
        {
            plist_close(s_hrate[num]);
            s_hrate[num] = NULL;
        }
    }

    if (NULL != s_hartist)
    {
        plist_close(s_hartist);
        s_hartist = NULL;
    }

    if (NULL != s_halbum)
    {
        plist_close(s_halbum);
        s_halbum = NULL;
    }

    if (NULL != s_hgenre)
    {
        plist_close(s_hgenre);
        s_hgenre = NULL;
    }

    if (NULL != s_hsearch)
    {
        num = plist_get_item_num(s_hsearch);
        if ( num > 0 )
        {
            plist_delete_by_handle(s_hsearch);   //delete
        }
        plist_close(s_hsearch);
        s_hsearch = NULL;
    }

    if (NULL != s_hmtplist)
    {
        plist_close(s_hmtplist);
        s_hmtplist = NULL;
    }

    if (-1 == plist_quit())
    {
        return false;
    }
    else
    {
        return true;
    }
    //TODO,ADD YOUR CODE HERE
}

/*根据levelinfo信息提取显示列表状态，用于后续提取列表记录。*/
/*只有音乐,视频 才需要提取过滤条件。-1--fail,other-列表当前状态。*/
/*根绝返回的状态值，提取过滤条件。*/
plistclass_state_e plistclass_get_state(plistclass_level_info_t* levelinfo)
{
    plistclass_state_e state = STATE_NONE;
    int dir;

    _plistclass_init_direction();

    dir = _plistclass_get_next_direction(levelinfo);
    
    while ( -1 != dir )
    {
        switch (state)
        {
        case STATE_NONE:
            switch (dir)
            {
                case LEVEL_ONE_ALLMUSIC:
                state = STATE_ALLMUSIC;
                break;

                case LEVEL_ONE_ALBUMS:
                state = STATE_ALBUM;
                break;

                case LEVEL_ONE_ARTISTS:
                state = STATE_ARTIST;
                break;

                case LEVEL_ONE_GENRES:
                state = STATE_GENRE;
                break;

                case LEVEL_ONE_MY_PLAYLIST:
                state = STATE_MYLIST;
                break;

                case LEVEL_ONE_MY_RECORDS:
                state = STATE_RECLIST;
                break;

                case LEVEL_ONE_MY_RATING:
                state = STATE_MYRATING;
                break;

                case LEVEL_ONE_MUSICTAGLIST:
                state = STATE_TAGLIST;
                break;

                default:
                goto err;
            }
            break;

        case STATE_ALLMUSIC:
            switch (dir)
            {
                case LEVEL_TWO_LIST:
                state = STATE_ALLMUSIC_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_ARTIST:
            switch (dir)
            {
                case LEVEL_TWO_LIST:
                state = STATE_ARTIST_LIST;
                break;

                default:
                goto err;
            }
            break;

        case STATE_ALBUM:
            switch (dir)
            {
                case LEVEL_TWO_LIST:
                state = STATE_ALBUM_LIST;
                break;

                default:
                goto err;
            }
            break;

        case STATE_GENRE:
            switch (dir)
            {
                case LEVEL_TWO_LIST:
                state = STATE_GENRE_LIST;
                break;

                default:
                goto err;
            }
            break;

        case STATE_MYLIST:
            switch (dir)
            {
                case LEVEL_TWO_LIST:
                state = STATE_MYLIST_LIST;
                break;

                default:
                goto err;
            }
            break;

        case STATE_RECLIST:
            switch (dir)
            {
                case LEVEL_TWO_VOCREC:
                state = STATE_VOICEREC;
                break;

                case LEVEL_TWO_FMREC:
                state = STATE_FMREC;
                break;

                default:
                goto err;
            }
            break;

        case STATE_MYRATING:
            switch (dir)
            {
                case LEVEL_TWO_LIST:
                state = STATE_MYRATING_LIST;
                break;

                default:
                goto err;
            }
            break;

        case STATE_TAGLIST:
            switch (dir)
            {
                case LEVEL_TWO_LIST:
                state = STATE_TAGLIST_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_ARTIST_LIST:
            switch (dir)
            {
                case LEVEL_THREE_FILELIST:
                state = STATE_ARTIST_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_ALBUM_LIST:
            switch (dir)
            {
                case LEVEL_THREE_FILELIST:
                state = STATE_ALBUM_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_GENRE_LIST:
            switch (dir)
            {
                case LEVEL_THREE_FILELIST:
                state = STATE_GENRE_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_MYLIST_LIST:
            switch (dir)
            {
                case LEVEL_THREE_FILELIST:
                state = STATE_MYLIST_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_VOICEREC:
            switch (dir)
            {
                case LEVEL_THREE_FILELIST:
                state = STATE_VOICEREC_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_FMREC:
            switch (dir)
            {
                case LEVEL_THREE_FILELIST:
                state = STATE_FMREC_FILE;
                break;

                default:
                goto err;
            }
            break;

        case STATE_MYRATING_LIST:
            switch (dir)
            {
                case LEVEL_THREE_FILELIST:
                state = STATE_MYRATING_FILE;
                break;

                default:
                goto err;
            }
            break;

        default:
            goto err;
        }
        
        dir = _plistclass_get_next_direction(levelinfo);
    }

    return state;

    err: 
    print_err("error occur!,state is %d ", state);
    
    return STATE_NONE;
}

bool plistclass_init_level(plistclass_level_info_t* levelinfo)
{
    levelinfo->level_one = LEVEL_ONE_NONE_LIST;
    levelinfo->level_two = LEVEL_TWO_NONE_LIST;
    levelinfo->level_three = LEVEL_THREE_NONE_LIST;
    levelinfo->sortstyle = PINYINSORT;
    
    memcpy(&s_tmplist_levelinfo, levelinfo, sizeof(plistclass_level_info_t));

    return true;
}

bool plistclass_next_level(plistclass_level_info_t* levelinfo, int current, int next)
{
    //set next level
    if (levelinfo->level_one == LEVEL_ONE_NONE_LIST)
    {
        levelinfo->level_one = (level_one_e) current;
        levelinfo->level_two = (level_two_e) next;
    }
    else if (levelinfo->level_two == LEVEL_TWO_NONE_LIST)
    {
        levelinfo->level_one = (level_one_e) current;
        levelinfo->level_two = (level_two_e) next;
    }
    else if (levelinfo->level_three == LEVEL_THREE_NONE_LIST)
    {
        levelinfo->level_two = (level_one_e) current;
        levelinfo->level_three = (level_three_e) next;
    }
    else
    {
    }

    memcpy(&s_tmplist_levelinfo, levelinfo, sizeof(plistclass_level_info_t));

    return true;
}

bool plistclass_prev_level(plistclass_level_info_t* levelinfo)
{
    //clear last level
    if (levelinfo->level_three != LEVEL_THREE_NONE_LIST)
    {
        levelinfo->level_three = LEVEL_THREE_NONE_LIST;
    }
    else if (levelinfo->level_two != LEVEL_TWO_NONE_LIST)
    {
        levelinfo->level_two = LEVEL_TWO_NONE_LIST;
    }
    else
    {
        levelinfo->level_one = LEVEL_ONE_NONE_LIST;
    }

    memcpy(&s_tmplist_levelinfo, levelinfo, sizeof(plistclass_level_info_t));

    return true;
}

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
int plistclass_get_item_num(plistclass_type_e type)
{
    //TODO,ADD YOUR CODE HERE
    int result = 0;

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hnpmusic);

    switch (s_curfiletype)
    {
        case MUSIC:
        switch (type)
        {
            case PLIST_NOWPLAYING:
            result = plist_get_item_num(s_hnpmusic);
            break;

            case PLIST_SHOWLIST:
            result = plist_get_item_num(s_hshowlist);
            break;

            default:
            break;
        }
        break;

        case VIDEO:
        break;

        case IMAGE:
        break;

        case TXT:
        break;

        default:
        result = -1;
        break;
    }

    if ( result < 0 )
    {
        result = 0;
    }
    
    return result;
}


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
int plistclass_get_album_file_num( char *album )
{
    //TODO,ADD YOUR CODE HERE
    int result = 0;
    
    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hsubmusiclib);
    
    /*根据状态设置过滤条件*/
    s_tmplistfilter.field_num = 1;
    s_tmplistfilter.is_pattern_search = 0;
    s_tmplistfilter.filter_field[0].fieldno = ALBUM;
    s_tmplistfilter.filter_field[0].content.pval = album;
    
    result = plist_get_item_num_filter( s_hsubmusiclib, &s_tmplistfilter );
    if (result < 0)
    {
        print_err("result=%d",result);
        result = plist_get_item_num_filter( s_hsubmusiclib, &s_tmplistfilter );
    }

    return result;
}

/*******************************************************************
 Function : plistclass_get_files
 Brief    : 从显示列表获取条目，并且支持排序
 Input    :
 1)  plistclass_level_info_t *level_info
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
int plistclass_get_files(plistclass_level_info_t *level_info, int start, int num, plist_detail_info_t **pid3_info)
{
    int recordnum = 0;

    /*检查是否需要重新提取显示列表*/
    if ( (0 != _plistclass_cmplevelinfo(&s_tmplist_levelinfo, level_info))
            || (true == s_filter_flag))
    {
        memcpy(&s_tmplist_levelinfo, level_info, sizeof(plistclass_level_info_t));

        /*提取记录填充到显示列表*/
        _plistclass_fill_showlist(level_info);

        s_filter_flag = false;
    }

    if ( ( num <= 0 ) || ( NULL == pid3_info ) )
    {
        return recordnum;
    }

    if ( num > ID3INFOSIZE )
    {
        num = ID3INFOSIZE;
    }

    /*现在从列表中提取记录。*/
//    recordnum = plist_get_item_num(s_hshowlist);

    plist_free_buf(s_id3info, ID3INFOSIZE);
    plist_init_detail_info_struct(s_id3info, ID3INFOSIZE);

    recordnum = plist_get_items_to_mallocbuf(s_hshowlist, s_id3info, start, num, 0);
    if ( recordnum > 0 )
    {
        (*pid3_info) = s_id3info;
    }

    return recordnum;
}

bool plistclass_set_filter(char *filter_str)
{
    char* p = filter_str;
    char* q = filter_str;
    size_t src_len = 0;
    size_t dst_len = 0;

    memset(s_filter, 0, sizeof(s_filter));

    /*
     * trimming the leading & trailing blanks
     */
    if ((filter_str != NULL) && (strlen(filter_str) != 0))
    {
        p = filter_str;
        q = filter_str + strlen(filter_str) - 1;

        while (*p == ' ')
        {
            p++;
        }

        while ((q >= p) && (*q == ' '))
        {
            q--;
        }

        if (q >= p)
        {
            strncpy(s_filter, "*", sizeof(s_filter));  //add first star
            
            src_len = (size_t) strlen(s_filter) + 1;
            dst_len = (size_t) (q-p + 1);

            if( dst_len > (sizeof(s_filter) - src_len) )
            {
                dst_len = sizeof(s_filter) - src_len;
            }

            strncat(s_filter, p, dst_len);
            strncat(s_filter, "*", 1);  //add last star
        }
    }

    s_filter_flag = true;

    return true;
}

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
plist_detail_info_t * plistclass_get_id3_by_index(int cur_index)
{
    bool bret;
    char filepath[PATH_MAX];

    bret = music_get_filepath(filepath, sizeof(filepath));
    if( bret == false )
    {
        return NULL;
    }

    return plistclass_get_id3_by_fullpath(filepath);
}

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
plist_detail_info_t * plistclass_get_id3_by_fullpath(const char* fullpath)
{
    int cur_index;
    int result = -1;
    void *htable;
    const char *music_dir = NULL;    
    unsigned int music_len;
    unsigned int path_len;

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hsubmusiclib);
    _plistclass_init_table(&s_hextramusiclib);

    //check path
    if ( fullpath == NULL )
    {
        print_err();
        return NULL;
    }
    else
    {
        music_dir = sys_get_main_disk_dir(DIR_MUSIC);
        if ( music_dir == NULL )
        {
            return NULL;    //main disk is not exist
        }
        
        music_len = strlen(music_dir);
        path_len = strlen(fullpath);        
    }

    plist_free_buf(&s_id3info[0], 1);
    plist_init_detail_info_struct(&s_id3info[0], 1);

    //main disk : music
    if ( ( path_len >= music_len ) && ( memcmp(fullpath, music_dir, music_len) == 0 ) )
    {
        htable = s_hsubmusiclib;
    }
    else //other       
    {
        htable = s_hextramusiclib;        
    }
    
    cur_index = plist_name_to_index(htable, fullpath, 0);
    if (cur_index < 0)
    {
        //set extra table
        htable = s_hextramusiclib;
        
        //add file
        if ( htable != NULL )
        {
            result = plist_add_file(htable, fullpath);
            if (result < 0)
            {
                print_err();
            }
            
            cur_index = plist_name_to_index(htable, fullpath, 0); 
        }
           
        if (cur_index < 0)
        {
            print_err("fullpath = %s",fullpath);    
            result = plist_get_ID3_by_name(fullpath, &s_id3info[0]);
            if (result < 0)
            {
                print_err();
                return NULL;
            }
            else
            {
                return &s_id3info[0];
            }
        }
    }
    
    result = plist_get_items_to_mallocbuf(htable, &s_id3info[0], cur_index, 1, false);
    if (result < 0)
    {
        print_err();
        return NULL;
    }
    else
    {
        return &s_id3info[0];
    }
}

/*******************************************************************
 Function : plistclass_add_item_to_np
 Brief    : 将显示列表条目添加到正在播放列表
 Input    :
 1)  item
 添加条目
 Output   : int
 -1,    fail
 other, 实际获取到的条目数
 ********************************************************************/
bool plistclass_add_showlist_item_to_np(int cur_index)
{
    int result = -1;

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hnpmusic);

    plist_free_buf(&s_id3info[0], 1);
    plist_init_detail_info_struct(&s_id3info[0], 1);

    result = plist_get_items_to_mallocbuf(s_hshowlist, &s_id3info[0], cur_index, 1, 0);
    if (result < 0)
    {
        print_err("result=%d",result);
        return false;
    }

    result = plist_add_item(s_hnpmusic, &s_id3info[0]);

    return (result == 0);
}

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
int plistclass_add_showlist_to_np(void)
{
    int result = 0;
    plistclass_state_e state = STATE_NONE;
    int rec_num;
    int mask = 0;

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hnpmusic);

    switch (s_curfiletype)
    {
        case MUSIC:
        //table清空
        _plistclass_clear_table(&s_hnpmusic);
        if (NULL == s_hnpmusic)
        {
            print_err("open nplist fail!");
            break;
        }

        state = plistclass_get_state(&s_tmplist_levelinfo);

        switch (state)
        {
            case STATE_ALLMUSIC_FILE:
            case STATE_ARTIST_FILE:
            case STATE_GENRE_FILE:
            case STATE_VOICEREC_FILE:
            case STATE_FMREC_FILE:
            case STATE_MYRATING_FILE:
            mask = TITLE;
            break;
            
            case STATE_ALBUM_FILE:
            mask = TRACK;
            break;
            
            case STATE_MYLIST_FILE:
            case STATE_TAGLIST_FILE:
            mask = -1;
            break;

            default:
            mask = -1;
            break;
        }
        
        rec_num = plist_get_item_num(s_hshowlist);
        result=plist_transfer_items(s_hshowlist, s_hnpmusic, mask, 0, rec_num);
        if (result < 0)
        {
            print_err("result=%d",result);
            return result;
        }
        break;

        case VIDEO:
        break;

        case IMAGE:
        break;

        case TXT:
        break;

        default:
        result = -1;
        break;
    }
    
    return result;

    //TODO,ADD YOUR CODE HERE
}

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
int plistclass_add_fullpath_to_np(char *fullpath)
{
    int result = 0;
    char path[PATH_MAX];

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hnpmusic);
    
    //table清空
    _plistclass_clear_table(&s_hnpmusic);
    
    if (NULL == s_hnpmusic)
    {
        print_err("open nplist fail!");
        return -1;
    }

    strcpy(path, fullpath);
    result = plist_add_dir( s_hnpmusic, dirname(path) );
    if (result < 0)
    {
        print_err("add dir fail!");
    }

    return result;
}

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
int plistclass_del_items_from_showlist(int start,int num)
{
    //TODO,ADD YOUR CODE HERE
    int result = 0;
    plistclass_state_e state;
    int cur_index;

    state = plistclass_get_state(&s_tmplist_levelinfo);

    //MTP播放列表内容不允许删除
    if ( state == STATE_MYLIST_FILE)
    {
        cur_index = s_rating_content.rating - 1;

        if (cur_index >= MAX_MYLIST)
        {
            return -1;
        }
    }
        
    switch (s_curfiletype)
    {
        case MUSIC:
        if (-1 == start)
        {
            start = 0;
            num = plist_get_item_num(s_hshowlist);
        }
        result = plist_del_items(s_hshowlist, start, num);
        break;

        case VIDEO:
        break;

        case IMAGE:
        break;

        case TXT:
        break;

        default:
        result = -1;
        break;
    }

    return result;
}

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
int plistclass_add_dirlist_to_np(void *handle, int start, int num)
{
    int result = 0;

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hnpmusic);

    switch (s_curfiletype)
    {
        case MUSIC:
        //table清空
        _plistclass_clear_table(&s_hnpmusic);
        result = plist_add_dirlist(s_hnpmusic, handle, 0);
        break;

        case VIDEO:
        break;

        case IMAGE:
        break;

        case TXT:
        break;

        default:
        break;
    }

    return result;
}

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
bool plistclass_add_item_to_bmlist(plist_detail_info_t *item)
{
    int ret = 0;
    int total = 0;

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hblmusic);

    //get display list number
    total = plist_get_item_num(s_hblmusic);
    if (total >= BOOKMARKSIZE)
    {
        //delete first item
        plist_del_items(s_hblmusic, 0, 1);
    }

    ret = plist_add_item(s_hblmusic, item);
    print_dbg("ret:%d", ret);

    return (ret == 0);
}

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
bool plistclass_add_item_to_mylist(int mylist_no, plist_detail_info_t *item)
{
    int ret = false;

    if ( ( mylist_no <= 0 ) || ( mylist_no > MAX_MYLIST ) )
    {
        return false;
    }

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hmymusic[mylist_no - 1]);

    ret = plist_add_item(s_hmymusic[mylist_no - 1], item);
    print_dbg("ret:%d", ret);

    return (ret == 0);
}

/********************************************************************************
 函数说明 : 改变指定位置文件的星级

 Input    :
 1)  const char *fullpath
 书签文件的全路径
 2) int rating
 文件的星级, 0-5 , 0为未分级
 Output   : bool
 0,      fail
 1,       success
 注意:
 ********************************************************************************/
bool plistclass_change_rating(const char *fullpath, int rating)
{
    plist_detail_info_t detailinfo;
    plist_modifyfield_t modify_field;
    int offset;
    int result = 0;
    int num;

    //table初始化( 内部只做1次 )
    _plistclass_init_table(&s_hsubmusiclib);
    _plistclass_init_table(&s_hmedia);

    detailinfo.rating = rating;

    modify_field.num = 1;
    modify_field.nfield[0] = RATING;

    //change music sub lib
    offset = plist_name_to_index(s_hsubmusiclib, fullpath, 0);
    if ( offset < 0 )
    {
        return false;
    }
    result = plist_modify_item(s_hsubmusiclib, offset, &detailinfo, &modify_field);

    //change media lib
    offset = plist_name_to_index(s_hmedia, fullpath, 0);
    plist_modify_item(s_hmedia, offset, &detailinfo, &modify_field);

    //set level lib invalid
    for (num = 0; num < MAX_MYRATE; num++)
    {
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hrate[num]);
        plist_set_table_invalid(s_hrate[num]);
    }

    return true;
}

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
bool plistclass_store_usrselcontent(plist_detail_mask_t mask, plistclass_usrselcontent_u *content)
{
    int result = 0;

    switch (mask)
    {
        case ALBUM:
        memcpy(&s_album_content, content, sizeof(plistclass_usrselcontent_u));
        break;

        case ARTIST:
        memcpy(&s_artist_content, content, sizeof(plistclass_usrselcontent_u));
        break;

        case GENRE:
        memcpy(&s_genre_content, content, sizeof(plistclass_usrselcontent_u));
        break;

        case FULLPATH:
        memcpy(&s_path_content, content, sizeof(plistclass_usrselcontent_u));
        break;

        case RATING:
        memcpy(&s_rating_content, content, sizeof(plistclass_usrselcontent_u));
        break;

        default:
        result = -1;
        break;
    }

    return result;
}

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
bool plistclass_sync_to_fs(char *fullpath)
{
    int result = 0;
    plist_filterfield_t filter;
    int num;

    if ( fullpath == NULL )
    {
        return false;
    }
        
    filter.field_num = 1;
    filter.is_pattern_search = 0;
    filter.filter_field[0].fieldno = FULLPATH;
    filter.filter_field[0].content.pval = fullpath;

    switch (s_curfiletype)
    {
        case MUSIC:
        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hmedia);
        _plistclass_init_table(&s_hsubmusiclib);
        _plistclass_init_table(&s_hnpmusic);
        _plistclass_init_table(&s_hblmusic);
        
        //delete item
        plist_filter_del_item(s_hmedia, &filter);
        plist_filter_del_item(s_hsubmusiclib, &filter);
        plist_filter_del_item(s_hnpmusic, &filter);
        plist_filter_del_item(s_hblmusic, &filter);
        for (num = 0; num < MAX_MYLIST; num++)
        {
            //table初始化( 内部只做1次 )
            _plistclass_init_table(&s_hmymusic[num]);
            plist_filter_del_item(s_hmymusic[num], &filter);
        }

        //table初始化( 内部只做1次 )
        _plistclass_init_table(&s_hartist);
        _plistclass_init_table(&s_halbum);
        _plistclass_init_table(&s_hgenre);
        _plistclass_init_table(&s_htmplist);
        _plistclass_init_table(&s_hsearch);

        //set invalid sign
        plist_set_table_invalid(s_hartist);
        plist_set_table_invalid(s_halbum);
        plist_set_table_invalid(s_hgenre);
        plist_set_table_invalid(s_htmplist);
        plist_set_table_invalid(s_hsearch);
        for (num = 0; num < MAX_MYRATE; num++)
        {
            //table初始化( 内部只做1次 )
            _plistclass_init_table(&s_hrate[num]);
            plist_set_table_invalid(s_hrate[num]);
        }
        break;

        case VIDEO:
        break;

        case IMAGE:
        break;

        case TXT:
        break;

        default:
        result = false;
        break;
    }

    return true;
    //TODO,ADD YOUR CODE HERE
}

int plistclass_get_mtplist_num(void)
{
    return plist_get_listnum_by_type(MUSICMTP);
}

int plistclass_get_mtplist_name(plist_name_t *buf, int start, int num)
{
    return plist_get_listname_by_type(MUSICMTP, buf, start, num);
}

