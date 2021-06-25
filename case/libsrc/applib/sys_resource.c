/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_resource.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-11             v1.0             build this file 
 ********************************************************************************/

#include <include_case.h>
#include <mlang.h>
#include "applib_config.h"

#ifdef PATH_SIZE
#undef PATH_SIZE
#endif

#define PATH_SIZE     (512)

#define MAX_LANGUAGES  (255)
#define APPS_DEFAULT_DIR   ADISK_ROOT"/apps"
#define MISC_DEFAULT_DIR   ADISK_ROOT"/misc"

#define LANG_CODES_SIZE  (32)  
#define THEME_NAME_SIZE  (128)  

#ifdef ARRAY_SIZE
#undef ARRAY_SIZE
#endif

#define ARRAY_SIZE(A)    (sizeof(A)/sizeof((A)[0]))


static char s_app_string_res_dir[PATH_SIZE] = {APPS_DEFAULT_DIR};
static char s_common_string_res_dir[PATH_SIZE] = {APPS_DEFAULT_DIR};
static char s_cur_lang_codes[LANG_CODES_SIZE] = {"zh_CN"}; 

static char s_default_font[PATH_SIZE] = {"FWQY"};

static char s_app_theme_res_dir[PATH_SIZE] = {APPS_DEFAULT_DIR};
static char s_common_theme_res_dir[PATH_SIZE] = {APPS_DEFAULT_DIR};
static char s_cur_theme_name[THEME_NAME_SIZE] = {"style_1"};

typedef struct 
{  
    /* language and country names codes according to ISO 936 and ISO 3166,
     * zh_CN , en_GB etc.
     */
    char * codes;   

    /* language id */ 
    int id;

}language_array_t;

static bool _check_dir( char * path, char * check_parent_start);

