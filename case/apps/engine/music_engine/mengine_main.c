/************************************************************************
*                             USDK1100                                
*                            music engin                              
*                                                                      
*                (c) Copyright 2003-2015, Actions Co,Ld.              
*                          All Right Reserved                         
*                 
*
* File: mengin_main.c                                               
* By  : JerryFu                                                       
* Version: 1> v1.00     first version     2009-03-10                   
************************************************************************/

/************************************************************************
*                                                                  
*                           head    file                              
*                                                                     
************************************************************************/
#include "include_case.h"


/************************************************************************
*                                                                   
*                      global  variable                             
*                                                                    
************************************************************************/
typedef struct app_mengine_s
{
    /* system resource */
    void * applib_lib;
    void *mmm_lib;     //mmm so handle
    void *config_lib;
    void *sqlite_lib;
    void *plist_lib;
    void *mnavi_lib;

}app_mengine_t;

static bool _lib_init(app_mengine_t *app_mengine);
static bool _lib_deinit(app_mengine_t *app_mengine);

static bool _app_init( app_mengine_t *app_mengine );
static bool _app_deinit( app_mengine_t *app_mengine );


/************************************************************************
*                                                                    
*                 add  function here                                           
*                                                                   
************************************************************************/

/*************************************************************************
* Function:            insmod_dsp_dec                              
* Description:        加载音频解码驱动                               
* para1：            none                                            
* Return：           none                                             
**************************************************************************/
static void insmod_dsp_dec(void)
{
    insmod("dspdec.ko",0);
    sys_mknod(DEVICE_DSP_AD, S_IFCHR, makedev(MAJOR_DSP_AD, 0));
}

/*************************************************************************
* Function:            rmmod_dsp_dec                              
* Description:        卸载音频解码驱动                               
* para1：            none                                            
* Return：           none                                             
**************************************************************************/
static void rmmod_dsp_dec(void)
{
    int ret;
    ret = rmmod("dspdec.ko",0);
    print_dbg("dspdec ret=%d",ret);
}

/*************************************************************************
* Function:            music_engin_cb                                  
* Description:        处理其他进程发给music engin的消息                
* para1：              none                                            
* Return：             int                                             
*************************************************************************/
static bool mengine_cb(const msg_apps_t *pmsg)
{
    bool retval = 0;
    
    //update
    mengine_update();
    
    switch(pmsg->type)
    {
    /* get status command */    
    case  MSG_MENGINE_GET_ENGINE_INFO_SYNC:
        retval = mengine_get_engine_info(pmsg);
        break;

    case  MSG_MENGINE_GET_FILEPATH_SYNC:
        retval = mengine_get_filepath(pmsg);
        break;
        

    /* set config command */
    case  MSG_MENGINE_SET_EQ_SYNC:
        retval = mengine_set_eq(pmsg);
        break;
        
    case  MSG_MENGINE_SET_PLAYMODE_SYNC:
        retval = mengine_set_playmode(pmsg);
        break;
        
    case  MSG_MENGINE_SET_DISPMODE_SYNC:
        retval = mengine_set_displaymode(pmsg);
        break;
        
    case  MSG_MENGINE_SET_FADE_SYNC:
        retval = mengine_set_fade(pmsg);
        break;
        
    case  MSG_MENGINE_SET_PLAYSPEED_SYNC:
        retval = mengine_set_playspeed(pmsg);
        break;
         
    case  MSG_MENGINE_SET_AB_AUTO_SYNC:
        retval = mengine_set_ab_auto(pmsg);
        break;
        
    case  MSG_MENGINE_SET_AB_COUNT_SYNC:
        retval = mengine_set_ab_count(pmsg);
        break;
        
    case  MSG_MENGINE_SET_AB_TIME_SYNC:
        retval = mengine_set_ab_time(pmsg);
        break;

        
    /* ab control command */  
    case  MSG_MENGINE_START_AB_SYNC:
        retval = mengine_start_ab(pmsg);
        break;
        
    case  MSG_MENGINE_CLEAR_AB_SYNC:
        retval = mengine_clear_ab(pmsg);
        break;
        
    case  MSG_MENGINE_SET_AB_NEXT_SYNC:
        retval = mengine_set_ab_next(pmsg);
        break;
        
    case  MSG_MENGINE_SET_AB_PREV_SYNC:
        retval = mengine_set_ab_prev(pmsg);
        break;

        
    /* normal play control command */  
    case  MSG_MENGINE_PLAY_SYNC:
        retval = mengine_play(pmsg);
        break;
          
    case  MSG_MENGINE_STOP_SYNC:
        retval = mengine_stop(pmsg);
        break;
          
    case  MSG_MENGINE_PAUSE_SYNC:
        retval = mengine_pause(pmsg);
        break;
          
    case  MSG_MENGINE_RESUME_SYNC:
        retval = mengine_resume(pmsg);
        break;
          
    case  MSG_MENGINE_SEEK_SYNC:
        retval = mengine_seek(pmsg);
        break;
          
    case  MSG_MENGINE_SET_FILEPATH_SYNC:
        retval = mengine_set_filepath(pmsg);
        break;
          
    case  MSG_MENGINE_SET_FILEINDEX_SYNC:
        retval = mengine_set_fileindex(pmsg);
        break;
        
    case  MSG_MENGINE_PLAY_NEXT_SYNC:
        retval = mengine_play_next(pmsg);
        break;
        
    case  MSG_MENGINE_PLAY_PREV_SYNC:
        retval = mengine_play_prev(pmsg);
        break;       
    
    case  MSG_MENGINE_DELETE_FILE_SYNC:
        retval = mengine_delete_file(pmsg);
        break;
        
        
    /* fast play control command */  
    case  MSG_MENGINE_FFWD_SYNC:
        retval = mengine_fast_forward(pmsg);
        break;
          
    case  MSG_MENGINE_FBWD_SYNC:
        retval = mengine_fast_backward(pmsg);
        break;
          
    case  MSG_MENGINE_CANCEL_FFB_SYNC:
        retval = mengine_cancel_ffb(pmsg);
        break;
          
    case  MSG_MENGINE_SET_FFBSTEP_SYNC:
        retval = mengine_set_ffb_step(pmsg);
        break;
          
    case  MSG_MENGINE_SET_FFBTIME_SYNC:
        retval = mengine_set_ffb_time(pmsg);
        break;
        
    /* playlist control command */  
    case  MSG_MENGINE_MAKE_BOOKMARK_SYNC:
        retval = mengine_make_bookmark(pmsg);
        break;    
        
    case  MSG_MENGINE_SET_LAST_FILE_SYNC:
        retval = mengine_set_last_file(pmsg);
        break;    
    
    case MSG_APP_QUIT:
        exit_msg_loop();
        break;
        
    case MSG_GSENSOR_SHAKED:
        print_info("received msg: MSG_GSENSOR_SHAKED");
        mengine_gsensor_shaked(pmsg);
        break;

    default:
        break;
    }

    //update
    mengine_update();
    
    return retval;
}

