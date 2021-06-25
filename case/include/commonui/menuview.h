/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menuview.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     menuview.h
 * \brief    menuview widget interface
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

/*! 
 * \defgroup   common_menuview    menuview¿Ø¼þ½Ó¿Úº¯Êý
 */

#ifndef __MENUVIEW_H__
#define __MENUVIEW_H__


#include "include_case.h"

#include "menu.h"

/*!
 * \par menuview ÏÔÊ¾ÀàÐÍ 
 *  \li MENUVIEW_OPTION: ×Ó²Ëµ¥--µ¥ÐÐÏÔÊ¾
 *  \li MENUVIEW_OPTION_INFO : ×Ó²Ëµ¥ÏêÏ¸ÐÅÏ¢--Ë«ÐÐÏÔÊ¾
 *  \li MENUVIEW_MENULIST_TEXT : ²Ëµ¥ÁÐ±í--µ¥ÐÐÏÔÊ¾(²»´øÍ¼±ê)
 *  \li MENUVIEW_MENULIST_ICON : ²Ëµ¥ÁÐ±í--µ¥ÐÐÏÔÊ¾(´øÍ¼±ê)
 *  \li MENUVIEW_FILELIST_TEXT : ÎÄ¼þÁÐ±í(²»´øÍ¼±ê), µ±Ç°ÐÐ: Ë«ÐÐÏÔÊ¾, ·Çµ±Ç°ÐÐ: µ¥ÐÐÏÔÊ¾
 *  \li MENUVIEW_FILELIST_ICON : ÎÄ¼þÁÐ±í(´øÍ¼±ê), µ±Ç°ÐÐ: Ë«ÐÐÏÔÊ¾, ·Çµ±Ç°ÐÐ: µ¥ÐÐÏÔÊ¾
 *  \li MENUVIEW_PICLIST_MUSIC : ÒôÀÖÁÐ±í: Ë«ÐÐÏÔÊ¾
 *  \li MENUVIEW_PICLIST_VIDEO : ÊÓÆµÁÐ±í: Ë«ÐÐÏÔÊ¾
 *  \li MENUVIEW_PICLIST_PICTURE : Í¼Æ¬ÁÐ±í: Ë«ÐÐÏÔÊ¾
 *  \li MENUVIEW_PICLIST_EBOOK : µç×ÓÊéÁÐ±í: Ë«ÐÐÏÔÊ¾
 *  \li MENUVIEW_PICLIST_INFO : ÉèÖÃÏêÏ¸ÐÅÏ¢--Ë«ÐÐÏÔÊ¾
 *  \li MENUVIEW_MAX : ËùÓÐÖÖÀàÊýÄ¿
 */
typedef enum
{
    MENUVIEW_OPTION,
    MENUVIEW_OPTION_INFO,
    MENUVIEW_MENULIST_TEXT,
    MENUVIEW_MENULIST_ICON,
    MENUVIEW_FILELIST_TEXT,
    MENUVIEW_FILELIST_ICON,
    MENUVIEW_PICLIST_MUSIC,
    MENUVIEW_PICLIST_VIDEO,
    MENUVIEW_PICLIST_PICTURE,
    MENUVIEW_PICLIST_EBOOK,
    MENUVIEW_PICLIST_INFO,
    MENUVIEW_MAX,
    
}menuview_type_e;

/*!
 *  \par menuview ¿Ø¼þ¶ÔÍâÐÅºÅÀàÐÍ
 *  \li WIDGET_SIG_MENUVIEW_UP : ¸ßÁÁÌõÉÏÒÆÒ»ÐÐ
 *  \li WIDGET_SIG_MENUVIEW_DOWN : ¸ßÁÁÌõÏÂÒÆÒ»ÐÐ
 *  \li WIDGET_SIG_MENUVIEW_OK : ²Ëµ¥È·¶¨
 *  \li WIDGET_SIG_MENUVIEW_QUIT : ²Ëµ¥ÍË³ö
 *  \li WIDGET_SIG_MENUVIEW_EXCEED_FIRST : ³¬³öÆðÊ¼²Ëµ¥ÏîÒ»Ïî
 *  \li WIDGET_SIG_MENUVIEW_EXCEED_LAST : ³¬³ö×îºó²Ëµ¥ÏîÒ»Ïî
 *  \li WIDGET_SIG_MENUVIEW_UPDATE_ALL : ÐèÒªÍâ²¿¸üÐÂÈ«²¿Êý¾Ý
 *  \li WIDGET_SIG_MENUVIEW_SHORT_MENU : MENU¶Ì°´,Èçµ¯³ö×Ó²Ëµ¥
 *  \li WIDGET_SIG_MENUVIEW_LONG_MENU : MENU³¤°´,Èç×¨¼­coverflowÇÐ»»
 *  \li WIDGET_SIG_MENUVIEW_DELETE : É¾³ý¼ü°´ÏÂ
 */
