/*
 * launcher.h
 *
 *  Created on: 2009-6-11
 *      Author: chencm
 */

#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#include <include_case.h>
#include <include_psp.h>

#define FULL_FILENAME_LEN PATH_MAX

typedef enum
{
    SCENE_EXIT = 0, //exit state
    SCENE_DESKTOP,
    SCENE_LAUNCH_APP,
    SCENE_LAUNCH,
} scene_status_e;

extern int debug_time;

#define DUMP_TIME(void) \
{\
    int cur_time = 0;\
    cur_time = get_ab_timer();\
    printf("%s,%d,( %d )-- %d ms\n",__FILE__,__LINE__,cur_time,cur_time - debug_time);\
    debug_time = cur_time;\
}

#define MY_FREE(p) \
{\
    if ((p) != NULL)\
    {\
        free((p));\
        (p) = NULL;\
    }\
}

#endif /* LAUNCHER_H_ */
