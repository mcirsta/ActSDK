/*******************************************************************************
 *                              USDK130
 *                            Module: GUI
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       ywwang     2006-09-28 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     gui_inc.h
 * \brief    GUI窗口管理以外的接口函数声明:包括2d图形绘制,字符串显示,资源字符串/位图显示
 * \author   ywwang
 * \version 1.0
 * \date  2006/09/28
 *******************************************************************************/
#ifndef  __GUI_INC_H__
#define  __GUI_INC_H__
#include <libc.h>
/*!\cond GUI_API*/
/*********************************************************************
 *
 *              数据结构与常量
 *
 **********************************************************************
 */

/*!
 * \brief
 *      矩形描述
 */
typedef struct
{
    /*! 左上角X坐标*/
    short x0;
    /*! 左上角Y坐标*/
    short y0;
    /*! 右上角X坐标*/
    short x1;
    /*! 右上角Y坐标*/
    short y1;
} gui_rect_t;

/*!
 * \brief
 *      framebuffer某块区域的数据
 */
typedef struct
{
    /*! framebuffer数据*/
    unsigned short *pdata;
    /*! 起始X坐标*/
    int x;
    /*! 起始Y坐标*/
    int y;
    /*! 区域长度*/
    int width;
    /*! 区域高度*/
    int height;
} gui_fb_area_t;

/*!
 * \brief
 *      字符串滚动信息
 */
typedef struct
{
    /*! 显示区域*/
    gui_rect_t rect;
    /*! 初始X坐标*/
    int disp_x0;
    /*! 字符串*/
    char *ptr;
    /*! 字符串的像素长度*/
    int pixel;
    /*! 滚动方向*/
    int direct;
    /*! 滚动模式*/
    int mode;
    /*! 滚动间距*/
    int space;
    /*! 对齐方式*/
    int align;

    /*! 字符串信息*/
    /*!
     * \brief
     *      待滚动字符串的基本数据
     */
    struct
    {
        /*! 字符串的指针，直接指向所提供的字符串，因此滚动期间必须保证参数字符串空间不被释放*/
        char *original_str;
        /*! 字符串的字节数*/
        int char_len;
        /*! 字符串的像素宽度*/
        int pixel_len;
        /*! 是否符合滚动条件（字符串像素宽度大于滚动框宽度才进行滚动）*/
        int property;
    } s_info;
} gui_scroll_info_t;

/*!
 * \brief
 *      描述位图信息
 */
typedef struct
{
    /*! 图片宽度*/
    unsigned short xsize;
    /*! 图片高度*/
    unsigned short ysize;
    /*! 每行字节数*/
    unsigned short bytesperpixel;
    /*! 位图数据地址*/
    const unsigned char *pdata;
} gui_bitmap_info_t;

/*!
 * \brief
 *      描述图层合并模式，该结构用于用于设置图层命令
 */
typedef struct
{
    /*! 如果是GUI_LAYER_MODE_STANDALONE模式，指定独立缓冲区的地址*/
    char *stbuf;
    /*! 指定图层合并模式，可选模式由枚举类型gui_layer_io_mode_t定义*/
    int newmode;
} gui_layer_assembly_info_t;

/*!
 * \brief
 *      图层创建信息，该结构体用于创建图层命令
 */
typedef struct
{
    /*! 图层数据区地址*/
    char *buf;
    /*! 所创建图层的颜色模式，可选模式由枚举类型gui_layer_format_t定义*/
    int fbformat;
    /*! 图层当前区域的矩形范围*/
    gui_rect_t rect1;
} gui_layer_createinfo_t;

/*!
 * \brief
 *      图层区域信息，该结构体用于添加图层区域命令
 */
typedef struct
{
    /*! 所要添加的区域的数据区地址*/
    char *buf;
    /*! 所要添加区域的图层*/
    int layer;
    /*! 所要添加的区域的矩形范围*/
    gui_rect_t rect1;
} gui_layer_regioninfo_t;

/*!
 * \brief
 *      图层区域清除信息，该结构体用于清除图层区域命令
 */
