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
 * \brief    ������������id
 * \author   yuchen
 * \version 1.0
 * \date  2008/11/22
 *******************************************************************************/
#ifndef __LOCALE_H__
#define __LOCALE_H__
/*!\cond MLANG*/
/*!
 *  \brief
 *  �����Һ͵���������id
 *  \li MLANG_INVALID : 0   �Ƿ�ֵ
 *  \li MLANG_AA_DF	: 1  	 Ĭ������
 *  \li MLANG_AE	: 2  	��ά˹����
 *  \li MLANG_AF_1	: 3  	�Ϸ���(��������˹��)
 *  \li MLANG_AK	: 4  	������
 *  \li MLANG_AM	: 4  	��ķ������
 *  \li MLANG_AN	: 6  	��������
 *  \li MLANG_AR	: 7  	��������
 *  \li MLANG_AS	: 8  	����ķ��
 *  \li MLANG_AV	: 9  	���߶���
 *  \li MLANG_AY	: 10  	��������
 *  \li MLANG_AZ_1	: 11  	�����ݽ���	�����ݽ���������ģ�	1251
 *  \li MLANG_AZ_2	: 12  	�����ݽ���	�����ݽ��������ģ�	1252
 *  \li MLANG_BA	: 13  	��ʲ������
 *  \li MLANG_BE_BY	: 14  	�׶���˹��	�׶���˹(Byelorussian)	1251
 *  \li MLANG_BG_BG	: 15  	����������	��������(Bulgarian)	1251
 *  \li MLANG_BH	: 16  	�ȹ�����
 *  \li MLANG_BI	: 17  	��˹������
 *  \li MLANG_BM	: 18  	�������
 *  \li MLANG_BN	: 19  	�ϼ�����
 *  \li MLANG_BO	: 20  	����
 *  \li MLANG_BR	: 21  	����������
 *  \li MLANG_BS_BA	: 22  	��˹������	��˹���Ǻͺ�����ά��
 *  \li MLANG_CA_AD	: 23  	��̩��������	���������ٷ�����	1252
 *  \li MLANG_CE	: 24  	������
 *  \li MLANG_CH	: 25  	��Ī����
 *  \li MLANG_CO	: 26  	��������
 *  \li MLANG_CR	: 27  	������
 *  \li MLANG_CS_CZ	: 28  	�ݿ���	�ݿ�(?esky)	1257
 *  \li MLANG_CU	: 29  	�Ž̻�˹������
 *  \li MLANG_CV	: 30  	����ʲ��
 *  \li MLANG_CY	: 31  	����ʿ�Welsh��	����ʿ	1252
 *  \li MLANG_CY_2	: 32  	����ʿ��		1252
 *  \li MLANG_DA_DK	: 33  	������    	����Dansk)	1250
 *  \li MLANG_DE_AT	: 34  	����	�µ���	1252
 *  \li MLANG_DE_CH	: 35  	����	��ʿ	1252
 *  \li MLANG_DE_GE	: 36  	����	�¹�(Deutsch)	1252
 *  \li MLANG_DE_LI	: 37  	����	��֧��ʿ�أ�Liechtenstein��	1252
 *  \li MLANG_DE_LU	: 38  	����	¬ɭ��	1252
 *  \li MLANG_DV	: 39  	��άϣ��
 *  \li MLANG_DZ	: 40  	������
 *  \li MLANG_EE	: 41  	��ά��
 *  \li MLANG_EL	: 42  	�ִ�ϣ����
 *  \li MLANG_EL_EG	: 43  	ϣ����	ϣ��(���˦˦Ǧͦɦʦ�)	1253
 *  \li MLANG_EN_AU	: 44  	Ӣ��	�Ĵ�����	1252
 *  \li MLANG_EN_BZ	: 45  	Ӣ��	������	1252
 *  \li MLANG_EN_CA	: 46  	Ӣ��	���ô�	1252
 *  \li MLANG_EN_GB	: 47  	Ӣ��	Ӣ��	1252
 *  \li MLANG_EN_IE	: 48  	Ӣ��	������	1252
 *  \li MLANG_EN_JM	: 49  	Ӣ��	�����	1252
 *  \li MLANG_EN_NZ	: 50  	Ӣ��	������	1252
 *  \li MLANG_EN_PH	: 51  	Ӣ��	���ɱ�	1252
 *  \li MLANG_EN_TT	: 52  	Ӣ��	�������	1252
 *  \li MLANG_EN_US	: 53  	Ӣ��	����Ӣ��	1252
 *  \li MLANG_EN_ZA	: 54  	Ӣ��	�Ϸ�	1252
 *  \li MLANG_EN_ZW	: 55  	Ӣ��	��Ͳ�Τ	1252
 *  \li MLANG_EO	: 56  	������
 *  \li MLANG_ES_AR	: 57  	��������	����͢	1252
 *  \li MLANG_ES_BO	: 58  	��������	����ά��	1252
 *  \li MLANG_ES_CL	: 59  	��������	����	1252
 *  \li MLANG_ES_CO	: 60  	��������	���ױ���	1252
 *  \li MLANG_ES_CR	: 61  	��������	��˹�����	1252
 *  \li MLANG_ES_DM	: 62  	��������	�������	1252
 *  \li MLANG_ES_EC	: 63  	��������	��϶��	1252
 *  \li MLANG_ES_ES	: 64  	��������	������(Espa?ol)	1252
 *  \li MLANG_ES_GT	: 65  	��������	Σ������	1252
 *  \li MLANG_ES_HN	: 66  	��������	�鶼��˹	1252
 *  \li MLANG_ES_MX	: 67  	��������	ī����	1252
 *  \li MLANG_ES_NI	: 68  	��������	�������	1252
 *  \li MLANG_ES_PA	: 69  	��������	������	1252
 *  \li MLANG_ES_PE	: 70  	��������	��³	1252
 *  \li MLANG_ES_PR	: 71  	��������	�������	1252
 *  \li MLANG_ES_SV	: 72  	��������	�����߶�
 *  \li MLANG_ES_1	: 73  	��������	������	1252
 *  \li MLANG_ES_2	: 74  	��������	������	1252
 *  \li MLANG_ES_VE	: 75  	��������	ί������	1252
 *  \li MLANG_ET_EE	: 76  	��ɳ������	��ɳ����(Estonian)	1257
 *  \li MLANG_EU_1	: 77  	��˹����	�������������İ�˹�˺����������������ݣ��Լ��������ϲ�	1252
 *  \li MLANG_FA_1	: 78  	��˹��
 *  \li MLANG_FA_2	: 79  	��˹��
 *  \li MLANG_FF	: 80  	������
 *  \li MLANG_FI_FI	: 81  	������	����(Suomi)	1252
 *  \li MLANG_FJ	: 82  	쳼���
 *  \li MLANG_FO_FO	: 83  	������	����Ⱥ����Faeroese)	1250
 *  \li MLANG_FR	: 84  	��������
 *  \li MLANG_FR_BE	: 85  	����	����ʱ	1252
 *  \li MLANG_FR_CA	: 86  	����	���ô�	1252
 *  \li MLANG_FR_CH	: 87  	����	��ʿ	1252
 *  \li MLANG_FR_FR	: 88  	����	����(Fran?ais)	1252
 *  \li MLANG_FR_LU	: 89  	����	¬ɭ��	1252
 *  \li MLANG_FR_MC	: 90  	����	Ħ���	1252
 *  \li MLANG_FY	: 91  	����������
 *  \li MLANG_GA	: 92  	��������
 *  \li MLANG_GD	: 93  	�ո����Ƕ���
 *  \li MLANG_GL	: 94  	����������
 *  \li MLANG_GL_1	: 95  	�������������	����������������	1252
 *  \li MLANG_GN	: 96  	��������
 *  \li MLANG_GU	: 97  	�ż�������
 *  \li MLANG_GV	: 98  	�������
 *  \li MLANG_HA	: 99  	������
 *  \li MLANG_HE_IL	: 100  	ϣ������	��ɫ��	1255
 *  \li MLANG_HI	: 101  	ӡ����
 *  \li MLANG_HO	: 102  	ϣ��Īͼ��
 *  \li MLANG_HR_HR	: 103  	���޵�����    	���޵��ǣ�Croatian��	1250
 *  \li MLANG_HT	: 104  	���ؿ���¶���
 *  \li MLANG_HU_HU	: 105  	��������	������(Hungarian)	1250
 *  \li MLANG_HY	: 106  	����������
 *  \li MLANG_HZ	: 107  	��������
 *  \li MLANG_IA	: 108  	������A
 *  \li MLANG_ID_ID	: 109  	ӡ����������	ӡ�������ǣ�Indonesian��	1252
 *  \li MLANG_IE	: 110  	������E
 *  \li MLANG_IG	: 111  	������
 *  \li MLANG_IK	: 112  	��Ŭ�ӿ���
 *  \li MLANG_IO	: 113  	������
 *  \li MLANG_IS_IS	: 114  	������	����(Icelandic)	1252
 *  \li MLANG_IT_CH	: 115  	�������	��ʿ	1252
 *  \li MLANG_IT_IT	: 116  	�������	�����(Italiano)	1252
 *  \li MLANG_IU	: 117  	��Ŧ����
 *  \li MLANG_JA_JP	: 118  	�ձ���	�ձ�	932
 *  \li MLANG_JV	: 119  	צ����
 *  \li MLANG_KA	: 120  	��³������
 *  \li MLANG_KG_CG	: 121  	�չ����ͼ��)	�չ�	1252(latin-extend b)
 *  \li MLANG_KI	: 122  	��������
 *  \li MLANG_KJ	: 123  	��������
 *  \li MLANG_KK_KZ	: 124  	��������	������˹̹	1251
 *  \li MLANG_KL	: 125  	��������
 *  \li MLANG_KM	: 126  	������
 *  \li MLANG_KN	: 127  	���ɴ���
 *  \li MLANG_KO_KP	: 128  	����	��������	949
 *  \li MLANG_KR	: 129  	��Ŭ����
 *  \li MLANG_KS	: 130  	��ʲ�׶���
 *  \li MLANG_KU	: 131  	�������
 *  \li MLANG_KV	: 132  	������
 *  \li MLANG_KW	: 133  	���ֶ���
 *  \li MLANG_KY_KG	: 134  	������˹��	������˹˹̹	1251
 *  \li MLANG_LA	: 135  	������
 *  \li MLANG_LB	: 136  	¬ɭ����
 *  \li MLANG_LG	: 137  	�ɴ���
 *  \li MLANG_LI	: 138  	�ֱ���
 *  \li MLANG_LN	: 139  	�ּ�����
 *  \li MLANG_LO	: 140  	������
 *  \li MLANG_LT_LT	: 141  	��������	������(Lithuanian)	1257
 *  \li MLANG_LU	: 142  	¬��-�ӵ�����
 *  \li MLANG_LV_LV	: 143  	����ά����	����ά�����	1257
 *  \li MLANG_MG	: 144  	����˹����
 *  \li MLANG_MH	: 145  	���ܶ���
 *  \li MLANG_MI_NZ	: 146  	ë����	������
 *  \li MLANG_MK_MK	: 147  	�������		1251
 *  \li MLANG_ML	: 148  	��������ķ��
 *  \li MLANG_MN_MN	: 149  	�ɹ���	�ɹ�	1251
 *  \li MLANG_MO	: 150  	Ħ����ά����
 *  \li MLANG_MR	: 151  	��������
 *  \li MLANG_MS_BN	: 152  	������	����	1252
 *  \li MLANG_MS_MY	: 153  	������	��������	1252
 *  \li MLANG_MT_MT	: 154  	�������	�����
 *  \li MLANG_MY	: 155  	�����
 *  \li MLANG_NA	: 156  	�³��
 *  \li MLANG_ND	: 157  	�����±�����
 *  \li MLANG_NE	: 158  	�Ჴ����
 *  \li MLANG_NG	: 159  	���ؼ���
 *  \li MLANG_NL_BE	: 160  	������	����ʱ	1252
 *  \li MLANG_NL_NL	: 161  	������	����(Nederland)	1252
 *  \li MLANG_NL_SA	: 162  	�ϷǺ�����	�Ϸ�	1252
 *  \li MLANG_NO_NO	: 163  	Ų����	Ų��	1250
 *  \li MLANG_NR	: 164  	�϶��±�����
 *  \li MLANG_NV	: 165  	���߻���
 *  \li MLANG_NY	: 166  	�������
 *  \li MLANG_OC	: 167  	�¿���
 *  \li MLANG_OJ	: 168  	�¼�������
 *  \li MLANG_OM	: 169  	����Ī��
 *  \li MLANG_OR	: 170  	��������
 *  \li MLANG_PA	: 171  	��������
 *  \li MLANG_PI	: 172  	������
 *  \li MLANG_PL_PL	: 173  	������	����(polski)	1257
 *  \li MLANG_PS	: 174  	��ʲͼ��
 *  \li MLANG_PT_BR	: 175  	��������	����	1252
 *  \li MLANG_PT_PT	: 176  	��������	������(Portugu��s)	1252
 *  \li MLANG_QU_BO	: 177  	�������Quechuan ��	����ά��
 *  \li MLANG_QU_EC	: 178  	�������Quechuan ��	��϶��
 *  \li MLANG_QU_PE	: 179  	�������Quechuan ��	��³
 *  \li MLANG_RM	: 180  	����ʲ��
 *  \li MLANG_RN	: 181  	��¡����
 *  \li MLANG_RO_RO	: 182  	����������	��������(Romanian)	1250
 *  \li MLANG_RU_RU	: 183  	����	���(russia)	1251
 *  \li MLANG_RW	: 184  	���ᰢ�����
 *  \li MLANG_SA	: 185  	����
 *  \li MLANG_SC	: 186  	������
 *  \li MLANG_SD	: 187  	�ŵ���
 *  \li MLANG_SE_1	: 188  	����ŷ��Ħ˹��	Ų��
 *  \li MLANG_SE_2	: 189  	����ŷ��Ħ˹��	���
 *  \li MLANG_SE_3	: 190  	����Ħ˹��	Ų��
 *  \li MLANG_SE_4	: 191  	����Ħ˹��	���
 *  \li MLANG_SE_5	: 192  	����Ħ˹��	����÷��
 *  \li MLANG_SE_6	: 193  	����Ħ˹��	��ŵ˹��
 *  \li MLANG_SE_7	: 194  	˹������Ħ˹��
 *  \li MLANG_SE_8	: 195  	��������Ħ˹��	����
 *  \li MLANG_SE_FI	: 196  	�����������Ħ˹��)	����	1252
 *  \li MLANG_SE_NO	: 197  	�����������Ħ˹��)	Ų��	1252
 *  \li MLANG_SE_SE	: 198  	�����������Ħ˹��)	���	1252
 *  \li MLANG_SG	: 199  	ɣ����
 *  \li MLANG_SH	: 200  	����ά��-���޵�����
 *  \li MLANG_SI	: 201  	ɮ������
 *  \li MLANG_SK_SK	: 202  	˹�工����	˹�工��(Slovak)	1250
 *  \li MLANG_SL_SL	: 203  	˹����������	˹��������(Slovenian)	1250
 *  \li MLANG_SN	: 204  	������
 *  \li MLANG_SO	: 205  	��������
 *  \li MLANG_SQ_AL	: 206  	������������	������������(Albania )	1250
 *  \li MLANG_SR	: 207  	����ά����	�������	1251
 *  \li MLANG_SR_RS	: 208  	����ά����	����ά��(Serbian)	1250
 *  \li MLANG_ST	: 209  	��������
 *  \li MLANG_SU	: 210  	������
 *  \li MLANG_SV_FI	: 211  	�����	����	1252
 *  \li MLANG_SV_SE	: 212  	�����	���(Svenska)	1252
 *  \li MLANG_SW	: 213  	˹�����Swahili��	̹ɣ����, ������, �ڸɴ�	1252
 *  \li MLANG_TA	: 214  	̩�׶���
 *  \li MLANG_TE	: 215  	̩¬����
 *  \li MLANG_TG	: 216  	��������
 *  \li MLANG_TH	: 217  	̩��
 *  \li MLANG_TI	: 218  	�����������
 *  \li MLANG_TK	: 219  	��������
 *  \li MLANG_TL	: 220  	��������
 *  \li MLANG_TN	: 221  	����������
 *  \li MLANG_TN_1	: 222  	������
 *  \li MLANG_TO	: 223  	������
 *  \li MLANG_TR_TR	: 224  	��������	������(T��rk?e)	1254
 *  \li MLANG_TS	: 225  	�ڼ���
 *  \li MLANG_TT_1	: 226  	������(������)
 *  \li MLANG_TW	: 227  	������
 *  \li MLANG_TY	: 228  	��ϣ����
 *  \li MLANG_UG	: 229  	ά�����
 *  \li MLANG_UK_UA	: 230  	�ڿ�����	�ڿ���(Ukrainian)	1251
 *  \li MLANG_UR	: 231  	�ڶ�����
 *  \li MLANG_UZ_1	: 232  	���ȱ����	���ȱ��˹̹(������ģ�	1251
 *  \li MLANG_UZ_4	: 233  	���ȱ����	���ȱ��˹̹(������	1254
 *  \li MLANG_VE	: 234  	�Ĵ���
 *  \li MLANG_VI	: 235  	Խ����
 *  \li MLANG_VO	: 236  	�����տ���
 *  \li MLANG_WA	: 237  	������
 *  \li MLANG_WO	: 238  	�������
 *  \li MLANG_XH	: 239  	������
 *  \li MLANG_YI	: 240  	������
 *  \li MLANG_YO	: 241  	Լ³����
 *  \li MLANG_ZA	: 242  	׳��
 *  \li MLANG_ZA_1	: 243  	��������	�Ϸ�	1252��
 *  \li MLANG_ZH_CN	: 244  	����	�й���������	936
 *  \li MLANG_ZH_SG	: 245  	����	�¼���	936
 *  \li MLANG_ZH_TW	: 246  	����	�й�����(�ۣ���,̨��	950
 *  \li MLANG_ZU_ZA	: 247  	��³��	�Ϸ�
 *  \li MLANG_RESERVED_01   : 248   ����ֵ
 *  \li MLANG_RESERVED_02   : 249   ����ֵ
 *  \li MLANG_RESERVED_03   : 250   ����ֵ
 *  \li MLANG_RESERVED_04   : 251   ����ֵ
 *  \li MLANG_RESERVED_05   : 252   ����ֵ
 *  \li MLANG_RESERVED_06   : 253   ����ֵ
 *  \MLANG_RESERVED_07   : 254   ����ֵ
 */
