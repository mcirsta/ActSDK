/*******************************************************************************
 *                              USDK1100
 *                            Module: ACT_TAR
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-7-21 11:06     1.0             build this file
 *******************************************************************************/
/*!
 * \file     hmwei
 * \brief    atar文件格式相关定义
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/
#ifndef __ACT_TAR_H__
#define __ACT_TAR_H__

#ifndef _ASSEMBLER_

/* Values used in typeflag field.  */
#define REGTYPE  '0'        /* regular file */
#define AREGTYPE '\0'       /* regular file */
#define LNKTYPE  '1'        /* link */
#define SYMTYPE  '2'        /* reserved */
#define CHRTYPE  '3'        /* character special */
#define BLKTYPE  '4'        /* block special */
#define DIRTYPE  '5'        /* directory */
#define FIFOTYPE '6'        /* FIFO special */
#define CONTTYPE '7'        /* reserved */

enum subcommand
{
    UNKNOWN_SUBCOMMAND, /* none of the following */
    APPEND_SUBCOMMAND, /* -r */
    CAT_SUBCOMMAND, /* -A */
    CREATE_SUBCOMMAND, /* -c */
    DELETE_SUBCOMMAND, /* -D */
    DIFF_SUBCOMMAND, /* -d */
    EXTRACT_SUBCOMMAND, /* -x */
    LIST_SUBCOMMAND, /* -t */
    UPDATE_SUBCOMMAND
/* -u */
};

/* POSIX tar Header Block, from POSIX 1003.1-1990  */
#define NAME_SIZE      100

typedef struct tar_header tar_header;
struct tar_header
{ /* byte offset */
    char name[NAME_SIZE]; /*   0-99 */
    char mode[8]; /* 100-107 */
    char uid[8]; /* 108-115 */
    char gid[8]; /* 116-123 */
    char size[12]; /* 124-135 */
    char mtime[12]; /* 136-147 */
    char chksum[8]; /* 148-155 */
    char typeflag; /* 156-156 */
    char linkname[NAME_SIZE]; /* 157-256 */
    /* POSIX:   "ustar" NUL "00" */
    /* GNU tar: "ustar  " NUL */
    /* Normally it's defined as magic[6] followed by
     * version[2], but we put them together to save code.
     */
    char magic[8]; /* 257-264 */
    char uname[32]; /* 265-296 */
    char gname[32]; /* 297-328 */
    char devmajor[8]; /* 329-336 */
    char devminor[8]; /* 337-344 */
    char prefix[155]; /* 345-499 */
    char padding[12]; /* 500-512 (pad to exactly TAR_BLOCK_SIZE) */
};

#define TAR_BLOCK_SIZE      512

/*!
 * \brief
 *      atar格式头
 */
typedef struct
{
    /*! magic:'A''T''A''R'*/
    char magic[4];
    /*! 目录项个数*/
    unsigned int dir_cnt;
    /*! 目录项校验和*/
    unsigned int checksum;
    /*! reserved*/
    char reserved[4];
}__attribute__ ((packed)) atar_header;

/*!
 * \brief
 *      atar格式目录项
 */
typedef struct
{
    /*! 完整路径名*/
    char path[128];
    /*! 类型：文件/目录*/
    char type;
    /*! 生成时间（标准秒）*/
    unsigned int time;
    /*! 保留*/
    char reserved_1[11];
    /*! 文件在在atar格式内的偏移*/
    unsigned int offset;
    /*! 保留*/
    char reserved_2[4];
    /*! 文件大小*/
    unsigned int len;
    /*! 文件校验和*/
    unsigned int checksum;
}__attribute__ ((packed)) atar_dir;

/*!
 * \brief
 *      apm文件头
 */
typedef struct
{
    /*! magic:'A''P''M''\0'*/
    char magic[4];
    /*! 文件头长度*/
    unsigned int len;
    /*! 补齐32个bytes*/
    char total_padding[20];
    /*! 头校验和*/
    unsigned int checksum;
    /*! 平台版本号*/
    char plf_version[13];
    /*! 补齐平台信息到64个bytes*/
    char plf_padding[51];
    /*! 应用文件类型*/
    char type;//app/res/str/sty/bin
    /*! 应用文件安装路径*/
    char path[100];
    /*! 应用uid*/
    char uid[8];
    /*! 应用版本号*/
    char version[13];
    /*! 应用语言*/
    char language;
    /*! 补齐应用信息到426个字节*/
    char padding[512 - 32 - 64 - 123];
}__attribute__ ((packed)) apm_header;

extern int install_apm(char *filename, char *path);

#endif /*_ASSEMBLER_*/
#endif /*__ACT_TAR_H__ */
