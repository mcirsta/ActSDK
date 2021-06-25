/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : softkeyboard.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-4-14             v1.0             build this file 
 ********************************************************************************/


#include <ime.h>
#include "commonui_sty.h"
#include "softkeyboard_private.h"


static void load_key_table_resource(resgroup_resource_t * resgroup, struct key_table_data_s * key_table, int count);

static void init_keyboard(softkeyboard_t * keyboard);

static void softkeyboard_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg );
static void ckb_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg );
static void ekb_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg );
static void nkb_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg );
static bool _check_text_len( softkeyboard_t * keyboard, unsigned short * text );


static void get_candidacy_data(softkeyboard_t * keyboard);

bool softkeyboard_proc_gui(softkeyboard_t * keyboard, gui_wm_message_t *pmsg )
{
    if(NULL == keyboard)
    {
        print_warning("input parameter(keyboard) is a NULL pointer!");
        return false;
    }

    switch ( pmsg->msgid ) 
    {
        case WM_MSG_KEY:
        {
            softkeyboard_proc_key( keyboard, (key_event_t *)(pmsg->data.p) );
            break;
        }

        default:
        {
            break;
        }
    }

    return true;
}

softkeyboard_t * softkeyboard_create( IME_type_e ime, softkeyboard_cb_t callback )
{
    softkeyboard_t * keyboard;
    resgroup_resource_t* reg;
    sty_rect_t rect;

    print_dbg();

    if(NULL == callback)
    {
        print_warning("input parameter(callback) is a NULL pointer!");
        return NULL;
    }

    keyboard = (softkeyboard_t *)malloc(sizeof(softkeyboard_t));		
    if ( keyboard == NULL )
    {
        print_err("malloc failed!");
        return NULL;	
    }	

    keyboard->data = (struct softkey_private_data_s *)malloc(sizeof(struct softkey_private_data_s));		
    if ( keyboard->data == NULL )
    {
        print_err("malloc failed!");
        free(keyboard);
        return NULL;	
    }

    //initial data
    keyboard->data->ime = ime;
    keyboard->callback = callback;
    keyboard->canvas_id   = -1;

    //load style resource
    softkeyboard_load_style( keyboard );

    //create canvas
    reg = common_resource.reg_kb;

    rect.x = reg->absolute_x;
    rect.y = reg->absolute_y;
    rect.width = reg->width;
    rect.height = reg->height;

    keyboard->canvas_id = canvas_create(&rect, TRANSPARENT );
    if ( keyboard->canvas_id <= 0 )
    {
        print_err("create canvas failed!");
        free(keyboard->data);
        free(keyboard);
        return NULL;	
    }	

    canvas_erase_all(keyboard->canvas_id);
    keyboard->hdc = gui_dc_get(keyboard->canvas_id); 

    if(keyboard->hdc <= 0)
    {
        print_err("get dc failed!");
        canvas_delete(keyboard->canvas_id);
        free(keyboard->data);
        free(keyboard);
        return NULL;	
    }

    if(ime_engine_init() != 0)
    {
        print_err("init IME engine failed!");
        canvas_delete(keyboard->canvas_id);
        free(keyboard->data);
        free(keyboard);
        return NULL;	
    }

    /* init key table*/
    init_keyboard(keyboard);

    return keyboard;
}

static void init_keyboard(softkeyboard_t * keyboard)
{

    /* init keyboard  display info*/
    memset(keyboard->data->text, 0, TEXT_MAX_LEN * 2);
    keyboard->data->text_index = 0;

    /* init chinese pinying keyboard */
    keyboard->data->ckb_data.curr_key = CKB_KEY_Q;
    keyboard->data->ckb_data.state = CKB_STATE_INPUT; 

    memset(keyboard->data->ckb_data.candidacy_buf, 0 , CKB_MAX_CANDIDACY_CHARS * 2);
    keyboard->data->ckb_data.candidates = 0; 
    keyboard->data->ckb_data.candidacy_view_start = 0; 
    keyboard->data->ckb_data.candidacy_view_focus = 0; 
    keyboard->data->ckb_data.candidacy_view_end = 0;
    memset(keyboard->data->ckb_data.pinyin_buf, 0, CKB_PINYIN_BUF_LEN); 

    /* init english keyboard */
    keyboard->data->ekb_data.curr_key = EKB_KEY_Q;

    /* init number keyboard */
    keyboard->data->nkb_data.curr_key = NKB_KEY_0;
}

