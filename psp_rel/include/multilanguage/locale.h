/*******************************************************************************
 *                              USDK130
 *                            Module: Multilanguage
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yuchen     2008-11-22 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     locale.h
 * \brief    描述各国语言id
 * \author   yuchen
 * \version 1.0
 * \date  2008/11/22
 *******************************************************************************/
#ifndef __LOCALE_H__
#define __LOCALE_H__
/*!\cond MLANG*/
/*!
 *  \brief
 *  各国家和地区的语言id
 *  \li MLANG_INVALID : 0   非法值
 *  \li MLANG_AA_DF	: 1  	 默认语言
 *  \li MLANG_AE	: 2  	阿维斯陀语
 *  \li MLANG_AF_1	: 3  	南非语(阿非利堪斯语)
 *  \li MLANG_AK	: 4  	阿坎语
 *  \li MLANG_AM	: 4  	阿姆哈拉语
 *  \li MLANG_AN	: 6  	阿拉贡语
 *  \li MLANG_AR	: 7  	阿拉伯语
 *  \li MLANG_AS	: 8  	阿萨姆语
 *  \li MLANG_AV	: 9  	阿瓦尔语
 *  \li MLANG_AY	: 10  	艾马拉语
 *  \li MLANG_AZ_1	: 11  	阿塞拜疆语	阿塞拜疆（西里尔文）	1251
 *  \li MLANG_AZ_2	: 12  	阿塞拜疆语	阿塞拜疆（拉丁文）	1252
 *  \li MLANG_BA	: 13  	巴什基尔语
 *  \li MLANG_BE_BY	: 14  	白俄罗斯语	白俄罗斯(Byelorussian)	1251
 *  \li MLANG_BG_BG	: 15  	保加利亚语	保加利亚(Bulgarian)	1251
 *  \li MLANG_BH	: 16  	比哈尔语
 *  \li MLANG_BI	: 17  	比斯拉马语
 *  \li MLANG_BM	: 18  	班巴拉语
 *  \li MLANG_BN	: 19  	孟加拉语
 *  \li MLANG_BO	: 20  	藏语
 *  \li MLANG_BR	: 21  	布列塔尼语
 *  \li MLANG_BS_BA	: 22  	波斯尼亚语	波斯尼亚和黑塞哥维那
 *  \li MLANG_CA_AD	: 23  	加泰罗尼亚语	安道尔国官方语言	1252
 *  \li MLANG_CE	: 24  	车臣语
 *  \li MLANG_CH	: 25  	查莫罗语
 *  \li MLANG_CO	: 26  	科西嘉语
 *  \li MLANG_CR	: 27  	克里语
 *  \li MLANG_CS_CZ	: 28  	捷克语	捷克(?esky)	1257
 *  \li MLANG_CU	: 29  	古教会斯拉夫语
 *  \li MLANG_CV	: 30  	楚瓦什语
 *  \li MLANG_CY	: 31  	威尔士语（Welsh）	威尔士	1252
 *  \li MLANG_CY_2	: 32  	威尔士语		1252
 *  \li MLANG_DA_DK	: 33  	丹麦语    	丹麦（Dansk)	1250
 *  \li MLANG_DE_AT	: 34  	德语	奥地利	1252
 *  \li MLANG_DE_CH	: 35  	德语	瑞士	1252
 *  \li MLANG_DE_GE	: 36  	德语	德国(Deutsch)	1252
 *  \li MLANG_DE_LI	: 37  	德语	列支敦士敦（Liechtenstein）	1252
 *  \li MLANG_DE_LU	: 38  	德语	卢森堡	1252
 *  \li MLANG_DV	: 39  	迪维希语
 *  \li MLANG_DZ	: 40  	不丹语
 *  \li MLANG_EE	: 41  	埃维语
 *  \li MLANG_EL	: 42  	现代希腊语
 *  \li MLANG_EL_EG	: 43  	希腊语	希腊(Ελληνικα)	1253
 *  \li MLANG_EN_AU	: 44  	英语	澳大利亚	1252
 *  \li MLANG_EN_BZ	: 45  	英语	伯利兹	1252
 *  \li MLANG_EN_CA	: 46  	英语	加拿大	1252
 *  \li MLANG_EN_GB	: 47  	英语	英国	1252
 *  \li MLANG_EN_IE	: 48  	英语	爱尔兰	1252
 *  \li MLANG_EN_JM	: 49  	英语	牙买加	1252
 *  \li MLANG_EN_NZ	: 50  	英语	新西兰	1252
 *  \li MLANG_EN_PH	: 51  	英语	菲律宾	1252
 *  \li MLANG_EN_TT	: 52  	英语	特立尼达	1252
 *  \li MLANG_EN_US	: 53  	英语	美国英语	1252
 *  \li MLANG_EN_ZA	: 54  	英语	南非	1252
 *  \li MLANG_EN_ZW	: 55  	英语	津巴布韦	1252
 *  \li MLANG_EO	: 56  	世界语
 *  \li MLANG_ES_AR	: 57  	西班牙语	阿根廷	1252
 *  \li MLANG_ES_BO	: 58  	西班牙语	玻利维亚	1252
 *  \li MLANG_ES_CL	: 59  	西班牙语	智利	1252
 *  \li MLANG_ES_CO	: 60  	西班牙语	哥伦比亚	1252
 *  \li MLANG_ES_CR	: 61  	西班牙语	哥斯达黎加	1252
 *  \li MLANG_ES_DM	: 62  	西班牙语	多米尼加	1252
 *  \li MLANG_ES_EC	: 63  	西班牙语	厄瓜多尔	1252
 *  \li MLANG_ES_ES	: 64  	西班牙语	西班牙(Espa?ol)	1252
 *  \li MLANG_ES_GT	: 65  	西班牙语	危地马拉	1252
 *  \li MLANG_ES_HN	: 66  	西班牙语	洪都拉斯	1252
 *  \li MLANG_ES_MX	: 67  	西班牙语	墨西哥	1252
 *  \li MLANG_ES_NI	: 68  	西班牙语	尼加拉瓜	1252
 *  \li MLANG_ES_PA	: 69  	西班牙语	巴拿马	1252
 *  \li MLANG_ES_PE	: 70  	西班牙语	秘鲁	1252
 *  \li MLANG_ES_PR	: 71  	西班牙语	波多黎哥	1252
 *  \li MLANG_ES_SV	: 72  	西班牙语	萨尔瓦多
 *  \li MLANG_ES_1	: 73  	西班牙语	乌拉圭	1252
 *  \li MLANG_ES_2	: 74  	西班牙语	乌拉圭	1252
 *  \li MLANG_ES_VE	: 75  	西班牙语	委内瑞拉	1252
 *  \li MLANG_ET_EE	: 76  	爱沙尼亚语	爱沙尼亚(Estonian)	1257
 *  \li MLANG_EU_1	: 77  	巴斯克语	西班牙东北部的巴斯克和纳瓦拉两个自治州，以及法国西南部	1252
 *  \li MLANG_FA_1	: 78  	波斯语
 *  \li MLANG_FA_2	: 79  	波斯语
 *  \li MLANG_FF	: 80  	富拉语
 *  \li MLANG_FI_FI	: 81  	芬兰语	芬兰(Suomi)	1252
 *  \li MLANG_FJ	: 82  	斐济语
 *  \li MLANG_FO_FO	: 83  	法罗语	法罗群岛（Faeroese)	1250
 *  \li MLANG_FR	: 84  	阿法尔语
 *  \li MLANG_FR_BE	: 85  	法语	比利时	1252
 *  \li MLANG_FR_CA	: 86  	法语	加拿大	1252
 *  \li MLANG_FR_CH	: 87  	法语	瑞士	1252
 *  \li MLANG_FR_FR	: 88  	法语	法国(Fran?ais)	1252
 *  \li MLANG_FR_LU	: 89  	法语	卢森堡	1252
 *  \li MLANG_FR_MC	: 90  	法语	摩洛哥	1252
 *  \li MLANG_FY	: 91  	弗里西亚语
 *  \li MLANG_GA	: 92  	爱尔兰语
 *  \li MLANG_GD	: 93  	苏格兰盖尔语
 *  \li MLANG_GL	: 94  	加利西亚语
 *  \li MLANG_GL_1	: 95  	加里（利）西亚语	西班牙加里西利亚	1252
 *  \li MLANG_GN	: 96  	瓜拉尼语
 *  \li MLANG_GU	: 97  	古吉拉特语
 *  \li MLANG_GV	: 98  	马恩岛语
 *  \li MLANG_HA	: 99  	豪萨语
 *  \li MLANG_HE_IL	: 100  	希伯来语	以色列	1255
 *  \li MLANG_HI	: 101  	印地语
 *  \li MLANG_HO	: 102  	希里莫图语
 *  \li MLANG_HR_HR	: 103  	克罗地亚语    	克罗地亚（Croatian）	1250
 *  \li MLANG_HT	: 104  	海地克里奥尔语
 *  \li MLANG_HU_HU	: 105  	匈牙利语	匈牙利(Hungarian)	1250
 *  \li MLANG_HY	: 106  	亚美尼亚语
 *  \li MLANG_HZ	: 107  	赫雷罗语
 *  \li MLANG_IA	: 108  	国际语A
 *  \li MLANG_ID_ID	: 109  	印度尼西亚语	印度尼西亚（Indonesian）	1252
 *  \li MLANG_IE	: 110  	国际语E
 *  \li MLANG_IG	: 111  	伊博语
 *  \li MLANG_IK	: 112  	依努庇克语
 *  \li MLANG_IO	: 113  	伊多语
 *  \li MLANG_IS_IS	: 114  	冰岛语	冰岛(Icelandic)	1252
 *  \li MLANG_IT_CH	: 115  	意大利语	瑞士	1252
 *  \li MLANG_IT_IT	: 116  	意大利语	意大利(Italiano)	1252
 *  \li MLANG_IU	: 117  	因纽特语
 *  \li MLANG_JA_JP	: 118  	日本语	日本	932
 *  \li MLANG_JV	: 119  	爪哇语
 *  \li MLANG_KA	: 120  	格鲁吉亚语
 *  \li MLANG_KG_CG	: 121  	刚果语（班图语)	刚果	1252(latin-extend b)
 *  \li MLANG_KI	: 122  	基库尤语
 *  \li MLANG_KJ	: 123  	宽亚玛语
 *  \li MLANG_KK_KZ	: 124  	哈萨克语	哈萨克斯坦	1251
 *  \li MLANG_KL	: 125  	格陵兰语
 *  \li MLANG_KM	: 126  	高棉语
 *  \li MLANG_KN	: 127  	卡纳达语
 *  \li MLANG_KO_KP	: 128  	韩语	韩国韩语	949
 *  \li MLANG_KR	: 129  	卡努里语
 *  \li MLANG_KS	: 130  	克什米尔语
 *  \li MLANG_KU	: 131  	库尔德语
 *  \li MLANG_KV	: 132  	科米语
 *  \li MLANG_KW	: 133  	康沃尔语
 *  \li MLANG_KY_KG	: 134  	吉尔吉斯语	吉尔吉斯斯坦	1251
 *  \li MLANG_LA	: 135  	拉丁语
 *  \li MLANG_LB	: 136  	卢森堡语
 *  \li MLANG_LG	: 137  	干达语
 *  \li MLANG_LI	: 138  	林堡语
 *  \li MLANG_LN	: 139  	林加拉语
 *  \li MLANG_LO	: 140  	老挝语
 *  \li MLANG_LT_LT	: 141  	立陶宛语	立陶宛(Lithuanian)	1257
 *  \li MLANG_LU	: 142  	卢巴-加丹加语
 *  \li MLANG_LV_LV	: 143  	拉脱维亚语	拉脱维亚语（）	1257
 *  \li MLANG_MG	: 144  	马达加斯加语
 *  \li MLANG_MH	: 145  	马绍尔语
 *  \li MLANG_MI_NZ	: 146  	毛利语	新西兰
 *  \li MLANG_MK_MK	: 147  	马其顿语		1251
 *  \li MLANG_ML	: 148  	马拉亚拉姆语
 *  \li MLANG_MN_MN	: 149  	蒙古语	蒙古	1251
 *  \li MLANG_MO	: 150  	摩尔达维亚语
 *  \li MLANG_MR	: 151  	马拉提语
 *  \li MLANG_MS_BN	: 152  	马来语	文莱	1252
 *  \li MLANG_MS_MY	: 153  	马来语	马来西亚	1252
 *  \li MLANG_MT_MT	: 154  	马耳它语	马耳它
 *  \li MLANG_MY	: 155  	缅甸语
 *  \li MLANG_NA	: 156  	瑙鲁语
 *  \li MLANG_ND	: 157  	北恩德贝勒语
 *  \li MLANG_NE	: 158  	尼泊尔语
 *  \li MLANG_NG	: 159  	恩敦加语
 *  \li MLANG_NL_BE	: 160  	荷兰语	比利时	1252
 *  \li MLANG_NL_NL	: 161  	荷兰语	荷兰(Nederland)	1252
 *  \li MLANG_NL_SA	: 162  	南非荷兰语	南非	1252
 *  \li MLANG_NO_NO	: 163  	挪威语	挪威	1250
 *  \li MLANG_NR	: 164  	南恩德贝勒语
 *  \li MLANG_NV	: 165  	纳瓦霍语
 *  \li MLANG_NY	: 166  	尼扬贾语
 *  \li MLANG_OC	: 167  	奥克语
 *  \li MLANG_OJ	: 168  	奥吉布瓦语
 *  \li MLANG_OM	: 169  	奥洛莫语
 *  \li MLANG_OR	: 170  	奥利亚语
 *  \li MLANG_PA	: 171  	旁遮普语
 *  \li MLANG_PI	: 172  	巴利语
 *  \li MLANG_PL_PL	: 173  	波兰语	波兰(polski)	1257
 *  \li MLANG_PS	: 174  	普什图语
 *  \li MLANG_PT_BR	: 175  	葡萄牙语	巴西	1252
 *  \li MLANG_PT_PT	: 176  	葡萄牙语	葡萄牙(Português)	1252
 *  \li MLANG_QU_BO	: 177  	克丘亚语（Quechuan ）	波利维亚
 *  \li MLANG_QU_EC	: 178  	克丘亚语（Quechuan ）	厄瓜多尔
 *  \li MLANG_QU_PE	: 179  	克丘亚语（Quechuan ）	秘鲁
 *  \li MLANG_RM	: 180  	罗曼什语
 *  \li MLANG_RN	: 181  	基隆迪语
 *  \li MLANG_RO_RO	: 182  	罗马尼亚语	罗马尼亚(Romanian)	1250
 *  \li MLANG_RU_RU	: 183  	俄语	俄国(russia)	1251
 *  \li MLANG_RW	: 184  	基尼阿万达语
 *  \li MLANG_SA	: 185  	梵语
 *  \li MLANG_SC	: 186  	撒丁语
 *  \li MLANG_SD	: 187  	信德语
 *  \li MLANG_SE_1	: 188  	律勒欧萨摩斯语	挪威
 *  \li MLANG_SE_2	: 189  	律勒欧萨摩斯语	瑞典
 *  \li MLANG_SE_3	: 190  	南萨摩斯语	挪威
 *  \li MLANG_SE_4	: 191  	南萨摩斯语	瑞典
 *  \li MLANG_SE_5	: 192  	南萨摩斯语	伯克梅尔
 *  \li MLANG_SE_6	: 193  	南萨摩斯语	尼诺斯克
 *  \li MLANG_SE_7	: 194  	斯科特萨摩斯语
 *  \li MLANG_SE_8	: 195  	伊那里萨摩斯语	芬兰
 *  \li MLANG_SE_FI	: 196  	北萨米语（北萨摩斯语)	芬兰	1252
 *  \li MLANG_SE_NO	: 197  	北萨米语（北萨摩斯语)	挪威	1252
 *  \li MLANG_SE_SE	: 198  	北萨米语（北萨摩斯语)	瑞典	1252
 *  \li MLANG_SG	: 199  	桑戈语
 *  \li MLANG_SH	: 200  	塞尔维亚-克罗地亚语
 *  \li MLANG_SI	: 201  	僧加罗语
 *  \li MLANG_SK_SK	: 202  	斯洛伐克语	斯洛伐克(Slovak)	1250
 *  \li MLANG_SL_SL	: 203  	斯洛文尼亚语	斯洛文尼亚(Slovenian)	1250
 *  \li MLANG_SN	: 204  	绍纳语
 *  \li MLANG_SO	: 205  	索马里语
 *  \li MLANG_SQ_AL	: 206  	阿尔巴尼亚语	阿尔巴尼亚语(Albania )	1250
 *  \li MLANG_SR	: 207  	塞尔维亚语	西里尔文	1251
 *  \li MLANG_SR_RS	: 208  	塞尔维亚语	塞尔维亚(Serbian)	1250
 *  \li MLANG_ST	: 209  	南索托语
 *  \li MLANG_SU	: 210  	巽他语
 *  \li MLANG_SV_FI	: 211  	瑞典语	芬兰	1252
 *  \li MLANG_SV_SE	: 212  	瑞典语	瑞典(Svenska)	1252
 *  \li MLANG_SW	: 213  	斯瓦西里（Swahili）	坦桑尼亚, 肯尼亚, 乌干达	1252
 *  \li MLANG_TA	: 214  	泰米尔语
 *  \li MLANG_TE	: 215  	泰卢固语
 *  \li MLANG_TG	: 216  	塔吉克语
 *  \li MLANG_TH	: 217  	泰语
 *  \li MLANG_TI	: 218  	提格里尼亚语
 *  \li MLANG_TK	: 219  	土库曼语
 *  \li MLANG_TL	: 220  	塔加洛语
 *  \li MLANG_TN	: 221  	塞茨瓦纳语
 *  \li MLANG_TN_1	: 222  	茨瓦纳
 *  \li MLANG_TO	: 223  	汤加语
 *  \li MLANG_TR_TR	: 224  	土耳其语	土耳其(Türk?e)	1254
 *  \li MLANG_TS	: 225  	宗加语
 *  \li MLANG_TT_1	: 226  	鞑靼语(塔塔尔)
 *  \li MLANG_TW	: 227  	特威语
 *  \li MLANG_TY	: 228  	塔希提语
 *  \li MLANG_UG	: 229  	维吾尔语
 *  \li MLANG_UK_UA	: 230  	乌克兰语	乌克兰(Ukrainian)	1251
 *  \li MLANG_UR	: 231  	乌尔都语
 *  \li MLANG_UZ_1	: 232  	乌兹别克语	乌兹别克斯坦(西里尔文）	1251
 *  \li MLANG_UZ_4	: 233  	乌兹别克语	乌兹别克斯坦(拉丁）	1254
 *  \li MLANG_VE	: 234  	文达语
 *  \li MLANG_VI	: 235  	越南语
 *  \li MLANG_VO	: 236  	沃拉普克语
 *  \li MLANG_WA	: 237  	沃伦语
 *  \li MLANG_WO	: 238  	沃洛夫语
 *  \li MLANG_XH	: 239  	科萨语
 *  \li MLANG_YI	: 240  	依地语
 *  \li MLANG_YO	: 241  	约鲁巴语
 *  \li MLANG_ZA	: 242  	壮语
 *  \li MLANG_ZA_1	: 243  	北梭托语	南非	1252？
 *  \li MLANG_ZH_CN	: 244  	汉语	中国简体中文	936
 *  \li MLANG_ZH_SG	: 245  	汉语	新加坡	936
 *  \li MLANG_ZH_TW	: 246  	汉语	中国繁体(港，澳,台）	950
 *  \li MLANG_ZU_ZA	: 247  	祖鲁语	南非
 *  \li MLANG_RESERVED_01   : 248   保留值
 *  \li MLANG_RESERVED_02   : 249   保留值
 *  \li MLANG_RESERVED_03   : 250   保留值
 *  \li MLANG_RESERVED_04   : 251   保留值
 *  \li MLANG_RESERVED_05   : 252   保留值
 *  \li MLANG_RESERVED_06   : 253   保留值
 *  \MLANG_RESERVED_07   : 254   保留值
 */
