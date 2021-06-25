/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : softkeyboard_private.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-6             v1.0             build this file 
 ********************************************************************************/

#ifndef __SOFT_KEYBOARD_PRIVATE_H__
#define __SOFT_KEYBOARD_PRIVATE_H__

#include <include_case.h>

#define CKB_MAX_CANDIDACY_CHARS      (250)
//#define CKB_MAX_CANDIDACY_VIEW_LEN   (9)
#define CKB_PINYIN_BUF_LEN           (6)

#define TEXT_MAX_LEN                 (100)

#ifdef MIN
#undef MIN
#endif
#define MIN(x, y)                    (((x) <= (y))?(x):(y))

enum ckb_key
{
    CKB_KEY_Q = 0, 
    CKB_KEY_W, 
    CKB_KEY_E, 
    CKB_KEY_R, 
    CKB_KEY_T, 
    CKB_KEY_Y, 
    CKB_KEY_U, 
    CKB_KEY_I, 
    CKB_KEY_O, 
    CKB_KEY_P, 

    CKB_KEY_A, 
    CKB_KEY_S, 
    CKB_KEY_D, 
    CKB_KEY_F, 
    CKB_KEY_G, 
    CKB_KEY_H, 
    CKB_KEY_J, 
    CKB_KEY_K, 
    CKB_KEY_L, 

    CKB_KEY_OK,
    CKB_KEY_Z, 
    CKB_KEY_X, 
    CKB_KEY_C, 
    CKB_KEY_V, 
    CKB_KEY_B, 
    CKB_KEY_N, 
    CKB_KEY_M, 
    CKB_KEY_DEL,

    CKB_KEY_IME,
    CKB_KEY_SPACE,
    CKB_KEY_RETURN, 
    CKB_KEY_MAX, 
};

enum ekb_key
{
    EKB_KEY_Q = 0, 
    EKB_KEY_W, 
    EKB_KEY_E, 
    EKB_KEY_R, 
    EKB_KEY_T, 
    EKB_KEY_Y, 
    EKB_KEY_U, 
    EKB_KEY_I, 
    EKB_KEY_O, 
    EKB_KEY_P, 

    EKB_KEY_A, 
    EKB_KEY_S, 
    EKB_KEY_D, 
    EKB_KEY_F, 
    EKB_KEY_G, 
    EKB_KEY_H, 
    EKB_KEY_J, 
    EKB_KEY_K, 
    EKB_KEY_L, 

    EKB_KEY_OK,
    EKB_KEY_Z, 
    EKB_KEY_X, 
    EKB_KEY_C, 
    EKB_KEY_V, 
    EKB_KEY_B, 
    EKB_KEY_N, 
    EKB_KEY_M, 
    EKB_KEY_DEL,

    EKB_KEY_IME,
    EKB_KEY_SPACE,
    EKB_KEY_RETURN, 
    EKB_KEY_MAX, 
};

enum nkb_key
{
    NKB_KEY_0, 
    NKB_KEY_1, 
    NKB_KEY_2, 
    NKB_KEY_3, 
    NKB_KEY_4, 

    NKB_KEY_5, 
    NKB_KEY_6, 
    NKB_KEY_7, 
    NKB_KEY_8, 
    NKB_KEY_9, 

    NKB_KEY_OK,
    NKB_KEY_STAR,
    NKB_KEY_QUESTION, 
    NKB_KEY_DEL,

    NKB_KEY_IME,
    NKB_KEY_SPACE,
    NKB_KEY_RETURN,
    NKB_KEY_MAX,
};

enum ckb_state_e 
{
    CKB_STATE_INPUT,
    CKB_STATE_CANDIDACY,
};

struct ckb_data_s
{
    enum ckb_state_e state; 

    unsigned short candidacy_buf[CKB_MAX_CANDIDACY_CHARS];
    int max_candidacy_view_len;
    int candidates;
    int candidacy_view_start;
    int candidacy_view_focus;
    int candidacy_view_end;

    char pinyin_buf[CKB_PINYIN_BUF_LEN];

    int curr_key;
};

struct ekb_data_s
{
    int curr_key;
};

struct nkb_data_s
{
    int curr_key;
};

struct softkey_private_data_s
{
    IME_type_e ime;

    struct ckb_data_s  ckb_data;
    struct ekb_data_s  ekb_data;
    struct nkb_data_s  nkb_data;

    unsigned short text[TEXT_MAX_LEN];
    int text_index;

};

struct key_table_data_s
{
    unsigned int pic_id;
    picture_resource_t * pic_res;
    char name[8];
};

extern struct key_table_data_s  ckb_key_table[CKB_KEY_MAX]; 
extern struct key_table_data_s  ekb_key_table[EKB_KEY_MAX];
extern struct key_table_data_s  nkb_key_table[NKB_KEY_MAX];

/*! Chinese style keyboard resource data */
struct ckb_resource_s
{
    resgroup_resource_t * reg_ckb;
    picture_resource_t * pic_ckb_key_table_bg;

    picture_resource_t * pic_ckb_candidacy;
    string_resource_t * str_ckb_candidacy;
    picture_resource_t * pic_ckb_focus;
    string_resource_t * str_ckb_pinyin;

    picture_resource_t * pic_ckb_left_arrow_1;
    picture_resource_t * pic_ckb_left_arrow_2;
    picture_resource_t * pic_ckb_right_arrow_1;
    picture_resource_t * pic_ckb_right_arrow_2;
}ckb_resource;

struct ekb_resource_s
{
    resgroup_resource_t * reg_ekb;
    picture_resource_t * pic_ekb_key_table_bg;
};

struct nkb_resource_s
{
    resgroup_resource_t * reg_nkb;
    picture_resource_t * pic_nkb_key_table_bg;
};

struct common_resource_s
{
    resgroup_resource_t * reg_kb;
    picture_resource_t * pic_display;
    string_resource_t * str_display;
    string_resource_t * str_display_two_line;
};   

/*! Chinese style keyboard resource data */
extern struct ckb_resource_s ckb_resource;

/*! English style keyboard resource data */
extern struct ekb_resource_s ekb_resource;

/*! Number style keyboard resource data */
extern struct nkb_resource_s nkb_resource;

/*! common resource data */
extern struct common_resource_s common_resource;    

extern void paint_key_table(softkeyboard_t * keyboard);
extern void paint_display_text(softkeyboard_t * keyboard);
extern void paint_candidacy(softkeyboard_t * keyboard);
extern void paint_left_arrow(softkeyboard_t * keyboard, bool highlight);
extern void paint_right_arrow(softkeyboard_t * keyboard, bool highlight);

#endif /* #ifndef __SOFT_KEYBOARD_PRIVATE_H__ */