typedef enum
{
    WIDGET_SIG_MENUVIEW_UP,
    WIDGET_SIG_MENUVIEW_DOWN,
    WIDGET_SIG_MENUVIEW_OK,
    WIDGET_SIG_MENUVIEW_QUIT,
    WIDGET_SIG_MENUVIEW_EXCEED_FIRST,
    WIDGET_SIG_MENUVIEW_EXCEED_LAST,
    WIDGET_SIG_MENUVIEW_UPDATE_ALL,
    WIDGET_SIG_MENUVIEW_SHORT_MENU,
    WIDGET_SIG_MENUVIEW_LONG_MENU,
    WIDGET_SIG_MENUVIEW_DELETE,
    
}menuview_sig_e;

/*!
 *  \brief 
 *  menuview ¿Ø¼þ¾ä±ú½á¹¹¶¨Òå
 */
typedef struct menuview_s
{
    /*! ÁÐ±íÏÔÊ¾ÀàÐÍ */
    menuview_type_e type;
    
    /*! »­²¼ID */
    int canvas_id;
    
    /*! ÐÅºÅ»Øµ÷º¯Êý */
    void (*callback)( struct menuview_s *, menuview_sig_e );

    /*! ²Ëµ¥Êý¾ÝÖ¸Õë */
    menu_t* menu;

    /*! ÄÚ²¿Ë½ÓÐÊý¾Ý½á¹¹Ö¸Õë£¬½ö¹© menuview ¿Ø¼þÄÚ²¿ÊµÏÖÊ¹ÓÃ */
    struct menuview_private_s * private; 

}menuview_t;


/*!
 *  \brief 
 *  menuview ¿Ø¼þµÄÐÅºÅ»Øµ÷º¯ÊýÀàÐÍ
 */
typedef void (*menuview_cb_t)( menuview_t *, menuview_sig_e );

/* menuview ¿Ø¼þ»ù±¾½Ó¿Ú */