bool softkeyboard_paint( softkeyboard_t * keyboard )
{
    print_dbg();

    if(NULL == keyboard)
    {
        print_warning("input parameter(keyboard) is a NULL pointer!");
        return false;
    }

    paint_key_table(keyboard);
    paint_display_text(keyboard);

    if(IME_TYPE_PINYIN == keyboard->data->ime)
    {
        paint_candidacy(keyboard);
        paint_left_arrow(keyboard, false);
        paint_right_arrow(keyboard, false);
    }

    return true;
}



bool softkeyboard_delete( softkeyboard_t * keyboard )
{
    if(NULL == keyboard)
    {
        print_warning("input parameter(keyboard) is a NULL pointer!");
        return false;
    }

    ime_engine_exit();

    if ( keyboard->canvas_id != 0 )
    {
        canvas_delete( keyboard->canvas_id );
    }

    free(keyboard->data);
    free(keyboard);

    return true;
}

bool softkeyboard_get_text( softkeyboard_t * keyboard , char * utf8_text, unsigned int len)
{
    char utf8_tmp_buf[128] = {0};
    unsigned int utf8_tmp_len = 128;

    if(NULL == keyboard)
    {
        print_warning("input parameter(keyboard) is a NULL pointer!");
        return false;
    }

    if(NULL == utf8_text)
    {
        print_warning("input parameter(utf8_text) is a NULL pointer!");
        return false;
    }

    if( mlang_unicode_to_utf8 (keyboard->data->text, keyboard->data->text_index * 2, utf8_tmp_buf, &utf8_tmp_len) < 0)
    {
        print_err("convert unicode string to utf8 string failed!");
        return false;
    }

    if(len < (utf8_tmp_len + 1))
    {
        print_err("the length of utf8_text is not enough!");
        return false;
    }

    memcpy(utf8_text, utf8_tmp_buf, utf8_tmp_len);
    utf8_text[utf8_tmp_len] = '\0';

    return true; 
}

bool softkeyboard_set_text( softkeyboard_t * keyboard , const char * utf8_text)
{
    unsigned short unicode_tmp_buf[128] = {0};
    unsigned int unicode_tmp_len = 128 * 2;

    if(NULL == keyboard)
    {
        print_warning("input parameter(keyboard) is a NULL pointer!");
        return false;
    }

    if(NULL == utf8_text)
    {
        print_warning("input parameter(utf8_text) is a NULL pointer!");
        return false;
    }

    if( mlang_utf8_to_unicode ((char *)utf8_text, (int)strlen(utf8_text), unicode_tmp_buf, &unicode_tmp_len) < 0)
    {
        print_err("convert utf8 string to unicode string failed!");
        return false;
    }

    if(_check_text_len(keyboard, unicode_tmp_buf) == false)
    {
        print_err(" string is too long for show !");
        return false;
    }

    memcpy(keyboard->data->text, unicode_tmp_buf, unicode_tmp_len);
    keyboard->data->text_index = (int)(unicode_tmp_len / 2);
    keyboard->data->text[keyboard->data->text_index] = 0;

    return true;
}

bool softkeyboard_clear( softkeyboard_t * keyboard)
{
    if(NULL == keyboard)
    {
        print_warning("input parameter(keyboard) is a NULL pointer!");
        return false;
    }

    memset(keyboard->data->text, 0 , TEXT_MAX_LEN * 2);
    keyboard->data->text_index = 0;

    return true;
}