static language_array_t lang_array[MAX_LANGUAGES] =
{
    { "invalid" ,        MLANG_INVALID },  /* 0   �Ƿ�ֵ */
    { "aa_DF" ,          MLANG_AA_DF   },  /* 1   Ĭ���� */
    { "ae" ,             MLANG_AE      },  /* 2   ��ά˹���� */
    { "af_1" ,           MLANG_AF_1    },  /* 3   �Ϸ���(��������˹��) */
    { "ak" ,             MLANG_AK      },  /* 4   ������ */
    { "am" ,             MLANG_AM      },  /* 5   ��ķ������ */
    { "an" ,             MLANG_AN      },  /* 6   ������ */
    { "ar" ,             MLANG_AR      },  /* 7   �������� */
    { "as" ,             MLANG_AS      },  /* 8   ����ķ�� */
    { "av" ,             MLANG_AV      },  /* 9   ���߶��� */
    { "ay" ,             MLANG_AY      },  /* 10  �������� */
    { "az_1" ,           MLANG_AZ_1    },  /* 11  �����ݽ���  �����ݽ����������    1251 */
    { "az_2" ,           MLANG_AZ_2    },  /* 12  �����ݽ���  �����ݽ��������ģ�1252    */
    { "ba" ,             MLANG_BA      },  /* 13  ��ʲ������ */
    { "be_BY" ,          MLANG_BE_BY   },  /* 14  �׶���˹��  �׶���˹(Byelorussian)    1251 */
    { "bg_BG" ,          MLANG_BG_BG   },  /* 15  ����������  ��������(Bulgarian)   1251 */
    { "bh"  ,            MLANG_BH      },  /* 16  �ȹ����� */
    { "bi"  ,            MLANG_BI      },  /* 17  ��˹������ */
    { "bm"  ,            MLANG_BM      },  /* 18  ������� */
    { "bn"  ,            MLANG_BN      },  /* 19  �ϼ����� */
    { "bo"  ,            MLANG_BO      },  /* 20  ���� */
    { "br"  ,            MLANG_BR      },  /* 21  ���������� */
    { "bs_BA" ,          MLANG_BS_BA   },  /* 22  ��˹������  ��˹���Ǻͺ�����ά�� */
    { "ca_AD" ,          MLANG_CA_AD   },  /* 23  ��̩�������� ���������ٷ����� 1252 */
    { "ce" ,             MLANG_CE      },  /* 24  ������ */
    { "ch" ,             MLANG_CH      },  /* 25  ��Ī���� */
    { "co" ,             MLANG_CO      },  /* 26  �������� */
    { "cr" ,             MLANG_CR      },  /* 27  ������ */
    { "cs_CZ" ,          MLANG_CS_CZ   },  /* 28  �ݿ���  �ݿ�(?esky) 1257 */
    { "cu" ,             MLANG_CU      },  /* 29  �Ž̻�˹������ */
    { "cv" ,             MLANG_CV      },  /* 30  ����ʲ�� */
    { "cy" ,             MLANG_CY      },  /* 31  ����ʿ�Welsh�� ����ʿ 1252 */
    { "cy_2" ,           MLANG_CY_2    },  /* 32  ����ʿ�� 1252 */
    { "da_DK" ,          MLANG_DA_DK   },  /* 33  ������ ����Dansk) 1250 */
    { "de_AT" ,          MLANG_DE_AT   },  /* 34  ����  �µ��� 1252 */
    { "de_CH" ,          MLANG_DE_CH   },  /* 35  ����  ��ʿ  1252 */
    { "de_GE" ,          MLANG_DE_GE   },  /* 36  ����  �¹�(Deutsch) 1252 */
    { "de_LI" ,          MLANG_DE_LI   },  /* 37  ����  ��֧��ʿ�أ�Liechtenstein��1252 */
    { "de_LU" ,          MLANG_DE_LU   },  /* 38  ����  ¬ɭ�� 1252 */
    { "dv" ,             MLANG_DV      },  /* 39  ��άϣ�� */
    { "dz" ,             MLANG_DZ      },  /* 40  ������ */
    { "ee" ,             MLANG_EE      },  /* 41  ��ά�� */
    { "el" ,             MLANG_EL      },  /* 42  �ִ�ϣ���� */
    { "el_EG" ,          MLANG_EL_EG   },  /* 43  ϣ����  ϣ��(���˦˦Ǧͦɦʦ�) 1253 */
    { "en_AU" ,          MLANG_EN_AU   },  /* 44  Ӣ��  �Ĵ����� 1252 */
    { "en_BZ" ,          MLANG_EN_BZ   },  /* 45  Ӣ��  ������ 1252 */
    { "en_CA" ,          MLANG_EN_CA   },  /* 46  Ӣ��  ���ô�  1252 */
    { "en_GB" ,          MLANG_EN_GB   },  /* 47  Ӣ��  Ӣ��    1252 */
    { "en_IE" ,          MLANG_EN_IE   },  /* 48  Ӣ��  ������  1252 */
    { "en_JM" ,          MLANG_EN_JM   },  /* 49  Ӣ��  �����  1252 */
    { "en_NZ" ,          MLANG_EN_NZ   },  /* 50  Ӣ��  ������  1252 */
    { "en_PH" ,          MLANG_EN_PH   },  /* 51  Ӣ��  ���ɱ�  1252 */
    { "en_TT" ,          MLANG_EN_TT   },  /* 52  Ӣ��  �������    1252 */
    { "en_US" ,          MLANG_EN_US   },  /* 53  Ӣ��  ����Ӣ��    1252 */
    { "en_ZA" ,          MLANG_EN_ZA   },  /* 54  Ӣ��  �Ϸ�    1252 */
    { "en_ZW" ,          MLANG_EN_ZW   },  /* 55  Ӣ��  ��Ͳ�Τ    1252 */
    { "eo" ,             MLANG_EO      },  /* 56  ������ */
    { "es_AR" ,          MLANG_ES_AR   },  /* 57  ��������  ����͢  1252 */
    { "es_BO" ,          MLANG_ES_BO   },  /* 58  ��������  ����ά��    1252 */
    { "es_CL" ,          MLANG_ES_CL   },  /* 59  ��������  ����    1252 */
    { "es_CO" ,          MLANG_ES_CO   },  /* 60  ��������  ���ױ���    1252 */
    { "es_CR" ,          MLANG_ES_CR   },  /* 61  ��������  ��˹�����  1252 */
    { "es_DM" ,          MLANG_ES_DM   },  /* 62  ��������  �������    1252 */
    { "es_EC" ,          MLANG_ES_EC   },  /* 63  ��������  ��϶��    1252 */
    { "es_ES" ,          MLANG_ES_ES   },  /* 64  ��������  ������(Espa?ol) 1252 */
    { "es_GT" ,          MLANG_ES_GT   },  /* 65  ��������  Σ������    1252 */
    { "es_HN" ,          MLANG_ES_HN   },  /* 66  ��������  �鶼��˹    1252 */
    { "es_MX" ,          MLANG_ES_MX   },  /* 67  ��������  ī����  1252 */
    { "es_NI" ,          MLANG_ES_NI   },  /* 68  ��������  �������    1252 */
    { "es_PA" ,          MLANG_ES_PA   },  /* 69  ��������  ������  1252 */
    { "es_PE" ,          MLANG_ES_PE   },  /* 70  ��������  ��³    1252 */
    { "es_PR" ,          MLANG_ES_PR   },  /* 71  ��������  �������    1252 */
    { "es_SV" ,          MLANG_ES_SV   },  /* 72  ��������  �����߶� */
    { "es_1" ,           MLANG_ES_1    },  /* 73  ��������  ������  1252 */
    { "es_2" ,           MLANG_ES_2    },  /* 74  ��������  ������  1252 */
    { "es_VE" ,          MLANG_ES_VE   },  /* 75  ��������  ί������    1252 */
    { "et_EE" ,          MLANG_ET_EE   },  /* 76  ��ɳ������    ��ɳ����(Estonian)  1257 */
    { "eu_1" ,           MLANG_EU_1    },  /* 77  ��˹����  �������������İ�˹�˺������������������Լ��������ϲ� 1252*/
    { "fa_1" ,           MLANG_FA_1    },  /* 78  ��˹�� */
    { "fa_2" ,           MLANG_FA_2    },  /* 79  ��˹�� */
    { "ff" ,             MLANG_FF      },  /* 80  ������ */
    { "fi_FI" ,          MLANG_FI_FI   },  /* 81  ������  ����(Suomi)   1252 */
    { "fj" ,             MLANG_FJ      },  /* 82  쳼���            */
    { "fo_FO" ,          MLANG_FO_FO   },  /* 83  ������  ����Ⱥ����Faeroese)   1250 */
    { "fr" ,             MLANG_FR      },  /* 84  �������� */
    { "fr_BE" ,          MLANG_FR_BE   },  /* 85  ����  ����ʱ  1252 */
    { "fr_CA" ,          MLANG_FR_CA   },  /* 86  ����  ���ô�  1252 */
    { "fr_CH" ,          MLANG_FR_CH   },  /* 87  ����  ��ʿ    1252 */
    { "fr_FR" ,          MLANG_FR_FR   },  /* 88  ����  ����(Fran?ais)  1252 */
    { "fr_LU" ,          MLANG_FR_LU   },  /* 89  ����  ¬ɭ��  1252 */
    { "fr_MC" ,          MLANG_FR_MC   },  /* 90  ����  Ħ���  1252 */
    { "fy" ,             MLANG_FY      },  /* 91  ���������� */
    { "ga" ,             MLANG_GA      },  /* 92  �������� */
    { "gd" ,             MLANG_GD      },  /* 93  �ո����Ƕ��� */
    { "gl" ,             MLANG_GL      },  /* 94  ���������� */
    { "gl_1" ,           MLANG_GL_1    },  /* 95  �������������  ����������������    1252 */
    { "gn" ,             MLANG_GN      },  /* 96  �������� */
    { "gu" ,             MLANG_GU      },  /* 97  �ż������� */
    { "gv" ,             MLANG_GV      },  /* 98  ������� */
    { "ha" ,             MLANG_HA      },  /* 99  ������ */
    { "he_IL" ,          MLANG_HE_IL   },  /* 100 ϣ������  ��ɫ��  1255 */
    { "hi" ,             MLANG_HI      },  /* 101 ӡ���� */
    { "ho" ,             MLANG_HO      },  /* 102 ϣ��Īͼ�� */
    { "hr_HR" ,          MLANG_HR_HR   },  /* 103 ���޵����� ���޵��ǣ�Croatian�� 1250 */
    { "ht" ,             MLANG_HT      },  /* 104 ���ؿ���¶���            */
    { "hu_HU" ,          MLANG_HU_HU   },  /* 105 ��������  ������(Hungarian) 1250 */
    { "hy" ,             MLANG_HY      },  /* 106 ����������            */
    { "hz" ,             MLANG_HZ      },  /* 107 �������� */
    { "ia" ,             MLANG_IA      },  /* 108 ������A */
    { "id_ID" ,          MLANG_ID_ID   },  /* 109 ӡ����������  ӡ�������ǣ�Indonesian��    1252 */
    { "ie" ,             MLANG_IE      },  /* 110 ������E */
    { "ig" ,             MLANG_IG      },  /* 111 ������ */
    { "ik" ,             MLANG_IK      },  /* 112 ��Ŭ�ӿ��� */
    { "io" ,             MLANG_IO      },  /* 113 ������ */
    { "is_IS" ,          MLANG_IS_IS   },  /* 114 ������  ����(Icelandic) 1252 */
    { "it_CH" ,          MLANG_IT_CH   },  /* 115 �������  ��ʿ    1252 */
    { "it_IT" ,          MLANG_IT_IT   },  /* 116 �������  �����(Italiano) 1252 */
    { "iu" ,             MLANG_IU      },  /* 117 ��Ŧ���� */
    { "ja_JP" ,          MLANG_JA_JP   },  /* 118 �ձ��� �ձ� 932 */
    { "jv" ,             MLANG_JV      },  /* 119 צ���� */
    { "ka" ,             MLANG_KA      },  /* 120 ��³������ */
    { "kg_CG" ,          MLANG_KG_CG   },  /* 121 �չ����ͼ��) �չ�  1252(latin-extend b) */
    { "ki" ,             MLANG_KI      },  /* 122 �������� */
    { "kj" ,             MLANG_KJ      },  /* 123 �������� */
    { "kk_KZ" ,          MLANG_KK_KZ   },  /* 124 ��������  ������˹̹  1251 */
    { "kl" ,             MLANG_KL      },  /* 125 �������� */
    { "km" ,             MLANG_KM      },  /* 126 ������ */
    { "kn" ,             MLANG_KN      },  /* 127 ���ɴ��� */
    { "ko_KP" ,          MLANG_KO_KP   },  /* 128 ����  �������� 949    */
    { "kr" ,             MLANG_KR      },  /* 129 ��Ŭ����  */
    { "ks" ,             MLANG_KS      },  /* 130 ��ʲ�׶��� */
    { "ku" ,             MLANG_KU      },  /* 131 ������� */
    { "kv" ,             MLANG_KV      },  /* 132 ������ */
    { "kw" ,             MLANG_KW      },  /* 133 ���ֶ��� */
    { "ky_KG" ,          MLANG_KY_KG   },  /* 134 ������˹�� ������˹˹̹   1251 */
    { "la" ,             MLANG_LA      },  /* 135 ������ */
    { "lb" ,             MLANG_LB      },  /* 136 ¬ɭ���� */
    { "lg" ,             MLANG_LG      },  /* 137 �ɴ��� */
    { "li" ,             MLANG_LI      },  /* 138 �ֱ��� */
    { "ln" ,             MLANG_LN      },  /* 139 �ּ����� */
    { "lo" ,             MLANG_LO      },  /* 140 ������ */
    { "lt_LT" ,          MLANG_LT_LT   },  /* 141 ��������  ������(Lithuanian)  1257 */
    { "lu" ,             MLANG_LU      },  /* 142 ¬��-�ӵ�����         */
    { "lv_LV" ,          MLANG_LV_LV   },  /* 143 ����ά����    ����ά�����  1257 */
    { "mg" ,             MLANG_MG      },  /* 144 ����˹���� */
    { "mh" ,             MLANG_MH      },  /* 145 ���ܶ��� */
    { "mi_NZ" ,          MLANG_MI_NZ   },  /* 146 ë����  ������ */
    { "mk_MK" ,          MLANG_MK_MK   },  /* 147 �������  1251 */
    { "ml" ,             MLANG_ML      },  /* 148 ��������ķ�� */
    { "mn_MN" ,          MLANG_MN_MN   },  /* 149 �ɹ���    �ɹ�    1251 */
    { "mo" ,             MLANG_MO      },  /* 150 Ħ����ά���� */
    { "mr" ,             MLANG_MR      },  /* 151 �������� */
    { "ms_BN" ,          MLANG_MS_BN   },  /* 152 ������  ����  1252 */
    { "ms_MY" ,          MLANG_MS_MY   },  /* 153 ������    �������� 1252 */
    { "mt_MT" ,          MLANG_MT_MT   },  /* 154 �������  ����� */
    { "my" ,             MLANG_MY      },  /* 155 ����� */
    { "na" ,             MLANG_NA      },  /* 156 �³�� */
    { "nd" ,             MLANG_ND      },  /* 157 �����±����� */
    { "ne" ,             MLANG_NE      },  /* 158 �Ჴ���� */
    { "ng" ,             MLANG_NG      },  /* 159 ���ؼ��� */
    { "nl_BE" ,          MLANG_NL_BE   },  /* 160 ������  ����ʱ  1252 */
    { "nl_NL" ,          MLANG_NL_NL   },  /* 161 ������  ����(Nederland)   1252 */
    { "nl_SA" ,          MLANG_NL_SA   },  /* 162 �ϷǺ�����  �Ϸ�  1252 */
    { "no_NO" ,          MLANG_NO_NO   },  /* 163 Ų����  Ų��  1250 */
    { "nr" ,             MLANG_NR      },  /* 164 �϶��±����� */
    { "nv" ,             MLANG_NV      },  /* 165 ���߻��� */
    { "ny" ,             MLANG_NY      },  /* 166 ������� */
    { "oc" ,             MLANG_OC      },  /* 167 �¿��� */
    { "oj" ,             MLANG_OJ      },  /* 168 �¼������� */
    { "om" ,             MLANG_OM      },  /* 169 ����Ī�� */
    { "or" ,             MLANG_OR      },  /* 170 �������� */
    { "pa" ,             MLANG_PA      },  /* 171 �������� */
    { "pi" ,             MLANG_PI      },  /* 172 ������ */
    { "pl_PL" ,          MLANG_PL_PL   },  /* 173 ������  ����(polski)  1257 */
    { "ps" ,             MLANG_PS      },  /* 174 ��ʲͼ�� */
    { "pt_BR" ,          MLANG_PT_BR   },  /* 175 ��������  ����    1252 */
    { "pt_PT" ,          MLANG_PT_PT   },  /* 176 ��������  ������(Portugu��s)  1252 */
    { "qu_BO" ,          MLANG_QU_BO   },  /* 177 �������Quechuan �� ����ά�� */
    { "qu_EC" ,          MLANG_QU_EC   },  /* 178 �������Quechuan �� ��϶�� */
    { "qu_PE" ,          MLANG_QU_PE   },  /* 179 �������Quechuan �� ��³ */
    { "rm" ,             MLANG_RM      },  /* 180 ����ʲ�� */
    { "rn" ,             MLANG_RN      },  /* 181 ��¡���� */
    { "ro_RO" ,          MLANG_RO_RO   },  /* 182 ���������� ��������(Romanian) 1250 */
    { "ru_RU" ,          MLANG_RU_RU   },  /* 183 ����  ���(russia)    1251 */
    { "rw" ,             MLANG_RW      },  /* 184 ���ᰢ����� */
    { "sa" ,             MLANG_SA      },  /* 185 ���� */
    { "sc" ,             MLANG_SC      },  /* 186 ������ */
    { "sd" ,             MLANG_SD      },  /* 187 �ŵ��� */
    { "se_1" ,           MLANG_SE_1    },  /* 188 ����ŷ��Ħ˹��  Ų�� */
    { "se_2" ,           MLANG_SE_2    },  /* 189 ����ŷ��Ħ˹��  ��� */
    { "se_3" ,           MLANG_SE_3    },  /* 190 ����Ħ˹��  Ų�� */
    { "se_4" ,           MLANG_SE_4    },  /* 191 ����Ħ˹��  ��� */
    { "se_5" ,           MLANG_SE_5    },  /* 192 ����Ħ˹��  ����÷�� */
    { "se_6" ,           MLANG_SE_6    },  /* 193 ����Ħ˹��  ��ŵ˹�� */
    { "se_7" ,           MLANG_SE_7    },  /* 194 ˹������Ħ˹�� */
    { "se_8" ,           MLANG_SE_8    },  /* 195 ��������Ħ˹��  ���� */
    { "se_FI" ,          MLANG_SE_FI   },  /* 196 �����������Ħ˹��) ����    1252 */
    { "se_NO" ,          MLANG_SE_NO   },  /* 197 �����������Ħ˹��) Ų��    1252 */
    { "se_SE" ,          MLANG_SE_SE   },  /* 198 �����������Ħ˹��) ���    1252 */
    { "sg" ,             MLANG_SG      },  /* 199 ɣ���� */
    { "sh" ,             MLANG_SH      },  /* 200 ����ά��-���޵����� */
    { "si" ,             MLANG_SI      },  /* 201 ɮ������ */
    { "sk_SK" ,          MLANG_SK_SK   },  /* 202 ˹�工����  ˹�工��(Slovak)  1250 */
    { "sl_SL" ,          MLANG_SL_SL   },  /* 203 ˹����������  ˹��������(Slovenian)   1250 */
    { "sn" ,             MLANG_SN      },  /* 204 ������ */
    { "so" ,             MLANG_SO      },  /* 205 �������� */
    { "sq_AL" ,          MLANG_SQ_AL   },  /* 206 ������������  ������������(Albania )  1250  */
    { "sr" ,             MLANG_SR      },  /* 207 ����ά����  �������  1251 */
    { "sr_RS" ,          MLANG_SR_RS   },  /* 208 ����ά����  ����ά��(Serbian) 1250 */
    { "st" ,             MLANG_ST      },  /* 209 �������� */
    { "su" ,             MLANG_SU      },  /* 210 ������ */
    { "sv_FI" ,          MLANG_SV_FI   },  /* 211 �����  ����  1252 */
    { "sv_SE" ,          MLANG_SV_SE   },  /* 212 �����  ���(Svenska) 1252 */
    { "sw" ,             MLANG_SW      },  /* 213 ˹�����Swahili��   ̹ɣ����, ������, �ڸɴ�   1252 */
    { "ta" ,             MLANG_TA      },  /* 214 ̩�׶��� */
    { "te" ,             MLANG_TE      },  /* 215 ̩¬���� */
    { "tg" ,             MLANG_TG      },  /* 216 �������� */
    { "th_TH" ,          MLANG_TH      },  /* 217 ̩�� */
    { "ti" ,             MLANG_TI      },  /* 218 ����������� */
    { "tk" ,             MLANG_TK      },  /* 219 �������� */
    { "tl" ,             MLANG_TL      },  /* 220 �������� */
    { "tn" ,             MLANG_TN      },  /* 221 ���������� */
    { "tn_1" ,           MLANG_TN_1    },  /* 222 ������ */
    { "to" ,             MLANG_TO      },  /* 223 ������ */
    { "tr_TR" ,          MLANG_TR_TR   },  /* 224 ��������  ������(T��rk?e) 1254 */
    { "ts" ,             MLANG_TS      },  /* 225 �ڼ��� */
    { "tt_1" ,           MLANG_TT_1    },  /* 226 ������(������) */
    { "tw" ,             MLANG_TW      },  /* 227 ������ */
    { "ty" ,             MLANG_TY      },  /* 228 ��ϣ���� */
    { "ug" ,             MLANG_UG      },  /* 229 ά����� */
    { "uk_UA" ,          MLANG_UK_UA   },  /* 230 �ڿ�����  �ڿ���(Ukrainian)   1251 */
    { "ur" ,             MLANG_UR      },  /* 231 �ڶ����� */
    { "uz_UZ" ,          MLANG_UZ_1    },  /* 232 ���ȱ����    ���ȱ��˹̹(������ģ�   1251 */
    { "uz_4" ,           MLANG_UZ_4    },  /* 233 ���ȱ����    ���ȱ��˹̹(������ 1254    */
    { "ve" ,             MLANG_VE      },  /* 234 �Ĵ��� */
    { "vi" ,             MLANG_VI      },  /* 235 Խ���� */
    { "vo" ,             MLANG_VO      },  /* 236 �����տ��� */
    { "wa" ,             MLANG_WA      },  /* 237 ������ */
    { "wo" ,             MLANG_WO      },  /* 238 ������� */
    { "xh" ,             MLANG_XH      },  /* 239 ������ */
    { "yi" ,             MLANG_YI      },  /* 240 ������ */
    { "yo" ,             MLANG_YO      },  /* 241 Լ³���� */
    { "za" ,             MLANG_ZA      },  /* 242 ׳�� */
    { "za_1" ,           MLANG_ZA_1    },  /* 243 ��������  �Ϸ�    1252��*/
    { "zh_CN" ,          MLANG_ZH_CN   },  /* 244 ����  �й���������    936 */
    { "zh_SG" ,          MLANG_ZH_SG   },  /* 245 ����  �¼���  936 */
    { "zh_TW" ,          MLANG_ZH_TW   },  /* 246 ����  �й�����(�ۣ���,̨�� 950 */
    { "zu_ZA" ,          MLANG_ZU_ZA   },  /* 247 ��³�� �Ϸ� */
    { "RESERVED_01" ,    MLANG_RESERVED_01 },  /* 248 ����ֵ */
    { "RESERVED_02" ,    MLANG_RESERVED_02 },  /* 249 ����ֵ */
    { "RESERVED_03" ,    MLANG_RESERVED_03 },  /* 250 ����ֵ */
    { "RESERVED_04" ,    MLANG_RESERVED_04 },  /* 251 ����ֵ */
    { "RESERVED_05" ,    MLANG_RESERVED_05 },  /* 252 ����ֵ */
    { "RESERVED_06" ,    MLANG_RESERVED_06 },  /* 253 ����ֵ */
    { "RESERVED_07" ,    MLANG_RESERVED_07 },  /* 254 ����ֵ */ 
};        


