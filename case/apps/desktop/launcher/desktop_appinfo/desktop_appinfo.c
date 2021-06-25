/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : app_desktop_info.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       herb hu    2009-0-23          v1.0               build this file
 ********************************************************************************/

#include <include_case.h>
#include "./../launcher.h"
#include "./../desktop_appinfo.h"

#define MAX_ITEM_NUM 50
static char * g_applications[MAX_ITEM_NUM];

/* by ccm, actually, the "DesktopPath" key value will be got it in config.txt */
//#define DESKTOP_PATH "/mnt/diska/desktop/"
//#define THIRD_PARTY_PATH "/mnt/disk0/3rdParty/"
/*desktop file name,actually the Applications key value will be got it in config.txt*/
//char * Applications= "music;video;tvin;photo;camera;recorder;radio;ebook;games;tools;browser;setting;test_comm;usb";
#define CFG_LENGTH 512
static char Applications[CFG_LENGTH] =
{ 0 };

//define .desktop key string
static char desktop_key[DKEY_COUNT][20] =
{ "Comment", "Path", "Type", "Child", "Exec", "Name", "Name[xxxxx]", "Icon", "Icon_id" };

const char * none_value = "None_Value";

//base current language to fill 'Name[xxxxx]' KEY in  desktop_key[DKEY_NAME_LANG].
static void _init_name_lang_key(void)
{
    const char *cur_codes;

    //get codes string of the current language. ex. the America English codes is 'en_US'
    cur_codes = sys_get_cur_language_codes();

    //ex. current is America English, so the KEY is 'Name' + '[en_US]'.
    snprintf(desktop_key[DKEY_NAME_LANG], sizeof(desktop_key[DKEY_NAME_LANG]), "%s[%s]", desktop_key[DKEY_NAME],
            cur_codes);

    print_dbg("%s", desktop_key[DKEY_NAME_LANG]);
}

static inline int _parse_item_from_string(char * string, char * item_array[], char symbol)
{
    char * begine = NULL;
    char * end = NULL;
    int i = 0;
    int num = 0;
    bool isfinish = false;

    while ((*string != '\0') && (i < MAX_ITEM_NUM))
    {
        SKIP_SPACE(string)
        begine = string;
        while ((*string != symbol) && (*string != '\0'))
        {
            string++;
        }
        if (*string == '\0')
        {
            isfinish = true;
        }

        end = string - 1; //ignore symbol or '\0'
        SKIP_BACK_SPACE(begine,end)
        if (end > begine)
        {
            *(end + 1) = '\0'; //change the symbol to '\0'
            item_array[num] = begine;
            print_dbg("No.%d applications = %s", i, item_array[num]);
            num++;
        }

        if (isfinish == true) //parse end of the string

        {
            break;
        }
        string++; //jump over the symbol
        i++;
    }
    return num;
}

static appinfo_list_t* _parse_desktop_file(char * filename[], int file_num)
{
    appinfo_list_t * head = NULL;
    appinfo_list_t * current = NULL;
    desktop_parser_handle dp = NULL;
    char * tempstr = NULL;
    char * child_desktop_filename[MAX_ITEM_NUM];
    char desktop_filename[FULL_FILENAME_LEN] =
    { '\0' };
    char desktop_root_path[FULL_FILENAME_LEN] =
    { '\0' };
    int i, j = 0x00;
    bool existed_file = true;
    int app_no = 0;
    struct stat stat_buf;
    appinfo_list_t * new;

    if (get_config(CFG_DESKTOP_PATH, desktop_root_path, CFG_LENGTH) == -1)
    {
        print_err("get DESKTOP_PATH key error!");
        strncpy(desktop_root_path, "ADSISK_ROOT/apps", FULL_FILENAME_LEN);
    }

    for (i = 0; i < file_num; i++)
    {
        //make the application desktop filename = file path + application name + Suffix('.desktop')

        snprintf(desktop_filename, sizeof(desktop_filename), "%s%s/%s%s", desktop_root_path, filename[i], filename[i],
                FILE_SUFFIX);

        //detect if the file have exited in that path.
        existed_file = true;
        if ((stat(desktop_filename, &stat_buf) < 0) || (S_ISDIR(stat_buf.st_mode)))
        {
            //CFG_DESKTOP_PATH/'filename'.desktop
            snprintf(desktop_filename, sizeof(desktop_filename), "%s%s%s", desktop_root_path, filename[i], FILE_SUFFIX);

            if ((stat(desktop_filename, &stat_buf) < 0) || (S_ISDIR(stat_buf.st_mode)))
            {
                print_err("can not find %s file!",desktop_filename);
                existed_file = false;
            }
        }

        if (existed_file == true)
        {
            print_dbg("%s", desktop_filename);
            dp = desktop_parser_create((char *) desktop_filename);
            if (NULL == dp)
            {
                continue;
            }

            new = (appinfo_list_t *) malloc(sizeof(appinfo_list_t));
            if (new == NULL)
            {
                print_err("Cann't malloc %d Bytes!", sizeof(appinfo_list_t));
            }
            else
            {
                new->prev = new->next = new->parent = new->child = NULL;
                if (head == NULL)
                {
                    head = new;
                    current = head;
                }
                else
                {
                    current->next = new;
                    new->prev = current;
                    current = new;
                }

                current->filename = strdup(desktop_filename);
                current->app_index = app_no;
                app_no++;
                for (j = 0; j < DKEY_COUNT; j++)
                {
                    tempstr = desktop_parser_get_key(dp, (char *) desktop_key[j % DKEY_COUNT]);

                    current->value[j] = (char *) none_value;
                    if ((tempstr != NULL) && (*tempstr != '\0'))
                    {
                        current->value[j] = strdup(tempstr);
                    }
                }
                if (!strcasecmp(current->value[DKEY_TYPE], DESKTOP_TYPE_DIR)) //same as : if (Type==directory)
                {
                    int num = _parse_item_from_string(current->value[DKEY_CHILD], child_desktop_filename, ';');
                    current->child = _parse_desktop_file(child_desktop_filename, num);
                    if (NULL != current->child)
                    {
                        current->child->parent = current;
                    }
                }
            }
            if (desktop_parser_delete(dp) == false)
            {
                print_err("%s", desktop_filename);
            }
        }
    }
    //make cycle list(include the child list)
    if ((NULL != current) && (NULL != head))
    {
        current->next = head;
        head->prev = current;
    }
    else
    {
        return NULL;
    }

    return head;
}