bool softkeyboard_load_style( softkeyboard_t * keyboard)
{
    scene_t* scene;	//!< commonui scene
    sty_rect_t rect;

    if(keyboard == NULL)
    {
        return false;
    }

    //get root resource
    scene = sty_get_common_scene();
    if ( scene == NULL )
    {
        print_err("get common scene failed!");
        return false;	
    }

    //load soft keyboard common resource such as text display
    common_resource.reg_kb = (resgroup_resource_t *)get_scene_child(scene, KEYBOARD_RESGROUP);

    common_resource.pic_display = \
         (picture_resource_t *)get_resgroup_child(common_resource.reg_kb, PIC_DISPLAY);
    common_resource.str_display = \
         (string_resource_t *)get_resgroup_child(common_resource.reg_kb, STR_DISPLAY);
    common_resource.str_display_two_line = \
         (string_resource_t *)get_resgroup_child(common_resource.reg_kb, STR_DISPLAY_TWO_LINE);

    //load chinese soft keyboard resource
    ckb_resource.reg_ckb = \
         (resgroup_resource_t *)get_resgroup_child(common_resource.reg_kb, REG_CKB);

    ckb_resource.pic_ckb_key_table_bg = \
         (picture_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, PIC_CKB_KEY_TABLE_BG);

    ckb_resource.pic_ckb_candidacy = \
         (picture_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, PIC_CKB_CANDIDACY);
    ckb_resource.str_ckb_candidacy = \
         (string_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, STR_CKB_CANDIDACY);

    ckb_resource.pic_ckb_focus = \
         (picture_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, PIC_CKB_FOCUS);
    ckb_resource.str_ckb_pinyin = \
         (string_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, STR_CKB_PINYIN);

    ckb_resource.pic_ckb_left_arrow_1 = \
         (picture_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, PIC_CKB_LEFT_ARROW_1);
    ckb_resource.pic_ckb_left_arrow_2 = \
         (picture_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, PIC_CKB_LEFT_ARROW_2);

    ckb_resource.pic_ckb_right_arrow_1 = \
         (picture_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, PIC_CKB_RIGHT_ARROW_1);
    ckb_resource.pic_ckb_right_arrow_2 = \
         (picture_resource_t *)get_resgroup_child(ckb_resource.reg_ckb, PIC_CKB_RIGHT_ARROW_2);

    load_key_table_resource(ckb_resource.reg_ckb, ckb_key_table, CKB_KEY_MAX);

    if( keyboard->data != NULL )
    {
        if((ckb_resource.pic_ckb_candidacy !=NULL) && (ckb_resource.pic_ckb_focus != NULL))
        {
            keyboard->data->ckb_data.max_candidacy_view_len = \
                    (ckb_resource.pic_ckb_candidacy->width - 4)/ckb_resource.pic_ckb_focus->width;
        }
    }

    //load english soft keyboard resource
    ekb_resource.reg_ekb = (resgroup_resource_t *)get_resgroup_child(common_resource.reg_kb, REG_EKB);

    ekb_resource.pic_ekb_key_table_bg = \
        (picture_resource_t *)get_resgroup_child(ekb_resource.reg_ekb, PIC_EKB_KEY_TABLE_BG);

    load_key_table_resource(ekb_resource.reg_ekb, ekb_key_table, EKB_KEY_MAX);

    //load number soft keyboard resource
    nkb_resource.reg_nkb = (resgroup_resource_t *)get_resgroup_child(common_resource.reg_kb, REG_NKB);

    nkb_resource.pic_nkb_key_table_bg = \
        (picture_resource_t *)get_resgroup_child(nkb_resource.reg_nkb, PIC_NKB_KEY_TABLE_BG);

    load_key_table_resource(nkb_resource.reg_nkb, nkb_key_table, NKB_KEY_MAX);

    //canvas resize    
    if ( (keyboard->canvas_id != -1) && (common_resource.reg_kb != NULL))
    {
        rect.x = common_resource.reg_kb->absolute_x;
        rect.y = common_resource.reg_kb->absolute_y;
        rect.width = common_resource.reg_kb->width;
        rect.height = common_resource.reg_kb->height;

        //set canvas region
        canvas_set_region( keyboard->canvas_id, &rect );
        canvas_erase_all(keyboard->canvas_id);
    }

    return true;
}

static void load_key_table_resource(resgroup_resource_t * resgroup, struct key_table_data_s * key_table, int count)
{
    int i ;
    for(i = 0; i < count; i++)
    {
        key_table[i].pic_res = ( picture_resource_t * )get_resgroup_child(resgroup, key_table[i].pic_id);
    }
}