const char* sys_get_app_theme_res_dir(void)
{         
    const char * app_name = NULL;
    char app_name_no_ext[32] = {0};
    int i = 0;

    app_name = get_app_name(getpid());
    if(app_name == NULL)
    {
        print_err("get current app name failed!");
        return s_app_theme_res_dir;
    }

    strncpy(app_name_no_ext, app_name, 32);

    for(i = 0 ; i < 32 ; i++)
    {
        if(app_name_no_ext[i] == '.')
        {
            app_name_no_ext[i] = '\0';
            break;
        }
    }

    sprintf(s_app_theme_res_dir, "%s/%s/%s", APPS_DEFAULT_DIR, app_name_no_ext, sys_get_cur_theme_name());
    return s_app_theme_res_dir;
}         

const char* sys_get_common_theme_res_dir(void)
{         
    sprintf(s_common_theme_res_dir, "%s/commonui/%s", APPS_DEFAULT_DIR, sys_get_cur_theme_name());
    return s_common_theme_res_dir;
}         

const char* sys_get_cur_theme_name(void)
{
    char text[THEME_NAME_SIZE] = {0};

    if(get_config(CFG_CUR_THEME, text, THEME_NAME_SIZE) < 0)
    {
        print_warning("get_config failed! write default value to it!");
        memcpy(text, s_cur_theme_name, THEME_NAME_SIZE);

        if(set_config(CFG_CUR_THEME, text, THEME_NAME_SIZE) < 0)
        {
            print_err("set_config failed!");
        }

        return s_cur_theme_name;
    }

    memcpy(s_cur_theme_name, text, THEME_NAME_SIZE);
    return s_cur_theme_name;
}

