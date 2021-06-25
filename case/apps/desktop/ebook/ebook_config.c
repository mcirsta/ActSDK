/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_config.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-06-09          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     ebook_config.c
 * \brief    ebook config
 * \author   zhaowenjie
 *
 * \version 1.0
 * \date  2009/06/09
 *******************************************************************************/

#include "include_case.h"

#include "ebook.h"

#define INT_LENGTH 4
#define SEPARATOR ","

bool _get_color( unsigned int cindex, char* string );

void get_configs(void)
{
    int ret;
    int value;
    char string[ PATH_MAX ];

    unsigned int i;
    bool result;

    ret = get_config( CFG_EBOOK_HAS_CHANGE_FONT_SIZE_ITEM, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        if ( value == 1 )
        {
            g_scene_read->set_option_has_change_size_item();
        }
    }

    /*******************       字体颜色相关     **********************/
    /* 缺省字体的颜色 */
    ret = get_config( CFG_EBOOK_DEFAULT_FONT_COLOR, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
        print_err(" get_config error == %d ", errno );
    }
    else
    {
        g_scene_read->set_ui_font_colors( DEFAULT_COLOR_INDEX, value, 0 );
    }

    
    /* 所有的颜色 */    
    for ( i = 0; i < FONT_MAX_COLOR; i++ )
    {
        result = _get_color( i, string );
        if ( result == false )
        {
            break;
        }
    }

    g_scene_read->set_font_color_num( i );

    /* 阅读界面是否使用纯色背景 */
    ret = get_config( CFG_EBOOK_USE_COLOR_BACKGROUND, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
    }
    else
    {
        if ( value == 1 )
        {
            g_scene_read->set_ui_use_color_background( true );
        }
        else
        {
            g_scene_read->set_ui_use_color_background( false );
        }      
    }

    /*******************       字体颜色相关     **********************/


    
    /* 文件名 */
    memset( string, 0, PATH_MAX );
    ret = get_config( CFG_EBOOK_FILE_NAME, (char*)string , PATH_MAX );
    if ( ret >= 0 )
    {
        g_scene_read->set_file_name( string );
    
        /* 文件完整路径 */ 
        memset( string, 0, PATH_MAX );
        ret = get_config( CFG_EBOOK_FILE_FULL_PATH, (char*)string , PATH_MAX );
        if ( ret >= 0 )
        {
            //print_dbg(" full path = %s ", string );
            g_scene_read->set_file_full_path( string );

            ret = get_config( CFG_EBOOK_PAGE_START_POINT, (char*)&value , INT_LENGTH );
            if ( ret < 0 )
            {
                print_err(" get_config( CFG_EBOOK_USE_COLOR_BACKGROUND) error ! ");
            }
            else
            {
                g_scene_read->set_current_page_start_point( value );
            }
        }
    }
    else
    {
        print_dbg("");
    }
        
    

    /* 阅读界面是否使用纯色背景 */
    ret = get_config( CFG_EBOOK_USE_COLOR_BACKGROUND, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
    }
    

    /* 语言编码 */
    ret = get_config( CFG_EBOOK_LANGUAGE_MODE, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
        g_scene_read->set_language_mode( (unsigned int)sys_get_cur_language_id() );
    }
    else
    {
        g_scene_read->set_language_mode( (unsigned int)value );
    }

    /* 当前选择的颜色 */
    ret = get_config( CFG_EBOOK_CUR_FONT_COLOR, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
        /* print_err(" get_config error == %d ", errno ); */
    }
    else
    {
        g_scene_read->set_font_color( (unsigned int)value );
    }

    /* 当前选择的字体类型 */
    ret = get_config( CFG_EBOOK_FONT_TYPE, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
        /* print_err(" get_config error == %d ", errno ); */
    }
    else
    {
        if ( value > 3 )
        {
            value = 3;
        }
    
        g_scene_read->set_font_type( (font_type_e)value );
    }

    /* 自动阅读时间间隔 */
    ret = get_config( CFG_EBOOK_AUTO_PLAY_GAP, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_read->set_auto_play_gap( (unsigned int)value );
    }

        /* 自动阅读时间间隔 */
    ret = get_config( CFG_EBOOK_FUSION_MODE, (char*)&value , INT_LENGTH );
    if ( ret >= 0 )
    {
        g_scene_read->set_fusion_mode( (fusion_mode_e)value );
    }
    
}