static void softkeyboard_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg )
{
    print_dbg();

    if(IME_TYPE_PINYIN == keyboard->data->ime)
    {
        ckb_proc_key(keyboard, pmsg);
    }
    else if(IME_TYPE_EN == keyboard->data->ime)
    { 
        ekb_proc_key(keyboard, pmsg);
    }
    else
    {
        nkb_proc_key(keyboard, pmsg);
    }

}

static void get_candidacy_data(softkeyboard_t * keyboard)
{
    struct ckb_data_s * ckb_data;
    ckb_data = &(keyboard->data->ckb_data);

    ckb_data->candidates = CKB_MAX_CANDIDACY_CHARS;
    if(ime_engine_get_candidates(ckb_data->pinyin_buf, ckb_data->candidacy_buf, \
                CKB_MAX_CANDIDACY_CHARS * 2, &(ckb_data->candidates), 1) < 0)
    {
        print_dbg(" get candidates failed! ");

        memset(ckb_data->candidacy_buf, 0, CKB_MAX_CANDIDACY_CHARS * 2);
        ckb_data->candidates = 0;
        ckb_data->candidacy_view_start = 0;
        ckb_data->candidacy_view_focus = 0;
        ckb_data->candidacy_view_end = 0; 
    }
    else 
    {
        ckb_data->candidacy_view_start = 0;
        ckb_data->candidacy_view_focus = 0;
        ckb_data->candidacy_view_end = MIN(ckb_data->max_candidacy_view_len - 1, ckb_data->candidates - 1);

        print_dbg(" get %d candidates, start:%d, focus:%d, end:%d", ckb_data->candidates,
                ckb_data->candidacy_view_start, ckb_data->candidacy_view_focus, ckb_data->candidacy_view_end);
    }
}

#pragma __PRQA_IGNORE_START__