typedef struct
{
    /*! 所要清除区域的图层*/
    int layer;
    /*! 清除区域所在的窗口*/
    int hwin;
    /*! 该区域相对于该窗口的坐标*/
    gui_rect_t rect;
} gui_layer_cleaninfo_t;

/*!
 * \brief
 *     字符串点阵信息
 */
typedef struct
{
    /*! 点阵显示时在x方向的偏移，目前为0*/
    int x_offset;
    /*! 点阵显示时在y方向的偏移，目前为0*/
    int y_offset;
    /*! 字符串点阵的像素宽度*/
    int width;
    /*!字符串点阵的高度，该高度为计算对齐的行高 */
    int height;
    /*! 存放字符串点阵数据的缓存地址*/
    char *pixbuf;
    /*! 存放字符串点阵数据的缓存区的长度*/
    int pixbuflen;
    /*! 字符串占用x方向空间，暂时不使用*/
    int x_advance; /**/
    /*!点阵格式,目前只有PIXEL_TYPE_GRAY256，每个像素由一个字节表示其alpha 值(灰度值) */
    int pix_type;
    /*! 每行点阵的字节数*/
    int bytes_per_line;
    /*! baseline和top 之间的距离>0,暂时不使用 */
    int ascend;
    /*!baseline和bottom之间的距离<0 ,暂时不使用*/
    int descend;
} char_pix_info_t;

/*!
 * \brief
 *     ascii字符宽度缓存配置信息
 */
typedef struct
{
    /*! 需缓存的最小字号*/
    int min_size;
    /*! 需缓存的最大字号*/
    int max_size;
} gui_ascii_cache_info_t;

/*!
 * \brief
 *     图片缓存配置信息
 */
typedef struct
{
    /*! 缓存最大大小*/
    int max_cache_size;
    /*! 能缓存的单个图片的最大大小*/
    int max_pic_size;
} gui_pic_cache_info_t;

/*!
 *  \brief
 *  GUI预定义的色彩值
 * \li  GUI_BLACK       : 黑色
 * \li  GUI_BLUE        : 蓝色
 * \li  GUI_GREEN       : 绿色
 * \li  GUI_CYAN        : 青色
 * \li  GUI_RED         : 红色
 * \li  GUI_MAGENTA     : 洋红色
 * \li  GUI_BROWN       : 褐色
 * \li  GUI_DARKGREY    : 深灰色
 * \li  GUI_GRAY        : 灰色
 * \li  GUI_LIGHTGREY   : 浅灰色
 * \li  GUI_LIGHTBLUE   : 浅蓝色
 * \li  GUI_LIGHTGREEN  : 浅绿色
 * \li  GUI_LIGHTCYAN   : 浅青色
 * \li  GUI_LIGHTRED    : 浅红色
 * \li  GUI_LIGHTMAGENTA: 浅洋红色
 * \li  GUI_YELLOW      : 黄色
 * \li  GUI_WHITE       : 白色 */
typedef enum
{
    GUI_COLOR_BLACK = 0x000000,
    GUI_COLOR_BLUE = 0x0000ff,
    GUI_COLOR_GREEN = 0x00ff00,
    GUI_COLOR_CYAN = 0x00ffff,
    GUI_COLOR_RED = 0xff0000,
    GUI_COLOR_MAGENTA = 0x8b008b,
    GUI_COLOR_BROWN = 0xa52a2a,
    GUI_COLOR_DARKGRAY = 0x404040,
    GUI_COLOR_GRAY = 0x808080,
    GUI_COLOR_LIGHTGRAY = 0xd3d3d3,
    GUI_COLOR_LIGHTBLUE = 0x8080ff,
    GUI_COLOR_LIGHTGREEN = 0x80ff80,
    GUI_COLOR_LIGHTCYAN = 0xffff80,
    GUI_COLOR_LIGHTRED = 0xff8080,
    GUI_COLOR_LIGHTMAGENTA = 0xff80ff,
    GUI_COLOR_YELLOW = 0xffff00,
    GUI_COLOR_WHITE = 0xffffff,
} gui_color_t;