static bool _lib_init(app_mengine_t *app_mengine)
{
    app_mengine->applib_lib = dlopen("applib.so", RTLD_NOW);
    if(app_mengine->applib_lib == NULL)
    {
        print_err("open applib err! \n");
        goto _lib_init_err;
    }

    app_mengine->mmm_lib = dlopen("a_codec.so", RTLD_NOW);
    if(app_mengine->mmm_lib == NULL)
    {
        print_err("open mmm lib err! \n");
        goto _lib_init_err;
    }

    app_mengine->config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (app_mengine->config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }

    app_mengine->sqlite_lib = dlopen("sqlite.so", RTLD_LAZY);
    if(app_mengine->sqlite_lib == NULL)
    {
        print_err("open sqlite_lib err! \n");
        goto _lib_init_err;
    }

    app_mengine->plist_lib = dlopen("playlist.so", RTLD_LAZY);
    if(app_mengine->plist_lib == NULL)
    {
        print_err("open plist_lib err! \n");
        goto _lib_init_err;
    }
    
    app_mengine->mnavi_lib = dlopen("mnavi.so", RTLD_LAZY);
    if(app_mengine->mnavi_lib == NULL)
    {
        print_err("open mnavi_lib err! \n");
        goto _lib_init_err;
    }
    
    return true;

    _lib_init_err:
    _lib_deinit(app_mengine);
    return false;
}

static bool _lib_deinit(app_mengine_t *app_mengine)
{

    if(app_mengine->applib_lib != NULL)
    {
        dlclose(app_mengine->applib_lib);
        app_mengine->applib_lib = NULL;
    }

    if (app_mengine->mmm_lib != NULL)
    {
        dlclose(app_mengine->mmm_lib);
        app_mengine->mmm_lib = NULL;
    }
    if (app_mengine->config_lib != NULL)
    {
        dlclose(app_mengine->config_lib);
        app_mengine->config_lib = NULL;
    }
    if (app_mengine->sqlite_lib != NULL)
    {
        dlclose(app_mengine->sqlite_lib);
        app_mengine->sqlite_lib = NULL;
    }
    if (app_mengine->plist_lib != NULL)
    {
        dlclose(app_mengine->plist_lib);
        app_mengine->plist_lib = NULL;
    }
    if (app_mengine->mnavi_lib != NULL)
    {
        dlclose(app_mengine->mnavi_lib);
        app_mengine->mnavi_lib = NULL;
    }
    return true;
}

/* application initial function */
/*************************************************************************
* Function:            music_engin_cb
* Description:        处理其他进程发给music engin的消息
* para1：              none
* Return：             int
*************************************************************************/
static bool _app_init( app_mengine_t *app_mengine )
{
    //insmod
    insmod_dsp_dec();

    //init codec
    mengine_init();
    
    return true;

}


/* application exit function */
static bool _app_deinit( app_mengine_t *app_mengine )
{
    //deinit codec
    mengine_deinit();
    
    //rmmode
    rmmod_dsp_dec();

    return true;
}

/*************************************************************************
*                                                                       
*                     全局函数原型                                      
*                                                                      
*************************************************************************/
int main(int argc, const char *argv[])
{
    app_mengine_t app_mengine;
    msg_apps_t  msg;

    memset(&app_mengine,0,sizeof(app_mengine_t));

    _lib_init( &app_mengine);
    /* 应用库初始化 */
    if( applib_init(argc,argv,APP_TYPE_CONSOLE) == false )
    {
        print_err("applib init error! \n");
        goto applib_init_err;
    }

    /* 应用初始化 */
    if( _app_init( &app_mengine ) == false )
    {
        print_err("app init error! \n");
        goto app_init_err;
    }

    /* 应用消息循环 */
    register_sys_dispatcher((sys_msg_proc)mengine_cb);

    while(get_msg(&msg) == TRUE)
    {
        dispatch_msg(&msg);        
    }                   
    unregister_sys_dispatcher();

    /* 应用退出 */
    if( _app_deinit( &app_mengine ) == false )
    {
        print_err("app exit error! \n");        
        goto app_deinit_err;
    }

    /* 应用库退出 */
    applib_quit();
    _lib_deinit( &app_mengine);

    return 0;

    app_deinit_err: 
    app_init_err:
    applib_quit();
    
    applib_init_err:
    _lib_deinit( &app_mengine);
    return -1;
}