static void ckb_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg )
{
    struct ckb_data_s * ckb_data;
    print_dbg();

    ckb_data = &(keyboard->data->ckb_data);

    if(CKB_STATE_INPUT == ckb_data->state)
    {
        print_dbg("state: input");

        switch ( pmsg->val | pmsg->type ) 
        {
            case KEY_PREV | KEY_TYPE_DOWN:		
            case KEY_PREV | KEY_TYPE_HOLD:		
            {
                if(ckb_data->curr_key > 0)
                {
                    ckb_data->curr_key--;
                }
                else if(0 == ckb_data->curr_key)
                {
                    ckb_data->curr_key = CKB_KEY_MAX - 1;
                }
                else
                {
                    print_err();
                }

                paint_key_table(keyboard);

                /* 清除因处理过慢累积的按键消息 */
                clear_key_msg_queue(-1,-1);
                break;
            }

            case KEY_NEXT | KEY_TYPE_DOWN:		
            case KEY_NEXT | KEY_TYPE_HOLD:		
            {
                if(ckb_data->curr_key < CKB_KEY_MAX - 1)
                {
                    ckb_data->curr_key++;
                }
                else if((CKB_KEY_MAX - 1) == ckb_data->curr_key)
                {
                    ckb_data->curr_key = 0;
                }
                else
                {
                    print_err();
                }

                paint_key_table(keyboard);

                /* 清除因处理过慢累积的按键消息 */
                clear_key_msg_queue(-1,-1);
                break;
            }

            case KEY_MENU | KEY_TYPE_DOWN:
            {
                if(strlen(ckb_data->pinyin_buf) > 0)
                {
                    ckb_data->state = CKB_STATE_CANDIDACY; 
                    
                    paint_key_table(keyboard);
                    
                    /* 清除因处理过慢累积的按键消息 */
                    clear_key_msg_queue(-1,-1);
                }
                break;
            }

            case KEY_OK | KEY_TYPE_SHORT_UP:
            {
                if(ckb_key_table[ckb_data->curr_key].pic_id == PIC_CKB_RETURN)
                {
                    keyboard->callback(keyboard, WIDGET_SIG_SKB_CANCEL);
                }
                else if(ckb_key_table[ckb_data->curr_key].pic_id == PIC_CKB_OK)
                {
                    keyboard->callback(keyboard, WIDGET_SIG_SKB_OK);
                }
                else if(ckb_key_table[ckb_data->curr_key].pic_id == PIC_CKB_IME)
                {
                    keyboard->data->ime = IME_TYPE_EN;
                    keyboard->data->ekb_data.curr_key = EKB_KEY_IME;

                    canvas_erase_all(keyboard->canvas_id);
                    softkeyboard_paint(keyboard);
                }
                else if(ckb_key_table[ckb_data->curr_key].pic_id == PIC_CKB_DEL)
                {
                    unsigned int len = strlen(ckb_data->pinyin_buf);
                    if(len > 0)
                    {
                        ckb_data->pinyin_buf[len - 1] = 0;
                        get_candidacy_data(keyboard);

                        paint_candidacy(keyboard);
                    }
                    else
                    {
                        if(keyboard->data->text_index > 0)
                        {
                            keyboard->data->text_index--;
                            keyboard->data->text[keyboard->data->text_index] = 0;
                            paint_display_text(keyboard);
                        }
                    }
                }
                else if(ckb_key_table[ckb_data->curr_key].pic_id == PIC_CKB_SPACE)
                {
                    if(keyboard->data->text_index < TEXT_MAX_LEN )
                    {
                        keyboard->data->text[keyboard->data->text_index++] = ' ';
                        if(_check_text_len(keyboard, keyboard->data->text) == true)
                        {
                            paint_display_text(keyboard);
                        }
                        else
                        {
                            keyboard->data->text_index--;
                            keyboard->data->text[keyboard->data->text_index] = 0;
                        }
                    }
                }
                else
                {
                    if(strlen(ckb_data->pinyin_buf) < (CKB_PINYIN_BUF_LEN - 1))
                    {
                        strncat((char *)ckb_data->pinyin_buf, ckb_key_table[ckb_data->curr_key].name, 1);
                        get_candidacy_data(keyboard);

                        paint_candidacy(keyboard);
                    }
                }

                break;
            }

            case KEY_RETURN | KEY_TYPE_SHORT_UP:
            {
                keyboard->callback(keyboard, WIDGET_SIG_SKB_CANCEL);
                break;
            }

            default:
            {
                break;
            }
        }    
    }
    else if(CKB_STATE_CANDIDACY == ckb_data->state)
    {
        print_dbg("state: candidacy");

        switch ( pmsg->val | pmsg->type ) 
        {
            case KEY_PREV | KEY_TYPE_DOWN:		
            case KEY_PREV | KEY_TYPE_HOLD:		
            {
                /* 清除因处理过慢累积的按键消息 */
                clear_key_msg_queue(-1,-1);

                if(ckb_data->candidacy_view_focus > 0)
                {
                    ckb_data->candidacy_view_focus--;

                    paint_candidacy(keyboard);
                }
                else if(0 == ckb_data->candidacy_view_focus)
                {
                    if(ckb_data->candidacy_view_start >= ckb_data->max_candidacy_view_len)
                    {
                        ckb_data->candidacy_view_start -= ckb_data->max_candidacy_view_len;
                        ckb_data->candidacy_view_focus = ckb_data->max_candidacy_view_len- 1;
                        ckb_data->candidacy_view_end = \
                             ckb_data->candidacy_view_start + ckb_data->max_candidacy_view_len- 1;

                        paint_candidacy(keyboard);
                    }
                }
                else
                {
                    print_err();
                }

                paint_left_arrow(keyboard, true);

                break;
            }

            case KEY_PREV | KEY_TYPE_SHORT_UP:		
            case KEY_PREV | KEY_TYPE_LONG_UP:		
            case KEY_PREV | KEY_TYPE_HOLD_UP:		
            {
                paint_left_arrow(keyboard, false);
                break;
            }

            case KEY_NEXT | KEY_TYPE_DOWN:		
            case KEY_NEXT | KEY_TYPE_HOLD:		
            {
                /* 清除因处理过慢累积的按键消息 */
                clear_key_msg_queue(-1,-1);

                if(ckb_data->candidacy_view_focus < (ckb_data->candidacy_view_end - ckb_data->candidacy_view_start))
                {
                    ckb_data->candidacy_view_focus++;
                    paint_candidacy(keyboard);
                }
                else if(ckb_data->candidacy_view_focus == (ckb_data->candidacy_view_end - ckb_data->candidacy_view_start))
                {
                    if((ckb_data->candidacy_view_end < (ckb_data->candidates - 1) ))
                    {
                        ckb_data->candidacy_view_start = ckb_data->candidacy_view_end + 1;
                        ckb_data->candidacy_view_focus = 0;
                        ckb_data->candidacy_view_end = ckb_data->candidacy_view_start + \
                        MIN(ckb_data->max_candidacy_view_len - 1, ckb_data->candidates -1 - ckb_data->candidacy_view_start);
                        paint_candidacy(keyboard);
                    }
                }
                else
                {
                    print_err();
                }

                paint_right_arrow(keyboard, true);

                break;
            }

            case KEY_NEXT | KEY_TYPE_SHORT_UP:		
            case KEY_NEXT | KEY_TYPE_LONG_UP:		
            case KEY_NEXT | KEY_TYPE_HOLD_UP:		
            {
                paint_right_arrow(keyboard, false);
                break;
            }

            case KEY_MENU | KEY_TYPE_DOWN:
            {
                ckb_data->state = CKB_STATE_INPUT; 
                
                paint_key_table(keyboard);
                
                /* 清除因处理过慢累积的按键消息 */
                clear_key_msg_queue(-1,-1);
                break;
            }

            case KEY_OK | KEY_TYPE_SHORT_UP:
            {

                if( keyboard->data->text_index < TEXT_MAX_LEN )
                {
                    keyboard->data->text[keyboard->data->text_index++] = \
                    ckb_data->candidacy_buf[ckb_data->candidacy_view_start + ckb_data->candidacy_view_focus];

                    if(_check_text_len(keyboard, keyboard->data->text) == true)
                    {
                        memset(ckb_data->candidacy_buf, 0, CKB_MAX_CANDIDACY_CHARS * 2);
                        ckb_data->candidates = 0;
                        ckb_data->candidacy_view_start = 0;
                        ckb_data->candidacy_view_focus = 0;
                        ckb_data->candidacy_view_end = 0; 

                        memset(ckb_data->pinyin_buf, 0, CKB_PINYIN_BUF_LEN);

                        ckb_data->state = CKB_STATE_INPUT; 

                        paint_display_text(keyboard);
                        paint_candidacy(keyboard);
                        paint_key_table(keyboard);

                        /*
                         * xxx:if the new inserted candidate character is NULL, it's invalid.
                         */
                        if(keyboard->data->text[keyboard->data->text_index -1] == 0)
                        {
                            keyboard->data->text_index--;
                        }
                    }
                    else
                    {
                        keyboard->data->text_index--;
                        keyboard->data->text[keyboard->data->text_index] = 0;
                    }
                }

                break;
            }

            case KEY_RETURN | KEY_TYPE_SHORT_UP:
            {
                keyboard->callback(keyboard, WIDGET_SIG_SKB_CANCEL);
                break;
            }

            default:
            {
                break;
            }
        }
    }
}