/*!
 *  \brief
 *  绘图模式（也用于设置文本模式）
 *  \li GUI_DRAW_MODE_NORMAL : 普通绘图模式（空白处填入背景色）
 *  \li GUI_DRAW_MODE_TRANS ：透明绘图模式（空白处不填色）
 *  \li GUI_DRAW_MODE_SHADOW: 阴影绘图模式，仅用于文本显示
 */
typedef enum
{
    GUI_DRAW_MODE_NORMAL = 0,
    GUI_DRAW_MODE_TRANS = (1 << 1),
    GUI_DRAW_MODE_SHADOW = (1 << 5),
} gui_draw_mode_t;

/*!
 *  \brief
 *  文本对齐方式
 *  \li GUI_ALIGN_LEFT : 左对齐，水平对齐方式
 *  \li GUI_ALIGN_RIGHT : 右对齐，水平对齐方式
 *  \li GUI_ALIGN_CENTER : 居中对齐，水平对齐方式
 *  \li GUI_ALIGN_TOP : 顶端对齐，垂直对齐方式
 *  \li GUI_ALIGN_BOTTOM : 底端对其，垂直对齐方式
 *  \li GUI_ALIGN_MIDDLE : 中线对齐，垂直对齐方式
 */
typedef enum
{
    GUI_ALIGN_LEFT = (0 << 0),
    GUI_ALIGN_RIGHT = (1 << 0),
    GUI_ALIGN_CENTER = (2 << 0),
    GUI_ALIGN_TOP = (0 << 2),
    GUI_ALIGN_BOTTOM = (1 << 2),
    GUI_ALIGN_MIDDLE = (3 << 2)
} gui_text_align_t;

/*!
 *  \brief
 *  编码方式
 *  \li GUI_MBCS_ENC : 内码编码
 *  \li GUI_UTF8_ENC : utf8编码
 *  \li GUI_UNICODE_ENC : unicode编码
 */
typedef enum
{
    GUI_MBCS_ENC = 0,
    GUI_UTF8_ENC = 1,
    GUI_UNICODE_ENC = 2
} gui_text_encode_t;

/*!
 *  \brief
 *  可用于操作GUI图层的命令
 *  \li GUI_LAYER_CREATE_FBDEV : 创建新图层，其参数为一个gui_layer_createinfo_t结构的指针
 *  \li GUI_LAYER_CHANGE_FBDEV_REGION : 改变一个图层的区域范围,其参数为gui_layer_createinfo_t结构的指针
 *  \li GUI_LAYER_DESTROY_FBDEV : 销毁一个图层，其参数为所要销毁的图层索引的指针(int *)
 *  \li GUI_LAYER_SET_FBDEV_VISIBLE: 设置土层是否可见
 *  \li GUI_LAYER_SET_FBDEV_USABLE: 设置图层是否可用
 *  \li GUI_LAYER_SET_FBDEV_ALPHA: 设置图层整体的Alpha值(仅对GUI_LAYER_FORMAT_RGB565A模式有效)
 *  \li GUI_LAYER_CLEAN_FBDEV_AREA : 清除一个图层某个区域为0,其参数为gui_layer_cleaninfo_t结构的指针
 *  \li GUI_LAYER_GET_LAYER_0_BUFFER: 获取图层0的buffer地址，其参数为一个字符类型指针的指针，用来存放buffer地址
 */
typedef enum
{
    GUI_LAYER_CREATE_FBDEV = 0,
    GUI_LAYER_CHANGE_FBDEV_REGION,
    GUI_LAYER_DESTROY_FBDEV,
    GUI_LAYER_SET_FBDEV_VISIBLE,
    GUI_LAYER_SET_FBDEV_USABLE,
    GUI_LAYER_SET_FBDEV_ALPHA,
    GUI_LAYER_CLEAN_FBDEV_AREA,
    GUI_LAYER_GET_LAYER_0_BUFFER
} gui_layer_cmd_t;

