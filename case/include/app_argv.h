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
 * \brief           Ӧ�ó���������������
 *
 * \description     ����������ָ���ݸ�Ӧ�ó������ں���main()��argv������Ӧ�ó�����Ը���
 *                  ��ͬ�Ĳ������벻ͬ�Ĺ���ģʽ������������process manager����������һ��Ӧ��
 *                  ����ʱ���롣��һ����ͨӦ�ý�������process manager ��������һ��Ӧ�ý���ʱ��
 *                  ����ͨ����Ϣ��ϣ���������������͸�process manager, process manager ���䴫�ݸ�
 *                  ������Ӧ�õ�main�������Ӷ�����������Ӧ�õĹ���ģʽ��
 *                  ���ļ�ͨ���궨������Ӧ�ó���֧�ֵ�����������
 *
 * \node            ���������������ַ�����ʽ��һ��Ӧ�ÿ�����ͬʱ��������������������֮���Կո����
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
 *    ͨ��Ӧ��������������
 *    APPLICATION_ARGV_RESUME    ֪ͨӦ�ó����Իָ�ģʽ����
 * \code  
 * \endcode
 *******************************************************************************/
#define APPLICATION_ARGV_RESUME             "resume" 


/******************************************************************************/
/*!                    
 * \par  Description:
 *    LaucherӦ�������������� 
 *    LAUNCHER_ARGV_CONSOLE : ֪ͨLauncher �Ժ�̨��ʽ�����������Ժ�̫��ʽ���������ô��ò�����
 *        
 * \code  
 *    //process manager ����LaucnherӦ��ʱָ���Ƿ��Ժ�̨��ʽ������
 *    msg_app_t msg; 
 *    msg.type = MSG_CREAT_APP;
 *    sprintf(msg.content, "launcher.app %s", LAUNCHER_ARGV_CONSOLE);
 *    send_async_msg("manager", &msg);
 *
 *    //LauncherӦ�õ�main����
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
 *    usbӦ�������������� 
 *    USB_ARGV_CHARGE : ָ��usbӦ�ó������ڳ��ģʽ
 *    USB_ARGV_UDISK  : ָ��usbӦ�ó�������U��ģʽ            
 * \code  
 *    //����process manager ����USBӦ�ã���ָ���乤���ڳ��ģʽ
 *    msg_app_t msg; 
 *    msg.type = MSG_CREAT_APP;
 *    sprintf(msg.content, "usb.app %s", USB_ARGV_CHARGE);
 *    send_async_msg("manager", &msg);
 *
 *    //USBӦ�õ�main����
 *    int main(int argc, char * argv[])
 *    {
 *        if(argc == 2)
 *        {
 *             // argv[0] ����Ӧ��������argv[1]��ʼ����������
 *             if(strcmp(agrv[1], USB_ARGV_CHARGE) == 0)
 *             {
 *                 //��繤��ģʽ
 *             }
 *             else if(strcmp(agrv[1], USB_ARGV_UDISK) == 0)
 *             {
 *                //u�̹���ģʽ
 *             }
 *             else 
 *             {
 *                //Ĭ�Ϲ���ģʽ
 *             }
 *        }
 *        else 
 *        {
 *                //Ĭ�Ϲ���ģʽ
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
 *    musicӦ�������������� 
 *    MUSIC_ARGV_PLAYING : ָ��musicӦ�ó������playing ���Ž���
 *    MUSIC_ARGV_LIST    : ָ��musicӦ�ó������list ����          
 * \code  
 *    //����process manager ����musicӦ�ã���ָ����ֱ�ӽ��벥�Ž��棬������ָ���ļ�
 *    msg_app_t msg; 
 *    msg.type = MSG_CREAT_APP;
 *    sprintf(msg.content, "music.app %s \"%s\" ", MUSIC_ARGV_PLAYING, filepath);
 *    send_async_msg("manager", &msg);
 *
 *    //musicӦ�õ�main����
 *    int main(int argc, char * argv[])
 *    {
 *        if(argc == 3)
 *        {
 *             // argv[0] ����Ӧ��������argv[1]��ʼ����������, argv[1]�ǹ���ģʽ������
 *             // argv[2] �������ļ�ȫ·����
 *             if(strcmp(agrv[1], MUSIC_ARGV_PLAYING) == 0)
 *             {
 *                 //playing����ģʽ
 *                 //����playing���棬�������ļ�argv[2]
 *             }
 *        }
 *        else 
 *        {
 *                //Ĭ�Ϲ���ģʽ
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
 *    photo Ӧ�������������� 
 * \code  
 * \endcode
 *******************************************************************************/
#define PHOTO_ARGV_PLAYING             "playing" 
#define PHOTO_ARGV_PREVIEW             "preview" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    video Ӧ�������������� 
 * \code  
 * \endcode
 *******************************************************************************/
#define VIDEO_ARGV_PLAYING             "playing" 
#define VIDEO_ARGV_LIST                "list" 
#define VIDEO_ARGV_DV_DIRLIST          "dv_dirlist" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ebook Ӧ�������������� 
 * \code  
 * \endcode
 *******************************************************************************/
#define EBOOK_ARGV_PLAYING             "playing" 
#define EBOOK_ARGV_LIST                "list" 


/******************************************************************************/
/*!                    
 * \par  Description:
 *    emulator Ӧ�������������� 
 * \code  
 * \endcode
 *******************************************************************************/
#define EMULATOR_ARGV_PLAYING             "playing" 
#define EMULATOR_ARGV_LIST                "list" 


/******************************************************************************/
/*!                    
 * \par  Description:
 *    camera Ӧ�������������� (pc-camera)
 * \code  
 * \endcode
 *******************************************************************************/
#define CAMERA_ARGV_PCCAM             "pccam" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    camera Ӧ�������������� (tvin)
 * \code  
 * \endcode
 *******************************************************************************/
#define TVIN_ARGV_ALARM             "alarm" 

/******************************************************************************/
/*!                    
 * \par  Description:
 *    camera Ӧ�������������� (tvin)
 * \code  
 * \endcode
 *******************************************************************************/
#define MSG_SERVER_ARGV_CONSOLE             "console" 

#endif /*#ifndef __APP_ARGV_H__*/
