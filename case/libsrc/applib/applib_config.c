/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : applib_config.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-11-12             v1.0             build this file 
 ********************************************************************************/

#include "applib_config.h"

#define APPLIB_DIR_CFG_SHM_NAME    "/shm/applib_dir_cfg"

static int s_shm_fd = -1;
static bool s_initialized = false;
static inline bool is_inited(void);

dir_cfg_t * g_udisk_dir_cfgs;
dir_cfg_t * g_card_dir_cfgs;
dir_cfg_t * g_external_dir_cfgs;

char * g_dir_all;

bool applib_dir_cfg_init( void )
{
    bool create = false;
    int fd = -1;
    unsigned int shm_size;

    if(true == s_initialized)
    {
        print_warning(" applib dir config has already s_initialized!");
        return false;
    }

    shm_size = (sizeof(dir_cfg_t) * CFG_DIR_ENTRY_NUM * 3) + DIR_ALL_BUF_SIZE;

    print_dbg("open shared memory: %s", APPLIB_DIR_CFG_SHM_NAME);
    fd = shm_open(APPLIB_DIR_CFG_SHM_NAME, O_RDWR, 0666);
    if(fd < 0)
    {
        print_info("create shared memory: %s",APPLIB_DIR_CFG_SHM_NAME);
        fd = shm_open(APPLIB_DIR_CFG_SHM_NAME, O_RDWR | O_CREAT, 0666);
        if(fd < 0)
        {
            print_err("create shared memory %s failed!", APPLIB_DIR_CFG_SHM_NAME);
            return false;
        }

#ifdef USE_SYS_CALL
        sys_ftruncate((unsigned int)fd, shm_size); 
#else
        ftruncate(fd, shm_size); 
#endif
        create = true;
    }

#ifdef USE_SYS_CALL
    g_udisk_dir_cfgs = (dir_cfg_t *)sys_mmap((unsigned long)NULL, shm_size, \
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#else
    g_udisk_dir_cfgs = (dir_cfg_t *)mmap(NULL, shm_size, \
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#endif


    if((void *)-1 == (void *)g_udisk_dir_cfgs)
    {
        print_err("map to %s failed!", APPLIB_DIR_CFG_SHM_NAME);
        close(fd);
        fd = -1;
        g_udisk_dir_cfgs = NULL;
        g_card_dir_cfgs = NULL;
        g_external_dir_cfgs = NULL;
        return false;
    }

    g_card_dir_cfgs = &g_udisk_dir_cfgs[CFG_DIR_ENTRY_NUM];
    g_external_dir_cfgs = &g_card_dir_cfgs[CFG_DIR_ENTRY_NUM];
    g_dir_all = (char *)&g_external_dir_cfgs[CFG_DIR_ENTRY_NUM];

    if(create == true)
    {
        memset(g_udisk_dir_cfgs, 0, shm_size);

        g_udisk_dir_cfgs[0].key = CFG_MEDIA_DIR_UDISK_MUSIC;
        strncpy(g_udisk_dir_cfgs[0].text, "/MUSIC/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[0].id = DIR_MUSIC;

        g_udisk_dir_cfgs[1].key = CFG_MEDIA_DIR_UDISK_VIDEO; 
        strncpy(g_udisk_dir_cfgs[1].text, "/VIDEO/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[1].id = DIR_VIDEO;

        g_udisk_dir_cfgs[2].key = CFG_MEDIA_DIR_UDISK_PHOTO; 
        strncpy(g_udisk_dir_cfgs[2].text, "/PICTURES/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[2].id = DIR_PHOTO;

        g_udisk_dir_cfgs[3].key = CFG_MEDIA_DIR_UDISK_EBOOK;  
        strncpy(g_udisk_dir_cfgs[3].text, "/EBOOK/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[3].id = DIR_EBOOK;

        g_udisk_dir_cfgs[4].key = CFG_MEDIA_DIR_UDISK_VIDEO_CAMERA;
        strncpy(g_udisk_dir_cfgs[4].text, "/VIDEO/CAMERA/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[4].id = DIR_VIDEO_CAMERA;

        g_udisk_dir_cfgs[5].key = CFG_MEDIA_DIR_UDISK_PHOTO_CAMERA;
        strncpy(g_udisk_dir_cfgs[5].text, "/PICTURES/CAMERA/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[5].id = DIR_PHOTO_CAMERA;

        g_udisk_dir_cfgs[6].key = CFG_MEDIA_DIR_UDISK_GAME;
        strncpy(g_udisk_dir_cfgs[6].text, "/CAME/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[6].id = DIR_GAME;

        g_udisk_dir_cfgs[7].key = CFG_MEDIA_DIR_UDISK_RECORDER;
        strncpy(g_udisk_dir_cfgs[7].text, "/RECORD/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[7].id = DIR_RECORDER;

        g_udisk_dir_cfgs[8].key = CFG_MEDIA_DIR_UDISK_RADIO_RECORD; 
        strncpy(g_udisk_dir_cfgs[8].text, "/RECORD/FM/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[8].id = DIR_RADIO_RECORD;

        g_udisk_dir_cfgs[9].key = CFG_MEDIA_DIR_UDISK_RECORDER_MIC; 
        strncpy(g_udisk_dir_cfgs[9].text, "/RECORD/MIC/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[9].id = DIR_RECORDER_MIC;

        g_udisk_dir_cfgs[10].key = CFG_MEDIA_DIR_UDISK_RECORDER_LINEIN;
        strncpy(g_udisk_dir_cfgs[10].text, "/RECORD/LINEIN/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[10].id = DIR_RECORDER_LINEIN;

        g_udisk_dir_cfgs[11].key = CFG_MEDIA_DIR_UDISK_TVIN; 
        strncpy(g_udisk_dir_cfgs[11].text, "/VIDEO/TVIN/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[11].id = DIR_TVIN;

        g_udisk_dir_cfgs[12].key = CFG_MEDIA_DIR_UDISK_SYSTEM; 
        strncpy(g_udisk_dir_cfgs[12].text, "/SYSTEM/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[12].id = DIR_SYSTEM;

        g_udisk_dir_cfgs[13].key = CFG_MEDIA_DIR_UDISK_SYSTEM_FM;
        strncpy(g_udisk_dir_cfgs[13].text, "/SYSTEM/FM/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[13].id = DIR_SYSTEM_FM;

        g_udisk_dir_cfgs[14].key = CFG_MEDIA_DIR_UDISK_SYSTEM_FONT;
        strncpy(g_udisk_dir_cfgs[14].text, "/SYSTEM/FONTS/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[14].id = DIR_SYSTEM_FONT;

        g_udisk_dir_cfgs[15].key = CFG_MEDIA_DIR_UDISK_SYSTEM_PLAYLIST;
        strncpy(g_udisk_dir_cfgs[15].text, "/SYSTEM/PLAYLIST/", CFG_TEXT_LEN);
        g_udisk_dir_cfgs[15].id = DIR_SYSTEM_PLAYLIST;


        g_card_dir_cfgs[0].key = CFG_MEDIA_DIR_CARD_MUSIC;
        strncpy(g_card_dir_cfgs[0].text, "/MUSIC/", CFG_TEXT_LEN);
        g_card_dir_cfgs[0].id = DIR_MUSIC;

        g_card_dir_cfgs[1].key = CFG_MEDIA_DIR_CARD_VIDEO; 
        strncpy(g_card_dir_cfgs[1].text, "/VIDEO/", CFG_TEXT_LEN);
        g_card_dir_cfgs[1].id = DIR_VIDEO;

        g_card_dir_cfgs[2].key = CFG_MEDIA_DIR_CARD_PHOTO; 
        strncpy(g_card_dir_cfgs[2].text, "/PICTURES/", CFG_TEXT_LEN);
        g_card_dir_cfgs[2].id = DIR_PHOTO;

        g_card_dir_cfgs[3].key = CFG_MEDIA_DIR_CARD_EBOOK;  
        strncpy(g_card_dir_cfgs[3].text, "/EBOOK/", CFG_TEXT_LEN);
        g_card_dir_cfgs[3].id = DIR_EBOOK;

        g_card_dir_cfgs[4].key = CFG_MEDIA_DIR_CARD_VIDEO_CAMERA;
        strncpy(g_card_dir_cfgs[4].text, "/VIDEO/CAMERA/", CFG_TEXT_LEN);
        g_card_dir_cfgs[4].id = DIR_VIDEO_CAMERA;

        g_card_dir_cfgs[5].key = CFG_MEDIA_DIR_CARD_PHOTO_CAMERA;
        strncpy(g_card_dir_cfgs[5].text, "/PICTURES/CAMERA/", CFG_TEXT_LEN);
        g_card_dir_cfgs[5].id = DIR_PHOTO_CAMERA;

        g_card_dir_cfgs[6].key = CFG_MEDIA_DIR_CARD_GAME;
        strncpy(g_card_dir_cfgs[6].text, "/CAME/", CFG_TEXT_LEN);
        g_card_dir_cfgs[6].id = DIR_GAME;

        g_card_dir_cfgs[7].key = CFG_MEDIA_DIR_CARD_RECORDER;
        strncpy(g_card_dir_cfgs[7].text, "/RECORD/", CFG_TEXT_LEN);
        g_card_dir_cfgs[7].id = DIR_RECORDER;

        g_card_dir_cfgs[8].key = CFG_MEDIA_DIR_CARD_RADIO_RECORD; 
        strncpy(g_card_dir_cfgs[8].text, "/RECORD/FM/", CFG_TEXT_LEN);
        g_card_dir_cfgs[8].id = DIR_RADIO_RECORD;

        g_card_dir_cfgs[9].key = CFG_MEDIA_DIR_CARD_RECORDER_MIC; 
        strncpy(g_card_dir_cfgs[9].text, "/RECORD/MIC/", CFG_TEXT_LEN);
        g_card_dir_cfgs[9].id = DIR_RECORDER_MIC;

        g_card_dir_cfgs[10].key = CFG_MEDIA_DIR_CARD_RECORDER_LINEIN;
        strncpy(g_card_dir_cfgs[10].text, "/RECORD/LINEIN/", CFG_TEXT_LEN);
        g_card_dir_cfgs[10].id = DIR_RECORDER_LINEIN;

        g_card_dir_cfgs[11].key = CFG_MEDIA_DIR_CARD_TVIN; 
        strncpy(g_card_dir_cfgs[11].text, "/VIDEO/TVIN/", CFG_TEXT_LEN);
        g_card_dir_cfgs[11].id = DIR_TVIN;

        g_card_dir_cfgs[12].key = CFG_MEDIA_DIR_CARD_SYSTEM; 
        strncpy(g_card_dir_cfgs[12].text, "/SYSTEM/", CFG_TEXT_LEN);
        g_card_dir_cfgs[12].id = DIR_SYSTEM;

        g_card_dir_cfgs[13].key = CFG_MEDIA_DIR_CARD_SYSTEM_FM;
        strncpy(g_card_dir_cfgs[13].text, "/SYSTEM/FM/", CFG_TEXT_LEN);
        g_card_dir_cfgs[13].id = DIR_SYSTEM_FM;

        g_card_dir_cfgs[14].key = CFG_MEDIA_DIR_CARD_SYSTEM_FONT;
        strncpy(g_card_dir_cfgs[14].text, "/SYSTEM/FONTS/", CFG_TEXT_LEN);
        g_card_dir_cfgs[14].id = DIR_SYSTEM_FONT;

        g_card_dir_cfgs[15].key = CFG_MEDIA_DIR_CARD_SYSTEM_PLAYLIST;
        strncpy(g_card_dir_cfgs[15].text, "/SYSTEM/PLAYLIST/", CFG_TEXT_LEN);
        g_card_dir_cfgs[15].id = DIR_SYSTEM_PLAYLIST;


        g_external_dir_cfgs[0].key = CFG_MEDIA_DIR_EXTERNAL_MUSIC;
        strncpy(g_external_dir_cfgs[0].text, "/MUSIC/", CFG_TEXT_LEN);
        g_external_dir_cfgs[0].id = DIR_MUSIC;

        g_external_dir_cfgs[1].key = CFG_MEDIA_DIR_EXTERNAL_VIDEO; 
        strncpy(g_external_dir_cfgs[1].text, "/VIDEO/", CFG_TEXT_LEN);
        g_external_dir_cfgs[1].id = DIR_VIDEO;

        g_external_dir_cfgs[2].key = CFG_MEDIA_DIR_EXTERNAL_PHOTO; 
        strncpy(g_external_dir_cfgs[2].text, "/PICTURES/", CFG_TEXT_LEN);
        g_external_dir_cfgs[2].id = DIR_PHOTO;

        g_external_dir_cfgs[3].key = CFG_MEDIA_DIR_EXTERNAL_EBOOK;  
        strncpy(g_external_dir_cfgs[3].text, "/EBOOK/", CFG_TEXT_LEN);
        g_external_dir_cfgs[3].id = DIR_EBOOK;

        g_external_dir_cfgs[4].key = CFG_MEDIA_DIR_EXTERNAL_VIDEO_CAMERA;
        strncpy(g_external_dir_cfgs[4].text, "/VIDEO/CAMERA/", CFG_TEXT_LEN);
        g_external_dir_cfgs[4].id = DIR_VIDEO_CAMERA;

        g_external_dir_cfgs[5].key = CFG_MEDIA_DIR_EXTERNAL_PHOTO_CAMERA;
        strncpy(g_external_dir_cfgs[5].text, "/PICTURES/CAMERA/", CFG_TEXT_LEN);
        g_external_dir_cfgs[5].id = DIR_PHOTO_CAMERA;

        g_external_dir_cfgs[6].key = CFG_MEDIA_DIR_EXTERNAL_GAME;
        strncpy(g_external_dir_cfgs[6].text, "/CAME/", CFG_TEXT_LEN);
        g_external_dir_cfgs[6].id = DIR_GAME;

        g_external_dir_cfgs[7].key = CFG_MEDIA_DIR_EXTERNAL_RECORDER;
        strncpy(g_external_dir_cfgs[7].text, "/RECORD/", CFG_TEXT_LEN);
        g_external_dir_cfgs[7].id = DIR_RECORDER;

        g_external_dir_cfgs[8].key = CFG_MEDIA_DIR_EXTERNAL_RADIO_RECORD; 
        strncpy(g_external_dir_cfgs[8].text, "/RECORD/FM/", CFG_TEXT_LEN);
        g_external_dir_cfgs[8].id = DIR_RADIO_RECORD;

        g_external_dir_cfgs[9].key = CFG_MEDIA_DIR_EXTERNAL_RECORDER_MIC; 
        strncpy(g_external_dir_cfgs[9].text, "/RECORD/MIC/", CFG_TEXT_LEN);
        g_external_dir_cfgs[9].id = DIR_RECORDER_MIC;

        g_external_dir_cfgs[10].key = CFG_MEDIA_DIR_EXTERNAL_RECORDER_LINEIN;
        strncpy(g_external_dir_cfgs[10].text, "/RECORD/LINEIN/", CFG_TEXT_LEN);
        g_external_dir_cfgs[10].id = DIR_RECORDER_LINEIN;

        g_external_dir_cfgs[11].key = CFG_MEDIA_DIR_EXTERNAL_TVIN; 
        strncpy(g_external_dir_cfgs[11].text, "/VIDEO/TVIN/", CFG_TEXT_LEN);
        g_external_dir_cfgs[11].id = DIR_TVIN;

        g_external_dir_cfgs[12].key = CFG_MEDIA_DIR_EXTERNAL_SYSTEM; 
        strncpy(g_external_dir_cfgs[12].text, "/SYSTEM/", CFG_TEXT_LEN);
        g_external_dir_cfgs[12].id = DIR_SYSTEM;

        g_external_dir_cfgs[13].key = CFG_MEDIA_DIR_EXTERNAL_SYSTEM_FM;
        strncpy(g_external_dir_cfgs[13].text, "/SYSTEM/FM/", CFG_TEXT_LEN);
        g_external_dir_cfgs[13].id = DIR_SYSTEM_FM;

        g_external_dir_cfgs[14].key = CFG_MEDIA_DIR_EXTERNAL_SYSTEM_FONT;
        strncpy(g_external_dir_cfgs[14].text, "/SYSTEM/FONTS/", CFG_TEXT_LEN);
        g_external_dir_cfgs[14].id = DIR_SYSTEM_FONT;

        g_external_dir_cfgs[15].key = CFG_MEDIA_DIR_EXTERNAL_SYSTEM_PLAYLIST;
        strncpy(g_external_dir_cfgs[15].text, "/SYSTEM/PLAYLIST/", CFG_TEXT_LEN);
        g_external_dir_cfgs[15].id = DIR_SYSTEM_PLAYLIST;

    }


    s_shm_fd = fd ;
    s_initialized = true;

    return true;
}

bool applib_dir_cfg_deinit( void )
{
    if(false == s_initialized)
    {
        print_warning("app running list has not s_initialized!");
        return false;
    }

    if(s_shm_fd > 0)
    {
        close(s_shm_fd);
        s_shm_fd = -1;
    }

    g_udisk_dir_cfgs = NULL; 
    g_card_dir_cfgs = NULL; 
    g_external_dir_cfgs = NULL; 
    g_dir_all = NULL; 

    s_initialized = false;

    return true;
}

bool applib_dir_cfg_read(dir_cfg_t * cfg, const char * prefix, bool force)
{
    char text[CFG_TEXT_LEN] = {0};

    if(NULL == cfg)
    {
        print_err("input parmeter(cfg) is a NULL pointer!");
        return false;
    }

    if(is_inited() == false)
    {
        return false;
    }

    if((force == false)
            && (cfg->is_read == true))
    {
        return true;
    }

    if(get_config(cfg->key, text, CFG_TEXT_LEN) < 0)
    {
        print_err("get_config(%s) failed! write default value to it!", cfg->key);
        strncpy(text, cfg->text, CFG_TEXT_LEN);
        if(set_config(cfg->key, cfg->text, (int)(strlen(cfg->text)+1)) < 0)
        {
            print_err("set_config(%s) failed!", cfg->key);
        }
    }

    os_sched_lock();
    if(prefix != NULL)
    {
        strncpy(cfg->text, prefix, CFG_TEXT_LEN);
    }

    strncat(cfg->text, text, CFG_TEXT_LEN);

    cfg->is_read = true;

    os_sched_unlock();

    return true;
}

bool applib_dir_cfg_read_all(dir_cfg_t cfgs[], int count, const char * prefix, bool force)
{
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    if(is_inited() == false)
    {
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(applib_dir_cfg_read(&cfgs[i], prefix, force) == false)
        {
            return false;
        }
    }

    return true;
}

dir_cfg_t * applib_dir_cfg_find(dir_cfg_t cfgs[], int count, int id)
{
    int i;

    if(NULL == cfgs)
    {
        print_err("input parmeter(cfgs) is a NULL pointer!");
        return false;
    }

    if(is_inited() == false)
    {
        return false;
    }

    for(i = 0; i < count; i++)
    {

        if(cfgs[i].id == id)
        {
            return &cfgs[i];
        }
    }

    print_err("connot find a dir config entry by %d", id);
    return NULL;
}

static inline bool is_inited(void)
{
    if(!s_initialized)
    {
        print_warning("applib dir config has not s_initialized!");
        return false;
    }

    return true;
}