const char* sys_get_app_string_res_dir(void)
{
    const char * app_name = NULL;
    char app_name_no_ext[32] = {0};
    int i = 0;

    app_name = get_app_name(getpid());
    if(app_name == NULL)
    {
        print_err("get current app name failed!");
        return s_app_string_res_dir;
    }

    strncpy(app_name_no_ext, app_name, 32);

    for(i = 0 ; i < 32 ; i++)
    {
        if(app_name_no_ext[i] == '.')
        {
            app_name_no_ext[i] = '\0';
            break;
        }
    }

    sprintf(s_app_string_res_dir, "%s/%s/%s", APPS_DEFAULT_DIR, app_name_no_ext, sys_get_cur_theme_name());

    return s_app_string_res_dir;
}

const char* sys_get_common_string_res_dir(void)
{

    sprintf(s_common_string_res_dir, "%s/commonui/%s", APPS_DEFAULT_DIR, sys_get_cur_theme_name());

    return s_common_string_res_dir;
}

const char* sys_get_cur_language_codes(void)
{
    char text[LANG_CODES_SIZE] = {0};

    if(get_config(CFG_LANG_CODES, text, LANG_CODES_SIZE) < 0)
    {
        print_warning("get_config failed! write default value to it!");
        memcpy(text, s_cur_lang_codes, LANG_CODES_SIZE);

        if(set_config(CFG_LANG_CODES, text, LANG_CODES_SIZE) < 0)
        {
            print_err("set_config failed!");
        }

        return s_cur_lang_codes;
    }

    memcpy(s_cur_lang_codes, text, LANG_CODES_SIZE);
    return s_cur_lang_codes;
}