typedef enum
{
    MLANG_INVALID,/* \li 0   非法值*/
    MLANG_AA_DF,/* \li 1  	 默认语言		*/
    MLANG_AE,/* \li 2  	阿维斯陀语			*/
    MLANG_AF_1,/* \li 3  	南非语(阿非利堪斯语)			*/
    MLANG_AK,/* \li 4  	阿坎语			*/
    MLANG_AM,/* \li 4  	阿姆哈拉语			*/
    MLANG_AN,/* \li 6  	阿拉贡语			*/
    MLANG_AR,/* \li 7  	阿拉伯语			*/
    MLANG_AS,/* \li 8  	阿萨姆语			*/
    MLANG_AV,/* \li 9  	阿瓦尔语			*/
    MLANG_AY,/* \li 10  	艾马拉语			*/
    MLANG_AZ_1,/* \li 11  	阿塞拜疆语	阿塞拜疆（西里尔文）	1251	*/
    MLANG_AZ_2,/* \li 12  	阿塞拜疆语	阿塞拜疆（拉丁文）	1252	*/
    MLANG_BA,/* \li 13  	巴什基尔语			*/
    MLANG_BE_BY,/* \li 14  	白俄罗斯语	白俄罗斯(Byelorussian)	1251	*/
    MLANG_BG_BG,/* \li 15  	保加利亚语	保加利亚(Bulgarian)	1251	*/
    MLANG_BH,/* \li 16  	比哈尔语			*/
    MLANG_BI,/* \li 17  	比斯拉马语			*/
    MLANG_BM,/* \li 18  	班巴拉语			*/
    MLANG_BN,/* \li 19  	孟加拉语			*/
    MLANG_BO,/* \li 20  	藏语			*/
    MLANG_BR,/* \li 21  	布列塔尼语			*/
    MLANG_BS_BA,/* \li 22  	波斯尼亚语	波斯尼亚和黑塞哥维那		*/
    MLANG_CA_AD,/* \li 23  	加泰罗尼亚语	安道尔国官方语言	1252	*/
    MLANG_CE,/* \li 24  	车臣语			*/
    MLANG_CH,/* \li 25  	查莫罗语			*/
    MLANG_CO,/* \li 26  	科西嘉语			*/
    MLANG_CR,/* \li 27  	克里语			*/
    MLANG_CS_CZ,/* \li 28  	捷克语	捷克(?esky)	1257	*/
    MLANG_CU,/* \li 29  	古教会斯拉夫语			*/
    MLANG_CV,/* \li 30  	楚瓦什语			*/
    MLANG_CY,/* \li 31  	威尔士语（Welsh）	威尔士	1252	*/
    MLANG_CY_2,/* \li 32  	威尔士语		1252	*/
    MLANG_DA_DK,/* \li 33  	丹麦语    	丹麦（Dansk)	1250	*/
    MLANG_DE_AT,/* \li 34  	德语	奥地利	1252	*/
    MLANG_DE_CH,/* \li 35  	德语	瑞士	1252	*/
    MLANG_DE_GE,/* \li 36  	德语	德国(Deutsch)	1252	*/
    MLANG_DE_LI,/* \li 37  	德语	列支敦士敦（Liechtenstein）	1252	*/
    MLANG_DE_LU,/* \li 38  	德语	卢森堡	1252	*/
    MLANG_DV,/* \li 39  	迪维希语			*/
    MLANG_DZ,/* \li 40  	不丹语			*/
    MLANG_EE,/* \li 41  	埃维语			*/
    MLANG_EL,/* \li 42  	现代希腊语			*/
    MLANG_EL_EG,/* \li 43  	希腊语	希腊(Ελληνικα)	1253	*/
    MLANG_EN_AU,/* \li 44  	英语	澳大利亚	1252	*/
    MLANG_EN_BZ,/* \li 45  	英语	伯利兹	1252	*/
    MLANG_EN_CA,/* \li 46  	英语	加拿大	1252	*/
    MLANG_EN_GB,/* \li 47  	英语	英国	1252	*/
    MLANG_EN_IE,/* \li 48  	英语	爱尔兰	1252	*/
    MLANG_EN_JM,/* \li 49  	英语	牙买加	1252	*/
    MLANG_EN_NZ,/* \li 50  	英语	新西兰	1252	*/
    MLANG_EN_PH,/* \li 51  	英语	菲律宾	1252	*/
    MLANG_EN_TT,/* \li 52  	英语	特立尼达	1252	*/
    MLANG_EN_US,/* \li 53  	英语	美国英语	1252	*/
    MLANG_EN_ZA,/* \li 54  	英语	南非	1252	*/
    MLANG_EN_ZW,/* \li 55  	英语	津巴布韦	1252	*/
    MLANG_EO,/* \li 56  	世界语			*/
    MLANG_ES_AR,/* \li 57  	西班牙语	阿根廷	1252	*/
    MLANG_ES_BO,/* \li 58  	西班牙语	玻利维亚	1252	*/
    MLANG_ES_CL,/* \li 59  	西班牙语	智利	1252	*/
    MLANG_ES_CO,/* \li 60  	西班牙语	哥伦比亚	1252	*/
    MLANG_ES_CR,/* \li 61  	西班牙语	哥斯达黎加	1252	*/
    MLANG_ES_DM,/* \li 62  	西班牙语	多米尼加	1252	*/
    MLANG_ES_EC,/* \li 63  	西班牙语	厄瓜多尔	1252	*/
    MLANG_ES_ES,/* \li 64  	西班牙语	西班牙(Espa?ol)	1252	*/
    MLANG_ES_GT,/* \li 65  	西班牙语	危地马拉	1252	*/
    MLANG_ES_HN,/* \li 66  	西班牙语	洪都拉斯	1252	*/
    MLANG_ES_MX,/* \li 67  	西班牙语	墨西哥	1252	*/
    MLANG_ES_NI,/* \li 68  	西班牙语	尼加拉瓜	1252	*/
    MLANG_ES_PA,/* \li 69  	西班牙语	巴拿马	1252	*/
    MLANG_ES_PE,/* \li 70  	西班牙语	秘鲁	1252	*/
    MLANG_ES_PR,/* \li 71  	西班牙语	波多黎哥	1252	*/
    MLANG_ES_SV,/* \li 72  	西班牙语	萨尔瓦多		*/
    MLANG_ES_1,/* \li 73  	西班牙语	乌拉圭	1252	*/
    MLANG_ES_2,/* \li 74  	西班牙语	乌拉圭	1252	*/
    MLANG_ES_VE,/* \li 75  	西班牙语	委内瑞拉	1252	*/
    MLANG_ET_EE,/* \li 76  	爱沙尼亚语	爱沙尼亚(Estonian)	1257	*/
    MLANG_EU_1,/* \li 77  	巴斯克语	西班牙东北部的巴斯克和纳瓦拉两个自治州，以及法国西南部	1252	*/
    MLANG_FA_1,/* \li 78  	波斯语			*/
    MLANG_FA_2,/* \li 79  	波斯语			*/
    MLANG_FF,/* \li 80  	富拉语			*/
    MLANG_FI_FI,/* \li 81  	芬兰语	芬兰(Suomi)	1252	*/
    MLANG_FJ,/* \li 82  	斐济语			*/
    MLANG_FO_FO,/* \li 83  	法罗语	法罗群岛（Faeroese)	1250	*/
    MLANG_FR,/* \li 84  	阿法尔语			*/
    MLANG_FR_BE,/* \li 85  	法语	比利时	1252	*/
    MLANG_FR_CA,/* \li 86  	法语	加拿大	1252	*/
    MLANG_FR_CH,/* \li 87  	法语	瑞士	1252	*/
    MLANG_FR_FR,/* \li 88  	法语	法国(Fran?ais)	1252	*/
    MLANG_FR_LU,/* \li 89  	法语	卢森堡	1252	*/
    MLANG_FR_MC,/* \li 90  	法语	摩洛哥	1252	*/
    MLANG_FY,/* \li 91  	弗里西亚语			*/
    MLANG_GA,/* \li 92  	爱尔兰语			*/
    MLANG_GD,/* \li 93  	苏格兰盖尔语			*/
    MLANG_GL,/* \li 94  	加利西亚语			*/
    MLANG_GL_1,/* \li 95  	加里（利）西亚语	西班牙加里西利亚	1252	*/
    MLANG_GN,/* \li 96  	瓜拉尼语			*/
    MLANG_GU,/* \li 97  	古吉拉特语			*/
    MLANG_GV,/* \li 98  	马恩岛语			*/
    MLANG_HA,/* \li 99  	豪萨语			*/
    MLANG_HE_IL,/* \li 100  	希伯来语	以色列	1255	*/
    MLANG_HI,/* \li 101  	印地语			*/
    MLANG_HO,/* \li 102  	希里莫图语			*/
    MLANG_HR_HR,/* \li 103  	克罗地亚语    	克罗地亚（Croatian）	1250	*/
    MLANG_HT,/* \li 104  	海地克里奥尔语			*/
    MLANG_HU_HU,/* \li 105  	匈牙利语	匈牙利(Hungarian)	1250	*/
    MLANG_HY,/* \li 106  	亚美尼亚语			*/
    MLANG_HZ,/* \li 107  	赫雷罗语			*/
    MLANG_IA,/* \li 108  	国际语A			*/
    MLANG_ID_ID,/* \li 109  	印度尼西亚语	印度尼西亚（Indonesian）	1252	*/
    MLANG_IE,/* \li 110  	国际语E			*/
    MLANG_IG,/* \li 111  	伊博语			*/
    MLANG_IK,/* \li 112  	依努庇克语			*/
    MLANG_IO,/* \li 113  	伊多语			*/
    MLANG_IS_IS,/* \li 114  	冰岛语	冰岛(Icelandic)	1252	*/
    MLANG_IT_CH,/* \li 115  	意大利语	瑞士	1252	*/
    MLANG_IT_IT,/* \li 116  	意大利语	意大利(Italiano)	1252	*/
    MLANG_IU,/* \li 117  	因纽特语			*/
    MLANG_JA_JP,/* \li 118  	日本语	日本	932	*/
    MLANG_JV,/* \li 119  	爪哇语			*/
    MLANG_KA,/* \li 120  	格鲁吉亚语			*/
    MLANG_KG_CG,/* \li 121  	刚果语（班图语)	刚果	1252(latin-extend b)	*/
    MLANG_KI,/* \li 122  	基库尤语			*/
    MLANG_KJ,/* \li 123  	宽亚玛语			*/
    MLANG_KK_KZ,/* \li 124  	哈萨克语	哈萨克斯坦	1251	*/
    MLANG_KL,/* \li 125  	格陵兰语			*/
    MLANG_KM,/* \li 126  	高棉语			*/
    MLANG_KN,/* \li 127  	卡纳达语			*/
    MLANG_KO_KP,/* \li 128  	韩语	韩国韩语	949	*/
    MLANG_KR,/* \li 129  	卡努里语			*/
    MLANG_KS,/* \li 130  	克什米尔语			*/
    MLANG_KU,/* \li 131  	库尔德语			*/
    MLANG_KV,/* \li 132  	科米语			*/
    MLANG_KW,/* \li 133  	康沃尔语			*/
    MLANG_KY_KG,/* \li 134  	吉尔吉斯语	吉尔吉斯斯坦	1251	*/
    MLANG_LA,/* \li 135  	拉丁语			*/
    MLANG_LB,/* \li 136  	卢森堡语			*/
    MLANG_LG,/* \li 137  	干达语			*/
    MLANG_LI,/* \li 138  	林堡语			*/
    MLANG_LN,/* \li 139  	林加拉语			*/
    MLANG_LO,/* \li 140  	老挝语			*/
    MLANG_LT_LT,/* \li 141  	立陶宛语	立陶宛(Lithuanian)	1257	*/
    MLANG_LU,/* \li 142  	卢巴-加丹加语			*/
    MLANG_LV_LV,/* \li 143  	拉脱维亚语	拉脱维亚语（）	1257	*/
    MLANG_MG,/* \li 144  	马达加斯加语			*/
    MLANG_MH,/* \li 145  	马绍尔语			*/
    MLANG_MI_NZ,/* \li 146  	毛利语	新西兰		*/
    MLANG_MK_MK,/* \li 147  	马其顿语		1251	*/
    MLANG_ML,/* \li 148  	马拉亚拉姆语			*/
    MLANG_MN_MN,/* \li 149  	蒙古语	蒙古	1251	*/
    MLANG_MO,/* \li 150  	摩尔达维亚语			*/
    MLANG_MR,/* \li 151  	马拉提语			*/
    MLANG_MS_BN,/* \li 152  	马来语	文莱	1252	*/
    MLANG_MS_MY,/* \li 153  	马来语	马来西亚	1252	*/
    MLANG_MT_MT,/* \li 154  	马耳它语	马耳它		*/
    MLANG_MY,/* \li 155  	缅甸语			*/
    MLANG_NA,/* \li 156  	瑙鲁语			*/
    MLANG_ND,/* \li 157  	北恩德贝勒语			*/
    MLANG_NE,/* \li 158  	尼泊尔语			*/
    MLANG_NG,/* \li 159  	恩敦加语			*/
    MLANG_NL_BE,/* \li 160  	荷兰语	比利时	1252	*/
    MLANG_NL_NL,/* \li 161  	荷兰语	荷兰(Nederland)	1252	*/
    MLANG_NL_SA,/* \li 162  	南非荷兰语	南非	1252	*/
    MLANG_NO_NO,/* \li 163  	挪威语	挪威	1250	*/
    MLANG_NR,/* \li 164  	南恩德贝勒语			*/
    MLANG_NV,/* \li 165  	纳瓦霍语			*/
    MLANG_NY,/* \li 166  	尼扬贾语			*/
    MLANG_OC,/* \li 167  	奥克语			*/
    MLANG_OJ,/* \li 168  	奥吉布瓦语			*/
    MLANG_OM,/* \li 169  	奥洛莫语			*/
    MLANG_OR,/* \li 170  	奥利亚语			*/
    MLANG_PA,/* \li 171  	旁遮普语			*/
    MLANG_PI,/* \li 172  	巴利语			*/
    MLANG_PL_PL,/* \li 173  	波兰语	波兰(polski)	1257	*/
    MLANG_PS,/* \li 174  	普什图语			*/
    MLANG_PT_BR,/* \li 175  	葡萄牙语	巴西	1252	*/
    MLANG_PT_PT,/* \li 176  	葡萄牙语	葡萄牙(Português)	1252	*/
    MLANG_QU_BO,/* \li 177  	克丘亚语（Quechuan ）	波利维亚		*/
    MLANG_QU_EC,/* \li 178  	克丘亚语（Quechuan ）	厄瓜多尔		*/
    MLANG_QU_PE,/* \li 179  	克丘亚语（Quechuan ）	秘鲁		*/
    MLANG_RM,/* \li 180  	罗曼什语			*/
    MLANG_RN,/* \li 181  	基隆迪语			*/
    MLANG_RO_RO,/* \li 182  	罗马尼亚语	罗马尼亚(Romanian)	1250	*/
    MLANG_RU_RU,/* \li 183  	俄语	俄国(russia)	1251	*/
    MLANG_RW,/* \li 184  	基尼阿万达语			*/
    MLANG_SA,/* \li 185  	梵语			*/
    MLANG_SC,/* \li 186  	撒丁语			*/
    MLANG_SD,/* \li 187  	信德语			*/
    MLANG_SE_1,/* \li 188  	律勒欧萨摩斯语	挪威		*/
    MLANG_SE_2,/* \li 189  	律勒欧萨摩斯语	瑞典		*/
    MLANG_SE_3,/* \li 190  	南萨摩斯语	挪威		*/
    MLANG_SE_4,/* \li 191  	南萨摩斯语	瑞典		*/
    MLANG_SE_5,/* \li 192  	南萨摩斯语	伯克梅尔		*/
    MLANG_SE_6,/* \li 193  	南萨摩斯语	尼诺斯克		*/
    MLANG_SE_7,/* \li 194  	斯科特萨摩斯语			*/
    MLANG_SE_8,/* \li 195  	伊那里萨摩斯语	芬兰		*/
    MLANG_SE_FI,/* \li 196  	北萨米语（北萨摩斯语)	芬兰	1252	*/
    MLANG_SE_NO,/* \li 197  	北萨米语（北萨摩斯语)	挪威	1252	*/
    MLANG_SE_SE,/* \li 198  	北萨米语（北萨摩斯语)	瑞典	1252	*/
    MLANG_SG,/* \li 199  	桑戈语			*/
    MLANG_SH,/* \li 200  	塞尔维亚-克罗地亚语			*/
    MLANG_SI,/* \li 201  	僧加罗语			*/
    MLANG_SK_SK,/* \li 202  	斯洛伐克语	斯洛伐克(Slovak)	1250	*/
    MLANG_SL_SL,/* \li 203  	斯洛文尼亚语	斯洛文尼亚(Slovenian)	1250	*/
    MLANG_SN,/* \li 204  	绍纳语			*/
    MLANG_SO,/* \li 205  	索马里语			*/
    MLANG_SQ_AL,/* \li 206  	阿尔巴尼亚语	阿尔巴尼亚语(Albania )	1250	*/
    MLANG_SR,/* \li 207  	塞尔维亚语	西里尔文	1251	*/
    MLANG_SR_RS,/* \li 208  	塞尔维亚语	塞尔维亚(Serbian)	1250	*/
    MLANG_ST,/* \li 209  	南索托语			*/
    MLANG_SU,/* \li 210  	巽他语			*/
    MLANG_SV_FI,/* \li 211  	瑞典语	芬兰	1252	*/
    MLANG_SV_SE,/* \li 212  	瑞典语	瑞典(Svenska)	1252	*/
    MLANG_SW,/* \li 213  	斯瓦西里（Swahili）	坦桑尼亚, 肯尼亚, 乌干达	1252	*/
    MLANG_TA,/* \li 214  	泰米尔语			*/
    MLANG_TE,/* \li 215  	泰卢固语			*/
    MLANG_TG,/* \li 216  	塔吉克语			*/
    MLANG_TH,/* \li 217  	泰语			*/
    MLANG_TI,/* \li 218  	提格里尼亚语			*/
    MLANG_TK,/* \li 219  	土库曼语			*/
    MLANG_TL,/* \li 220  	塔加洛语			*/
    MLANG_TN,/* \li 221  	塞茨瓦纳语			*/
    MLANG_TN_1,/* \li 222  	茨瓦纳			*/
    MLANG_TO,/* \li 223  	汤加语			*/
    MLANG_TR_TR,/* \li 224  	土耳其语	土耳其(Türk?e)	1254	*/
    MLANG_TS,/* \li 225  	宗加语			*/
    MLANG_TT_1,/* \li 226  	鞑靼语(塔塔尔)			*/
    MLANG_TW,/* \li 227  	特威语			*/
    MLANG_TY,/* \li 228  	塔希提语			*/
    MLANG_UG,/* \li 229  	维吾尔语			*/
    MLANG_UK_UA,/* \li 230  	乌克兰语	乌克兰(Ukrainian)	1251	*/
    MLANG_UR,/* \li 231  	乌尔都语			*/
    MLANG_UZ_1,/* \li 232  	乌兹别克语	乌兹别克斯坦(西里尔文）	1251	*/
    MLANG_UZ_4,/* \li 233  	乌兹别克语	乌兹别克斯坦(拉丁）	1254	*/
    MLANG_VE,/* \li 234  	文达语			*/
    MLANG_VI,/* \li 235  	越南语			*/
    MLANG_VO,/* \li 236  	沃拉普克语			*/
    MLANG_WA,/* \li 237  	沃伦语			*/
    MLANG_WO,/* \li 238  	沃洛夫语			*/
    MLANG_XH,/* \li 239  	科萨语			*/
    MLANG_YI,/* \li 240  	依地语			*/
    MLANG_YO,/* \li 241  	约鲁巴语			*/
    MLANG_ZA,/* \li 242  	壮语			*/
    MLANG_ZA_1,/* \li 243  	北梭托语	南非	1252？	*/
    MLANG_ZH_CN,/* \li 244  	汉语	中国简体中文	936	*/
    MLANG_ZH_SG,/* \li 245  	汉语	新加坡	936	*/
    MLANG_ZH_TW,/* \li 246  	汉语	中国繁体(港，澳,台）	950	*/
    MLANG_ZU_ZA,/* \li 247  	祖鲁语	南非		*/
    MLANG_RESERVED_01,/* \li 248   保留值*/
    MLANG_RESERVED_02,/* \li 249   保留值*/
    MLANG_RESERVED_03,/* \li 250   保留值*/
    MLANG_RESERVED_04,/* \li 251   保留值*/
    MLANG_RESERVED_05,/* \li 252   保留值*/
    MLANG_RESERVED_06,/* \li 253   保留值*/
    MLANG_RESERVED_07,
/* \li 254   保留值*/
} mlang_language_id_t;
/*!\endcond*/
#endif /*_LOCALE_H_*/

