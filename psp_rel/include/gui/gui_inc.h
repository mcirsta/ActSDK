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
 * \brief    GUI���ڹ�������Ľӿں�������:����2dͼ�λ���,�ַ�����ʾ,��Դ�ַ���/λͼ��ʾ
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
 *              ���ݽṹ�볣��
 *
 **********************************************************************
 */

/*!
 * \brief
 *      ��������
 */
typedef struct
{
    /*! ���Ͻ�X����*/
    short x0;
    /*! ���Ͻ�Y����*/
    short y0;
    /*! ���Ͻ�X����*/
    short x1;
    /*! ���Ͻ�Y����*/
    short y1;
} gui_rect_t;

/*!
 * \brief
 *      framebufferĳ�����������
 */
typedef struct
{
    /*! framebuffer����*/
    unsigned short *pdata;
    /*! ��ʼX����*/
    int x;
    /*! ��ʼY����*/
    int y;
    /*! ���򳤶�*/
    int width;
    /*! ����߶�*/
    int height;
} gui_fb_area_t;

/*!
 * \brief
 *      �ַ���������Ϣ
 */
typedef struct
{
    /*! ��ʾ����*/
    gui_rect_t rect;
    /*! ��ʼX����*/
    int disp_x0;
    /*! �ַ���*/
    char *ptr;
    /*! �ַ��������س���*/
    int pixel;
    /*! ��������*/
    int direct;
    /*! ����ģʽ*/
    int mode;
    /*! �������*/
    int space;
    /*! ���뷽ʽ*/
    int align;

    /*! �ַ�����Ϣ*/
    /*!
     * \brief
     *      �������ַ����Ļ�������
     */
    struct
    {
        /*! �ַ�����ָ�룬ֱ��ָ�����ṩ���ַ�������˹����ڼ���뱣֤�����ַ����ռ䲻���ͷ�*/
        char *original_str;
        /*! �ַ������ֽ���*/
        int char_len;
        /*! �ַ��������ؿ��*/
        int pixel_len;
        /*! �Ƿ���Ϲ����������ַ������ؿ�ȴ��ڹ������ȲŽ��й�����*/
        int property;
    } s_info;
} gui_scroll_info_t;

/*!
 * \brief
 *      ����λͼ��Ϣ
 */
typedef struct
{
    /*! ͼƬ���*/
    unsigned short xsize;
    /*! ͼƬ�߶�*/
    unsigned short ysize;
    /*! ÿ���ֽ���*/
    unsigned short bytesperpixel;
    /*! λͼ���ݵ�ַ*/
    const unsigned char *pdata;
} gui_bitmap_info_t;

/*!
 * \brief
 *      ����ͼ��ϲ�ģʽ���ýṹ������������ͼ������
 */
typedef struct
{
    /*! �����GUI_LAYER_MODE_STANDALONEģʽ��ָ�������������ĵ�ַ*/
    char *stbuf;
    /*! ָ��ͼ��ϲ�ģʽ����ѡģʽ��ö������gui_layer_io_mode_t����*/
    int newmode;
} gui_layer_assembly_info_t;

/*!
 * \brief
 *      ͼ�㴴����Ϣ���ýṹ�����ڴ���ͼ������
 */
typedef struct
{
    /*! ͼ����������ַ*/
    char *buf;
    /*! ������ͼ�����ɫģʽ����ѡģʽ��ö������gui_layer_format_t����*/
    int fbformat;
    /*! ͼ�㵱ǰ����ľ��η�Χ*/
    gui_rect_t rect1;
} gui_layer_createinfo_t;

/*!
 * \brief
 *      ͼ��������Ϣ���ýṹ���������ͼ����������
 */
typedef struct
{
    /*! ��Ҫ��ӵ��������������ַ*/
    char *buf;
    /*! ��Ҫ��������ͼ��*/
    int layer;
    /*! ��Ҫ��ӵ�����ľ��η�Χ*/
    gui_rect_t rect1;
} gui_layer_regioninfo_t;

/*!
 * \brief
 *      ͼ�����������Ϣ���ýṹ���������ͼ����������
 */
typedef struct
{
    /*! ��Ҫ��������ͼ��*/
    int layer;
    /*! ����������ڵĴ���*/
    int hwin;
    /*! ����������ڸô��ڵ�����*/
    gui_rect_t rect;
} gui_layer_cleaninfo_t;

/*!
 * \brief
 *     �ַ���������Ϣ
 */