/*!
 *  \brief
 *  GUI图层的合并模式
 *  \li GUI_LAYER_MODE_DIRECTIO : 将图层合并的结果直接写入framebuffer
 *  \li GUI_LAYER_MODE_STANDALONE : 将图层合并的结果写入一个独立的缓冲区
 */
typedef enum
{
    GUI_LAYER_MODE_DIRECTIO = 0,
    GUI_LAYER_MODE_STANDALONE = 1,
} gui_layer_io_mode_t;

/*!
 *  \brief
 *  图层的颜色模式
 *  \li RGB565 : 每像素2字节中低5位为红色，中间6位为绿色，高5位为蓝色的颜色模式
 *  \li RGB565A : 每像素3字节，其前2字节为RGB565格式，后一个字节表示alpha值
 */
typedef enum
{
    GUI_LAYER_FORMAT_RGB565 = 0,
    GUI_LAYER_FORMAT_RGB565A,
} gui_layer_format_t;

/*!
 *  \brief
 *  显示模式
 *  \li GUI_DISPLAY_MODE_NON_YUV : 非YUV模式
 *  \li GUI_DISPLAY_MODE_YUV : YUV模式
 */
typedef enum
{
    GUI_DISPLAY_MODE_NON_YUV = 0,
    GUI_DISPLAY_MODE_YUV,
} gui_display_mode_t;

/*!
 *  \brief
 *  GUI配置命令
 *  \li GUI_ASCII_WIDTH_CACHE_SET_UP : 开启并设置ascii字符宽度缓存
 *  \li GUI_ASCII_WIDTH_CACHE_SHUT_DOWN : 关闭ascii字符宽度缓存
 *  \li GUI_ASCII_WIDTH_CACHE_INVALIDATE: 使当前ascii字符宽度缓存无效
 *  \li GUI_RES_PIC_CACHE_SET_UP : 开启并设置资源小图片缓存
 *  \li GUI_RES_PIC_CACHE_SHUT_DOWN : 关闭资源小图片缓存
 *  \li GUI_RES_PIC_CACHE_INVALIDATE: 使当前资源小图片缓存无效
 *  \li GUI_TTF_DRV_CACHE_SET_SIZE: 设置ttf引擎内部cache大小，以KB为单位
 */
typedef enum
{
    GUI_ASCII_WIDTH_CACHE_SET_UP = 0,
    GUI_ASCII_WIDTH_CACHE_SHUT_DOWN,
    GUI_ASCII_WIDTH_CACHE_INVALIDATE,
    GUI_RES_PIC_CACHE_SET_UP,
    GUI_RES_PIC_CACHE_SHUT_DOWN,
    GUI_RES_PIC_CACHE_INVALIDATE,
    GUI_TTF_DRV_CACHE_SET_SIZE,
} gui_config_cmd_t;

/*********************************************************************
 *
 *              2D绘图函数
 *
 **********************************************************************
 */

gui_draw_mode_t gui_set_draw_mode(gui_draw_mode_t dm);

void gui_set_background_color(int color);
void gui_set_color(int color);
void gui_clear(void);
void gui_clear_rect(int x0, int y0, int x1, int y1);
void gui_draw_rect(int x0, int y0, int x1, int y1);

void gui_draw_rect_ex(const gui_rect_t *prect);
void gui_get_client_rect(gui_rect_t *prect);

unsigned char gui_set_pen_size(unsigned char pen_size);

void gui_draw_line(int x0, int y0, int x1, int y1);

int gui_set_language_mode(int language);
int gui_get_language_mode();

void gui_set_dimension(int width, int height);
int gui_set_encoding(gui_text_encode_t host_enc, gui_text_encode_t local_enc,
        gui_text_encode_t fontlib_enc);

int gui_get_area(gui_fb_area_t *pfb_area);

/*********************************************************************
 *
 *              字符串显示函数
 *
 **********************************************************************
 */
int gui_select_font_face(int fontdesc);
int gui_select_font_size(int fontsize);
int gui_create_font(const char* family_name, int height);
void gui_destroy_font(int fontdesc);