static void ekb_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg )
{

    struct ekb_data_s * ekb_data;
    print_dbg();

    ekb_data = &(keyboard->data->ekb_data);

    switch ( pmsg->val | pmsg->type ) 
    {
        case KEY_PREV | KEY_TYPE_DOWN:		
        case KEY_PREV | KEY_TYPE_HOLD:		
        {
            if(ekb_data->curr_key > 0)
            {
                ekb_data->curr_key--;
            }
            else if(0 == ekb_data->curr_key)
            {
                ekb_data->curr_key = EKB_KEY_MAX - 1;
            }
            else
            {
                print_err();
            }

            paint_key_table(keyboard);

            /* 清除因处理过慢累积的按键消息 */
            clear_key_msg_queue(-1,-1);
            break;
        }

        case KEY_NEXT | KEY_TYPE_DOWN:		
        case KEY_NEXT | KEY_TYPE_HOLD:		
        {
            if(ekb_data->curr_key < EKB_KEY_MAX - 1)
            {
                ekb_data->curr_key++;
            }
            else if((EKB_KEY_MAX - 1) == ekb_data->curr_key)
            {
                ekb_data->curr_key = 0;
            }
            else
            {
                print_err();
            }

            paint_key_table(keyboard);

            /* 清除因处理过慢累积的按键消息 */
            clear_key_msg_queue(-1,-1);
            break;
        }

        case KEY_OK | KEY_TYPE_SHORT_UP:
        {
            if(ekb_key_table[ekb_data->curr_key].pic_id == PIC_EKB_RETURN)
            {
                keyboard->callback(keyboard, WIDGET_SIG_SKB_CANCEL);
            }
            else if(ekb_key_table[ekb_data->curr_key].pic_id == PIC_EKB_OK)
            {
                keyboard->callback(keyboard, WIDGET_SIG_SKB_OK);
            }
            else if(ekb_key_table[ekb_data->curr_key].pic_id == PIC_EKB_IME)
            {
                keyboard->data->ime = IME_TYPE_NUM;
                keyboard->data->nkb_data.curr_key = NKB_KEY_IME;

                canvas_erase_all(keyboard->canvas_id);
                softkeyboard_paint(keyboard);
            }
            else if(ekb_key_table[ekb_data->curr_key].pic_id == PIC_EKB_DEL)
            {
                if(keyboard->data->text_index > 0)
                {
                    keyboard->data->text_index--;
                    keyboard->data->text[keyboard->data->text_index] = 0;
                    paint_display_text(keyboard);
                }
            }
            else if(ekb_key_table[ekb_data->curr_key].pic_id == PIC_EKB_SPACE)
            {
                if(keyboard->data->text_index < TEXT_MAX_LEN )
                {
                    keyboard->data->text[keyboard->data->text_index++] = ' ';
                    if(_check_text_len(keyboard, keyboard->data->text) == true)
                    {
                        paint_display_text(keyboard);
                    }
                    else
                    {
                        keyboard->data->text_index--;
                        keyboard->data->text[keyboard->data->text_index] = 0;
                    }
                }
            }
            else
            {
                if(keyboard->data->text_index < TEXT_MAX_LEN )
                {
                    keyboard->data->text[keyboard->data->text_index++] = (unsigned short)ekb_key_table[ekb_data->curr_key].name[0];
                    if(_check_text_len(keyboard, keyboard->data->text) == true)
                    {
                        paint_display_text(keyboard);
                    }
                    else
                    {
                        keyboard->data->text_index--;
                        keyboard->data->text[keyboard->data->text_index] = 0;
                    }
                }
            }

            break;
        }

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        {
            keyboard->callback(keyboard, WIDGET_SIG_SKB_CANCEL);
            break;
        }

        default:
        {
            break;
        }
    }
}

