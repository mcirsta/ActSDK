/************************************************************************
*                             USDK1100                                
*                            music engin                              
*                                                                      
*                (c) Copyright 2003-2015, Actions Co,Ld.              
*                          All Right Reserved                         
*                 
*
* File: mengin_playlist.c                                               
* By  : JerryFu                                                       
* Version: 1> v1.00     first version     2009-03-10                   
************************************************************************/

/************************************************************************
*                                                                  
*                           head    file                              
*                                                                     
************************************************************************/
#include "include_case.h"


#include "mengine_playlist.h"

/************************************************************************
*                                                                     
*                       const  defination                            
*                                                                     
************************************************************************/

/************************************************************************
*                                                                     
*                      micro    defination                           
*                                                                   
************************************************************************/

//对于正在播放列表（具有path,bookmark字段）,音乐有一个,视频有一个，图片有一个，电子书有一个。

/*正在播放列表列表类型*/
#define MUSICNPLIST   MUSICRESERVE2

/************************************************************************
*                                                                   
*                      global  variable                             
*                                                                    
************************************************************************/

/*音乐正在播放列表句柄*/
static void* l_hnpmusic=NULL;

/*正在播放列表的名称*/
static char* l_sznpmusictbl="npmusiclist";

/*正在播放列表的结构对于不同类型的文件来说是相同的，即只有全路径字段和书签信息字段。*/
static plist_trivefield_t l_nplist_field;

/*暂存显示列表中记录的buffer*/
static plist_detail_info_t l_id3info;

/*plist init flag*/
static bool l_plist_inited = false;

/* fsel */
static FSEL_HANDLE s_fsel = NULL;

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


bool _fsel_init( char *filepath )
{
    char path[PATH_MAX];
    int result;
    char ext_music[PATH_MAX];
/*
    if ( s_fsel != NULL )
    {
        fsel_close(s_fsel);
        s_fsel = NULL;
    }
*/    
    if ( s_fsel == NULL )
    {
        //open
        s_fsel = fsel_open();
        if ( s_fsel == NULL )
        {
            print_err();
            return false;
        }    
        
        /* set ext */
        result = get_config( CFG_APP_SUPPORT_EXT_MUSIC, (char*)ext_music, sizeof(ext_music) );
        if( result < 0 )
        {
            print_err("get music ext failed!");
        } 
        else
        {
            fsel_setfilter( s_fsel, ext_music );
        }        
        
        //set subdir
        fsel_subdir_setting(s_fsel, FSEL_NOSUB, FSEL_FILENAME);   

        //sort
        fsel_sortstyle(s_fsel, FSEL_PINYIN);
    }

    //set dir
    strcpy(path, filepath);
    result = fsel_setworkdir(s_fsel, dirname(path));  
    if( result < 0 )
    {
        print_err("set workdir: %s failed!", dirname(path));
    } 

    l_id3info.fullpath = NULL;
    
    return true;
}

bool _fsel_deinit( void )
{
    if ( s_fsel != NULL )
    {
        fsel_close(s_fsel);
        s_fsel = NULL;
    }
    
    if ( l_id3info.fullpath != NULL )
    {
        free( l_id3info.fullpath );
        l_id3info.fullpath = NULL;
    }
    
    return true;
}

bool _plist_init( void )
{    
    if ( l_plist_inited == true )
    {
        return true;
    }
        
    /*设置数据库路径*/
    plist_set_rootdir(sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST));

    /*初始化播放列表*/
    if( -1 == plist_init() )
    {
        print_err("playlist init fail!");
        return false;
    }

    /*初始化列表结构*/ 
    /*正在播放列表的结构对于不同类型的文件来说是相同的，即只有全路径字段和书签信息字段。*/
    l_nplist_field.num = 2;
    l_nplist_field.nfield[0] = FULLPATH;
    l_nplist_field.nfield[1] = MARK;
    
    /*打开正在播放列表*/
    l_hnpmusic = plist_open( l_sznpmusictbl, MUSICNPLIST, &l_nplist_field) ;
    if(NULL==l_hnpmusic)
    {
        print_err("playlist open fail!");
        return false;
    }
    
    /*初始化记录buffer*/
    plist_init_detail_info_struct( &l_id3info, 1 );   

    l_plist_inited = true;
    
    return true;
}