int sys_get_cur_language_id(void)
{
    return sys_get_language_id(sys_get_cur_language_codes());
}

const char* sys_get_language_codes(int lang_id)
{
    int i;

    if( (lang_id < 0) || (lang_id > 254) )
    {
        print_err("lang_id is not in [0, 254]!");
        return NULL;
    }

    for(i = 0; i < MAX_LANGUAGES; i++)
    {
        if(lang_array[i].id == lang_id)
        {
            return lang_array[i].codes;
        }
    }

    return NULL;
}

int sys_get_language_id(const char* lang_codes)
{
    int i;

    if(NULL == lang_codes)
    {
        print_err("input parameter(lang_codes) is a NULL pointer!");
        return -1;
    }

    for(i = 0; i < MAX_LANGUAGES; i++)
    {
        if(strcmp(lang_array[i].codes, lang_codes) == 0)
        {
            return lang_array[i].id;
        }
    }

    return -1;
}

support_language_list_t* sys_get_cur_support_language_list(void)
{
    char file_lang_des[128];
    desktop_parser_handle dp_handle = NULL; 
    desktop_pair_t * dp_pair = NULL;

    support_language_list_t * support_lang_head = NULL;
    support_language_list_t * support_lang_tail = NULL;
    support_language_list_t * support_lang_new = NULL;

    int i;
    char key[128] = {0};

    strcpy(file_lang_des, MISC_DEFAULT_DIR);
    strcat(file_lang_des, "/lang.desktop");

    dp_handle = desktop_parser_create(file_lang_des);
    if(NULL == dp_handle)
    {
        print_err("create desktop parser failed!");
        return NULL;
    }

    for(dp_pair = (desktop_pair_t *)(dp_handle->pairs) ; dp_pair != NULL ; dp_pair = dp_pair->next)
    {
        for(i = 0; i < MAX_LANGUAGES; i++)
        {
            sprintf(key, "Name[%s]", lang_array[i].codes);

            if((dp_pair->key != NULL) && (strcmp(dp_pair->key, key) == 0))
            {
                if(dp_pair->value != NULL)
                {
                    print_dbg("value:%s",dp_pair->value);
                    support_lang_new = (support_language_list_t *)malloc(sizeof(support_language_list_t));
                    if(NULL == support_lang_new)
                    {
                        print_err("malloc failed!");
                        break;
                    }

                    support_lang_new->next = NULL;
                    support_lang_new->codes = lang_array[i].codes;
                    support_lang_new->name = strdup(dp_pair->value);
                    support_lang_new->id = lang_array[i].id;

                    if(NULL == support_lang_head)
                    {
                        support_lang_head = support_lang_new;
                        support_lang_tail = support_lang_new;
                    }
                    else
                    {
                        support_lang_tail->next = support_lang_new;
                        support_lang_tail = support_lang_tail->next;
                    }
                }

                break;
            }
        }
    }

    desktop_parser_delete(dp_handle);

    return support_lang_head;
}