static void nkb_proc_key( softkeyboard_t * keyboard, key_event_t *pmsg )
{
    struct nkb_data_s * nkb_data;
    print_dbg();

    nkb_data = &(keyboard->data->nkb_data);

    switch ( pmsg->val | pmsg->type ) 
    {
        case KEY_PREV | KEY_TYPE_DOWN:		
        case KEY_PREV | KEY_TYPE_HOLD:		
        {
            if(nkb_data->curr_key > 0)
            {
                nkb_data->curr_key--;
            }
            else if(0 == nkb_data->curr_key)
            {
                nkb_data->curr_key = NKB_KEY_MAX - 1;
            }
            else  
            {
                print_err();
            }

            paint_key_table(keyboard);

            /* 清除因处理过慢累积的按键消息 */
            clear_key_msg_queue(-1,-1);
            break;
        }

        case KEY_NEXT | KEY_TYPE_DOWN:		
        case KEY_NEXT | KEY_TYPE_HOLD:		
        {
            if(nkb_data->curr_key < NKB_KEY_MAX - 1)
            {
                nkb_data->curr_key++;
            }
            else if((NKB_KEY_MAX - 1) == nkb_data->curr_key)
            {
                nkb_data->curr_key = 0;
            }
            else
            {
                print_err();
            }

            paint_key_table(keyboard);

            /* 清除因处理过慢累积的按键消息 */
            clear_key_msg_queue(-1,-1);
            break;
        }

        case KEY_OK | KEY_TYPE_SHORT_UP:
        {
            if(nkb_key_table[nkb_data->curr_key].pic_id == PIC_NKB_RETURN)
            {
                keyboard->callback(keyboard, WIDGET_SIG_SKB_CANCEL);
            }
            else if(nkb_key_table[nkb_data->curr_key].pic_id == PIC_NKB_OK)
            {
                keyboard->callback(keyboard, WIDGET_SIG_SKB_OK);
            }
            else if(nkb_key_table[nkb_data->curr_key].pic_id == PIC_NKB_IME)
            {
                keyboard->data->ime = IME_TYPE_PINYIN;
                keyboard->data->ckb_data.curr_key = CKB_KEY_IME;

                canvas_erase_all(keyboard->canvas_id);
                softkeyboard_paint(keyboard);
            }
            else if(nkb_key_table[nkb_data->curr_key].pic_id == PIC_NKB_DEL)
            {
                if(keyboard->data->text_index > 0)
                {
                    keyboard->data->text_index--;
                    keyboard->data->text[keyboard->data->text_index] = 0;
                    paint_display_text(keyboard);
                }
            }
            else if(nkb_key_table[nkb_data->curr_key].pic_id == PIC_NKB_SPACE)
            {
                if(keyboard->data->text_index < TEXT_MAX_LEN )
                {
                    keyboard->data->text[keyboard->data->text_index++] = ' ';

                    if(_check_text_len(keyboard, keyboard->data->text) == true)
                    {
                        paint_display_text(keyboard);
                    }
                    else
                    {
                        keyboard->data->text_index--;
                        keyboard->data->text[keyboard->data->text_index] = 0;
                    }
                }
            }
            else
            {
                if(keyboard->data->text_index < TEXT_MAX_LEN )
                {
                    keyboard->data->text[keyboard->data->text_index++] = (unsigned short)nkb_key_table[nkb_data->curr_key].name[0];
                    if(_check_text_len(keyboard, keyboard->data->text) == true)
                    {
                        paint_display_text(keyboard);
                    }
                    else
                    {
                        keyboard->data->text_index--;
                        keyboard->data->text[keyboard->data->text_index] = 0;
                    }
                }
            }

            break;
        }

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        {
            keyboard->callback(keyboard, WIDGET_SIG_SKB_CANCEL);
            break;
        }

        default:
        {
            break;
        }
    }
}

