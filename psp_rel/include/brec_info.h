#ifndef _BREC_INFO_H
#define _BREC_INFO_H

#ifndef _ASSEMBLER_

typedef struct
{

    unsigned int ic_version; /*IC version 'C','E' or other*/
    unsigned int boot_pic_addr; /*logo physical address*/
    unsigned int low_power_pic_addr; /*low power logo physical address*/
    unsigned int light_level; /*default backlight level*/
    unsigned int *lcd_type; /*return   0:CPU；1：RGB；else:other*/

} welcome_cfg_t;

typedef struct
{
    int checksum; /*除去checksum字段外的所有数据（图片数据及文件头）的校验和*/
    int width; /*图片每行的像素个数*/
    int height; /*图片的行数*/
    int bytes_per_pix; /*每个像素占用的BYTE数*/
    int length; /*文件总长度，包括文件头及data数据*/
    int reserved[3]; /*保留位*/

} boot_pic_info_t;

#endif  //_ASSEMBLER_
///*for afinfo*/
//#define SEC_STAGE_MBRC_START    0xb4042a00  /* start address of second mbrc stage */
//#define AFINFO_ADDRESS  (SEC_STAGE_MBRC_START+0x3a00)

#endif	//_BREC_INFO_H
