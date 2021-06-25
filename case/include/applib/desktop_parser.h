/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : desktop_parser.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       chencm    2009-0-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     desktop_parser.h
* \brief
* \author   chencm
*
* \version 1.0
* \date  2009/04/23
*******************************************************************************/

/*! 
 * \defgroup   desktop_parser    .desktop�ļ���parser�ӿ�
 */


#ifndef __DESKTOP_PARSER_H__
#define __DESKTOP_PARSER_H__



typedef struct desktop_pair
{
    char* key; //for example:"name[cn] = music","name[cn]" is a key.
    char* value; //for example:"name[cn] = music","music" is a value.
    struct desktop_pair* next;
} desktop_pair_t;

typedef struct desktop_info
{
    char* filename;
    struct desktop_pair * pairs;
} desktop_info_t;

typedef desktop_info_t* desktop_parser_handle;

#define FILE_SUFFIX ".desktop"

/******************************************************************************/
/*!                    
 * \par  Description:
 *  �����ڲ���һ��ȫ·����.desktop�ļ������ļ��е�key��value������������������Ҫmalloc�ڴ�������ķ�ʽ�洢���е�key��value��
 *  �����ظ�������׵�ַ��Ϊ �������ļ���parser handle��
 * \ingroup desktop_parser 
 * \param [in] filename: ��Ҫ�������ļ�����ȫ·�����ļ����ַ������磺"/mnt/diska/desktop/music.desktop" 
 * 
 * \return  desktop_parser_handle: ���ظý����ļ��Ľ������(= NULL,��ʾ����ʧ�ܡ���ʵ���Ǵ洢���ļ���������������׵�ַ)
 *******************************************************************************/
desktop_parser_handle desktop_parser_create(char* filename);

/******************************************************************************/
/*!                    
 * \par  Description:
 *  ����һ��key��Ӧ��value��
 *  ��ĳ��desktop�ļ���(����ͨ�� ����desktop_parser_create()���صľ�� ��ȷ���Ƕ�Ӧ�ĸ�desktop�ļ�),����һ��key���ҵ��򷵻ظ�key��Ӧ��value��
 * \ingroup desktop_parser 
 * \param [in] handle: ���ĸ�desktop�ļ��в��ң�������ļ�desktop_parser_create()���ص�desktop_parser_handle
 * \param [in] user_key: ��Ҫ���ҵ�key�ַ������磺"Name��Exec"�� 
 * \return : ���� ��key��Ӧ��value�ַ����׵�ַ����ΪNULL�����ʾ���Ҳ�����key��
 *******************************************************************************/
char * desktop_parser_get_key(desktop_parser_handle handle, char* user_key);

/******************************************************************************/
/*!                    
 * \par  Description:
 *  �ͷ� desktop_parser_create��malloc�������ڴ档
 * \ingroup desktop_parser 
 * \param [in] handle: ��Ҫ�ͷŵľ��
 * \return : true - �ͷųɹ���false - ʧ��
 * \par    exmaple code
 * \code
 *  ...
 *  
 *  #include "desktop_parser.h"
 *  #define TEST_DESKTOP_FILE "/mnt/diska/desktop/music.desktop"
 *  desktop_parser_handle dp;
 *  char * Name;
 *  char * Exec;
 *   
 *  if (dp = desktop_parser_create((char*)TEST_DESKTOP_FILE))
 *  {
 *      if (Name = desktop_parser_get_key(dp,"Name"))
 *          print_info("Name=%s",Name);
 *      if (Exec = desktop_parser_get_key(dp,"Exec"))
 *          print_info("Exec=%s",Exec);
 *          
 *      print_dbg(" %d",desktop_parser_delete(dp));
 *  }
 *  ...
 *
 * \endcode
 *******************************************************************************/
bool desktop_parser_delete(desktop_parser_handle handle);


#endif /* #ifndef __LAUNCHER_SCENE_LAUNCH_H__*/