#pragma __PRQA_IGNORE_END__

static bool _check_text_len( softkeyboard_t * keyboard, unsigned short * text )
{
    int len = 0;
    int len_2 = 0;
    unsigned short first_line_text[TEXT_MAX_LEN] = {0};
    unsigned short * next_line_text = NULL;
    gui_text_encode_t  cur_encoding;

    if((text != NULL) && (keyboard != NULL))
    {

        gui_dc_set_font_size( keyboard->hdc, (int)common_resource.str_display->font_height );
        len = gui_dc_get_string_dist_x(keyboard->hdc, (char *)text, GUI_UNICODE_ENC);

        if(len < common_resource.str_display->width)
        {
            return true;
        }
        else
        {
            gui_dc_set_font_size( keyboard->hdc, (int)common_resource.str_display_two_line->font_height );
            len = gui_dc_get_string_dist_x(keyboard->hdc, (char *)text, GUI_UNICODE_ENC);

            if(len < common_resource.str_display->width)
            {
                return true;
            }
            else
            {
                cur_encoding = sty_get_text_encoding();
                sty_set_text_encoding(GUI_UNICODE_ENC);

                next_line_text = (unsigned short *)sty_truncate_string_by_width( keyboard->hdc, (char *)text, 
                        (char *)first_line_text, common_resource.str_display->width);    

                len_2 = gui_dc_get_string_dist_x(keyboard->hdc, (char *)next_line_text, GUI_UNICODE_ENC);

                sty_set_text_encoding(cur_encoding);

                if(len_2 < common_resource.str_display_two_line->width)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }

    return false;
}

