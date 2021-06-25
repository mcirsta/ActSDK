#ifndef __CALCUL_UI__
#define __CALCUL_UI__

#include "calcul_scene.h"

void calcul_draw_str(scene_calcul_t *my_scene, char *str);
void calcul_draw_mflag(scene_calcul_t *my_scene);
void calcul_wipe_mflag(scene_calcul_t *my_scene);
void calcul_draw_key(scene_calcul_t *my_scene, int key);
void calcul_wipe_key(scene_calcul_t *my_scene, int key);
void draw_key_when_move(scene_calcul_t *my_scene, int oldkey, int curkey);


#endif

