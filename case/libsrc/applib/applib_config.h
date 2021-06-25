/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : applib_config.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-11-12             v1.0             build this file 
 ********************************************************************************/

#ifndef __APPLIB_CONFIG_H__
#define __APPLIB_CONFIG_H__

#include <include_case.h>

#define CFG_TEXT_LEN  128 

#define CFG_DIR_ENTRY_NUM   16

#define DIR_ALL_BUF_SIZE (1024)

typedef struct
{
    /* config key name */
    char * key;

    /* to store string type value */
    char text[CFG_TEXT_LEN];

    /* id */
    int id;

    /* is read */
    bool is_read;
}dir_cfg_t;

extern dir_cfg_t * g_udisk_dir_cfgs;
extern dir_cfg_t * g_card_dir_cfgs;
extern dir_cfg_t * g_external_dir_cfgs;

extern char * g_dir_all;

extern bool applib_dir_cfg_init(void);
extern bool applib_dir_cfg_deinit(void);

extern dir_cfg_t * applib_dir_cfg_find(dir_cfg_t cfgs[], int count, int id);
extern bool applib_dir_cfg_read(dir_cfg_t * cfg, const char * prefix, bool force);
extern bool applib_dir_cfg_read_all(dir_cfg_t cfgs[], int count, const char * prefix, bool force);

#endif /* #ifndef __APPLIB_CONFIG_H__ */
