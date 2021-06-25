#ifndef CALCUL_H
#define CALCUL_H


#include "calcul_scene.h"



#define ERR_DOUBLE_OPERATOR   0       //连续输入了两个运算符
#define ERR_MISSING_OPERATOR  1      //缺少运算符
#define ERR_MISSING_DECIAML   2     //输入了小数点，但是没有输入小数部分
#define ERR_MSVALUE_NOT_VALID 3

#define MAX_STR_LEN  100
#define MAX_NUM_SUPPORT 12
//定义输入状态

#define WAIT_FOR_OPERATOR       0            /*等待输入操作数*/
#define INPUT_OPERATOR          1         /* 正在输入*/
#define HAS_INPUT_A_OPERAND     2	    /* 上一个输入是运算符*/
#define WAIT_FOR_DECIMAL        3           /*输入了小数点，等待输入小数部分*/
#define HAS_CALCULATE_RESULT    4

#define CAL_NULL        0
#define CAL_BACKSPACE   1
#define CAL_CE          2
#define CAL_C           3

#define CAL_SQRT        4
#define CAL_MOD         5
#define CAL_DAO         6
#define CAL_CHG         7	 /*  '+/-' */
#define CAL_DIV         8    /*  '/'   */

#define CAL_MC          9
#define CAL_7           10
#define CAL_8           11
#define CAL_9           12
#define CAL_MUL         13  /*  '*' */

#define CAL_MR          14
#define CAL_4           15
#define CAL_5           16
#define CAL_6           17
#define CAL_SUB         18     /*     '-'      */

#define CAL_MS          19    /* 'MS' */
#define CAL_1           20   /*   '1' */
#define CAL_2           21   /*  '2'  */
#define CAL_3           22
#define CAL_ADD         23    /*  '+' */

#define CAL_MPLUS       24    /* 'M+' */
#define CAL_0           25    /*  '0' */
#define CAL_DOT         26	  /*  '.' */
#define CAL_EQU         27	  /* '='  */

#define CAL_MMINUS      28	  /* 'M-'为了便于移植，额外加了 MS  */



void dealInputError(int errNo);
int expressionCheck(char *expstr);

void calcul_init(void);
void calcul_key_manage(scene_calcul_t *my_scene, int key);
void paint_when_rotate(scene_calcul_t *my_scene);



#endif

