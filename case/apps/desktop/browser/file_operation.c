/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : file_operation.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/
#include "include_case.h"
#include "include_psp.h"
#include "act_mem.h"
#include "file_operation.h"
#include "browser_scene_browsing.h"
#include "browser_browsing_widget_filelist.h"

#define APP_TYPE_SUM 5

#define BROWSER_COPY_BUFFER_SIZE (64*1024)

static void _load_config(void);

static char s_music_ext[CFG_MAX_BUFF_LENGTH];
static char s_video_ext[CFG_MAX_BUFF_LENGTH];
static char s_photo_ext[CFG_MAX_BUFF_LENGTH];
static char s_ebook_ext[CFG_MAX_BUFF_LENGTH];
static char s_emulator_ext[CFG_MAX_BUFF_LENGTH];



app_ext_t s_map[APP_TYPE_SUM] =
{
    { TYPE_MUSIC, APP_NAME_MUSIC,s_music_ext, MUSIC_ARGV_PLAYING},
    { TYPE_VIDEO, APP_NAME_VIDEO,s_video_ext,VIDEO_ARGV_PLAYING },
    { TYPE_PHOTO, APP_NAME_PHOTO, s_photo_ext,PHOTO_ARGV_PLAYING },
    { TYPE_TXT, APP_NAME_EBOOK, s_ebook_ext,EBOOK_ARGV_PLAYING },
    { TYPE_EMULATOR, APP_NAME_EMULATOR, s_emulator_ext,EMULATOR_ARGV_PLAYING },
};

/*
 *  we set the file_op_errno to the value the system call returns when failed. Afterward, we can
 *  use the file_op_errno to indicator the exact cause of the failure.
 */
int g_file_op_errno = 0;

/*
 * when we in search mode & the multiply files operation mode, we only update the fsel db once after
 * the whole transaction is complete.
 * here we use the update_fsel flag to indicate this situation.
 */
bool g_update_fsel = true;
/********************************************************************************/

bool is_file_exist(const char *file_path)
{
    int ret = -1;
    struct stat s;
    ret = stat(file_path, &s);
    if(ret == 0)
    {
        return S_ISREG(s.st_mode);
    }
    else
    {
        return false;
    }
}

bool is_dir_exist(const char *dir_path)
{
    int ret = -1;
    struct stat s;
    ret = stat(dir_path, &s);
    if(ret == 0)
    {
        return S_ISDIR(s.st_mode);
    }
    else
    {
        return false;
    }
}

