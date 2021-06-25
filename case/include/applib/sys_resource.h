/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_resource.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-11             v1.0             build this file 
 ********************************************************************************/

#ifndef __SYS_RESOURCE_H__
#define __SYS_RESOURCE_H__

#include "sys_info.h"

/*! 
 * \defgroup   applib_system_resouce   ϵͳ��Դ���ʽӿ� 
 */

/*!
 * \par  ����Ӧ��Ŀ¼���� 
 */
typedef enum 
{
    DIR_MUSIC = 0,              
    DIR_VIDEO,          
    DIR_PHOTO,          
    DIR_EBOOK,           
    DIR_VIDEO_CAMERA,   
    DIR_PHOTO_CAMERA,    
    DIR_GAME,            
    DIR_RECORDER,        
    DIR_RADIO_RECORD,    
    DIR_RECORDER_MIC,    
    DIR_RECORDER_LINEIN, 
    DIR_TVIN,            
    DIR_SYSTEM,          
    DIR_SYSTEM_FM,       
    DIR_SYSTEM_FONT,     
    DIR_SYSTEM_PLAYLIST, 
    DIR_ROOT,              
    DIR_ALL,                 
}dir_type_e;


typedef struct  support_language_list_s
{  
    /*! ����ISO 936��ISO 3166 ��׼�����Ժ͵������Ʊ��룬����zh_CN, en_GB */
    char * codes;   

    /*! �������ƣ����� "��������", "English(America)" */
    char * name;

    /*! ���� id */ 
    int id;

    struct support_language_list_s * next;
}support_language_list_t;


/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡӦ�õ�ǰ������Դ�ļ��Ĵ��Ŀ¼ 
 * \ingroup applib_system_resouce
 * \return Ӧ�õ�ǰ������Դ�ļ����Ŀ¼     
 *******************************************************************************/
extern const char* sys_get_app_theme_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡcommonui��ǰ������Դ�ļ��Ĵ��Ŀ¼ 
 * \ingroup applib_system_resouce
 * \return commonui��ǰ������Դ�ļ����Ŀ¼     
 *******************************************************************************/
extern const char* sys_get_common_theme_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡ��ǰ�������� 
 * \ingroup applib_system_resouce
 * \return ��ǰ��������     
 *******************************************************************************/
extern const char* sys_get_cur_theme_name(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡӦ�������������ַ�����Դ�ļ��Ĵ��Ŀ¼ 
 * \ingroup applib_system_resouce
 * \return  Ӧ�������������ַ�����Դ�ļ��Ĵ��Ŀ¼    
 *******************************************************************************/
extern const char* sys_get_app_string_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡcommonui�Ķ�������ַ�����Դ�ļ��Ĵ��Ŀ¼ 
 * \ingroup applib_system_resouce
 * \return  commonui�Ķ�������ַ�����Դ�ļ��Ĵ��Ŀ¼    
 *******************************************************************************/
extern const char* sys_get_common_string_res_dir(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡ��ǰ���Եı������ƣ����磺zh_CN, en_GB�� 
 * \ingroup applib_system_resouce
 * \return  ��ǰ���Եı�������    
 *******************************************************************************/
extern const char* sys_get_cur_language_codes(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡ��ǰ����id 
 * \ingroup applib_system_resouce
 * \return  ��ǰ����id    
 *******************************************************************************/
extern int sys_get_cur_language_id(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��������id���Ҷ�Ӧ�����Ա�������
 * \ingroup applib_system_resouce
 * \param[in] lang_id -- ָ������id
 * \return  �ɹ����ظ�������id�ı������ƣ����򷵻�NULL    
 *******************************************************************************/
extern const char* sys_get_language_codes(int lang_id);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     �������Ա������Ʋ��Ҷ��ڵ�����id
 * \ingroup applib_system_resouce
 * \param[in] lang_codes -- ָ�����Ա�������
 * \return  �ɹ����ظ������Ա������Ƶ�����id�����򷵻�-1    
 *******************************************************************************/
extern int sys_get_language_id(const char* lang_codes);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡ��ǰ֧�����Ե��б� 
 * \ingroup applib_system_resouce
 * \return  �ɹ����ص�ǰ֧�����Ե��б�ͷָ�룬���򷵻�NULL    
 *******************************************************************************/
extern support_language_list_t* sys_get_cur_support_language_list(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ɾ����ǰ֧�ֵ������б� 
 * \ingroup applib_system_resouce
 * \param[in] list -- ��ǰ֧�������б� 
 * \return void 
 *******************************************************************************/
extern void sys_delete_cur_support_language_list(support_language_list_t * list);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡ��ǰĬ�������ļ��� 
 * \ingroup applib_system_resouce
 * \return  ��ǰĬ�������ļ���    
 *******************************************************************************/
extern const char* sys_get_default_font_file(void);

/******************************************************************************/
/*!
 * \par  Description:
 * ��ȡý����ԴĿ¼ 
 * \param[in] dev --  �洢�豸��
 * \param[in] type -- ý����ԴĿ¼���ͣ�
 * \result ָ�����͵�ý����ԴĿ¼ 
 *******************************************************************************/
extern const char * sys_get_media_dir(hotplugin_dev_type_e dev, dir_type_e type);

/******************************************************************************/
/*!
 * \par  Description:
 * ��ȡ�����ϵ�Ӧ��Ŀ¼ 
 * \param[in] type -- Ӧ��Ŀ¼���ͣ�
 * \result Ŀ¼���ڣ����ظ�·�������򷵻�NULL
 *******************************************************************************/
extern const char * sys_get_main_disk_dir(dir_type_e type);

/******************************************************************************/
/*!
 * \par  Description:
 * �������ϵĶ�ý����Դ�ļ�Ŀ¼�Ƿ����
 * \param[in] path_sequence -- ָ��Ŀ¼ȫ·��(�����Ƕ��ȫ·�������У���';'����)��
 *             ��ʹ��app_define.h�еĺ꣬
 * \param[in] root_dir -- ָ����ʼ���ĸ�·��
 * \result �������Ŀ¼�����ڣ����Զ�����֮���������ʧ�ܣ�����false, ���򷵻�true
 *******************************************************************************/
extern bool sys_check_media_dir(const char * path_sequence, const char * root_dir);

#endif /* #ifndef __SYS_RESOURCE_H__ */