int gui_set_text_align(int align);
gui_draw_mode_t gui_set_text_mode(gui_draw_mode_t mode);
int gui_set_shadow_info(int shadowcolor, int xoff, int yoff);

int gui_config_font_glyph_cache(int *fontsize_list, int *cachesize_list);
int gui_config_font_width_cache(int *fontsize_list);
int gui_destroy_font_glyph_cache();
int gui_destroy_font_width_cache();

int gui_display_char(unsigned int c, gui_text_encode_t encoding);

int gui_display_char_at(unsigned int c, int x, int y,
        gui_text_encode_t encoding);

int gui_display_string(char *s, gui_text_encode_t encoding);

int gui_display_string_at(char *s, int x, int y, gui_text_encode_t encoding);
int gui_display_string_at_ceol(char *s, int x, int y,
        gui_text_encode_t encoding);

int gui_display_string_in_rect(char *s, gui_rect_t *prect, int align,
        gui_text_encode_t encoding);

int gui_get_display_pos_x(void);
int gui_get_display_pos_y(void);

int gui_get_char_dist_x(unsigned int c, gui_text_encode_t encoding);

int gui_get_char_fixed_width(unsigned int c);

int gui_get_string_dist_x(char *s, gui_text_encode_t encoding);

int gui_get_char_advance(unsigned int unicode, int fontsize, int encoding);

int gui_display_string_hcenter_at(char *s, int x, int y,
        gui_text_encode_t encoding);

void gui_display_ceol(void);

int gui_set_scroll_string_in_rect(char *s, gui_rect_t* prect, int textalign,
        gui_scroll_info_t *scroll_info, int direct, int pixel,
        unsigned int space, gui_text_encode_t encoding);

int gui_scroll_string_in_rect(gui_scroll_info_t *scroll_info,
        gui_text_encode_t encoding);

int gui_revert_string(unsigned char *revert, gui_text_encode_t encoding);
int gui_get_word_length(unsigned char *s, int language);

int gui_get_char_pixel_buf(unsigned int achar, int fontsize,
        char_pix_info_t *ppixinfo, int encoding);
int gui_get_string_pixel_buf(const char* str, int fontsize,
        char_pix_info_t *ppixinfo, int encoding);
int gui_display_string_pixel_buf(const char_pix_info_t *ppixinfo, int fontsize,
        int x, int y, int textmode);

/*********************************************************************
 *
 *              资源文件显示
 *
 **********************************************************************
 */
int gui_set_common_ui_res(const unsigned char *resfilepath);
int gui_set_common_string_res(const unsigned char * resfilepath);
int gui_show_picture(FILE *fpres, int id, int x, int y);
int gui_show_sub_picture(FILE *fpres, int id, int x, int y, gui_rect_t *rect);

int gui_load_bitmap_into_buffer(FILE *fpres, int id, char *buffer,
        int bufferlen, gui_bitmap_info_t *bitmapinfo);
int gui_load_sub_bitmap_into_buffer(FILE *fpres, int id, char *buffer,
        int bufferlen, gui_bitmap_info_t *bitmapinfo, gui_rect_t *rect);
int gui_bitmap_metrics(FILE *fpres, int id, gui_bitmap_info_t *bitmapinfo);

int gui_draw_bitmap_ext(unsigned char* buffer, int x, int y, int widht,
        int height, int bytesperpix);
int gui_draw_sub_bitmap_ext(unsigned char* buffer, int subx, int suby,
        int subw, int subh, int x, int y, int width, int height,
        int bytesperpix);

const char * gui_get_string_by_id(FILE *fpres, int id, int *length,
        gui_text_encode_t*encoding);

FILE *gui_open_ui_res(const unsigned char *resfilepath);
FILE *gui_open_string_res(const unsigned char *resfilepath);

int gui_close_resource_file(FILE *fpres);

/*********************************************************************
 *
 *              互斥
 *
 **********************************************************************
 */

int gui_exec(void);

int gui_set_display_mode(gui_display_mode_t mode);
void gui_screen_update(void);
void gui_framebuffer_update(void);