void sys_delete_cur_support_language_list(support_language_list_t * list)
{
    support_language_list_t * node;

    while(list != NULL) 
    {
        node = list;
        list = list->next;
        free(node->name); 
        free(node);
    }
}

const char * sys_get_default_font_file(void)
{
    int value = 0;
    char tmp_path[PATH_SIZE] = {0};
    char text[256] = {0};
    struct stat stat_buf;

    if(get_config(CFG_USER_FONT_VALID, (char *)&value, sizeof(value)) < 0)
    {
        print_warning("get_config (CFG_USER_FONT_VALID) failed!");
        value = 0;
    }

    if(value == 1)
    {
        /* user font file valid */
        if(get_config(CFG_USER_FONT, text, sizeof(text)) < 0)
        {
            print_warning("get_config (CFG_USER_FONT) failed!");
            goto get_user_font_err;
        }

        sprintf(tmp_path, "%s%s", sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL, DIR_SYSTEM_FONT), text);

        if(stat(tmp_path, &stat_buf) == 0)
        {
            /* file exists */
            strcpy(s_default_font, tmp_path);
            return s_default_font;
        }
        else
        {
            value = 0;
            if(set_config(CFG_USER_FONT_VALID,(char *)&value, sizeof(value)) < 0 )
            {
                print_warning("set_config (CFG_USER_FONT_VALID) failed!");
            }
        }
    }

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    get_user_font_err:

    if(get_config(CFG_DEFAULT_FONT, text, sizeof(text)) < 0)
    {
        print_warning("get_config (CFG_DEFAULT_FONT) failed!");
        strcpy(text, "FWQY");
    }

    sprintf(tmp_path, "%s", text);

    strcpy(s_default_font, tmp_path);
    return s_default_font;
}

