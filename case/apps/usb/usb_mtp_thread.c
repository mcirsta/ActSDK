/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : mtp_thread.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     mtp_thread.c
* \brief    mtp thread
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/01
*******************************************************************************/

#include "usb.h"
#include "usb_mtp_thread.h"
#include "mtp.h"

static char s_playlist_db[ PATH_MAX  ];
static char s_mtp_storage_path[ PATH_MAX  ];

static void* s_mtp_dev = NULL;
static pthread_t s_pthread_id;
static bool s_is_running = false;

/* mtp 修改数据的数目 */
static unsigned int s_change_count = 0;

bool start_mtp_thread(void);
void stop_mtp_thread(void);
int get_mtp_status(void);
bool is_running(void);

static void* _start_mtp_run( void* arg );
static int _mtp_callback(void *pMtp,void *pPara,int type);

unsigned int get_mtp_change_count( void );

static mtp_thread_t s_mtp_thread_api = 
{
    start_mtp_thread,
    stop_mtp_thread,

    is_running,
    get_mtp_status,

    get_mtp_change_count
};

bool start_mtp_thread(void)
{
    int ret;
    pthread_attr_t attr;
    const char *dir;
    struct sched_param param;
    int policy;

    print_dbg(" **********************       start_mtp_thread         ********************************* ");

    ret = (int)strlen( s_playlist_db );
    if ( ret == 0 )
    {
        /* 如果没有配置值 */
        dir = sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST);
        if ( dir != NULL )
        {
            strcpy( s_playlist_db, dir );
            strcat( s_playlist_db, PLAYLIST_DB );
        }
    }

    print_dbg(" s_playlist_db = %s ", s_playlist_db );
    
    ret = mtp_init( &s_mtp_dev, s_playlist_db );
    if (ret < 0)
    {
        print_err(" mtp_init error !!!!!!!!!!! ");
        return false;
    }

    ret = (int)strlen( s_mtp_storage_path );
    if ( ret == 0 )
    {
        /* 如果没有配置值 */
        strcpy( s_mtp_storage_path, UDISK_ROOT );
    }

    ret = mtp_set_store_root_path( s_mtp_dev, MTP_STORE_ID_NAND, s_mtp_storage_path );
    if (ret < 0)
    {
        print_err(" mtp_set_store_root_path error = %d ", ret );
        return false;
    }

    ret  = mtp_set_store_dev ( s_mtp_dev, MTP_STORE_ID_NAND, DEVICE_FLASH "u" );

    print_dbg(" mtp_init succeed ...................... ");

    /* 设置线程属性 */
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_getschedparam( pthread_self(), &policy, &param );
    param.sched_priority = param.sched_priority - 2;
    pthread_attr_setschedparam(&attr, &param);

    ret = pthread_create( &s_pthread_id, &attr, _start_mtp_run, NULL );
    if( ret != 0 )
    {
        print_err( "can’t create thread: %s \n", strerror( ret ) );
        pthread_attr_destroy( &attr );

        return false;
    }

    pthread_attr_destroy( &attr );
    
    return true;
}

static void* _start_mtp_run( void* arg )
{
    int ret;
    
    print_dbg(" mtp is running.......... ");

    s_is_running = true;
    ret = mtp_run( s_mtp_dev, DEVICE_TUNNEL,DEVICE_MTPDRM,DEVICE_VRAM, _mtp_callback);

    ret = mtp_get_obj_modify_cnt( s_mtp_dev, &s_change_count );
    if ( ret < 0 )
    {
        print_err( "mtp_get_obj_modify_cnt()  faild ........" );
    }

    mtp_exit( s_mtp_dev );

    print_dbg(" mtp thread exit ................ ");

    return NULL;
}

void stop_mtp_thread(void)
{
    mtp_stop( s_mtp_dev );
}

int get_mtp_status(void)
{
    return mtp_get_running_status( s_mtp_dev ); 
}

bool is_running(void)
{
    return s_is_running;
}

unsigned int get_mtp_change_count( void )
{
    return s_change_count;
}

static int _mtp_callback(void *pMtp,void *pPara,int type)
{    
    int ret;        
    ret=0;

    return ret;
}


mtp_thread_t* create_mtp_thread(void)
{
    return &s_mtp_thread_api;
}


