/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : softkeyboard_private.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-6             v1.0             build this file 
 ********************************************************************************/

#include "softkeyboard_private.h"

struct key_table_data_s  ckb_key_table[CKB_KEY_MAX] = {
    { PIC_CKB_Q, NULL, "q" },
    { PIC_CKB_W, NULL, "w" },
    { PIC_CKB_E, NULL, "e" },
    { PIC_CKB_R, NULL, "r" },
    { PIC_CKB_T, NULL, "t" },
    { PIC_CKB_Y, NULL, "y" },
    { PIC_CKB_U, NULL, "u" },
    { PIC_CKB_I, NULL, "i" },
    { PIC_CKB_O, NULL, "o" },
    { PIC_CKB_P, NULL, "p" },

    { PIC_CKB_A, NULL, "a" },
    { PIC_CKB_S, NULL, "s" },
    { PIC_CKB_D, NULL, "d" },
    { PIC_CKB_F, NULL, "f" },
    { PIC_CKB_G, NULL, "g" },
    { PIC_CKB_H, NULL, "h" },
    { PIC_CKB_J, NULL, "j" },
    { PIC_CKB_K, NULL, "k" },
    { PIC_CKB_L, NULL, "l" },

    { PIC_CKB_OK, NULL, "OK"},
    { PIC_CKB_Z, NULL, "z" },
    { PIC_CKB_X, NULL, "x" },
    { PIC_CKB_C, NULL, "c" },
    { PIC_CKB_V, NULL, "v" },
    { PIC_CKB_B, NULL, "b" },
    { PIC_CKB_N, NULL, "n" },
    { PIC_CKB_M, NULL, "m" },
    { PIC_CKB_DEL, NULL, "DEL" },

    { PIC_CKB_IME, NULL, "IME" },
    { PIC_CKB_SPACE, NULL, "SPACE" },
    { PIC_CKB_RETURN, NULL, "RETURN"}

};

struct key_table_data_s  ekb_key_table[EKB_KEY_MAX] = {
    { PIC_EKB_Q, NULL, "Q" },
    { PIC_EKB_W, NULL, "W" },
    { PIC_EKB_E, NULL, "E" },
    { PIC_EKB_R, NULL, "R" },
    { PIC_EKB_T, NULL, "T" },
    { PIC_EKB_Y, NULL, "Y" },
    { PIC_EKB_U, NULL, "U" },
    { PIC_EKB_I, NULL, "I" },
    { PIC_EKB_O, NULL, "O" },
    { PIC_EKB_P, NULL, "P" },

    { PIC_EKB_A, NULL, "A" },
    { PIC_EKB_S, NULL, "S" },
    { PIC_EKB_D, NULL, "D" },
    { PIC_EKB_F, NULL, "F" },
    { PIC_EKB_G, NULL, "G" },
    { PIC_EKB_H, NULL, "H" },
    { PIC_EKB_J, NULL, "J" },
    { PIC_EKB_K, NULL, "K" },
    { PIC_EKB_L, NULL, "L" },

    { PIC_EKB_OK, NULL, "OK"},
    { PIC_EKB_Z, NULL, "Z" },
    { PIC_EKB_X, NULL, "X" },
    { PIC_EKB_C, NULL, "C" },
    { PIC_EKB_V, NULL, "V" },
    { PIC_EKB_B, NULL, "B" },
    { PIC_EKB_N, NULL, "N" },
    { PIC_EKB_M, NULL, "M" },
    { PIC_EKB_DEL, NULL, "DEL" },

    { PIC_EKB_IME, NULL, "IME" },
    { PIC_EKB_SPACE, NULL, "SPACE" },
    { PIC_EKB_RETURN, NULL, "RETURN"}

};

struct key_table_data_s  nkb_key_table[NKB_KEY_MAX] = {
    { PIC_NKB_0, NULL, "0" },
    { PIC_NKB_1, NULL, "1" },
    { PIC_NKB_2, NULL, "2" },
    { PIC_NKB_3, NULL, "3" },
    { PIC_NKB_4, NULL, "4" },

    { PIC_NKB_5, NULL, "5" },
    { PIC_NKB_6, NULL, "6" },
    { PIC_NKB_7, NULL, "7" },
    { PIC_NKB_8, NULL, "8" },
    { PIC_NKB_9, NULL, "9" },

    { PIC_NKB_OK, NULL, "OK"},
    { PIC_NKB_STAR, NULL, "*" },
    { PIC_NKB_QUESTION, NULL, "?" },
    { PIC_NKB_DEL, NULL, "DEL" },

    { PIC_NKB_IME, NULL, "IME" },
    { PIC_NKB_SPACE, NULL, "SPACE" },
    { PIC_NKB_RETURN, NULL, "RETURN"}
};


/*! Chinese style keyboard resource data */
struct ckb_resource_s ckb_resource;

/*! English style keyboard resource data */
struct ekb_resource_s ekb_resource;

/*! Number style keyboard resource data */
struct nkb_resource_s nkb_resource;

/*! common resource data */
struct common_resource_s common_resource;    
