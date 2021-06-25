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
 * \defgroup   desktop_parser    .desktop文件的parser接口
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
 *  函数内部打开一个全路径的.desktop文件，将文件中的key、value解析出来，并根据需要malloc内存以链表的方式存储所有的key和value。
 *  并返回该链表的首地址做为 创建该文件的parser handle。
 * \ingroup desktop_parser 
 * \param [in] filename: 需要解析的文件绝对全路径和文件名字符串，如："/mnt/diska/desktop/music.desktop" 
 * 
 * \return  desktop_parser_handle: 返回该解析文件的解析句柄(= NULL,表示创建失败。其实就是存储该文件解析结果的链表首地址)
 *******************************************************************************/
desktop_parser_handle desktop_parser_create(char* filename);

/******************************************************************************/
/*!                    
 * \par  Description:
 *  查找一个key对应的value。
 *  在某个desktop文件中(这里通过 传入desktop_parser_create()返回的句柄 来确定是对应哪个desktop文件),查找一个key，找到则返回该key对应的value。
 * \ingroup desktop_parser 
 * \param [in] handle: 在哪个desktop文件中查找，则传入该文件desktop_parser_create()返回的desktop_parser_handle
 * \param [in] user_key: 需要查找的key字符串，如："Name、Exec"等 
 * \return : 返回 该key对应的value字符串首地址，若为NULL，则表示查找不到该key。
 *******************************************************************************/
char * desktop_parser_get_key(desktop_parser_handle handle, char* user_key);

/******************************************************************************/
/*!                    
 * \par  Description:
 *  释放 desktop_parser_create所malloc的所有内存。
 * \ingroup desktop_parser 
 * \param [in] handle: 需要释放的句柄
 * \return : true - 释放成功，false - 失败
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
