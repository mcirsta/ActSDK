#ifndef _BREC_INFO_H
#define _BREC_INFO_H

#ifndef _ASSEMBLER_

typedef struct
{

    unsigned int ic_version; /*IC version 'C','E' or other*/
    unsigned int boot_pic_addr; /*logo physical address*/
    unsigned int low_power_pic_addr; /*low power logo physical address*/
    unsigned int light_level; /*default backlight level*/
    unsigned int *lcd_type; /*return   0:CPU��1��RGB��else:other*/

} welcome_cfg_t;

typedef struct
{
    int checksum; /*��ȥchecksum�ֶ�����������ݣ�ͼƬ���ݼ��ļ�ͷ����У���*/
    int width; /*ͼƬÿ�е����ظ���*/
    int height; /*ͼƬ������*/
    int bytes_per_pix; /*ÿ������ռ�õ�BYTE��*/
    int length; /*�ļ��ܳ��ȣ������ļ�ͷ��data����*/
    int reserved[3]; /*����λ*/

} boot_pic_info_t;

#endif  //_ASSEMBLER_
///*for afinfo*/
//#define SEC_STAGE_MBRC_START    0xb4042a00  /* start address of second mbrc stage */
//#define AFINFO_ADDRESS  (SEC_STAGE_MBRC_START+0x3a00)

#endif	//_BREC_INFO_H