typedef struct
{
    /*! ������ʾʱ��x�����ƫ�ƣ�ĿǰΪ0*/
    int x_offset;
    /*! ������ʾʱ��y�����ƫ�ƣ�ĿǰΪ0*/
    int y_offset;
    /*! �ַ�����������ؿ��*/
    int width;
    /*!�ַ�������ĸ߶ȣ��ø߶�Ϊ���������и� */
    int height;
    /*! ����ַ����������ݵĻ����ַ*/
    char *pixbuf;
    /*! ����ַ����������ݵĻ������ĳ���*/
    int pixbuflen;
    /*! �ַ���ռ��x����ռ䣬��ʱ��ʹ��*/
    int x_advance; /**/
    /*!�����ʽ,Ŀǰֻ��PIXEL_TYPE_GRAY256��ÿ��������һ���ֽڱ�ʾ��alpha ֵ(�Ҷ�ֵ) */
    int pix_type;
    /*! ÿ�е�����ֽ���*/
    int bytes_per_line;
    /*! baseline��top ֮��ľ���>0,��ʱ��ʹ�� */
    int ascend;
    /*!baseline��bottom֮��ľ���<0 ,��ʱ��ʹ��*/
    int descend;
} char_pix_info_t;

/*!
 * \brief
 *     ascii�ַ���Ȼ���������Ϣ
 */
typedef struct
{
    /*! �軺�����С�ֺ�*/
    int min_size;
    /*! �軺�������ֺ�*/
    int max_size;
} gui_ascii_cache_info_t;

/*!
 * \brief
 *     ͼƬ����������Ϣ
 */
typedef struct
{
    /*! ��������С*/
    int max_cache_size;
    /*! �ܻ���ĵ���ͼƬ������С*/
    int max_pic_size;
} gui_pic_cache_info_t;

/*!
 *  \brief
 *  GUIԤ�����ɫ��ֵ
 * \li  GUI_BLACK       : ��ɫ
 * \li  GUI_BLUE        : ��ɫ
 * \li  GUI_GREEN       : ��ɫ
 * \li  GUI_CYAN        : ��ɫ
 * \li  GUI_RED         : ��ɫ
 * \li  GUI_MAGENTA     : ���ɫ
 * \li  GUI_BROWN       : ��ɫ
 * \li  GUI_DARKGREY    : ���ɫ
 * \li  GUI_GRAY        : ��ɫ
 * \li  GUI_LIGHTGREY   : ǳ��ɫ
 * \li  GUI_LIGHTBLUE   : ǳ��ɫ
 * \li  GUI_LIGHTGREEN  : ǳ��ɫ
 * \li  GUI_LIGHTCYAN   : ǳ��ɫ
 * \li  GUI_LIGHTRED    : ǳ��ɫ
 * \li  GUI_LIGHTMAGENTA: ǳ���ɫ
 * \li  GUI_YELLOW      : ��ɫ
 * \li  GUI_WHITE       : ��ɫ */
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
 *  ��ͼģʽ��Ҳ���������ı�ģʽ��
 *  \li GUI_DRAW_MODE_NORMAL : ��ͨ��ͼģʽ���հ״����뱳��ɫ��
 *  \li GUI_DRAW_MODE_TRANS ��͸����ͼģʽ���հ״�����ɫ��
 *  \li GUI_DRAW_MODE_SHADOW: ��Ӱ��ͼģʽ���������ı���ʾ
 */
typedef enum
{
    GUI_DRAW_MODE_NORMAL = 0,
    GUI_DRAW_MODE_TRANS = (1 << 1),
    GUI_DRAW_MODE_SHADOW = (1 << 5),
} gui_draw_mode_t;

/*!
 *  \brief
 *  �ı����뷽ʽ
 *  \li GUI_ALIGN_LEFT : ����룬ˮƽ���뷽ʽ
 *  \li GUI_ALIGN_RIGHT : �Ҷ��룬ˮƽ���뷽ʽ
 *  \li GUI_ALIGN_CENTER : ���ж��룬ˮƽ���뷽ʽ
 *  \li GUI_ALIGN_TOP : ���˶��룬��ֱ���뷽ʽ
 *  \li GUI_ALIGN_BOTTOM : �׶˶��䣬��ֱ���뷽ʽ
 *  \li GUI_ALIGN_MIDDLE : ���߶��룬��ֱ���뷽ʽ
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
 *  ���뷽ʽ
 *  \li GUI_MBCS_ENC : �������
 *  \li GUI_UTF8_ENC : utf8����
 *  \li GUI_UNICODE_ENC : unicode����
 */
typedef enum
{
    GUI_MBCS_ENC = 0,
    GUI_UTF8_ENC = 1,
    GUI_UNICODE_ENC = 2
} gui_text_encode_t;