const char * sys_get_media_dir(hotplugin_dev_type_e dev, dir_type_e type)
{
    dir_cfg_t * cfg_entry = NULL;
    dir_cfg_t * cfg = NULL;
    int i;
    char * root_dir;
    int val = 0 ;

    switch(dev)
    {
        case HOTPLUGIN_DEV_TYPE_LOCAL:
        {
            cfg_entry = g_udisk_dir_cfgs;
            root_dir = UDISK_ROOT;            
            break;
        }
        case HOTPLUGIN_DEV_TYPE_CARD:
        {
            cfg_entry = g_card_dir_cfgs;
            root_dir = CARD_ROOT;
            break;
        }
        case HOTPLUGIN_DEV_TYPE_UHOST:
        {
            cfg_entry = g_external_dir_cfgs;
            root_dir = EXTERNAL_ROOT;
            break;
        }
        default :
        {
            /* no disk */
            return NULL;
        }
        
    }
      
    if(type == DIR_SYSTEM_PLAYLIST)
    { 
        if(get_config(CFG_PUT_PLIST_IN_ADISK, (char *)&val, sizeof(val)) < 0)
        {
            print_err("get_config(%s) failed!", CFG_PUT_PLIST_IN_ADISK);
            val = 1;
        }
        
        if(val == 1)
        {
            cfg = applib_dir_cfg_find(cfg_entry, CFG_DIR_ENTRY_NUM, type);
            if(cfg != NULL)
            {
                strncpy(cfg->text, ADISK_SYSTEM_PLAYLIST, CFG_TEXT_LEN);
                return cfg->text;
            }
        }
    }
    
    if(type == DIR_ROOT)
    { 
        return  root_dir;
    }

    if(type == DIR_ALL)
    {
        memset(g_dir_all, 0, DIR_ALL_BUF_SIZE);
        applib_dir_cfg_read_all(cfg_entry, CFG_DIR_ENTRY_NUM, root_dir, false);

        for(i = 0; i < CFG_DIR_ENTRY_NUM; i++)
        {
            strcat(g_dir_all, cfg_entry[i].text);
            strcat(g_dir_all, ";");
        }

        return g_dir_all;
    }
        
    cfg = applib_dir_cfg_find(cfg_entry, CFG_DIR_ENTRY_NUM, type);

    if(cfg != NULL)
    {
        applib_dir_cfg_read(cfg, root_dir, false);
        return cfg->text;
    }

    return NULL;
}