/******************************************************************************/
/*!                    
* \par  Description:
*     ´´½¨Ò»¸ömenuview¿Ø¼þ  
* \ingroup common_menuview
* \param [in] type: ÁÐ±íÏÔÊ¾ÀàÐÍ
* \param [in] callback: ½ÓÊÜ menuview ÐÅºÅµÄ»Øµ÷º¯Êý 
* \return  ³É¹¦·µ»ØÒ»¸ö menuview ¾ä±ú, Ê§°Ü·µ»ØNULL      
*******************************************************************************/
menuview_t* menuview_create( menuview_type_e type, menuview_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     É¾³ýÒ»¸ömenuview¿Ø¼þ  
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
*******************************************************************************/
bool menuview_delete( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview »æÍ¼½Ó¿Ú  
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
* \note ´Ëº¯Êý»æÖÆmenuview¿Ø¼þµÄËùÓÐÔªËØ£¬Ò»°ãÓÃÓÚÐèÒªÕûÌå¸üÐÂmenuview¿Ø¼þ
*******************************************************************************/
bool menuview_paint( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þGUIÏûÏ¢´¦Àíº¯Êý£¬µ÷ÓÃÕßÐèÒª½«GUIÏûÏ¢´«µÝ¸ø´Ëº¯Êý
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \param [in] pmsg: GUIÏûÏ¢ 
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
* \note ¿Ø¼þÄÚ²¿ÓÐÊÂ¼þ·¢ÉúÊ±,ÏÈµ÷ÓÃµ±Ç°²Ëµ¥ÏîµÄ»Øµ÷, ÔÙµ÷ÓÃ¿Ø¼þµÄÐÅºÅ»Øµ÷º¯Êý
*******************************************************************************/
bool menuview_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ¼ÓÔØstyle×ÊÔ´  
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \param [in] type: ÁÐ±íÏÔÊ¾ÀàÐÍ
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
* \note Íâ²¿µ÷ÓÃ½öÓÃÓÚ¶¯Ì¬ÇÐ»»ÏÔÊ¾·ç¸ñÊ±
*******************************************************************************/
bool menuview_load_style( menuview_t *menuview, menuview_type_e type );

/******************************************************************************/
/*!                    
* \par  Description:
*     ÏÔÊ¾menuview¿Ø¼þ  
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
*******************************************************************************/
bool menuview_show( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     Òþ²Ømenuview¿Ø¼þ  
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
*******************************************************************************/
bool menuview_hide( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þÉèÖÃ²Ëµ¥Êý¾Ý
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \param [in] menu: ²Ëµ¥Êý¾ÝÖ¸Õë
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
* \note menuview Ö»¸ºÔðÏÔÊ¾ºÍÊäÈë´¦Àí, ²Ëµ¥Êý¾Ý±ØÐëÓÉÍâ²¿ÉèÖÃºÍ±£´æ
*******************************************************************************/
bool menuview_set_menu( menuview_t *menuview, menu_t* menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ»ñÈ¡²Ëµ¥Êý¾Ý
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú     
* \return  ³É¹¦·µ»ØÒ»¸ö ²Ëµ¥Êý¾Ý Ö¸Õë, Ê§°Ü·µ»ØNULL   
*******************************************************************************/
menu_t* menuview_get_menu( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þÉèÖÃÏÔÊ¾ÐÐÊý
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \param [in] lines: ÏÔÊ¾ÐÐÊý
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
* \note menuview ´´½¨Ê±¶ÁÈ¡style×ÊÔ´Ê±×Ô¶¯¼ÆËãÐÐÊý, Ò»°ãÎÞÐèµ÷ÓÃ
*******************************************************************************/
bool menuview_set_lines( menuview_t *menuview, int lines );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ»ñÈ¡ÏÔÊ¾ÐÐÊý
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú     
* \return  ³É¹¦·µ»ØÒ»¸ö ÏÔÊ¾ÐÐÊý, Ê§°Ü·µ»Ø -1  Ð
*******************************************************************************/
int menuview_get_lines( menuview_t *menuview );


/* menuview ¿Ø¼þ¸ß¼¶½Ó¿Ú */

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ»ñÈ¡¶¯Ì¬Í¼Æ¬µÄ¿í¶È
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú     
* \return  ³É¹¦·µ»ØÒ»¸ö ¶¯Ì¬Í¼Æ¬µÄ¿í¶È, Ê§°Ü·µ»Ø -1  
* \note menuview ÖÐ¶¯Ì¬Í¼Æ¬ÓÉÍâ²¿ÁíÆðÏß³ÌÀ´½âÂë, ÈçmusicµÄ×¨¼­Í¼Æ¬»òvideoÔ¤ÀÀÍ¼,
*       ½âÂëÇ°ÏÈ»ñÈ¡Í¼Æ¬³ß´ç
*******************************************************************************/
int menuview_get_bitmap_width( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ»ñÈ¡¶¯Ì¬Í¼Æ¬µÄ¸ß¶È
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú     
* \return  ³É¹¦·µ»ØÒ»¸ö ¶¯Ì¬Í¼Æ¬µÄ¸ß¶È, Ê§°Ü·µ»Ø -1  
* \note menuview ÖÐ¶¯Ì¬Í¼Æ¬ÓÉÍâ²¿ÁíÆðÏß³ÌÀ´½âÂë, ÈçmusicµÄ×¨¼­Í¼Æ¬»òvideoÔ¤ÀÀÍ¼,
*       ½âÂëÇ°ÏÈ»ñÈ¡Í¼Æ¬³ß´ç
*******************************************************************************/
int menuview_get_bitmap_height( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ»æÖÆÒ»ÕÅ¶¯Ì¬Í¼Æ¬
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú  
* \param [in] cur_index: menuview¿Ø¼þÁÐ±íÏîµÄË÷Òý
* \return  ³É¹¦·µ»ØÒ»¸ö ¶¯Ì¬Í¼Æ¬µÄ¿í¶È, Ê§°Ü·µ»Ø -1  
* \note menuview ÖÐ¶¯Ì¬Í¼Æ¬ÓÉÍâ²¿ÁíÆðÏß³ÌÀ´½âÂë, ÈçmusicµÄ×¨¼­Í¼Æ¬»òvideoÔ¤ÀÀÍ¼,
*       ½âÂëÇ°ÏÈ»ñÈ¡Í¼Æ¬³ß´ç, ½âÂë½áÊøºóµ÷ÓÃ¸Ã½Ó¿ÚÀ´»æÖÆ¶¯Ì¬Í¼Æ¬
*******************************************************************************/
bool menuview_paint_bitmap( menuview_t *menuview, int cur_index );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þÉèÖÃÔ¤¼ÓÔØÐÐÊý
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú 
* \param [in] lines: Ô¤¼ÓÔØÐÐÊý
* \return  ³É¹¦·µ»Øtrue, Ê§°Ü·µ»Øfalse      
*******************************************************************************/
bool menuview_set_prefetch_num( menuview_t *menuview, int lines );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ»ñÈ¡Ô¤¼ÓÔØÐÐÊý
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú     
* \return  ³É¹¦·µ»ØÔ¤¼ÓÔØÐÐÊý, Ê§°Ü·µ»Ø -1  
*******************************************************************************/
int menuview_get_prefetch_num( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ¿Ø¼þ»ñÈ¡Ô¤¼ÓÔØ´¦ÀíµÄÆðÊ¼ÐÐ
* \ingroup common_menuview
* \param [in] menuview: menuview¿Ø¼þ¾ä±ú     
* \return  ³É¹¦·µ»ØÔ¤¼ÓÔØ´¦ÀíµÄÆðÊ¼ÐÐ, Ê§°Ü·µ»Ø -1  
*******************************************************************************/
int menuview_get_prefetch_start( menuview_t *menuview );

#endif