/*!
 *  \brief
 *  �����ڲ���GUIͼ�������
 *  \li GUI_LAYER_CREATE_FBDEV : ������ͼ�㣬�����Ϊһ��gui_layer_createinfo_t�ṹ��ָ��
 *  \li GUI_LAYER_CHANGE_FBDEV_REGION : �ı�һ��ͼ�������Χ,�����Ϊgui_layer_createinfo_t�ṹ��ָ��
 *  \li GUI_LAYER_DESTROY_FBDEV : ����һ��ͼ�㣬�����Ϊ��Ҫ���ٵ�ͼ��������ָ��(int *)
 *  \li GUI_LAYER_SET_FBDEV_VISIBLE: ���������Ƿ�ɼ�
 *  \li GUI_LAYER_SET_FBDEV_USABLE: ����ͼ���Ƿ����
 *  \li GUI_LAYER_SET_FBDEV_ALPHA: ����ͼ�������Alphaֵ(����GUI_LAYER_FORMAT_RGB565Aģʽ��Ч)
 *  \li GUI_LAYER_CLEAN_FBDEV_AREA : ���һ��ͼ��ĳ������Ϊ0,�����Ϊgui_layer_cleaninfo_t�ṹ��ָ��
 *  \li GUI_LAYER_GET_LAYER_0_BUFFER: ��ȡͼ��0��buffer��ַ�������Ϊһ���ַ�����ָ���ָ�룬�������buffer��ַ
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
 *  GUIͼ��ĺϲ�ģʽ
 *  \li GUI_LAYER_MODE_DIRECTIO : ��ͼ��ϲ��Ľ��ֱ��д��framebuffer
 *  \li GUI_LAYER_MODE_STANDALONE : ��ͼ��ϲ��Ľ��д��һ�������Ļ�����
 */
typedef enum
{
    GUI_LAYER_MODE_DIRECTIO = 0,
    GUI_LAYER_MODE_STANDALONE = 1,
} gui_layer_io_mode_t;

/*!
 *  \brief
 *  ͼ�����ɫģʽ
 *  \li RGB565 : ÿ����2�ֽ��е�5λΪ��ɫ���м�6λΪ��ɫ����5λΪ��ɫ����ɫģʽ
 *  \li RGB565A : ÿ����3�ֽڣ���ǰ2�ֽ�ΪRGB565��ʽ����һ���ֽڱ�ʾalphaֵ
 */
typedef enum
{
    GUI_LAYER_FORMAT_RGB565 = 0,
    GUI_LAYER_FORMAT_RGB565A,
} gui_layer_format_t;

/*!
 *  \brief
 *  ��ʾģʽ
 *  \li GUI_DISPLAY_MODE_NON_YUV : ��YUVģʽ
 *  \li GUI_DISPLAY_MODE_YUV : YUVģʽ
 */
typedef enum
{
    GUI_DISPLAY_MODE_NON_YUV = 0,
    GUI_DISPLAY_MODE_YUV,
} gui_display_mode_t;

/*!
 *  \brief
 *  GUI��������
 *  \li GUI_ASCII_WIDTH_CACHE_SET_UP : ����������ascii�ַ���Ȼ���
 *  \li GUI_ASCII_WIDTH_CACHE_SHUT_DOWN : �ر�ascii�ַ���Ȼ���
 *  \li GUI_ASCII_WIDTH_CACHE_INVALIDATE: ʹ��ǰascii�ַ���Ȼ�����Ч
 *  \li GUI_RES_PIC_CACHE_SET_UP : ������������ԴСͼƬ����
 *  \li GUI_RES_PIC_CACHE_SHUT_DOWN : �ر���ԴСͼƬ����
 *  \li GUI_RES_PIC_CACHE_INVALIDATE: ʹ��ǰ��ԴСͼƬ������Ч
 *  \li GUI_TTF_DRV_CACHE_SET_SIZE: ����ttf�����ڲ�cache��С����KBΪ��λ
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
 *              2D��ͼ����
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
 *              �ַ�����ʾ����
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
 *              ��Դ�ļ���ʾ
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
 *              ����
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
 *              GDI�ӿ�
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
 *              �ַ���������(��δʵ��)
 *
 **********************************************************************
 */
int gui_config_font_glyph_cache(int *fontsize_list, int *cachesize_list);
int gui_config_font_width_cache(int *fontsize_list);
int gui_destroy_font_glyph_cache();
int gui_destroy_font_width_cache();

/*!\endcond*/
#endif   /* ifdef GUI_H_INC */