static bool _update_desktop_key(appinfo_list_t* appinfo_list, desktop_key_e key)
{
    desktop_parser_handle dp = NULL;
    char * tempstr = NULL;

    appinfo_list_t * head = appinfo_list;
    appinfo_list_t * p = head;

    do
    {
        dp = desktop_parser_create(p->filename);
        if (NULL != dp)
        {
            tempstr = desktop_parser_get_key(dp, (char *) desktop_key[key % DKEY_COUNT]);

            if ((p->value[key % DKEY_COUNT] != NULL) && (p->value[key % DKEY_COUNT] != none_value))
            {
                MY_FREE(p->value[key % DKEY_COUNT])
            }

            if ((tempstr != NULL) && (*tempstr != '\0'))
            {
                p->value[key % DKEY_COUNT] = strdup(tempstr);
            }
            // 注释掉该部分，若找不到也不以 name来补充，而是填none_value，让问题暴露出来。
            else if (DKEY_NAME_LANG == key) //若找不到name[lang],则以name补充。
            {
                print_err("can not get the '%s' key!", desktop_key[key]);
                p->value[DKEY_NAME_LANG] = (char *) p->value[DKEY_NAME];
            }

            else
            {
                p->value[key % DKEY_COUNT] = (char *) none_value;
            }

            if (desktop_parser_delete(dp) == false)
            {
                print_err("%s", p->filename);
            }
        }
        else
        {
            return false;
        }

        if (p->child != NULL) //same as : if (Type==directory)

        {
            _update_desktop_key(p->child, key);
        }

        p = p->next;

    } while (p != head);

    return true;
}

appinfo_list_t * appinfo_list_create(int appid)
{
    appinfo_list_t* head = NULL;
    appinfo_list_t* p = NULL;

    int apps_num = 0x00;

    // actually, the "Applications" key value will be got it in config.txt
    // char * Applications = config_get("Applications");
    if (get_config(CFG_APPLICATIONS, Applications, CFG_LENGTH) == -1)
    {
        print_err("get Applications key error(%d)!",errno);
    }
    else
    {
        //initial the Name[xxxxx] KEY in desktop_key[] base current language.
        _init_name_lang_key();

        apps_num = _parse_item_from_string(Applications, g_applications, ';');
        head = _parse_desktop_file(g_applications, apps_num);

    }

    // find the 'appid' app and return the app node of the list.
    if (head != NULL)
    {
        p = head;
        do
        {
            if (p->app_index == appid)
            {
                return p;
            }
            else
            {
                p = p->next;
            }
        } while (p != head);
    }

    return head;
}

