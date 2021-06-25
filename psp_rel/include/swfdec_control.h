#ifndef  SWFDEC_CONTROL_H__
#define  SWFDEC_CONTROL_H__

void * swfdec_open (void *param);

int   swfdec_close (void* player);

int   swfdec_cmd (void* player, int cmd, void* param);

#endif