bool _plist_deinit( void )
{
    if ( l_plist_inited == false )
    {
        return true;
    }
        
    /*释放记录buffer*/
    plist_free_buf( &l_id3info, 1 );   
    
    /*关闭正在播放列表*/
    if( l_hnpmusic != NULL )
    {
        plist_close( l_hnpmusic );
        l_hnpmusic = NULL;
    }
    
    /*退出播放列表*/
    if( -1 == plist_quit() )
    {
        print_err("playlist quit fail!");
        return false;
    }

    l_plist_inited = false;

    return true;
}

/************************************************************************
*                                                                    
*                 add  function here                                           
*                                                                   
************************************************************************/
bool nplist_init( bool fsel_mode, char *path )
{
    if ( fsel_mode == true )
    {
        _plist_deinit();
        return _fsel_init( path );
    }
    else
    {
        _fsel_deinit();
        return _plist_init();
    }    
}

bool nplist_quit( void )
{
    _fsel_deinit();
    _plist_deinit();
    
    return true;
}

int nplist_get_item_num( void )
{
    if( s_fsel != NULL )
    {
        return fsel_getfilenum( s_fsel, FSEL_FILENUM );
    }
    else
    {
        return plist_get_item_num( l_hnpmusic );
    }    
}

int nplist_get_index_by_fullpath( char* fullpath )
{
    int idx;
    
    if( s_fsel != NULL )
    {
        idx = fsel_getidbyname( s_fsel, fullpath );        
        if( idx <= 0 ) //文件不存在
        {
            fsel_update(s_fsel, (char*) fullpath, FSEL_DELDATA);
        }
        return ( idx - 1 );
    }
    else
    {
        return plist_name_to_index( l_hnpmusic, fullpath, 0 );
    }    
}

plist_detail_info_t* nplist_get_item_by_index( int idx )
{
    int result;
    char path[PATH_MAX];
    fsel_filename_t filename;

    /*释放记录buffer*/
    plist_free_buf( &l_id3info, 1 );   
    
    /*初始化记录buffer*/
    plist_init_detail_info_struct( &l_id3info, 1 );   
    
    if( s_fsel != NULL )
    {
        result = fsel_getfullpath(s_fsel, path, idx+1);
        if(result < 0)
        {
            print_err();
        }
        else
        {
            result = fsel_listfile(s_fsel, &filename, idx+1, 1, FSEL_POSITIVE);
            if(result < 0)
            {
                print_err();
            }
            else
            {
                strcat(path, filename.name);
                l_id3info.fullpath = strdup(path);
            }
        }  
    }
    else
    {
        plist_get_items_to_mallocbuf( l_hnpmusic, &l_id3info, idx, 1, false );
    }    

    return &l_id3info;
}

bool nplist_del_item_by_index( int idx )
{
    int result = -1;
    plist_detail_info_t *id3_info;

    if( s_fsel != NULL )
    {
        id3_info = nplist_get_item_by_index( idx );
        if ( id3_info != NULL )
        {
            result = fsel_update(s_fsel, (char*)id3_info->fullpath, FSEL_DELDATA);
        }
        return ( result == 0 );
    }
    
    result = plist_del_items( l_hnpmusic, idx, 1 );
    if( result < 0 )
    {
        print_err("playlist delete fail!");
        return false;
    }

    return true;
}

bool nplist_del_item_by_fullpath( char* fullpath )
{
    int result;
    plist_filterfield_t filter;

    if( s_fsel != NULL )
    {
        result = fsel_update(s_fsel, (char*) fullpath, FSEL_DELDATA);        
        return ( result == 0 );
    }
    
    filter.field_num = 1;
    filter.is_pattern_search = 0;
    filter.filter_field[0].fieldno = FULLPATH;
    filter.filter_field[0].content.pval = fullpath;

    //delete item
    result = plist_filter_del_item(l_hnpmusic, &filter);
    if( result < 0 )
    {
        print_err("playlist delete fail!");
        return false;
    }

    return true;
}

