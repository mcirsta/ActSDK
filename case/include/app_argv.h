/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : app_argv.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-6-11             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file            app_argv.h
 * \brief           应用程序启动参数定义
 *
 * \description     启动参数是指传递给应用程序的入口函数main()的argv参数，应用程序可以根据
 *                  不同的参数进入不同的工作模式。启动参数由process manager进程在启动一个应用
 *                  进程时传入。当一个普通应用进程请求process manager 启动另外一个应用进程时，
 *                  可以通过消息将希望的启动参数发送给process manager, process manager 将其传递给
 *                  新启动应用的main函数，从而决定新启动应用的工作模式。
 *                  本文件通过宏定义所有应用程序支持的启动参数。
 *
 * \node            启动参数必须是字符串形式，一个应用可允许同时传入多个启动参数，参数之间以空格隔开
 *
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/06/11
 *******************************************************************************/



#ifndef __APP_ARGV_H__
#define __APP_ARGV_H__


/******************************************************************************/
/*!                    
 * \par  Description:
 *    通用应用启动参数定义
 *    APPLICATION_ARGV_RESUME    通知应用程序以恢复模式启动
 * \code  
 * \endcode
 *******************************************************************************/
#define APPLICATION_ARGV_RESUME             "resume" 


/******************************************************************************/
/*!                    
 * \par  Description:
 *    Laucher应用启动参数定义 
 *    LAUNCHER_ARGV_CONSOLE : 通知Launcher 以后台方式启动，若不以后太方式启动，则不用穿该参数。
 *        
 * \code  
 *    //process manager 启动Laucnher应用时指定是否以后台方式启动。
 *    msg_app_t msg; 
 *    msg.type = MSG_CREAT_APP;
 *    sprintf(msg.content, "launcher.app %s", LAUNCHER_ARGV_CONSOLE);
 *    send_async_msg("manager", &msg);
 *
 *    //Launcher应用的main函数
 *    int main(int argc, char * argv[])
 *    {
 *        ...
 *        if (argc > 1)
 *        {
 *              // if load launcher by console mode then desktop scene will be create by background.
 *              if (!strcasecmp(argv[1], LAUNCHER_ARGV_CONSOLE))
 *              {
 *                  app_data.scene_desktop_data.is_console = true;
 *              }
 *              else
 *              {
 *                  app_data.scene_desktop_data.is_console = false;
 *              }
 *        }
 *        else
 *        {
 *            app_data.scene_desktop_data.is_console = false;
 *        }
 *        ...
 *    }
 *    
 * \endcode
 *******************************************************************************/
#define LAUNCHER_ARGV_CONSOLE             "console" 




/******************************************************************************/
/*!                    
 * \par  Description:
 *    usb应用启动参数定义 
 *    USB_ARGV_CHARGE : 指定usb应用程序工作于充电模式
 *    USB_ARGV_UDISK  : 指定usb应用程序工作与U盘模式            
 * \code  
 *    //请求process manager 启动USB应用，并指定其工作于充电模式
 *    msg_app_t msg; 
 *    msg.type = MSG_CREAT_APP;
 *    sprintf(msg.content, "usb.app %s", USB_ARGV_CHARGE);
 *    send_async_msg("manager", &msg);
 *
 *    //USB应用的main函数
 *    int main(int argc, char * argv[])
 *    {
 *        if(argc == 2)
 *        {
 *             // argv[0] 代表本应用名，从argv[1]开始是启动参数
 *             if(strcmp(agrv[1], USB_ARGV_CHARGE) == 0)
 *             {
 *                 //充电工作模式
 *             }
 *             else if(strcmp(agrv[1], USB_ARGV_UDISK) == 0)
 *             {
 *                //u盘工作模式
 *             }
 *             else 
 *             {
 *                //默认工作模式
 *             }
 *        }
 *        else 
 *        {
 *                //默认工作模式
 *        }
 *    }
 *    
 * \endcode
 *******************************************************************************/
#define USB_ARGV_UDISK               "udisk" 
#define USB_ARGV_HOST                "host" 
#define USB_ARGV_MTP                 "mtp" 
#define USB_ARGV_CHARGE              "charge" 
#define USB_ARGV_CHARGE_PLAY         "charge_play" 



/******************************************************************************/
/*!                    
 * \par  Description:
 *    music应用启动参数定义 
 *    MUSIC_ARGV_PLAYING : 指定music应用程序进入playing 播放界面
 *    MUSIC_ARGV_LIST    : 指定music应用程序进入list 界面          
 * \code  
 *    //请求process manager 启动music应用，并指定其直接进入播放界面，并播放指定文件
 *    msg_app_t msg; 
 *    msg.type = MSG_CREAT_APP;
 *    sprintf(msg.content, "music.app %s \"%s\" ", MUSIC_ARGV_PLAYING, filepath);
 *    send_async_msg("manager", &msg);
 *
 *    //music应用的main函数
 *    int main(int argc, char * argv[])
 *    {
 *        if(argc == 3)
 *        {
 *             // argv[0] 代表本应用名，从argv[1]开始是启动参数, argv[1]是工作模式参数，
 *             // argv[2] 是音乐文件全路径名
 *             if(strcmp(agrv[1], MUSIC_ARGV_PLAYING) == 0)
 *             {
 *                 //playing工作模式
 *                 //进入playing界面，并播放文件argv[2]
 *             }
 *        }
 *        else 
 *        {
 *                //默认工作模式
 *        }
 *    }
 *    
 * \endcode
 *******************************************************************************/
#define MUSIC_ARGV_PLAYING             "playing" 
#define MUSIC_ARGV_LIST                "list" 



/******************************************************************************/
/*!                    
 * \par  Description:
 *    photo 应用启动参数定义 
 * \code  
 * \endcode
 *******************************************************************************/
#define PHOTO_ARGV_PLAYING             "playing" 
#define PHOTO_ARGV_PREVIEW             "preview" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    video 应用启动参数定义 
 * \code  
 * \endcode
 *******************************************************************************/
#define VIDEO_ARGV_PLAYING             "playing" 
#define VIDEO_ARGV_LIST                "list" 
#define VIDEO_ARGV_DV_DIRLIST          "dv_dirlist" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ebook 应用启动参数定义 
 * \code  
 * \endcode
 *******************************************************************************/
#define EBOOK_ARGV_PLAYING             "playing" 
#define EBOOK_ARGV_LIST                "list" 


/******************************************************************************/
/*!                    
 * \par  Description:
 *    emulator 应用启动参数定义 
 * \code  
 * \endcode
 *******************************************************************************/
#define EMULATOR_ARGV_PLAYING             "playing" 
#define EMULATOR_ARGV_LIST                "list" 


/******************************************************************************/
/*!                    
 * \par  Description:
 *    camera 应用启动参数定义 (pc-camera)
 * \code  
 * \endcode
 *******************************************************************************/
#define CAMERA_ARGV_PCCAM             "pccam" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    camera 应用启动参数定义 (tvin)
 * \code  
 * \endcode
 *******************************************************************************/
#define TVIN_ARGV_ALARM             "alarm" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    camera 应用启动参数定义 (tvin)
 * \code  
 * \endcode
 *******************************************************************************/
#define MSG_SERVER_ARGV_CONSOLE             "console" 

#endif /*#ifndef __APP_ARGV_H__*/
