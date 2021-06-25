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
    { "invalid" ,        MLANG_INVALID },  /* 0   非法值 */
    { "aa_DF" ,          MLANG_AA_DF   },  /* 1   默认语 */
    { "ae" ,             MLANG_AE      },  /* 2   阿维斯陀语 */
    { "af_1" ,           MLANG_AF_1    },  /* 3   南非语(阿非利堪斯语) */
    { "ak" ,             MLANG_AK      },  /* 4   阿坎语 */
    { "am" ,             MLANG_AM      },  /* 5   阿姆哈拉语 */
    { "an" ,             MLANG_AN      },  /* 6   阿拉贡 */
    { "ar" ,             MLANG_AR      },  /* 7   阿拉伯语 */
    { "as" ,             MLANG_AS      },  /* 8   阿萨姆语 */
    { "av" ,             MLANG_AV      },  /* 9   阿瓦尔语 */
    { "ay" ,             MLANG_AY      },  /* 10  艾马拉语 */
    { "az_1" ,           MLANG_AZ_1    },  /* 11  阿塞拜疆语  阿塞拜疆（西里尔文    1251 */
    { "az_2" ,           MLANG_AZ_2    },  /* 12  阿塞拜疆语  阿塞拜疆（拉丁文）1252    */
    { "ba" ,             MLANG_BA      },  /* 13  巴什基尔语 */
    { "be_BY" ,          MLANG_BE_BY   },  /* 14  白俄罗斯语  白俄罗斯(Byelorussian)    1251 */
    { "bg_BG" ,          MLANG_BG_BG   },  /* 15  保加利亚语  保加利亚(Bulgarian)   1251 */
    { "bh"  ,            MLANG_BH      },  /* 16  比哈尔语 */
    { "bi"  ,            MLANG_BI      },  /* 17  比斯拉马语 */
    { "bm"  ,            MLANG_BM      },  /* 18  班巴拉语 */
    { "bn"  ,            MLANG_BN      },  /* 19  孟加拉语 */
    { "bo"  ,            MLANG_BO      },  /* 20  藏语 */
    { "br"  ,            MLANG_BR      },  /* 21  布列塔尼语 */
    { "bs_BA" ,          MLANG_BS_BA   },  /* 22  波斯尼亚语  波斯尼亚和黑塞哥维那 */
    { "ca_AD" ,          MLANG_CA_AD   },  /* 23  加泰罗尼亚语 安道尔国官方语言 1252 */
    { "ce" ,             MLANG_CE      },  /* 24  车臣语 */
    { "ch" ,             MLANG_CH      },  /* 25  查莫罗语 */
    { "co" ,             MLANG_CO      },  /* 26  科西嘉语 */
    { "cr" ,             MLANG_CR      },  /* 27  克里语 */
    { "cs_CZ" ,          MLANG_CS_CZ   },  /* 28  捷克语  捷克(?esky) 1257 */
    { "cu" ,             MLANG_CU      },  /* 29  古教会斯拉夫语 */
    { "cv" ,             MLANG_CV      },  /* 30  楚瓦什语 */
    { "cy" ,             MLANG_CY      },  /* 31  威尔士语（Welsh） 威尔士 1252 */
    { "cy_2" ,           MLANG_CY_2    },  /* 32  威尔士语 1252 */
    { "da_DK" ,          MLANG_DA_DK   },  /* 33  丹麦语 丹麦（Dansk) 1250 */
    { "de_AT" ,          MLANG_DE_AT   },  /* 34  德语  奥地利 1252 */
    { "de_CH" ,          MLANG_DE_CH   },  /* 35  德语  瑞士  1252 */
    { "de_GE" ,          MLANG_DE_GE   },  /* 36  德语  德国(Deutsch) 1252 */
    { "de_LI" ,          MLANG_DE_LI   },  /* 37  德语  列支敦士敦（Liechtenstein）1252 */
    { "de_LU" ,          MLANG_DE_LU   },  /* 38  德语  卢森堡 1252 */
    { "dv" ,             MLANG_DV      },  /* 39  迪维希语 */
    { "dz" ,             MLANG_DZ      },  /* 40  不丹语 */
    { "ee" ,             MLANG_EE      },  /* 41  埃维语 */
    { "el" ,             MLANG_EL      },  /* 42  现代希腊语 */
    { "el_EG" ,          MLANG_EL_EG   },  /* 43  希腊语  希腊(Ελληνικα) 1253 */
    { "en_AU" ,          MLANG_EN_AU   },  /* 44  英语  澳大利亚 1252 */
    { "en_BZ" ,          MLANG_EN_BZ   },  /* 45  英语  伯利兹 1252 */
    { "en_CA" ,          MLANG_EN_CA   },  /* 46  英语  加拿大  1252 */
    { "en_GB" ,          MLANG_EN_GB   },  /* 47  英语  英国    1252 */
    { "en_IE" ,          MLANG_EN_IE   },  /* 48  英语  爱尔兰  1252 */
    { "en_JM" ,          MLANG_EN_JM   },  /* 49  英语  牙买加  1252 */
    { "en_NZ" ,          MLANG_EN_NZ   },  /* 50  英语  新西兰  1252 */
    { "en_PH" ,          MLANG_EN_PH   },  /* 51  英语  菲律宾  1252 */
    { "en_TT" ,          MLANG_EN_TT   },  /* 52  英语  特立尼达    1252 */
    { "en_US" ,          MLANG_EN_US   },  /* 53  英语  美国英语    1252 */
    { "en_ZA" ,          MLANG_EN_ZA   },  /* 54  英语  南非    1252 */
    { "en_ZW" ,          MLANG_EN_ZW   },  /* 55  英语  津巴布韦    1252 */
    { "eo" ,             MLANG_EO      },  /* 56  世界语 */
    { "es_AR" ,          MLANG_ES_AR   },  /* 57  西班牙语  阿根廷  1252 */
    { "es_BO" ,          MLANG_ES_BO   },  /* 58  西班牙语  玻利维亚    1252 */
    { "es_CL" ,          MLANG_ES_CL   },  /* 59  西班牙语  智利    1252 */
    { "es_CO" ,          MLANG_ES_CO   },  /* 60  西班牙语  哥伦比亚    1252 */
    { "es_CR" ,          MLANG_ES_CR   },  /* 61  西班牙语  哥斯达黎加  1252 */
    { "es_DM" ,          MLANG_ES_DM   },  /* 62  西班牙语  多米尼加    1252 */
    { "es_EC" ,          MLANG_ES_EC   },  /* 63  西班牙语  厄瓜多尔    1252 */
    { "es_ES" ,          MLANG_ES_ES   },  /* 64  西班牙语  西班牙(Espa?ol) 1252 */
    { "es_GT" ,          MLANG_ES_GT   },  /* 65  西班牙语  危地马拉    1252 */
    { "es_HN" ,          MLANG_ES_HN   },  /* 66  西班牙语  洪都拉斯    1252 */
    { "es_MX" ,          MLANG_ES_MX   },  /* 67  西班牙语  墨西哥  1252 */
    { "es_NI" ,          MLANG_ES_NI   },  /* 68  西班牙语  尼加拉瓜    1252 */
    { "es_PA" ,          MLANG_ES_PA   },  /* 69  西班牙语  巴拿马  1252 */
    { "es_PE" ,          MLANG_ES_PE   },  /* 70  西班牙语  秘鲁    1252 */
    { "es_PR" ,          MLANG_ES_PR   },  /* 71  西班牙语  波多黎哥    1252 */
    { "es_SV" ,          MLANG_ES_SV   },  /* 72  西班牙语  萨尔瓦多 */
    { "es_1" ,           MLANG_ES_1    },  /* 73  西班牙语  乌拉圭  1252 */
    { "es_2" ,           MLANG_ES_2    },  /* 74  西班牙语  乌拉圭  1252 */
    { "es_VE" ,          MLANG_ES_VE   },  /* 75  西班牙语  委内瑞拉    1252 */
    { "et_EE" ,          MLANG_ET_EE   },  /* 76  爱沙尼亚语    爱沙尼亚(Estonian)  1257 */
    { "eu_1" ,           MLANG_EU_1    },  /* 77  巴斯克语  西班牙东北部的巴斯克和纳瓦拉两个自治州以及法国西南部 1252*/
    { "fa_1" ,           MLANG_FA_1    },  /* 78  波斯语 */
    { "fa_2" ,           MLANG_FA_2    },  /* 79  波斯语 */
    { "ff" ,             MLANG_FF      },  /* 80  富拉语 */
    { "fi_FI" ,          MLANG_FI_FI   },  /* 81  芬兰语  芬兰(Suomi)   1252 */
    { "fj" ,             MLANG_FJ      },  /* 82  斐济语            */
    { "fo_FO" ,          MLANG_FO_FO   },  /* 83  法罗语  法罗群岛（Faeroese)   1250 */
    { "fr" ,             MLANG_FR      },  /* 84  阿法尔语 */
    { "fr_BE" ,          MLANG_FR_BE   },  /* 85  法语  比利时  1252 */
    { "fr_CA" ,          MLANG_FR_CA   },  /* 86  法语  加拿大  1252 */
    { "fr_CH" ,          MLANG_FR_CH   },  /* 87  法语  瑞士    1252 */
    { "fr_FR" ,          MLANG_FR_FR   },  /* 88  法语  法国(Fran?ais)  1252 */
    { "fr_LU" ,          MLANG_FR_LU   },  /* 89  法语  卢森堡  1252 */
    { "fr_MC" ,          MLANG_FR_MC   },  /* 90  法语  摩洛哥  1252 */
    { "fy" ,             MLANG_FY      },  /* 91  弗里西亚语 */
    { "ga" ,             MLANG_GA      },  /* 92  爱尔兰语 */
    { "gd" ,             MLANG_GD      },  /* 93  苏格兰盖尔语 */
    { "gl" ,             MLANG_GL      },  /* 94  加利西亚语 */
    { "gl_1" ,           MLANG_GL_1    },  /* 95  加里（利）西亚语  西班牙加里西利亚    1252 */
    { "gn" ,             MLANG_GN      },  /* 96  瓜拉尼语 */
    { "gu" ,             MLANG_GU      },  /* 97  古吉拉特语 */
    { "gv" ,             MLANG_GV      },  /* 98  马恩岛语 */
    { "ha" ,             MLANG_HA      },  /* 99  豪萨语 */
    { "he_IL" ,          MLANG_HE_IL   },  /* 100 希伯来语  以色列  1255 */
    { "hi" ,             MLANG_HI      },  /* 101 印地语 */
    { "ho" ,             MLANG_HO      },  /* 102 希里莫图语 */
    { "hr_HR" ,          MLANG_HR_HR   },  /* 103 克罗地亚语 克罗地亚（Croatian） 1250 */
    { "ht" ,             MLANG_HT      },  /* 104 海地克里奥尔语            */
    { "hu_HU" ,          MLANG_HU_HU   },  /* 105 匈牙利语  匈牙利(Hungarian) 1250 */
    { "hy" ,             MLANG_HY      },  /* 106 亚美尼亚语            */
    { "hz" ,             MLANG_HZ      },  /* 107 赫雷罗语 */
    { "ia" ,             MLANG_IA      },  /* 108 国际语A */
    { "id_ID" ,          MLANG_ID_ID   },  /* 109 印度尼西亚语  印度尼西亚（Indonesian）    1252 */
    { "ie" ,             MLANG_IE      },  /* 110 国际语E */
    { "ig" ,             MLANG_IG      },  /* 111 伊博语 */
    { "ik" ,             MLANG_IK      },  /* 112 依努庇克语 */
    { "io" ,             MLANG_IO      },  /* 113 伊多语 */
    { "is_IS" ,          MLANG_IS_IS   },  /* 114 冰岛语  冰岛(Icelandic) 1252 */
    { "it_CH" ,          MLANG_IT_CH   },  /* 115 意大利语  瑞士    1252 */
    { "it_IT" ,          MLANG_IT_IT   },  /* 116 意大利语  意大利(Italiano) 1252 */
    { "iu" ,             MLANG_IU      },  /* 117 因纽特语 */
    { "ja_JP" ,          MLANG_JA_JP   },  /* 118 日本语 日本 932 */
    { "jv" ,             MLANG_JV      },  /* 119 爪哇语 */
    { "ka" ,             MLANG_KA      },  /* 120 格鲁吉亚语 */
    { "kg_CG" ,          MLANG_KG_CG   },  /* 121 刚果语（班图语) 刚果  1252(latin-extend b) */
    { "ki" ,             MLANG_KI      },  /* 122 基库尤语 */
    { "kj" ,             MLANG_KJ      },  /* 123 宽亚玛语 */
    { "kk_KZ" ,          MLANG_KK_KZ   },  /* 124 哈萨克语  哈萨克斯坦  1251 */
    { "kl" ,             MLANG_KL      },  /* 125 格陵兰语 */
    { "km" ,             MLANG_KM      },  /* 126 高棉语 */
    { "kn" ,             MLANG_KN      },  /* 127 卡纳达语 */
    { "ko_KP" ,          MLANG_KO_KP   },  /* 128 韩语  韩国韩语 949    */
    { "kr" ,             MLANG_KR      },  /* 129 卡努里语  */
    { "ks" ,             MLANG_KS      },  /* 130 克什米尔语 */
    { "ku" ,             MLANG_KU      },  /* 131 库尔德语 */
    { "kv" ,             MLANG_KV      },  /* 132 科米语 */
    { "kw" ,             MLANG_KW      },  /* 133 康沃尔语 */
    { "ky_KG" ,          MLANG_KY_KG   },  /* 134 吉尔吉斯语 吉尔吉斯斯坦   1251 */
    { "la" ,             MLANG_LA      },  /* 135 拉丁语 */
    { "lb" ,             MLANG_LB      },  /* 136 卢森堡语 */
    { "lg" ,             MLANG_LG      },  /* 137 干达语 */
    { "li" ,             MLANG_LI      },  /* 138 林堡语 */
    { "ln" ,             MLANG_LN      },  /* 139 林加拉语 */
    { "lo" ,             MLANG_LO      },  /* 140 老挝语 */
    { "lt_LT" ,          MLANG_LT_LT   },  /* 141 立陶宛语  立陶宛(Lithuanian)  1257 */
    { "lu" ,             MLANG_LU      },  /* 142 卢巴-加丹加语         */
    { "lv_LV" ,          MLANG_LV_LV   },  /* 143 拉脱维亚语    拉脱维亚语（）  1257 */
    { "mg" ,             MLANG_MG      },  /* 144 马达加斯加语 */
    { "mh" ,             MLANG_MH      },  /* 145 马绍尔语 */
    { "mi_NZ" ,          MLANG_MI_NZ   },  /* 146 毛利语  新西兰 */
    { "mk_MK" ,          MLANG_MK_MK   },  /* 147 马其顿语  1251 */
    { "ml" ,             MLANG_ML      },  /* 148 马拉亚拉姆语 */
    { "mn_MN" ,          MLANG_MN_MN   },  /* 149 蒙古语    蒙古    1251 */
    { "mo" ,             MLANG_MO      },  /* 150 摩尔达维亚语 */
    { "mr" ,             MLANG_MR      },  /* 151 马拉提语 */
    { "ms_BN" ,          MLANG_MS_BN   },  /* 152 马来语  文莱  1252 */
    { "ms_MY" ,          MLANG_MS_MY   },  /* 153 马来语    马来西亚 1252 */
    { "mt_MT" ,          MLANG_MT_MT   },  /* 154 马耳它语  马耳它 */
    { "my" ,             MLANG_MY      },  /* 155 缅甸语 */
    { "na" ,             MLANG_NA      },  /* 156 瑙鲁语 */
    { "nd" ,             MLANG_ND      },  /* 157 北恩德贝勒语 */
    { "ne" ,             MLANG_NE      },  /* 158 尼泊尔语 */
    { "ng" ,             MLANG_NG      },  /* 159 恩敦加语 */
    { "nl_BE" ,          MLANG_NL_BE   },  /* 160 荷兰语  比利时  1252 */
    { "nl_NL" ,          MLANG_NL_NL   },  /* 161 荷兰语  荷兰(Nederland)   1252 */
    { "nl_SA" ,          MLANG_NL_SA   },  /* 162 南非荷兰语  南非  1252 */
    { "no_NO" ,          MLANG_NO_NO   },  /* 163 挪威语  挪威  1250 */
    { "nr" ,             MLANG_NR      },  /* 164 南恩德贝勒语 */
    { "nv" ,             MLANG_NV      },  /* 165 纳瓦霍语 */
    { "ny" ,             MLANG_NY      },  /* 166 尼扬贾语 */
    { "oc" ,             MLANG_OC      },  /* 167 奥克语 */
    { "oj" ,             MLANG_OJ      },  /* 168 奥吉布瓦语 */
    { "om" ,             MLANG_OM      },  /* 169 奥洛莫语 */
    { "or" ,             MLANG_OR      },  /* 170 奥利亚语 */
    { "pa" ,             MLANG_PA      },  /* 171 旁遮普语 */
    { "pi" ,             MLANG_PI      },  /* 172 巴利语 */
    { "pl_PL" ,          MLANG_PL_PL   },  /* 173 波兰语  波兰(polski)  1257 */
    { "ps" ,             MLANG_PS      },  /* 174 普什图语 */
    { "pt_BR" ,          MLANG_PT_BR   },  /* 175 葡萄牙语  巴西    1252 */
    { "pt_PT" ,          MLANG_PT_PT   },  /* 176 葡萄牙语  葡萄牙(Português)  1252 */
    { "qu_BO" ,          MLANG_QU_BO   },  /* 177 克丘亚语（Quechuan ） 波利维亚 */
    { "qu_EC" ,          MLANG_QU_EC   },  /* 178 克丘亚语（Quechuan ） 厄瓜多尔 */
    { "qu_PE" ,          MLANG_QU_PE   },  /* 179 克丘亚语（Quechuan ） 秘鲁 */
    { "rm" ,             MLANG_RM      },  /* 180 罗曼什语 */
    { "rn" ,             MLANG_RN      },  /* 181 基隆迪语 */
    { "ro_RO" ,          MLANG_RO_RO   },  /* 182 罗马尼亚语 罗马尼亚(Romanian) 1250 */
    { "ru_RU" ,          MLANG_RU_RU   },  /* 183 俄语  俄国(russia)    1251 */
    { "rw" ,             MLANG_RW      },  /* 184 基尼阿万达语 */
    { "sa" ,             MLANG_SA      },  /* 185 梵语 */
    { "sc" ,             MLANG_SC      },  /* 186 撒丁语 */
    { "sd" ,             MLANG_SD      },  /* 187 信德语 */
    { "se_1" ,           MLANG_SE_1    },  /* 188 律勒欧萨摩斯语  挪威 */
    { "se_2" ,           MLANG_SE_2    },  /* 189 律勒欧萨摩斯语  瑞典 */
    { "se_3" ,           MLANG_SE_3    },  /* 190 南萨摩斯语  挪威 */
    { "se_4" ,           MLANG_SE_4    },  /* 191 南萨摩斯语  瑞典 */
    { "se_5" ,           MLANG_SE_5    },  /* 192 南萨摩斯语  伯克梅尔 */
    { "se_6" ,           MLANG_SE_6    },  /* 193 南萨摩斯语  尼诺斯克 */
    { "se_7" ,           MLANG_SE_7    },  /* 194 斯科特萨摩斯语 */
    { "se_8" ,           MLANG_SE_8    },  /* 195 伊那里萨摩斯语  芬兰 */
    { "se_FI" ,          MLANG_SE_FI   },  /* 196 北萨米语（北萨摩斯语) 芬兰    1252 */
    { "se_NO" ,          MLANG_SE_NO   },  /* 197 北萨米语（北萨摩斯语) 挪威    1252 */
    { "se_SE" ,          MLANG_SE_SE   },  /* 198 北萨米语（北萨摩斯语) 瑞典    1252 */
    { "sg" ,             MLANG_SG      },  /* 199 桑戈语 */
    { "sh" ,             MLANG_SH      },  /* 200 塞尔维亚-克罗地亚语 */
    { "si" ,             MLANG_SI      },  /* 201 僧加罗语 */
    { "sk_SK" ,          MLANG_SK_SK   },  /* 202 斯洛伐克语  斯洛伐克(Slovak)  1250 */
    { "sl_SL" ,          MLANG_SL_SL   },  /* 203 斯洛文尼亚语  斯洛文尼亚(Slovenian)   1250 */
    { "sn" ,             MLANG_SN      },  /* 204 绍纳语 */
    { "so" ,             MLANG_SO      },  /* 205 索马里语 */
    { "sq_AL" ,          MLANG_SQ_AL   },  /* 206 阿尔巴尼亚语  阿尔巴尼亚语(Albania )  1250  */
    { "sr" ,             MLANG_SR      },  /* 207 塞尔维亚语  西里尔文  1251 */
    { "sr_RS" ,          MLANG_SR_RS   },  /* 208 塞尔维亚语  塞尔维亚(Serbian) 1250 */
    { "st" ,             MLANG_ST      },  /* 209 南索托语 */
    { "su" ,             MLANG_SU      },  /* 210 巽他语 */
    { "sv_FI" ,          MLANG_SV_FI   },  /* 211 瑞典语  芬兰  1252 */
    { "sv_SE" ,          MLANG_SV_SE   },  /* 212 瑞典语  瑞典(Svenska) 1252 */
    { "sw" ,             MLANG_SW      },  /* 213 斯瓦西里（Swahili）   坦桑尼亚, 肯尼亚, 乌干达   1252 */
    { "ta" ,             MLANG_TA      },  /* 214 泰米尔语 */
    { "te" ,             MLANG_TE      },  /* 215 泰卢固语 */
    { "tg" ,             MLANG_TG      },  /* 216 塔吉克语 */
    { "th_TH" ,          MLANG_TH      },  /* 217 泰语 */
    { "ti" ,             MLANG_TI      },  /* 218 提格里尼亚语 */
    { "tk" ,             MLANG_TK      },  /* 219 土库曼语 */
    { "tl" ,             MLANG_TL      },  /* 220 塔加洛语 */
    { "tn" ,             MLANG_TN      },  /* 221 塞茨瓦纳语 */
    { "tn_1" ,           MLANG_TN_1    },  /* 222 茨瓦纳 */
    { "to" ,             MLANG_TO      },  /* 223 汤加语 */
    { "tr_TR" ,          MLANG_TR_TR   },  /* 224 土耳其语  土耳其(Türk?e) 1254 */
    { "ts" ,             MLANG_TS      },  /* 225 宗加语 */
    { "tt_1" ,           MLANG_TT_1    },  /* 226 鞑靼语(塔塔尔) */
    { "tw" ,             MLANG_TW      },  /* 227 特威语 */
    { "ty" ,             MLANG_TY      },  /* 228 塔希提语 */
    { "ug" ,             MLANG_UG      },  /* 229 维吾尔语 */
    { "uk_UA" ,          MLANG_UK_UA   },  /* 230 乌克兰语  乌克兰(Ukrainian)   1251 */
    { "ur" ,             MLANG_UR      },  /* 231 乌尔都语 */
    { "uz_UZ" ,          MLANG_UZ_1    },  /* 232 乌兹别克语    乌兹别克斯坦(西里尔文）   1251 */
    { "uz_4" ,           MLANG_UZ_4    },  /* 233 乌兹别克语    乌兹别克斯坦(拉丁） 1254    */
    { "ve" ,             MLANG_VE      },  /* 234 文达语 */
    { "vi" ,             MLANG_VI      },  /* 235 越南语 */
    { "vo" ,             MLANG_VO      },  /* 236 沃拉普克语 */
    { "wa" ,             MLANG_WA      },  /* 237 沃伦语 */
    { "wo" ,             MLANG_WO      },  /* 238 沃洛夫语 */
    { "xh" ,             MLANG_XH      },  /* 239 科萨语 */
    { "yi" ,             MLANG_YI      },  /* 240 依地语 */
    { "yo" ,             MLANG_YO      },  /* 241 约鲁巴语 */
    { "za" ,             MLANG_ZA      },  /* 242 壮语 */
    { "za_1" ,           MLANG_ZA_1    },  /* 243 北梭托语  南非    1252？*/
    { "zh_CN" ,          MLANG_ZH_CN   },  /* 244 汉语  中国简体中文    936 */
    { "zh_SG" ,          MLANG_ZH_SG   },  /* 245 汉语  新加坡  936 */
    { "zh_TW" ,          MLANG_ZH_TW   },  /* 246 汉语  中国繁体(港，澳,台） 950 */
    { "zu_ZA" ,          MLANG_ZU_ZA   },  /* 247 祖鲁语 南非 */
    { "RESERVED_01" ,    MLANG_RESERVED_01 },  /* 248 保留值 */
    { "RESERVED_02" ,    MLANG_RESERVED_02 },  /* 249 保留值 */
    { "RESERVED_03" ,    MLANG_RESERVED_03 },  /* 250 保留值 */
    { "RESERVED_04" ,    MLANG_RESERVED_04 },  /* 251 保留值 */
    { "RESERVED_05" ,    MLANG_RESERVED_05 },  /* 252 保留值 */
    { "RESERVED_06" ,    MLANG_RESERVED_06 },  /* 253 保留值 */
    { "RESERVED_07" ,    MLANG_RESERVED_07 },  /* 254 保留值 */ 
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