const char * sys_get_main_disk_dir(dir_type_e type)
{
    const char * dir = NULL;
    const char * root_dir = NULL;
    hotplugin_dev_type_e main_disk ;

    main_disk = sys_get_main_disk();

    dir = sys_get_media_dir(main_disk, type);

    switch(main_disk)
    {
        case HOTPLUGIN_DEV_TYPE_LOCAL:
        {
            root_dir = UDISK_ROOT;
            break;
        }
        case HOTPLUGIN_DEV_TYPE_CARD:
        {
            root_dir = CARD_ROOT;
            break;
        }
        case HOTPLUGIN_DEV_TYPE_UHOST:
        {
            root_dir = EXTERNAL_ROOT;
            break;
        }
        default :
        {
            root_dir = NULL;
            break;
        }
        
    }

    if((dir != NULL) && (root_dir != NULL))
    {
        if(sys_check_media_dir(dir, root_dir) == true)
        {
            return dir;
        }
        else
        {
            return NULL;
        }
    }
    else 
    {
        /* no main disk */
        print_err("has no main disk");
        return NULL;
    }
}

bool sys_check_media_dir(const char * path_sequence, const char * root_dir)
{
    char path[PATH_SIZE] = {0};
    char * next = (char *)path_sequence;
    char * r  = NULL;

    bool result = true;

    while( (*next) != '\0' ) 
    {
        /* Get a real path from a sequence of paths separated by ';' */
        r = next;
        while ( (*r) != '\0')
        {
            if(*r == ';')
            {
                break;
            }
            r++;
        }

        memcpy(path, next, (size_t)(r - next));
        path[r - next] = 0;

        if(*r != 0)
        {
            next = r + 1;
        }
        else
        {
            next = r;
        }

        if(root_dir != NULL)
        {
            if(_check_dir(path, &path[strlen(root_dir) + 1]) == false)
            {
                result = false;
            }
        }
        else
        {
            if(_check_dir(path, path) == false)
            {
                result = false;
            }

        }
    }

    return result;
}

static bool _check_dir( char * path, char * check_parent_start)
{
    char *s = check_parent_start;
    char c;
    struct stat st;

    do 
    {

        c = 0;
        /* Bypass leading non-'/'s and then subsequent '/'s. */
        while ( (*s) != '\0') 
        {
            if (*s == '/')
            {
                do 
                {
                    ++s;
                } while (*s == '/');
                c = *s;		/* Save the current char */
                *s = 0;		/* and replace it with nul. */
                break;
            }
            ++s;
        }

        print_info("mkdir:%s", path);

        if (mkdir(path, 0777) < 0) 
        {
            /* If we failed for any other reason than the directory
             * already exists, output a diagnostic and return -1.*/
            if ( (errno != EEXIST) || ( (stat(path, &st) < 0) || (!S_ISDIR(st.st_mode)) )) 
            {
                print_err("Cannot create directory '%s'", path);
                break;
            }
            /* Since the directory exists, don't attempt to change
             * permissions if it was the full target.  Note that
             * this is not an error conditon. */
        }

        if (!c) 
        {
            /* Done. */ 
            return true;
        }

        /* Remove any inserted nul from the path (recursive mode). */
        *s = c;
    } while (1);

    return false;
}