typedef enum
{
    MLANG_INVALID,/* \li 0   �Ƿ�ֵ*/
    MLANG_AA_DF,/* \li 1  	 Ĭ������		*/
    MLANG_AE,/* \li 2  	��ά˹����			*/
    MLANG_AF_1,/* \li 3  	�Ϸ���(��������˹��)			*/
    MLANG_AK,/* \li 4  	������			*/
    MLANG_AM,/* \li 4  	��ķ������			*/
    MLANG_AN,/* \li 6  	��������			*/
    MLANG_AR,/* \li 7  	��������			*/
    MLANG_AS,/* \li 8  	����ķ��			*/
    MLANG_AV,/* \li 9  	���߶���			*/
    MLANG_AY,/* \li 10  	��������			*/
    MLANG_AZ_1,/* \li 11  	�����ݽ���	�����ݽ���������ģ�	1251	*/
    MLANG_AZ_2,/* \li 12  	�����ݽ���	�����ݽ��������ģ�	1252	*/
    MLANG_BA,/* \li 13  	��ʲ������			*/
    MLANG_BE_BY,/* \li 14  	�׶���˹��	�׶���˹(Byelorussian)	1251	*/
    MLANG_BG_BG,/* \li 15  	����������	��������(Bulgarian)	1251	*/
    MLANG_BH,/* \li 16  	�ȹ�����			*/
    MLANG_BI,/* \li 17  	��˹������			*/
    MLANG_BM,/* \li 18  	�������			*/
    MLANG_BN,/* \li 19  	�ϼ�����			*/
    MLANG_BO,/* \li 20  	����			*/
    MLANG_BR,/* \li 21  	����������			*/
    MLANG_BS_BA,/* \li 22  	��˹������	��˹���Ǻͺ�����ά��		*/
    MLANG_CA_AD,/* \li 23  	��̩��������	���������ٷ�����	1252	*/
    MLANG_CE,/* \li 24  	������			*/
    MLANG_CH,/* \li 25  	��Ī����			*/
    MLANG_CO,/* \li 26  	��������			*/
    MLANG_CR,/* \li 27  	������			*/
    MLANG_CS_CZ,/* \li 28  	�ݿ���	�ݿ�(?esky)	1257	*/
    MLANG_CU,/* \li 29  	�Ž̻�˹������			*/
    MLANG_CV,/* \li 30  	����ʲ��			*/
    MLANG_CY,/* \li 31  	����ʿ�Welsh��	����ʿ	1252	*/
    MLANG_CY_2,/* \li 32  	����ʿ��		1252	*/
    MLANG_DA_DK,/* \li 33  	������    	����Dansk)	1250	*/
    MLANG_DE_AT,/* \li 34  	����	�µ���	1252	*/
    MLANG_DE_CH,/* \li 35  	����	��ʿ	1252	*/
    MLANG_DE_GE,/* \li 36  	����	�¹�(Deutsch)	1252	*/
    MLANG_DE_LI,/* \li 37  	����	��֧��ʿ�أ�Liechtenstein��	1252	*/
    MLANG_DE_LU,/* \li 38  	����	¬ɭ��	1252	*/
    MLANG_DV,/* \li 39  	��άϣ��			*/
    MLANG_DZ,/* \li 40  	������			*/
    MLANG_EE,/* \li 41  	��ά��			*/
    MLANG_EL,/* \li 42  	�ִ�ϣ����			*/
    MLANG_EL_EG,/* \li 43  	ϣ����	ϣ��(���˦˦Ǧͦɦʦ�)	1253	*/
    MLANG_EN_AU,/* \li 44  	Ӣ��	�Ĵ�����	1252	*/
    MLANG_EN_BZ,/* \li 45  	Ӣ��	������	1252	*/
    MLANG_EN_CA,/* \li 46  	Ӣ��	���ô�	1252	*/
    MLANG_EN_GB,/* \li 47  	Ӣ��	Ӣ��	1252	*/
    MLANG_EN_IE,/* \li 48  	Ӣ��	������	1252	*/
    MLANG_EN_JM,/* \li 49  	Ӣ��	�����	1252	*/
    MLANG_EN_NZ,/* \li 50  	Ӣ��	������	1252	*/
    MLANG_EN_PH,/* \li 51  	Ӣ��	���ɱ�	1252	*/
    MLANG_EN_TT,/* \li 52  	Ӣ��	�������	1252	*/
    MLANG_EN_US,/* \li 53  	Ӣ��	����Ӣ��	1252	*/
    MLANG_EN_ZA,/* \li 54  	Ӣ��	�Ϸ�	1252	*/
    MLANG_EN_ZW,/* \li 55  	Ӣ��	��Ͳ�Τ	1252	*/
    MLANG_EO,/* \li 56  	������			*/
    MLANG_ES_AR,/* \li 57  	��������	����͢	1252	*/
    MLANG_ES_BO,/* \li 58  	��������	����ά��	1252	*/
    MLANG_ES_CL,/* \li 59  	��������	����	1252	*/
    MLANG_ES_CO,/* \li 60  	��������	���ױ���	1252	*/
    MLANG_ES_CR,/* \li 61  	��������	��˹�����	1252	*/
    MLANG_ES_DM,/* \li 62  	��������	�������	1252	*/
    MLANG_ES_EC,/* \li 63  	��������	��϶��	1252	*/
    MLANG_ES_ES,/* \li 64  	��������	������(Espa?ol)	1252	*/
    MLANG_ES_GT,/* \li 65  	��������	Σ������	1252	*/
    MLANG_ES_HN,/* \li 66  	��������	�鶼��˹	1252	*/
    MLANG_ES_MX,/* \li 67  	��������	ī����	1252	*/
    MLANG_ES_NI,/* \li 68  	��������	�������	1252	*/
    MLANG_ES_PA,/* \li 69  	��������	������	1252	*/
    MLANG_ES_PE,/* \li 70  	��������	��³	1252	*/
    MLANG_ES_PR,/* \li 71  	��������	�������	1252	*/
    MLANG_ES_SV,/* \li 72  	��������	�����߶�		*/
    MLANG_ES_1,/* \li 73  	��������	������	1252	*/
    MLANG_ES_2,/* \li 74  	��������	������	1252	*/
    MLANG_ES_VE,/* \li 75  	��������	ί������	1252	*/
    MLANG_ET_EE,/* \li 76  	��ɳ������	��ɳ����(Estonian)	1257	*/
    MLANG_EU_1,/* \li 77  	��˹����	�������������İ�˹�˺����������������ݣ��Լ��������ϲ�	1252	*/
    MLANG_FA_1,/* \li 78  	��˹��			*/
    MLANG_FA_2,/* \li 79  	��˹��			*/
    MLANG_FF,/* \li 80  	������			*/
    MLANG_FI_FI,/* \li 81  	������	����(Suomi)	1252	*/
    MLANG_FJ,/* \li 82  	쳼���			*/
    MLANG_FO_FO,/* \li 83  	������	����Ⱥ����Faeroese)	1250	*/
    MLANG_FR,/* \li 84  	��������			*/
    MLANG_FR_BE,/* \li 85  	����	����ʱ	1252	*/
    MLANG_FR_CA,/* \li 86  	����	���ô�	1252	*/
    MLANG_FR_CH,/* \li 87  	����	��ʿ	1252	*/
    MLANG_FR_FR,/* \li 88  	����	����(Fran?ais)	1252	*/
    MLANG_FR_LU,/* \li 89  	����	¬ɭ��	1252	*/
    MLANG_FR_MC,/* \li 90  	����	Ħ���	1252	*/
    MLANG_FY,/* \li 91  	����������			*/
    MLANG_GA,/* \li 92  	��������			*/
    MLANG_GD,/* \li 93  	�ո����Ƕ���			*/
    MLANG_GL,/* \li 94  	����������			*/
    MLANG_GL_1,/* \li 95  	�������������	����������������	1252	*/
    MLANG_GN,/* \li 96  	��������			*/
    MLANG_GU,/* \li 97  	�ż�������			*/
    MLANG_GV,/* \li 98  	�������			*/
    MLANG_HA,/* \li 99  	������			*/
    MLANG_HE_IL,/* \li 100  	ϣ������	��ɫ��	1255	*/
    MLANG_HI,/* \li 101  	ӡ����			*/
    MLANG_HO,/* \li 102  	ϣ��Īͼ��			*/
    MLANG_HR_HR,/* \li 103  	���޵�����    	���޵��ǣ�Croatian��	1250	*/
    MLANG_HT,/* \li 104  	���ؿ���¶���			*/
    MLANG_HU_HU,/* \li 105  	��������	������(Hungarian)	1250	*/
    MLANG_HY,/* \li 106  	����������			*/
    MLANG_HZ,/* \li 107  	��������			*/
    MLANG_IA,/* \li 108  	������A			*/
    MLANG_ID_ID,/* \li 109  	ӡ����������	ӡ�������ǣ�Indonesian��	1252	*/
    MLANG_IE,/* \li 110  	������E			*/
    MLANG_IG,/* \li 111  	������			*/
    MLANG_IK,/* \li 112  	��Ŭ�ӿ���			*/
    MLANG_IO,/* \li 113  	������			*/
    MLANG_IS_IS,/* \li 114  	������	����(Icelandic)	1252	*/
    MLANG_IT_CH,/* \li 115  	�������	��ʿ	1252	*/
    MLANG_IT_IT,/* \li 116  	�������	�����(Italiano)	1252	*/
    MLANG_IU,/* \li 117  	��Ŧ����			*/
    MLANG_JA_JP,/* \li 118  	�ձ���	�ձ�	932	*/
    MLANG_JV,/* \li 119  	צ����			*/
    MLANG_KA,/* \li 120  	��³������			*/
    MLANG_KG_CG,/* \li 121  	�չ����ͼ��)	�չ�	1252(latin-extend b)	*/
    MLANG_KI,/* \li 122  	��������			*/
    MLANG_KJ,/* \li 123  	��������			*/
    MLANG_KK_KZ,/* \li 124  	��������	������˹̹	1251	*/
    MLANG_KL,/* \li 125  	��������			*/
    MLANG_KM,/* \li 126  	������			*/
    MLANG_KN,/* \li 127  	���ɴ���			*/
    MLANG_KO_KP,/* \li 128  	����	��������	949	*/
    MLANG_KR,/* \li 129  	��Ŭ����			*/
    MLANG_KS,/* \li 130  	��ʲ�׶���			*/
    MLANG_KU,/* \li 131  	�������			*/
    MLANG_KV,/* \li 132  	������			*/
    MLANG_KW,/* \li 133  	���ֶ���			*/
    MLANG_KY_KG,/* \li 134  	������˹��	������˹˹̹	1251	*/
    MLANG_LA,/* \li 135  	������			*/
    MLANG_LB,/* \li 136  	¬ɭ����			*/
    MLANG_LG,/* \li 137  	�ɴ���			*/
    MLANG_LI,/* \li 138  	�ֱ���			*/
    MLANG_LN,/* \li 139  	�ּ�����			*/
    MLANG_LO,/* \li 140  	������			*/
    MLANG_LT_LT,/* \li 141  	��������	������(Lithuanian)	1257	*/
    MLANG_LU,/* \li 142  	¬��-�ӵ�����			*/
    MLANG_LV_LV,/* \li 143  	����ά����	����ά�����	1257	*/
    MLANG_MG,/* \li 144  	����˹����			*/
    MLANG_MH,/* \li 145  	���ܶ���			*/
    MLANG_MI_NZ,/* \li 146  	ë����	������		*/
    MLANG_MK_MK,/* \li 147  	�������		1251	*/
    MLANG_ML,/* \li 148  	��������ķ��			*/
    MLANG_MN_MN,/* \li 149  	�ɹ���	�ɹ�	1251	*/
    MLANG_MO,/* \li 150  	Ħ����ά����			*/
    MLANG_MR,/* \li 151  	��������			*/
    MLANG_MS_BN,/* \li 152  	������	����	1252	*/
    MLANG_MS_MY,/* \li 153  	������	��������	1252	*/
    MLANG_MT_MT,/* \li 154  	�������	�����		*/
    MLANG_MY,/* \li 155  	�����			*/
    MLANG_NA,/* \li 156  	�³��			*/
    MLANG_ND,/* \li 157  	�����±�����			*/
    MLANG_NE,/* \li 158  	�Ჴ����			*/
    MLANG_NG,/* \li 159  	���ؼ���			*/
    MLANG_NL_BE,/* \li 160  	������	����ʱ	1252	*/
    MLANG_NL_NL,/* \li 161  	������	����(Nederland)	1252	*/
    MLANG_NL_SA,/* \li 162  	�ϷǺ�����	�Ϸ�	1252	*/
    MLANG_NO_NO,/* \li 163  	Ų����	Ų��	1250	*/
    MLANG_NR,/* \li 164  	�϶��±�����			*/
    MLANG_NV,/* \li 165  	���߻���			*/
    MLANG_NY,/* \li 166  	�������			*/
    MLANG_OC,/* \li 167  	�¿���			*/
    MLANG_OJ,/* \li 168  	�¼�������			*/
    MLANG_OM,/* \li 169  	����Ī��			*/
    MLANG_OR,/* \li 170  	��������			*/
    MLANG_PA,/* \li 171  	��������			*/
    MLANG_PI,/* \li 172  	������			*/
    MLANG_PL_PL,/* \li 173  	������	����(polski)	1257	*/
    MLANG_PS,/* \li 174  	��ʲͼ��			*/
    MLANG_PT_BR,/* \li 175  	��������	����	1252	*/
    MLANG_PT_PT,/* \li 176  	��������	������(Portugu��s)	1252	*/
    MLANG_QU_BO,/* \li 177  	�������Quechuan ��	����ά��		*/
    MLANG_QU_EC,/* \li 178  	�������Quechuan ��	��϶��		*/
    MLANG_QU_PE,/* \li 179  	�������Quechuan ��	��³		*/
    MLANG_RM,/* \li 180  	����ʲ��			*/
    MLANG_RN,/* \li 181  	��¡����			*/
    MLANG_RO_RO,/* \li 182  	����������	��������(Romanian)	1250	*/
    MLANG_RU_RU,/* \li 183  	����	���(russia)	1251	*/
    MLANG_RW,/* \li 184  	���ᰢ�����			*/
    MLANG_SA,/* \li 185  	����			*/
    MLANG_SC,/* \li 186  	������			*/
    MLANG_SD,/* \li 187  	�ŵ���			*/
    MLANG_SE_1,/* \li 188  	����ŷ��Ħ˹��	Ų��		*/
    MLANG_SE_2,/* \li 189  	����ŷ��Ħ˹��	���		*/
    MLANG_SE_3,/* \li 190  	����Ħ˹��	Ų��		*/
    MLANG_SE_4,/* \li 191  	����Ħ˹��	���		*/
    MLANG_SE_5,/* \li 192  	����Ħ˹��	����÷��		*/
    MLANG_SE_6,/* \li 193  	����Ħ˹��	��ŵ˹��		*/
    MLANG_SE_7,/* \li 194  	˹������Ħ˹��			*/
    MLANG_SE_8,/* \li 195  	��������Ħ˹��	����		*/
    MLANG_SE_FI,/* \li 196  	�����������Ħ˹��)	����	1252	*/
    MLANG_SE_NO,/* \li 197  	�����������Ħ˹��)	Ų��	1252	*/
    MLANG_SE_SE,/* \li 198  	�����������Ħ˹��)	���	1252	*/
    MLANG_SG,/* \li 199  	ɣ����			*/
    MLANG_SH,/* \li 200  	����ά��-���޵�����			*/
    MLANG_SI,/* \li 201  	ɮ������			*/
    MLANG_SK_SK,/* \li 202  	˹�工����	˹�工��(Slovak)	1250	*/
    MLANG_SL_SL,/* \li 203  	˹����������	˹��������(Slovenian)	1250	*/
    MLANG_SN,/* \li 204  	������			*/
    MLANG_SO,/* \li 205  	��������			*/
    MLANG_SQ_AL,/* \li 206  	������������	������������(Albania )	1250	*/
    MLANG_SR,/* \li 207  	����ά����	�������	1251	*/
    MLANG_SR_RS,/* \li 208  	����ά����	����ά��(Serbian)	1250	*/
    MLANG_ST,/* \li 209  	��������			*/
    MLANG_SU,/* \li 210  	������			*/
    MLANG_SV_FI,/* \li 211  	�����	����	1252	*/
    MLANG_SV_SE,/* \li 212  	�����	���(Svenska)	1252	*/
    MLANG_SW,/* \li 213  	˹�����Swahili��	̹ɣ����, ������, �ڸɴ�	1252	*/
    MLANG_TA,/* \li 214  	̩�׶���			*/
    MLANG_TE,/* \li 215  	̩¬����			*/
    MLANG_TG,/* \li 216  	��������			*/
    MLANG_TH,/* \li 217  	̩��			*/
    MLANG_TI,/* \li 218  	�����������			*/
    MLANG_TK,/* \li 219  	��������			*/
    MLANG_TL,/* \li 220  	��������			*/
    MLANG_TN,/* \li 221  	����������			*/
    MLANG_TN_1,/* \li 222  	������			*/
    MLANG_TO,/* \li 223  	������			*/
    MLANG_TR_TR,/* \li 224  	��������	������(T��rk?e)	1254	*/
    MLANG_TS,/* \li 225  	�ڼ���			*/
    MLANG_TT_1,/* \li 226  	������(������)			*/
    MLANG_TW,/* \li 227  	������			*/
    MLANG_TY,/* \li 228  	��ϣ����			*/
    MLANG_UG,/* \li 229  	ά�����			*/
    MLANG_UK_UA,/* \li 230  	�ڿ�����	�ڿ���(Ukrainian)	1251	*/
    MLANG_UR,/* \li 231  	�ڶ�����			*/
    MLANG_UZ_1,/* \li 232  	���ȱ����	���ȱ��˹̹(������ģ�	1251	*/
    MLANG_UZ_4,/* \li 233  	���ȱ����	���ȱ��˹̹(������	1254	*/
    MLANG_VE,/* \li 234  	�Ĵ���			*/
    MLANG_VI,/* \li 235  	Խ����			*/
    MLANG_VO,/* \li 236  	�����տ���			*/
    MLANG_WA,/* \li 237  	������			*/
    MLANG_WO,/* \li 238  	�������			*/
    MLANG_XH,/* \li 239  	������			*/
    MLANG_YI,/* \li 240  	������			*/
    MLANG_YO,/* \li 241  	Լ³����			*/
    MLANG_ZA,/* \li 242  	׳��			*/
    MLANG_ZA_1,/* \li 243  	��������	�Ϸ�	1252��	*/
    MLANG_ZH_CN,/* \li 244  	����	�й���������	936	*/
    MLANG_ZH_SG,/* \li 245  	����	�¼���	936	*/
    MLANG_ZH_TW,/* \li 246  	����	�й�����(�ۣ���,̨��	950	*/
    MLANG_ZU_ZA,/* \li 247  	��³��	�Ϸ�		*/
    MLANG_RESERVED_01,/* \li 248   ����ֵ*/
    MLANG_RESERVED_02,/* \li 249   ����ֵ*/
    MLANG_RESERVED_03,/* \li 250   ����ֵ*/
    MLANG_RESERVED_04,/* \li 251   ����ֵ*/
    MLANG_RESERVED_05,/* \li 252   ����ֵ*/
    MLANG_RESERVED_06,/* \li 253   ����ֵ*/
    MLANG_RESERVED_07,
/* \li 254   ����ֵ*/
} mlang_language_id_t;
/*!\endcond*/
#endif /*_LOCALE_H_*/