void set_configs(void)
{
    int ret;
    int value;
    char* temp;

    /* 文件名 */
    ret = set_config( CFG_EBOOK_FILE_NAME, g_scene_read->get_file_name() , PATH_MAX );
    if ( ret < 0 )
    {
//	        print_err(" set_config error == %d ", errno );
    }

    /* 正在阅读文件完整路径 */
    temp = g_scene_read->get_file_path();
    
    if ( temp == NULL )
    {
        const char* null_string = "";    
        //print_dbg(" clear config( CFG_EBOOK_FILE_FULL_PATH ); ");

        ret = set_config( CFG_EBOOK_FILE_FULL_PATH, (char*)null_string , 1 );

        if ( ret < 0 )
        {
            print_err(" set_config error == %d ", errno );
        }

        ret = set_config( CFG_EBOOK_FILE_NAME, (char*)null_string , 1 );

        if ( ret < 0 )
        {
            print_err(" set_config error == %d ", errno );
        }
    }
    else
    {
        ret = set_config( CFG_EBOOK_FILE_FULL_PATH, temp , PATH_MAX );
        if ( ret >= 0 )
        {
            /* 文件当前页起始点 */
            value = (int)g_scene_read->get_current_page_start_point();
            ret = set_config( CFG_EBOOK_PAGE_START_POINT, (char*)&value , INT_LENGTH );
            if ( ret < 0 )
            {
                print_err(" set_config error == %d ", errno );
            }
        }
    }
    
    /* 字体颜色 */
    value = (int)g_scene_read->get_font_color();
    ret = set_config( CFG_EBOOK_CUR_FONT_COLOR, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
        print_err(" set_config error == %d ", errno );
    } 

    /* 语言编码 */
    value = (int)g_scene_read->get_language_mode();
    ret = set_config( CFG_EBOOK_LANGUAGE_MODE, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
        print_err(" set_config error == %d ", errno );
    } 

    /* 字体类型 */
    value = (int)g_scene_read->get_font_type();
    ret = set_config( CFG_EBOOK_FONT_TYPE, (char*)&value , INT_LENGTH );
    if ( ret < 0 )
    {
        print_err(" set_config error == %d ", errno );
    }

    /* 自动阅读时间间隔 */
    value = (int)g_scene_read->get_auto_play_gap();
    ret = set_config( CFG_EBOOK_AUTO_PLAY_GAP, (char*)&value , INT_LENGTH );

    /* 特效 */
    value = (int)g_scene_read->get_fusion_mode();
    ret = set_config( CFG_EBOOK_FUSION_MODE, (char*)&value , INT_LENGTH );
    
}

bool _get_color( unsigned int cindex, char* string )
{
    int ret;
    char font_color[ 20 ];
    int font_value;
    char bg_color[ 20 ];
    int bg_value;

    memset( font_color, 0, 20 );
    strcpy( font_color, CFG_EBOOK_FONT_COLOR );
    snprintf( font_color, sizeof( font_color ), "%s%d", font_color, cindex );

    ret = get_config( font_color, (char*)&font_value , INT_LENGTH );
    if ( ret < 0 )
    {
        return false;
    }

    memset( bg_color, 0, 20 );
    strcpy( bg_color, CFG_EBOOK_BG_COLOR );
    snprintf( bg_color, sizeof( bg_color ), "%s%d", bg_color, cindex );

    ret = get_config( bg_color, (char*)&bg_value , INT_LENGTH );
    if ( ret < 0 )
    {
        return false;
    }

    g_scene_read->set_ui_font_colors( (unsigned int)( cindex + 1 ), font_value, bg_value );

    return true;
    
//	    ret = get_config( temp, (char*)string , PATH_MAX );
//	    if ( ret < 0 )
//	    {
//	        return false;
//	    }
//	    else
//	    {
//	        int color = 0;
//	        int bg_color = 0;
//	        char* sub_string;
//	        
//	        sub_string = strtok( string, SEPARATOR );
//	        if ( sub_string != NULL )
//	        {
//	            color = (int)strtol( sub_string, NULL, 16 );
//	        }
//	
//	        sub_string = strtok( NULL, SEPARATOR );
//	        if ( sub_string != NULL )
//	        {
//	            bg_color = (int)strtol( sub_string, NULL, 16 );
//	        }
//	
//	        if ( color != ERANGE )
//	        {
//	            g_scene_read->set_ui_font_colors( (unsigned int)( cindex + 1 ), color, bg_color );
//	        }
//	    }

//    return true;
}

