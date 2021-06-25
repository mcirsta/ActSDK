
#ifndef __EBOOK_GLOBAL_H__
#define __EBOOK_GLOBAL_H__

//	#include "debug.h"

#include "include_case.h"


#include "ebook_sty.h"
#include "style.h"

#include "ebook_sty_dc_api.h"


#define EBOOK_APP "ebook.app"

#define NULL_TIMER -1

#define NULL_CHAR 0
#define MAX_BOOK_LENGTH 1024*1024*10 /* 支持的最大文件长度 10 M */
#define BOOK_BUFFER_LENGTH 516

#define ORIGINAL_X 0   /* 坐标原点X 坐标 */
#define ORIGINAL_Y 0   /* Y 坐标 */
	
#define LOW_TXT_FILE_FORMAT ".txt"
#define TXT_FILE_FORMAT ".TXT"

#define JAR_FILE_FORMAT ".jar"

#define READ_ONLY "r"
#define SHOW_BUFFER_SIZE 4096   /* LINE_MAX_CHARS * 20 行 */
#define ONCE_READ_SIZE 512    /* 每次从文件(flash) 中读取的最大size */

#define SKIP_POINT_DISTANCE 512

#define LINE_MAX_CHARS 200    /* 一行最多能显示的字符数 */

#define FONT_MAX_COLOR 10
#define DEFAULT_COLOR_INDEX 0

/* 文件打开错误 */
typedef enum
{
    OPEN_SUCCESS = 0,
    OPEN_FAIL,
    TOO_LARGE
}open_file_result_e;

/* scene type define */
typedef enum
{
    SCENE_EXIT = 0,	/* exit state */
    SCENE_MAIN_MENU,
    SCENE_DIRECTORY_LIST,
    SCENE_BOOKMARK_LIST,
    SCENE_READ
}scene_e;

typedef enum
{
    TXT_FILE = 0 ,
    JAR_FILE

}file_format_e;

typedef enum
{
    TXT_ASCII_MBCS = 0 ,
    TXT_UTF8,
    TXT_UNICODE_LITTLE,
    TXT_UNICODE_BIG
}file_type_e;

typedef enum
{
    GUI_MBCS_ENCODE = GUI_MBCS_ENC,
    GUI_UNICODE_ENCODE = GUI_UNICODE_ENC,    
}gui_encode_e;

typedef enum
{
    SMALL_FONT  = 0,
    MEDIUM_FONT = 1,
    LARGE_FONT = 2
}font_type_e;

typedef enum
{
    SMALL_FONT_LINE_HEIGHT  = 20, /* SMALL_FONT */
    MEDIUM_FONT_LINE_HEIGHT  = 24, /* MEDIUM_FONT */
    LARGE_FONT_LINE_HEIGHT = 28   /* LARGE_FONT */
}line_height_e;

typedef enum
{
    SMALL_FONT_MAX_LINE  = 12, /* SMALL_FONT */
    MEDIUM_FONT_MAX_LINE  = 10, /* MEDIUM_FONT */
    LARGE_FONT_MAX_LINE = 9   /* LARGE_FONT */
}max_line_e;

typedef struct 
{
    int x;              /* 显示的起始位置 */
    int y;
    unsigned int width;          /* 显示的最大宽度 */
    unsigned int font_size;      /* 字体的大小 */
    unsigned int line_height;    /* 每行的高度 */
    unsigned int max_line;       /* 显示的行数 */
}font_layout_t;

#include "ebook_main.h"
#include "ebook_scene_main_menu.h"
#include "ebook_scene_dirlist.h"
#include "ebook_scene_bookmark.h"
#include "ebook_scene_read.h"

sty_dc_api_t* g_sty_dc;
style_api_t* g_style_api;

ebook_main_t* g_ebook_main;
scene_mainmenu_t* g_scene_mainmenu;
scene_dirlist_t* g_scene_dirlist;
scene_bookmark_t* g_scene_bookmark;
scene_read_t* g_scene_read;

#endif /* __EBOOK_GLOBAL_H__ */