//update appinfo_list, same as: create again
bool appinfo_list_update_name(appinfo_list_t * appinfo_list)
{
    _init_name_lang_key();
    if (true == _update_desktop_key(appinfo_list, DKEY_NAME_LANG))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool appinfo_list_delete(appinfo_list_t * appinfo_list)
{
    bool ret = false;
    int i = 0;
    appinfo_list_t * head = appinfo_list;
    appinfo_list_t * bak = head;

    bak = head;
    if (head == NULL)
    {
        print_err("Cann't delete NULL buffer!", head);
    }
    else
    {
        //break the cycle list to single direction list.
        head->prev->next = NULL;

        //delete the single direction list
        while (head != NULL)
        {
            MY_FREE(head->filename)

            for (i = 0; i < DKEY_COUNT; i++)
            {
                if ((head->value[i]) && (head->value[i] != none_value))
                {
                    MY_FREE(head->value[i])
                }
            }

            if (head->child != NULL)
            {
                appinfo_list_delete(head->child);
            }
            bak = head->next;

            free(head);
            head = NULL;

            head = bak;
        }
        ret = true;
    }
    return ret;
}

///* if use real desktop files */
///*
// * parse the given desktop files, and analyse their child desktop files recursively.
// * desktop_files stored the main application desktop file names from config.txt.
// *
// * */
///*
// static appinfo_list_t * parse_desktop_files( const char * desktop_files[])
// {
// appinfo_list_t * head = NULL;
// appinfo_list_t * p = NULL;
// char sub_desktop_files[10][32];
// int i ;
//
// for(i = 0; desktop_files[i] != NULL ; i++)
// {
// appinfo_list_t * new = (appinfo_list_t *) malloc ( sizeof(appinfo_list_t));
//
// if( desktop == NULL )
// {
// print_err("malloc failed!");
// return NULL;
// }
//
// if(get_desktop_info(desktop_files[i], new, sub_desktop_files) == false)
// {
// return NULL;
// }
//
// new->prev = NULL;
// new->next = NULL;
//
// new->parent = NULL;
// new->child = NULL;
//
// if(NULL == head)
// {
// head = new;
// p = head;
// p->prev = p;
// p->next = p;
// }
//
// p->next = new;
// new->prev = p;
//
// head->prev = new;
// new->next = head;
//
// p = new;
//
// if(DESKTOP_TYPE_DIR == p->type)
// {
// p->child = parse_desktop_files(sub_desktop_files);
// p->child->parent = p;
// }
// }
//
// return head;
// }
//
// */
///*
///* for test */
//static appinfo_list_t * parse_desktop_files(const struct test_desktop_file * desktop_files)
//{
//    appinfo_list_t * head = NULL;
//    appinfo_list_t * p = NULL;
//    int i;
//
//    /* just for test */
//    for (i = 0; desktop_files[i].file_name != NULL; i++)
//    {
//        appinfo_list_t * new = (appinfo_list_t *) malloc(sizeof(appinfo_list_t));
//
//        if (new == NULL)
//        {
//            print_err("malloc failed!");
//            return NULL;
//        }
//
//        new->comment = NULL;
//        new->path = NULL;
//        new->type = desktop_files[i].type;
//        new->icon_id = 0;
//        new->name = desktop_files[i].name;
//        new->exec = desktop_files[i].exec;
//        new->arg = desktop_files[i].arg;
//
//        new->prev = NULL;
//        new->next = NULL;
//
//        new->parent = NULL;
//        new->child = NULL;
//
//        /* 鍒涘缓鍙屽悜寰幆閾捐〃 */
//
//        if (NULL == head)
//        {
//            head = new;
//            p = head;
//            p->prev = p;
//            p->next = p;
//        }
//
//        p->next = new;
//        new->prev = p;
//
//        head->prev = new;
//        new->next = head;
//
//        p = new;
//
//        if (DESKTOP_TYPE_DIR == p->type)
//        {
//            p->child = parse_desktop_files(desktop_files[i].child);
//            p->child->parent = p;
//        }
//    }
//
//    return head;
//}
//
//bool get_desktop_info(const char * file_name, appinfo_list_t * desktop_info, char * sub_desktop_files[])
//{
//    /* TODO: read a desktop file, parse it, then fill values to the appinfo_list_t structure */
//    /* if the current desktop type is directory, then write the sub desktop file names into sub_desktop_files */
//
//}
//
//static void print_desktop_info_list(appinfo_list_t * plist)
//{
//    appinfo_list_t * p;
//    int i = 0;
//    static int indent = 0;
//
//    if (0 == indent)
//    {
//        printf("\nLauncher: desktop info list:\n");
//        printf("name\t\t\texec\t\t\targ\t\t\ttype\n\n");
//    }
//
//    p = plist;
//
//    do
//    {
//        for (i = 0; i < indent; i++)
//        {
//            printf("    ");
//        }
//
//        printf("%-12s\t\t%-12s\t\t%-12s\t\t%-12s\n", p->name, p->exec, p->arg, p->type == DESKTOP_TYPE_APP ? "App"
//                : "Dir");
//
//        if (DESKTOP_TYPE_DIR == p->type)
//        {
//            indent++;
//            print_desktop_info_list(p->child);
//            indent--;
//        }
//        p = p->next;
//    } while (p != plist);
//}
//