int gui_fb_device_control(int layer, int cmd, void * param);
unsigned int gui_select_layer(unsigned int index);

int gui_setting_config(gui_config_cmd_t cmd, void *param);

int gui_set_fontsize_mapping(int *original_size, int default_size);

int gui_set_update_region(int x0, int y0, int x1, int y1);

/*********************************************************************
 *
 *              GDI接口
 *
 **********************************************************************
 */
int gui_dc_get(int hwin);
int gui_dc_release(int hdc);
int gui_dc_set_default_fontface(int fontface);

int gui_dc_set_text_mode(int hdc, int textmode);
int gui_dc_set_color(int hdc, int color);
int gui_dc_set_background_color(int hdc, int bgcolor);
int gui_dc_set_alpha(int hdc, int alpha);
int gui_dc_set_font_face(int hdc, int fontfacedesc);
int gui_dc_set_font_size(int hdc, int fontsize);
int gui_dc_set_text_align(int hdc, int align);
int gui_dc_set_pen_size(int hdc, int pensize);

int gui_dc_get_text_mode(int hdc);
int gui_dc_get_color(int hdc);
int gui_dc_get_background_color(int hdc);
int gui_dc_get_alpha(int hdc);
int gui_dc_get_fontface(int hdc);
int gui_dc_get_fontsize(int hdc);
int gui_dc_get_text_align(int hdc);
int gui_dc_get_pen_size(int hdc);

int gui_dc_get_char_dist_x(int hdc, unsigned int c, gui_text_encode_t encoding);
int gui_dc_get_string_dist_x(int hdc, char *s, gui_text_encode_t encoding);
int gui_dc_display_char_at(int hdc, unsigned int c, int x, int y,
        gui_text_encode_t encoding);
int gui_dc_display_string_at(int hdc, char *s, int x, int y,
        gui_text_encode_t encoding);
int gui_dc_display_string_in_rect(int hdc, char *s, gui_rect_t* prect,
        int align, gui_text_encode_t encoding);
int gui_dc_scroll_string_in_rect(int hdc, gui_scroll_info_t *scroll_info,
        gui_text_encode_t encoding);
int gui_dc_set_scroll_string_in_rect(int hdc, unsigned char *s,
        gui_rect_t* prect, int textalign, gui_scroll_info_t *scroll_info,
        int direct, int pixel, unsigned int space, gui_text_encode_t encoding);
int gui_dc_display_string_hcenter_at(int hdc, char *s, int x, int y,
        gui_text_encode_t encoding);
int gui_dc_display_string_at_ceol(int hdc, char *s, int x, int y,
        gui_text_encode_t encoding);
int gui_dc_display_ceol(int hdc);
int gui_dc_show_picture(int hdc, FILE *fpres, int id, int x, int y);
int gui_dc_show_sub_picture(int hdc, FILE *fpres, int id, int x, int y,
        gui_rect_t *prect);
int gui_dc_draw_bitmap_ext(int hdc, unsigned char* buffer, int x, int y,
        int width, int height, int bytesperpix);
int gui_dc_draw_sub_bitmap_ext(int hdc, unsigned char* buffer, int subx,
        int suby, int subw, int subh, int x, int y, int width, int height,
        int bytesperpix);
int gui_dc_clear(int hdc);
int gui_dc_draw_rect_ex(int hdc, const gui_rect_t *prect);
int gui_dc_clear_rect(int hdc, int x0, int y0, int x1, int y1);
int gui_dc_draw_rect(int hdc, int x0, int y0, int x1, int y1);
int gui_dc_draw_line(int hdc, int x0, int y0, int x1, int y1);

/*********************************************************************
 *
 *              字符缓存配置(尚未实现)
 *
 **********************************************************************
 */
int gui_config_font_glyph_cache(int *fontsize_list, int *cachesize_list);
int gui_config_font_width_cache(int *fontsize_list);
int gui_destroy_font_glyph_cache();
int gui_destroy_font_width_cache();

/*!\endcond*/
#endif   /* ifdef GUI_H_INC */