bool is_sub_dir(const char* parent, const char* sub)
{
    bool ret;
    char rel_parent[PATH_MAX];
    char rel_sub[PATH_MAX];
    size_t parent_len;
    size_t rel_parent_len;
    size_t sub_len;
    size_t rel_sub_len;

    if ((parent == NULL) || (sub == NULL))
    {
        return false;
    }

    if ((parent[0] != '/') || (sub[0] != '/'))
    {
        return false;
    }

    memset(rel_parent, 0, PATH_MAX);
    memset(rel_sub, 0, PATH_MAX);

    strncpy(rel_parent, parent, PATH_MAX);
    parent_len = strlen(parent);
    rel_parent_len = parent_len;

    strncpy(rel_sub, sub, PATH_MAX);
    sub_len = strlen(sub);
    rel_sub_len = sub_len;

    while (rel_parent[rel_parent_len - 1] == '/')
    {
        rel_parent[rel_parent_len - 1] = '\0';
        rel_parent_len--;
    }

    while (rel_sub[rel_sub_len - 1] == '/')
    {
        rel_sub[rel_sub_len - 1] = '\0';
        rel_sub_len--;
    }

    if (strncmp(rel_parent, rel_sub, rel_parent_len) != 0)
    {
        ret = false;
    }
    else
    {
        if (rel_sub[rel_parent_len] == '/')
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    return ret;
}

bool in_same_disk(const char* src, const char* dest)
{
    if (strstr(src, UDISK_ROOT) != NULL)
    {
        if (strstr(dest, UDISK_ROOT) != NULL)
        {
            return true;
        }
    }
    else if (strstr(src, CARD_ROOT) != NULL)
    {
        if (strstr(dest, CARD_ROOT) != NULL)
        {
            return true;
        }
    }
    else if (strstr(src, EXTERNAL_ROOT) != NULL)
    {
        if (strstr(dest, EXTERNAL_ROOT) != NULL)
        {
            return true;
        }
    }
    else
    {
        print_err();
    }
    return false;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    copy the source directory src to destionation dest.
 * \param[in]    src    the source directory
 * \param[in]    dest the destination directory
 * \return       the result
 * \retval           0 success
 * \retval           -1 failed
 * \retval           -2 failed with critical err
 * \ingroup      file_operation
 * \note    when copy_dir encounters a normal err such as can't close a dir, or can't mkdir, just return -1;
 *              if  encounters a cirtial err such as the dest is the subdir of the src, return -2. The outer
 *              caller can use the two return value to determine whether to stop the whole operation or just ignore
 *              this err and go on the next operation.
*******************************************************************************/
int copy_dir(const char *src, const char *dest)
{
    int r;
    DIR *dir;
    struct dirent *dentry;

    if (strcmp(src, dest) == 0)
    {
        return 0;
    }

    if (is_sub_dir(src, dest) == true)
    {
/*
        dialog_init_t t = { DIALOG_HEAD_FAIL, g_scene_browsing->str_subdir_err->str_id };
        com_create_modal_dialog(&t);
     */
        g_sem_modal.modal.head = DIALOG_HEAD_FAIL;
        g_sem_modal.modal.str_id = g_scene_browsing->str_subdir_err->str_id;
        set_single_shot_timer(1,(timer_proc)com_create_modal_dialog_widget,NULL);
        sem_wait(&g_sem_modal.sem);
        return -2;
    }

    if (is_dir_exist(dest) == true)
    {
        if ( (g_scene_browsing->filelist->replace_yes) == true)
        {
            remove_dir_recursive(dest);
        }
        else
        {
/*
            dialog_init_t t = { DIALOG_HEAD_QUERY, g_scene_browsing->str_replace_comfirm->str_id };
            widget_sig_dialog_e ret = com_create_modal_dialog(&t);
*/
            g_sem_modal.modal.head = DIALOG_HEAD_QUERY;
            g_sem_modal.modal.str_id = g_scene_browsing->str_replace_comfirm->str_id;
            g_sem_modal.modal.def_sel=WIDGET_SIG_DIALOG_CANCAL;
            set_single_shot_timer(1,(timer_proc)com_create_modal_dialog_widget,NULL);
            sem_wait(&g_sem_modal.sem);

            if (g_sem_modal.ret == WIDGET_SIG_DIALOG_OK)
            {
                g_scene_browsing->filelist->replace_yes = true;
                remove_dir_recursive(dest);
            }
            else
            {
                return -2;
            }
        }
    }

    dir = opendir(src);
    if (dir == NULL)
    {
        print_err("failed to open directory: %s\n", src);
        return -1;
    }

    if (mkdir(dest, S_IRWXU | S_IRWXG | S_IRWXO) < 0)
    {
        print_err("failed to mkdir %s.",dest);
        closedir(dir);
        return -1;
    }

    //fsel_update((char *) dest, FSEL_ADDDATA, FSEL_DIRNAME);
    if(g_update_fsel == true)
    {
        fsel_update(g_fsel_handle,(char*)dest,FSEL_ADDDATA);
    }

    while (1)
    {
        char *old_file, *new_file;
        size_t len_src;
        size_t len_dst;
        size_t len_dname;

        dentry = readdir(dir);
        if(dentry == NULL)
        {
            break;
        }

        if ((strcmp(dentry->d_name, ".") == 0 )||( strcmp(dentry->d_name, "..") == 0))
        {
            continue;
        }

        len_src = strlen(src);
        len_dst = strlen(dest);
        len_dname = strlen(dentry->d_name);

        old_file = (char *) malloc(len_src + len_dname + 1 + 1);
        if(old_file == NULL)
        {
            print_err();
            closedir(dir);
            return -1;
        }
        memset(old_file, 0, len_src + len_dname + 2);
        strncpy(old_file, src, len_src);
        strncat(old_file, "/", 1);
        strncat(old_file, dentry->d_name, len_dname);

        new_file = (char *) malloc(len_dst + len_dname + 1 + 1);
        if(new_file == NULL)
        {
            print_err();
            free(old_file);
            closedir(dir);
            return -1;
        }
        memset(new_file, 0, len_dst + len_dname + 2);
        strncpy(new_file, dest, len_dst);
        strncat(new_file, "/", 1);
        strncat(new_file, dentry->d_name, len_dname);

        print_dbg("copying: %s -> %s\n", old_file, new_file);

        if ((dentry->d_attrs & ATTR_DIRECTORY) != 0)
        {
            r = copy_dir(old_file, new_file);
            free(old_file);
            free(new_file);
            if (r < 0)
            {
                closedir(dir);
                return r;
            }
        }
        else
        {
            r = copy_file(old_file, new_file);
            free(old_file);
            free(new_file);
            if (r < 0)
            {
                closedir(dir);
                return r;
            }
        }
    }
    closedir(dir);
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    copy the source file src to destionation dest.
 * \param[in]    src    the source file
 * \param[in]    dest the destination file
 * \return       the result
 * \retval           0 success
 * \retval           -1 failed
 * \retval           -2 failed with critical err
 * \ingroup      file_operation
 * \note    when copy_file encounters a normal err such as can't write to the dest file, just return -1;
 *              if  encounters a cirtial err such as dlopening the act_mem failed, return -2. The outer
 *              caller can use the two return value to determine whether to stop the whole operation or just ignore
 *              this err and go on the next operation.
*******************************************************************************/
int copy_file(const char *src, const char *dest)
{
    int result = 0;
    int src_fp, dest_fp;
    int n_read;
    char *buf;
    void *act_mem_lib;

    if(g_msg_lowpower_received == true)
    {
        return -2;
    }

    if (strcmp(src, dest) == 0)
    {
        return 0;
    }
    if (is_file_exist(dest) == true)
    {
        if (( g_scene_browsing->filelist->replace_yes) == true)
        {
            remove_file(dest);
        }
        else
        {
 /*
            dialog_init_t t = { DIALOG_HEAD_QUERY, g_scene_browsing->str_replace_comfirm->str_id };
            widget_sig_dialog_e ret = com_create_modal_dialog(&t);
*/
            g_sem_modal.modal.head = DIALOG_HEAD_QUERY;
            g_sem_modal.modal.str_id = g_scene_browsing->str_replace_comfirm->str_id;
            g_sem_modal.modal.def_sel=WIDGET_SIG_DIALOG_CANCAL;
            set_single_shot_timer(1,(timer_proc)com_create_modal_dialog_widget,NULL);
            sem_wait(&g_sem_modal.sem);


            if (g_sem_modal.ret == WIDGET_SIG_DIALOG_OK)
            {
                g_scene_browsing->filelist->replace_yes = true;
                remove_file(dest);
            }
            else
            {
                return -2;
            }
        }
    }
    act_mem_lib = dlopen("act_mem.so", RTLD_NOW);
    if( act_mem_lib == NULL )
    {
        print_err("dlopen act_mem.so failed.");
        return -2;
    }
    
    buf = act_mem_allocate(BROWSER_COPY_BUFFER_SIZE, MEM_CONTINUOUS | WRITE_THROUGH_MEM);
    if( buf == NULL )
    {
        print_err("act_mem_allocate failed.");
        dlclose(act_mem_lib);
        return -2;
    }

    src_fp = open(src, O_RDONLY);
    if (src_fp < 0)
    {
        print_err("open %s failed.",src);
        act_mem_free(buf);
        dlclose(act_mem_lib);
        return -1;
    }
    fcntl(src_fp,F_BUILDINDEX,0);

    dest_fp = open(dest, O_WRONLY|O_CREAT);
    if (dest_fp < 0)
    {
        print_err("open %s failed.",dest);
        act_mem_free(buf);
        dlclose(act_mem_lib);
        close(src_fp);

        return -1;
    }
    fcntl(dest_fp,F_BUILDINDEX,0);

    while (1)
    {
        n_read = read(src_fp, buf, BROWSER_COPY_BUFFER_SIZE);

        if(n_read == 0)
        {
            result = 0;
            break;
        }

        if(n_read == -1)
        {
            g_file_op_errno = errno;
            result = -1;
            print_err();
            break;
        }

        assert(n_read <= BROWSER_COPY_BUFFER_SIZE);
        if( (write(dest_fp, buf, (unsigned int)n_read) < n_read) ||( g_msg_lowpower_received == true))
        {
            g_file_op_errno = errno;
            result =  -1;
            print_err();
            break;
        }
    }

    act_mem_free(buf);
    dlclose(act_mem_lib);
    close(src_fp);

    if(close(dest_fp) == -1)
    {
        print_err();
        result = -1;
    }

    if(result == 0)
    {
        if(g_update_fsel == true)
        {
            fsel_update(g_fsel_handle,(char*)dest,FSEL_ADDDATA);
        }
    }
    else // result == -1
    {
        print_err();
        remove(dest);
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    move the source directory src to destionation dest.
 * \param[in]    src    the source directory
 * \param[in]    dest the destination directory
 * \return       the result
 * \retval           0 success
 * \retval           -1 failed
 * \retval           -2 failed with critical err
 * \ingroup      file_operation
*******************************************************************************/
int move_dir(const char *src, const char *dest)
{
    if (strcmp(src, dest) == 0)
    {
        return 0;
    }
    if (copy_dir(src, dest) == 0)
    {
        return remove_dir_recursive(src);
    }
    else
    {
        return -1;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    move the source file src to destionation dest.
 * \param[in]    src    the source file
 * \param[in]    dest the destination file
 * \return       the result
 * \retval           0 success
 * \retval           -1 failed
 * \retval           -2 failed with critical err
 * \ingroup      file_operation
*******************************************************************************/
int move_file(const char *src, const char *dest)
{
    int ret;

    if (strcmp(src, dest) == 0)
    {
        return 0;
    }

    if(g_msg_lowpower_received == true)
    {
        return -2;
    }

    if (is_file_exist(dest) == true)
    {
        if ((g_scene_browsing->filelist->replace_yes) == true)
        {
            ret = remove_file(dest);
            if(ret < 0)
            {
                return ret;
            }
        }
        else
        {
  /*
            dialog_init_t t = { DIALOG_HEAD_QUERY, g_scene_browsing->str_replace_comfirm->str_id };
            widget_sig_dialog_e ret = com_create_modal_dialog(&t);
*/
            g_sem_modal.modal.head = DIALOG_HEAD_QUERY;
            g_sem_modal.modal.str_id = g_scene_browsing->str_replace_comfirm->str_id;
            g_sem_modal.modal.def_sel=WIDGET_SIG_DIALOG_CANCAL;
            set_single_shot_timer(1,(timer_proc)com_create_modal_dialog_widget,NULL);
            sem_wait(&g_sem_modal.sem);

            if (g_sem_modal.ret == WIDGET_SIG_DIALOG_OK)
            {
                g_scene_browsing->filelist->replace_yes = true;
                ret = remove_file(dest);
                if(ret < 0)
                {
                    return ret;
                }
            }
            else
            {
                return -2;
            }
        }
    }

    if (rename(src, dest) == 0)
    {
        if(g_update_fsel == true)
        {
            fsel_update(g_fsel_handle,(char*)dest,FSEL_ADDDATA);
            fsel_update(g_fsel_handle,(char*)src,FSEL_DELDATA);
        }
        return 0;
    }

    ret = copy_file(src, dest);
    if (ret  < 0)
    {
        return ret;
    }

    ret = remove(src);
    if(ret == 0)
    {
        if (g_update_fsel == true)
        {
            fsel_update(g_fsel_handle, (char*) src, FSEL_DELDATA);
        }
    }
    else
    {
        remove_file(dest);
    }
    return ret;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    remove the file.
 * \param[in]    file    the file to be removed
 * \return       the result
 * \retval           0 success
 * \retval           -1 failed
 * \retval           -2 failed with critical err
 * \ingroup      file_operation
*******************************************************************************/
int remove_file(const char *file)
{
    if(g_msg_lowpower_received == true)
    {
        return -2;
    }

    if (remove(file) == 0)
    {
        if(g_update_fsel == true)
        {
            fsel_update(g_fsel_handle,(char*)file,FSEL_DELDATA);
        }
        return 0;
    }
    else
    {
        print_err(" remove(%s) failed.",file);
        return -1;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    remove the directory recursively.
 * \param[in]    dir    the dir to be removed
 * \return       the result
 * \retval           0 success
 * \retval           -1 failed
 * \retval           -2 failed with critical err
 * \ingroup      file_operation
*******************************************************************************/
int remove_dir_recursive(const char *dir)
{
    int result =0;
    DIR *dp;
    struct dirent *dentry;

    dp = opendir(dir);
    if (dp == NULL)
    {
        print_err("failed to open directory: %s\n", dir);
        return -1;
    }

    /* remove all files in the directory */
    while (1)
    {
        size_t len_cwddir;
        size_t len_dname;
        char *entry_name;

        dentry = readdir(dp);
        if(dentry == NULL)
        {
            if(errno == EBADF)
            {
                result = -1;
            }
            break;
        }

        if(g_msg_lowpower_received == true)
        {
            result = -2;
            break;
        }

        if ((strcmp(dentry->d_name, ".") == 0 )|| (strcmp(dentry->d_name, "..") == 0 ))
        {
            continue;
        }

        len_cwddir = strlen(dir);
        len_dname = strlen(dentry->d_name);
        entry_name = (char *) malloc(len_cwddir + len_dname + 1 + 1);
        if(entry_name == NULL)
        {
            print_err();
            result = -1;
            break;
        }

        memset(entry_name, 0, len_cwddir + len_dname + 2);
        strncpy(entry_name, dir, len_cwddir);
        strncat(entry_name, "/", 1);
        strncat(entry_name, dentry->d_name, len_dname);
        if ((dentry->d_attrs & ATTR_DIRECTORY) != 0)
        {
            result =remove_dir_recursive(entry_name);
            free(entry_name);
            if (result  < 0)
            {
                print_err("can't remove directory[%s]\n", dentry->d_name);
                break;
            }
        }
        else
        {
            result = remove_file(entry_name);
            if (result == 0)
            {
                if (g_update_fsel == true)
                {
                    fsel_update(g_fsel_handle, (char*) entry_name, FSEL_DELDATA);
                }
            }
            else
            {
                print_err("can't remove file[%s]\n", entry_name);
                free(entry_name);
                break;
            }
            free(entry_name);
        }
    }

    closedir(dp);

    if(result == 0)
    {
        result = rmdir(dir);

        if (result == 0)
        {
            if (g_update_fsel == true)
            {
                fsel_update(g_fsel_handle, (char*) dir, FSEL_DELDATA);
            }
        }
        else
        {
            print_err("can't remove directory[%s]\n", dir);
        }
    }
    return result;
}

app_ext_t *find_open_app(const char *file_name)
{
    int i;
    int idx =APP_TYPE_SUM;
    char *ext = strrchr(file_name, '.');

    if(s_music_ext[0] !='|')
    {
        _load_config();
    }

    if (ext != NULL)
    {
        size_t ext_len;
        char* str;
        char* p_str;
        ext++;
        //char* str = strdup(ext);

        ext_len = strlen(ext);
        str = (char*)malloc(ext_len + 3);
        if(str == NULL)
        {
            print_err();
            return NULL;
        }

        memset(str,0,ext_len+3);
        strcpy(str,"|");
        strcat(str,ext);
        strcat(str,"|");

        p_str = str;
        for(; *p_str !='\0'; p_str++)
        {
            if(isalpha(*p_str) != 0 )
            {
                *p_str = (char)tolower(*p_str);
            }
        }

        for (i = 0; i < APP_TYPE_SUM; i++)
        {
            if (strstr(s_map[i].ext, str) != NULL)
            {
                idx = i;
            }
        }
        free(str);
    }

    if(idx < APP_TYPE_SUM)
    {
        return &s_map[idx];
    }
    else
    {
        return NULL;
    }
}

file_type_e find_file_type(const char *file_name)
{
    int i;
    int idx =APP_TYPE_SUM;
    char *ext = strrchr(file_name, '.');

    if(s_music_ext[0] !='|')
    {
        _load_config();
    }

    if (ext != NULL)
    {
        size_t ext_len;
        char* str;
        char* p_str;

        ext++;
        //char* str = strdup(ext);

        ext_len = strlen(ext);
        str = (char*)malloc(ext_len + 3);
        if(str == NULL)
        {
            print_err();
            return TYPE_DEFAULT;
        }
        memset(str,0,ext_len+3);
        strcpy(str,"|");
        strcat(str,ext);
        strcat(str,"|");

        p_str = str;
        for(; *p_str !='\0'; p_str++)
        {
            if(isalpha(*p_str) != 0 )
            {
                *p_str = (char)tolower(*p_str);
            }
        }


        for (i = 0; i < APP_TYPE_SUM; i++)
        {
            if (strstr(s_map[i].ext, str) != NULL)
            {
                idx = i;
            }
        }
        free(str);
    }

    if(idx < APP_TYPE_SUM)
    {
        return s_map[idx].app_type;
    }
    else
    {
        return TYPE_DEFAULT;
    }
}


static void _load_config(void)
{
    int ret;
    int swf_ret;
    char text[CFG_MAX_BUFF_LENGTH];

    /*
     * load music file extension.
     */
    s_music_ext[0] ='|';
    ret = get_config(CFG_APP_SUPPORT_EXT_MUSIC, s_music_ext + 1, CFG_MAX_BUFF_LENGTH -2);
    if (ret== -1)
    {
        print_err();
    }
    strncat(s_music_ext,"|",1);

    /*
     * load video file extension.
     */

    memset(text, 0, CFG_MAX_BUFF_LENGTH);
    swf_ret = get_config(CFG_APP_SUPPORT_EXT_VIDEO_SWF, text, CFG_MAX_BUFF_LENGTH);

    if(swf_ret == -1)
    {
        swf_ret = 0;
    }

    s_video_ext[0]='|';
    ret = get_config(CFG_APP_SUPPORT_EXT_VIDEO, s_video_ext + 1, CFG_MAX_BUFF_LENGTH - swf_ret -2);
    if (ret == -1)
    {
        print_err();
    }
    strncat(s_video_ext, "|", 1);

    strncat(s_video_ext, text, (size_t)swf_ret);

    strncat(s_video_ext,"|",1);

    /*
     * load photo file extension.
     */
    s_photo_ext[0] ='|';
    ret = get_config(CFG_APP_SUPPORT_EXT_PHOTO, s_photo_ext + 1, CFG_MAX_BUFF_LENGTH -2);
    if (ret== -1)
    {
        print_err();
    }

    strncat(s_photo_ext,"|",1);

    /*
     * load ebook file extension.
     */
    s_ebook_ext[0] ='|';
    ret = get_config(CFG_APP_SUPPORT_EXT_EBOOK, s_ebook_ext + 1, CFG_MAX_BUFF_LENGTH -2);
    if (ret== -1)
    {
        print_err();
    }

    strncat(s_ebook_ext,"|",1);

    /*
     * load emulator file extension.
     */
    s_emulator_ext[0] ='|';
    ret = get_config(CFG_APP_SUPPORT_EXT_EMULATOR, s_emulator_ext + 1, CFG_MAX_BUFF_LENGTH -2);
    if (ret== -1)
    {
        print_err();
    }

    strncat(s_emulator_ext,"|",1);
}
