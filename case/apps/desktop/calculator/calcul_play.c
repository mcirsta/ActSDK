/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : stopcalcul_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-28          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     calclul_play.c
 * \brief    calculator 计算器的实现和按键控制
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "calcul_play.h"
#include "calcul_scene.h"
#include "BigInteger.h"
#include "Float_simlib.h"
#include "calcul_ui.h"
#include "calculErrProcess.h"
#include "calc_expr.h"



/*                 计算器应用各文件的说明
 ******************************************************************
 *                  计算器应用各文件的说明
 * float_smlib.c : 模拟浮点运算
 * BigIntege.c   : 定义大整数运算
 * cal_expr.c    : 表达式的处理和实现[用于扩展]
 * calculErrProcess: 错误处理和信息
 *######[以上的几个文件和UI、系统平台无关，可以当作库，便于移植]####
 *
 * calcul_ui.c   : UI 绘图相关
 * calcul_play.c : 按键处理和对上面各个模块的组装，实现计算器功能.
 * calcul_main.c : 应用入口、退出
 * calcul_scene.c: 计算器场景相关函数
 ******************************************************************
 */

#define REPAINT_ALL 0x01
#define ERR_PAINT 0x02

#define MAXCOL 6
#define MAXROW 5
#define MAX_SUPPORT_LEN  MAX_STR_LEN

/* 内部静态数据区*/
static char s_errStr[4][48] =
{ "Divided by zero", "Out of range", "Cannot apply SQRT to negative", "Expression too long" };
/* 当前行/列 */
static int s_curRow = 0;
static int s_curCol = 0;
static int s_curKey = CAL_NULL;
/*先前焦点的键 */
static int s_oldKey;
/* memory 中是否有数据 */
static char s_mes_mValueExist = 0;
static char s_inputStr[640];
static char s_dispStr[640];
static char s_inputStatus; /*输入状态*/
/* 可能值为1, 2, 3, 分别对于表达式输入时能否输入小数点 */
static char s_hasMeetDecimal = 0;
/* 计算机的寄存器值,用于MR,MS,MC,M+,M- */
static float_type s_mValue;


static void _dealDigitalKey(int key);
static int _dealFunctionKey(int key);
static void _dealOperandKey(void);
static void _dealZeroSub(void); //正负数反号
static void _dealFloatZeroDisplay(char *str);
static void _dealZeroDisplay(char *str);
static void _onBackSpace(void);
static void _paintStringArea(scene_calcul_t *my_scene);
static int _getDigitKeyStr(int cal_key_id);
//static int _getCalKey(int row, int col);
static inline void _resets_inputString(void);
static inline int _calcul_draw_keys(scene_calcul_t *my_scene);
static void _calcul_key_right(scene_calcul_t *my_scene);
static void _calcul_key_left(scene_calcul_t *my_scene);
static void _calcul_key_ok(scene_calcul_t *my_scene);
static void _copy_str(void);
static void _isDecimal(void);
static void _checkMaxStrlen(char * str);
static void _miscellousCheck(void);




/*
 变量初始化
 */
void calcul_init(void)
{
    //	const char *str;
    //	int encoding;

    s_curRow = 0;
    s_curCol = 0;
    s_curKey = CAL_NULL;
    s_oldKey = CAL_NULL;
    s_mes_mValueExist = 0;
    s_inputStatus = WAIT_FOR_OPERATOR;
    s_hasMeetDecimal = 0;
    memset(&s_mValue, 0, sizeof(float_type));

}

/*
 获取数字键代表的字符
 */
static int _getDigitKeyStr(int cal_key_id)
{
    char ch = 0;
	
    switch (cal_key_id)
    {
        case CAL_0:
        ch = '0';
        break;
        case CAL_1:
        ch = '1';
        break;
        case CAL_2:
        ch = '2';
        break;
        case CAL_3:
        ch = '3';
        break;
        case CAL_4:
        ch = '4';
        break;
        case CAL_5:
        ch = '5';
        break;
        case CAL_6:
        ch = '6';
        break;
        case CAL_7:
        ch = '7';
        break;
        case CAL_8:
        ch = '8';
        break;
        case CAL_9:
        ch = '9';
        break;
        case CAL_DIV:
        ch = '/';
        break;
        case CAL_MUL:
        ch = '*';
        break;
        case CAL_SUB:
        ch = '-';
        break;
        case CAL_ADD:
        ch = '+';
        break;
        case CAL_DOT:
        ch = '.';
        break;

        default:
        ch = 0;
        break;
    }
    return (int)ch;
}



///////////////////////////////////////////////
static inline void _resets_inputString(void)
{
    memset(s_inputStr, 0, sizeof(s_inputStr));
    s_inputStr[0] = '0';
}

/*!\cond CALCULATOR*/
/******************************************************************************/
/*!
 * \par  Description: 按键处理函数，不同的按键进入不同的按键处理子程序
 * \param[in]   my_scene: 场景结构体
 * \param[in]   str:  按键名称索引
 * \param[out]  none
 * \return       the result
 * \retval
 * \ingroup      CALCUL_FUNC
 * \par          exmaple code
 * \code
 char pstr = "12345.6789";
 calcul_draw_str(my_scene, KEY_NEXT);
 * \endcode
 *******************************************************************************/

/*
 按键在移动时都要做一下处理:
 1) 按键所在的行列是否正确的判断,并取得新位置的键索引值
 2) 擦除先前的焦点键
 3) 画当前的焦点键
 */
static inline int _calcul_draw_keys(scene_calcul_t *my_scene)
{
    draw_key_when_move(my_scene, s_oldKey, s_curKey);

    return 0;
}

static void _calcul_key_right(scene_calcul_t *my_scene)
{
    s_curKey++;
    if (s_curKey > CAL_EQU)
    {
        s_curKey = CAL_BACKSPACE;
    }
    _calcul_draw_keys(my_scene);
    s_oldKey = s_curKey;
}

static void _calcul_key_left(scene_calcul_t *my_scene)
{
    s_curKey--;
    if (s_curKey < CAL_BACKSPACE)
    {
        s_curKey = CAL_EQU;
    }
    _calcul_draw_keys(my_scene);
    s_oldKey = s_curKey;
}




static void _calcul_key_ok(scene_calcul_t *my_scene)
{
    switch (s_curKey)
    {
        case CAL_BACKSPACE:
        case CAL_CE:
        case CAL_C:
        case CAL_EQU:
        case CAL_SQRT:
        case CAL_DAO:
        case CAL_MOD:
        case CAL_MC:
        case CAL_MR:
        case CAL_MS:
        case CAL_MPLUS:
        _dealFunctionKey(s_curKey);
        break;

        case CAL_0:
        case CAL_1:
        case CAL_2:
        case CAL_3:
        case CAL_4:
        case CAL_5:
        case CAL_6:
        case CAL_7:
        case CAL_8:
        case CAL_9:
        case CAL_DOT:
        _dealDigitalKey(s_curKey);
        break;

        case CAL_DIV:
        case CAL_MUL:
        case CAL_SUB:
        case CAL_ADD:
        _dealOperandKey();
        break;

        case CAL_CHG:
        _dealZeroSub(); /* 正负数反号 */
        break;

        default:
        break;
    }
    _paintStringArea(my_scene);
}

/*!\cond CALCULATOR*/
/******************************************************************************/
/*!
 * \par  Description: 按键处理函数，不同的按键进入不同的按键处理子程序
 * \param[in]   my_scene: 场景结构体
 * \param[in]   str:  按键名称索引
 * \param[out]  none
 * \return       the result
 * \retval
 * \ingroup      CALCUL_FUNC
 * \par          exmaple code
 * \code
 char pstr = "12345.6789";
 calcul_draw_str(my_scene, KEY_NEXT);
 * \endcode
 *******************************************************************************/
void calcul_key_manage(scene_calcul_t *my_scene, int key)
{
    switch (key)
    {
        case KEY_NEXT:
        _calcul_key_right(my_scene);
        break;

        case KEY_PREV:
        _calcul_key_left(my_scene);
        break;

        case KEY_OK:
        _calcul_key_ok(my_scene);
        if ((s_curKey == CAL_MS) && (s_mes_mValueExist == 1))
        {
            calcul_draw_mflag(my_scene);
        }
        if (s_curKey == CAL_MC)
        {
            calcul_wipe_mflag(my_scene);
        }
        break;

        default:
        break;
    }
}

/**
 *     处理输入的数字如:0,1,2,3,4,5,6,7,8,9,'.'
 **/
static void _dealDigitalKey(int hardkey)
{
    int key;
    int indx = (int)strlen(s_inputStr);

    if (indx >= (MAX_STR_LEN - 2))
    {
        print_err("buffer overflow! \n");
    }

    key = _getDigitKeyStr(hardkey);

    switch (s_inputStatus)
    //状态变迁
    {
        case WAIT_FOR_DECIMAL: //上次输入了一个小数点
        if (key == '.')
        {
            print_err("error , double dot input! ");
            break;
        }
        if (key > 0)
        {
            s_inputStr[indx] = (char)key;
            indx++;
        }
        s_inputStatus = INPUT_OPERATOR;
        break;

        case INPUT_OPERATOR: /* 正在输入*/
        if (key == '.')
        {
            if ((s_hasMeetDecimal == 1) || (s_hasMeetDecimal == 2))
            {
                break;
            }
            if (s_hasMeetDecimal == 0)
            {
                s_hasMeetDecimal = 1;
                s_inputStatus = WAIT_FOR_DECIMAL;
            }
        }
        if (key > 0)
        {
            s_inputStr[indx] = (char)key;
            indx++;
            if (0 == maxNumberCheck(s_inputStr)) //处理操作数过大的问题
            {
                strcpy(s_inputStr, s_errStr[1]);
                s_inputStatus = WAIT_FOR_OPERATOR;
            }
            else
            {
                if (s_hasMeetDecimal == 2)
                {
                    s_hasMeetDecimal = 0;
                }
            }
        }
        break;

        case HAS_INPUT_A_OPERAND: /* 上一个输入是运算符*/
        if (key > 0)
        {
            if (key != '.') //运算符后面不能直接跟小数点
            {
                s_inputStr[indx] = (char)key;
                indx++;
                s_hasMeetDecimal = 0;
                s_inputStatus = INPUT_OPERATOR;
            }
        }
        break;

        case WAIT_FOR_OPERATOR: //第一次输入
        s_hasMeetDecimal = 0;
        _resets_inputString();
        if (key == '.')
        {

            //print_info("不能以小数点开头");
            break;
        }
        if (key > 0)
        {
            s_inputStr[0] = (char)key;
            s_inputStatus = INPUT_OPERATOR;
        }
        break;
        case HAS_CALCULATE_RESULT:
        s_inputStatus = WAIT_FOR_OPERATOR;
        _dealDigitalKey(hardkey);
        break;

        default:
        break;
    }
}

/**
 *     处理功能键如: mc,mr,m+,m-,backspace,c,ce,sqrt,%,1/x,=
 *      正确返回1
 **/
static int _dealFunctionKey(int key)
{
    float_type result;
    float_type retVal;
    //char tmp[32];
    int indx = 0;
    int errNo;
    int flag = 0;

    switch (key)
    {
        case CAL_EQU:
        if ((s_inputStatus != INPUT_OPERATOR) && (s_inputStatus != HAS_CALCULATE_RESULT))
        {
            return 0;
        }
        if (expressionCheck(s_inputStr) == 1)
        {
            errNo = calc(s_inputStr, &result);
            if (errNo != 1)
            {
                if (errNo == 0)
                {
                    strcpy(s_inputStr, s_errStr[0]);
                    s_inputStatus = WAIT_FOR_OPERATOR;
                    return 0;
                }
            }
            memset(s_inputStr, 0, sizeof(s_inputStr));

            getStrFromFloat(s_inputStr, &result);

            //printf("CAL_EQ 结果 %s \n", s_inputStr);

            _dealFloatZeroDisplay(s_inputStr); //除去形如0.0
            _dealZeroDisplay(s_inputStr); //除去形如-0
            //printf("ZERO处理后 结果 %s \n", s_inputStr);
            
            // 检查计算结果是否过大
            if (maxNumberCheck(s_inputStr) == 0) 
            {
                memset(s_inputStr, 0, sizeof(s_inputStr));
                strcpy(s_inputStr, s_errStr[1]);
                s_inputStatus = WAIT_FOR_OPERATOR;
                break;
            }

            s_inputStatus = HAS_CALCULATE_RESULT;
        }
        else
        {
            strcpy(s_inputStr, s_errStr[1]);
            s_inputStatus = WAIT_FOR_OPERATOR;
            return 0;

        }
        break;

        case CAL_SQRT:
        if ((s_inputStatus != INPUT_OPERATOR) && (s_inputStatus != HAS_CALCULATE_RESULT))
        {
            return 0;
        }
        if (expressionCheck(s_inputStr) == 0) //检查是不是有太大的数
        {
            strcpy(s_inputStr, s_errStr[1]);
            s_inputStatus = WAIT_FOR_OPERATOR;
            return 0;
        }
        errNo = calc(s_inputStr, &result);
        if (errNo != 1)
        {
            if (errNo == 0)
            {
                strcpy(s_inputStr, s_errStr[0]);
                s_inputStatus = WAIT_FOR_OPERATOR;
                return 0;
            }
        }
        if (result.bInteger.sbit == 0) //>0的数才能开方
        {
            if (float_sqrt(&result, &retVal) == 1)
            {
                memset(s_inputStr, 0, sizeof(s_inputStr));
                getStrFromFloat(s_inputStr, &retVal);
                _dealFloatZeroDisplay(s_inputStr); //除去形如0.0
                _dealZeroDisplay(s_inputStr); //除去形如-0
                s_inputStatus = HAS_CALCULATE_RESULT;
            }
            else //输入数太大
            {
                strcpy(s_inputStr, s_errStr[1]);
                s_inputStatus = WAIT_FOR_OPERATOR;
            }
        }
        else
        {
            memset(s_inputStr, 0, sizeof(s_inputStr));
            strcpy(s_inputStr, s_errStr[2]);
            s_inputStatus = WAIT_FOR_OPERATOR;
        }
        break;

        case CAL_MOD:
        if ((s_inputStatus != INPUT_OPERATOR) && (s_inputStatus != HAS_CALCULATE_RESULT))
        {
            return 0;
        }
        indx = (int)strlen(s_inputStr);
        strcpy(&s_inputStr[indx], "/100");
        _dealFunctionKey(CAL_EQU);
        s_inputStatus = HAS_CALCULATE_RESULT;
        break;

        case CAL_DAO:
        if ((s_inputStatus != INPUT_OPERATOR) && (s_inputStatus != HAS_CALCULATE_RESULT))
        {
            return 0;
        }
        if (expressionCheck(s_inputStr) == 0)
        {
            strcpy(s_inputStr, s_errStr[1]);
            s_inputStatus = WAIT_FOR_OPERATOR;
            return 0;
        }
        errNo = calc(s_inputStr, &result);
        if (errNo != 1)
        {
            if (errNo == 0)
            {
                strcpy(s_inputStr, s_errStr[0]);
                s_inputStatus = WAIT_FOR_OPERATOR;
                return 0;
            }
        }
        if (0 == float_reverse(&result, &retVal))
        {
            strcpy(s_inputStr, s_errStr[0]);
            s_inputStatus = WAIT_FOR_OPERATOR;
            return 0;
        }
        memset(s_inputStr, 0, sizeof(s_inputStr));

        getStrFromFloat(s_inputStr, &retVal);
        _dealFloatZeroDisplay(s_inputStr); //除去形如0.0
        _dealZeroDisplay(s_inputStr); //除去形如-0

        s_inputStatus = HAS_CALCULATE_RESULT;
        break;

        case CAL_CE:
        _resets_inputString();
        s_inputStatus = WAIT_FOR_OPERATOR;
        break;

        case CAL_C:
        /* 清除输入 */
        s_mes_mValueExist = 0;
        memset(&s_mValue, 0, sizeof(float_type));
        _resets_inputString();
        s_inputStatus = WAIT_FOR_OPERATOR;
        break;

        case CAL_BACKSPACE: //退格
        _onBackSpace();
        break;

        case CAL_MC:
        memset(&s_mValue, 0, sizeof(float_type));
        s_mes_mValueExist = 0;
        break;

        case CAL_MR:
        switch (s_inputStatus)
        {
            case WAIT_FOR_OPERATOR:
            case HAS_CALCULATE_RESULT:
            if (s_mes_mValueExist != 0)
            {
                memset(s_inputStr, 0, sizeof(s_inputStr));
                getStrFromFloat(s_inputStr, &s_mValue);
                s_inputStatus = INPUT_OPERATOR;
            }
            break;

            case HAS_INPUT_A_OPERAND:
            if (s_mes_mValueExist != 0)
            {
                indx = (int)strlen(s_inputStr);
                getStrFromFloat(&s_inputStr[indx], &s_mValue);
                if (isStringContainDot(&s_inputStr[indx]) == 1)
                {
                    s_hasMeetDecimal = 1;
                }
                else
                {
                    s_hasMeetDecimal = 0;
                }
                s_inputStatus = INPUT_OPERATOR;
            }
            break;

            case INPUT_OPERATOR:
            if ((s_mes_mValueExist != 0) && (isStringSingleOperand(s_inputStr) == 1))
            {
                memset(s_inputStr, 0, sizeof(s_inputStr));
                getStrFromFloat(s_inputStr, &s_mValue);
                if (isStringContainDot(s_inputStr) == 1)
                {
                    s_hasMeetDecimal = 1;
                }
                else
                {
                    s_hasMeetDecimal = 0;
                }

                s_inputStatus = INPUT_OPERATOR;
            }

            default:
            break;
            //dealInputError(ERR_MISSING_OPERATOR);
        }
        break;

        case CAL_MS:

        if ((s_inputStr[0] == '0') && (s_inputStr[1] == '\0'))
        {
            return 0;
        }
        if ((s_inputStatus == INPUT_OPERATOR) || (s_inputStatus == HAS_CALCULATE_RESULT))
        {
            if (_dealFunctionKey(CAL_EQU) == 1)
            {
                //printf("MS要保存的数  =%s   \n", inputStr);
                getFloatFromStr(s_inputStr, &s_mValue);
                if (((s_inputStr[0] == '0') && (s_inputStr[1] == '\0')) || 
                    ((s_inputStr[0] == '-') && (s_inputStr[1] == '\0')) || 
                    ((s_inputStr[0] == '-') && (s_inputStr[1] == '0') && (s_inputStr[2] == '.') 
                            && (s_inputStr[3] == '0') && (s_inputStr[4] == '\0')))
                               
                {
                    s_mes_mValueExist = 0;
                }
                else
                {
                    s_mes_mValueExist = 1;
                }
                s_inputStatus = HAS_CALCULATE_RESULT;
            }
            else
            {
                s_inputStatus = WAIT_FOR_OPERATOR;
            }
        }
        else
        {
            dealInputError(ERR_MSVALUE_NOT_VALID);
        }
        break;

        case CAL_MPLUS:
        if (((s_inputStatus == INPUT_OPERATOR) && (isStringSingleOperand(s_inputStr))) 
            || (s_inputStatus == HAS_CALCULATE_RESULT))
        {
            char tmp2[MAX_SUPPORT_LEN];
            unsigned int length = strlen(s_inputStr) + 1;
            if (length > MAX_SUPPORT_LEN)
            {
                length = MAX_SUPPORT_LEN;
            }
            memcpy(tmp2, s_inputStr, length);
            tmp2[MAX_SUPPORT_LEN - 1] = '\0';

            s_inputStatus = INPUT_OPERATOR;
            indx = (int)strlen(s_inputStr);
            s_inputStr[indx] = '+';
            indx++;
            if (s_mes_mValueExist == 0)
            {
                if(indx >= (int)sizeof(s_inputStr))
                {
                    indx = sizeof(s_inputStr) - 1;
                    memset(s_inputStr, 0, sizeof(s_inputStr));	
                }
                s_inputStr[indx] = '0';
                indx++;
                if(indx >= (int)sizeof(s_inputStr))
                {
                    indx = sizeof(s_inputStr) - 1;	
                    memset(s_inputStr, 0, sizeof(s_inputStr));
                } 
                if(indx < 639)
                {               
                    s_inputStr[indx] = '\0';
                }
                indx++;
            }
            else
            {
                getStrFromFloat(&s_inputStr[indx], &s_mValue);
            }

            _dealFunctionKey(CAL_EQU);
            _dealFunctionKey(CAL_MS); //有可能改变s_mes_mValueExist的标志位

            memcpy(s_inputStr, tmp2, strlen(tmp2) + 1);
            s_inputStatus = HAS_CALCULATE_RESULT;
        }
        else
        {
            dealInputError(ERR_MISSING_OPERATOR);

        }
        //return 0;
        flag = 1;
        break;

        case CAL_MMINUS:
        if (((s_inputStatus == INPUT_OPERATOR) && (isStringSingleOperand(s_inputStr))) 
                || (s_inputStatus == HAS_CALCULATE_RESULT))
        {
            char tmp3[641]; //tmp3[MAX_SUPPORT_LEN];
            memset(tmp3, 0, sizeof(tmp3));
            memcpy(tmp3, s_inputStr, strlen(s_inputStr) + 1);

            s_inputStatus = INPUT_OPERATOR;
            indx = (int)strlen(s_inputStr);
            s_inputStr[indx] = '-';
            indx++;
            if (0 == s_mes_mValueExist)
            {
                if (indx < 639)
                {
                    s_inputStr[indx] = '0';
                    indx++;
                    s_inputStr[indx] = '\0';
                    indx++;
                }
                else
                {
                    dealInputError(ERR_MISSING_OPERATOR);
                    print_err("ERROR!!!!! ");
                    memset(s_inputStr, 0, 640);
                    return 0;
                }
            }
            else
            {
                getStrFromFloat(&s_inputStr[indx], &s_mValue);
            }

            _dealFunctionKey(CAL_EQU);
            _dealFunctionKey(CAL_MS); /* 有可能改变s_mes_mValueExist的标志位 */

            memcpy(s_inputStr, tmp3, strlen(tmp3) + 1);
            s_inputStatus = HAS_CALCULATE_RESULT;
        }
        else
        {
            dealInputError(ERR_MISSING_OPERATOR);
            print_err("ERROR!!!!! ");

        }
        //return 0;
        flag = 1;
        break;

        default:
        print_warning("funciton key not  processed !\n");
        break;
    }
    if(flag == 1)
    {
        return 0;	
    }
    else
    {
        return 1;
    }
}

/**
 *     处理操作符如: '+','-','*','/'
 **/
static void _dealOperandKey(void)
{
    char key;
    int indx;

    _dealFunctionKey(CAL_EQU);
    indx = (int)strlen(s_inputStr);
    
    if (indx >= (MAX_STR_LEN - 2))
    {
        //print_info("buffer overflow! \n");
        return;
    }
    key = (char)_getDigitKeyStr(s_curKey);

    switch (s_inputStatus)
    /* 状态变迁 */
    {
        case INPUT_OPERATOR:
        if (maxNumberCheck(s_inputStr) == 0) /* 处理操作数过大的问题 */
        {
            memset(s_inputStr, 0, sizeof(s_inputStr));
            strcpy(s_inputStr, s_errStr[1]);
            s_hasMeetDecimal = 0;
            s_inputStatus = WAIT_FOR_OPERATOR;
        }
        else
        {
            if (key > 0)
            {
                s_inputStr[indx] = key;
                indx++;
                if (s_hasMeetDecimal == 1) /* 如果已经输入一个小数再输入的运算符 */
                {
                    s_hasMeetDecimal = 2;
                }
                s_inputStatus = HAS_INPUT_A_OPERAND;
            }
        }
        break;

        case WAIT_FOR_DECIMAL:
        //print_info("应该输入小数部分，而不是运算符");
        break;

        case HAS_INPUT_A_OPERAND:
        if(indx > 1)
        {
            s_inputStr[indx - 1] = key;
            //print_info("不能连续输入两个运算符\n");
        }
        break;

        case WAIT_FOR_OPERATOR:
        _resets_inputString();
        //print_info("表达式不能以运算符开始\n");
        break;

        case HAS_CALCULATE_RESULT:
        if (maxNumberCheck(s_inputStr) == 0) /* 处理操作数过大的问题 */
        {
            memset(s_inputStr, 0, sizeof(s_inputStr));
            strcpy(s_inputStr, s_errStr[1]);
            s_hasMeetDecimal = 0;
            s_inputStatus = WAIT_FOR_OPERATOR;
            break;
        }

        if (key > 0)
        {
            s_inputStr[indx] = key;
            indx++;
            s_inputStatus = HAS_INPUT_A_OPERAND;
        }
        break;

        default:
        break;
    }
}

/**
 *     正负数反号:'+/-'
 **/
static void _dealZeroSub(void)
{
    char tmp[640];

    switch (s_inputStatus)
    {
        case WAIT_FOR_OPERATOR: /*从后向前找到第一个运算符 */
        s_inputStr[0] = '-';
        s_inputStr[1] = '\0';
        s_inputStatus = INPUT_OPERATOR;
        break;

        case HAS_CALCULATE_RESULT:

        memset(tmp, 0, sizeof(tmp));
        if (s_inputStr[0] == '-')
        {
            memcpy(tmp, s_inputStr + 1, 639);
            memcpy(s_inputStr, tmp, 639);
        }
        else
        {
            memcpy(tmp + 1, s_inputStr, 639);
            tmp[0] = '-';
            memcpy(s_inputStr, tmp, 640);
        }

        _dealZeroDisplay(s_inputStr);
        break;

        case INPUT_OPERATOR:
        if (isStringSingleOperand(s_inputStr) == 1)
        {
            memset(tmp, 0, sizeof(tmp));
            if (s_inputStr[0] == '-')
            {
                memcpy(tmp, s_inputStr + 1, 639);
                memcpy(s_inputStr, tmp, 639);
            }
            else
            {
                memcpy(tmp + 1, s_inputStr, 639);
                tmp[0] = '-';
                memcpy(s_inputStr, tmp, 640);
            }
            if (isStringContainDot(s_inputStr) == 1)
            {
                s_hasMeetDecimal = 1;
            }
            else
            {
                s_hasMeetDecimal = 0;
            }
            _dealZeroDisplay(s_inputStr);
        }
        break;

        default:
        break;
    }
}

/*
 ** 除去形如0.0
 */
static void _dealFloatZeroDisplay(char *str)
{
    if (((str[0] == '0') && (str[1] == '.') && (str[2] == '0') && (str[3] == '\0')) || 
            ((str[0] == '-') && (str[1] == '0') && (str[2] == '.') && (str[3] == '0') && (str[4] == '\0')))
    {
        memset(s_inputStr, 0, sizeof(s_inputStr));
        memset(s_dispStr, 0, sizeof(s_dispStr));
        s_dispStr[0] = '0';
        s_dispStr[1] = '\0';
        s_inputStr[0] = '0';
        s_inputStr[1] = '\0';

    }
}

/*
 **除去形如-0的显示
 */
static void _dealZeroDisplay(char *str)
{
    int len = (int)strlen(str);
    if ((len == 2) && (*str == '-') && (*(str + 1) == '0'))
    {
        str[0] = '0';
        str[1] = '\0';
    }
}

/*
 **退格
 */
static void _onBackSpace(void)
{
    int indx;

    indx = (int)strlen(s_inputStr);

    switch (s_inputStatus)
    {
        case WAIT_FOR_OPERATOR:
        return;

        case INPUT_OPERATOR:

        if (indx > 1) //防止s_inputStr为空的情况
        {
            if (s_inputStr[indx - 2] == '.')
            {
                s_inputStatus = WAIT_FOR_DECIMAL;
                if (s_hasMeetDecimal == 1)
                {
                    s_hasMeetDecimal = 0;
                }

            }
            else if ((s_inputStr[indx - 2] == '+') || (s_inputStr[indx - 2] == '-') || (s_inputStr[indx - 2] == '*')
                    || (s_inputStr[indx - 2] == '/'))
            {

                s_inputStatus = HAS_INPUT_A_OPERAND;
            }
            else
            {
                //print_dbg();
            }
            s_inputStr[indx - 1] = '\0';
            
            if ( (s_inputStr[0] == '-') && (indx == 2) ) //防止负数最后不能去掉符号
            {
                s_inputStr[0] = '\0';	
            }
        }
        else
        {
            _resets_inputString();
            s_inputStatus = WAIT_FOR_OPERATOR;
        }
        break;

        case HAS_INPUT_A_OPERAND:

        if (indx > 1) //防止s_inputStr为空的情况
        {

            s_inputStr[indx - 1] = '\0';
            s_inputStatus = INPUT_OPERATOR;
            if (s_hasMeetDecimal == 2)
            {
                s_hasMeetDecimal = 1;
            }

        }
        break;

        case WAIT_FOR_DECIMAL:
        if (indx > 1) //防止s_inputStr为空的情况
        {
            if (s_inputStr[indx - 1] == '.')
            {
                s_inputStr[indx - 1] = '\0';
                s_inputStatus = INPUT_OPERATOR;
            }
            if (s_hasMeetDecimal == 1)
            {
                s_hasMeetDecimal = 0;
            }
        }
        else
        {
            _resets_inputString();

            s_inputStatus = WAIT_FOR_OPERATOR;
        }
        break;

        case HAS_CALCULATE_RESULT:
        return;

        default:
        break;
    }
}

static void _copy_str(void)
{
    int count;
    int length = (int)strlen(s_dispStr);

    if (s_inputStatus == HAS_INPUT_A_OPERAND)
    {
        if (length > 0)
        {
            s_dispStr[length - 1] = '\0';
        }
        else
        {
            s_dispStr[0] = '\0';
        }
    }
    else
    {
        if(length > 0)
        {	
            count = length - 1;
            while ((count >= 0) && (s_dispStr[count] != '+') && (s_dispStr[count] != '-') 
                    && (s_dispStr[count] != '*') && (s_dispStr[count] != '/'))
            {
                count--;
            }

            if (count > 0)
            {
                memcpy(s_dispStr, (s_dispStr + count) + 1, (unsigned int)((length - count) - 1));
                if ((length - count) >= 1)
                {
                    s_dispStr[(length - count) - 1] = '\0';
                }
            }
        }
        else
        {
            s_dispStr[0] = '\0'; 	
        }
    }
}

static void _isDecimal(void)
{
    int indx = 0;
    int length = (int)strlen(s_dispStr);

    while ((indx < length) && (s_dispStr[indx] != '.'))
    {
        indx++;
    }

    if (indx < MAX_NUM_SUPPORT)
    {
        memset(s_dispStr + (MAX_NUM_SUPPORT + 1), 0, (640 - MAX_NUM_SUPPORT) - 1);
        s_dispStr[MAX_NUM_SUPPORT + 1] = '\0';
        memcpy(s_inputStr, s_dispStr, 640);
    }
    else
    {
        s_inputStatus = WAIT_FOR_OPERATOR;
        _resets_inputString();
        //strcpy(s_dispStr, s_errStr[3]);
    }
}

/*
 *   最大允许输入表达式长度
 */
static void _checkMaxStrlen(char * str)
{
    if (strlen(s_inputStr) > MAX_SUPPORT_LEN)
    {
        memset(s_inputStr, 0, sizeof(s_inputStr));
        strcpy(s_dispStr, s_errStr[3]);
        s_inputStatus = WAIT_FOR_OPERATOR;
    }
}

/*
 ** 判断输入的数字串前面是否有0，如果有则去掉0
 ** 例如：把000012345 变成 12345
 */
static void _miscellousCheck(void)
{
    int len;
    int i = 0;

    len = (int)strlen(s_inputStr);
    memset(s_inputStr + len, 0, sizeof(s_inputStr) - (unsigned int)len);
    memset(s_dispStr, 0, sizeof(s_dispStr));
    //print_dbg("before _miscellousCheck= %s \n", s_inputStr);
    while ((*(s_inputStr + i) == '0') && (*(s_inputStr + i + 1) != '.') //如果inputStr=0
        && (*(s_inputStr + i + 1) != '+') && (*(s_inputStr + i + 1) != '-') 
        && (*(s_inputStr + i + 1) != '*') && (*(s_inputStr + i + 1) != '/'))
    {
        i++;
    }

    if (i == len)
    {
        memset(s_inputStr, 0, sizeof(s_inputStr));
        memset(s_dispStr, 0, sizeof(s_dispStr));
        s_dispStr[0] = '0';
        s_dispStr[1] = '\0';
        s_inputStr[0] = '0';
        s_inputStr[1] = '\0';
    }
    else
    {
        memcpy(s_dispStr, s_inputStr + i, sizeof(s_inputStr) - (unsigned int)i);
        memcpy(s_inputStr, s_dispStr, sizeof(s_dispStr));
        memcpy(s_dispStr, s_inputStr, strlen(s_inputStr));
    }

    //print_info("after _miscellousCheck= %s \n", s_inputStr);
}

static void _paintStringArea(scene_calcul_t *my_scene)
{
    exprValidate(s_inputStr); //表达式合法性检查
    _miscellousCheck(); //去掉数字串前面的无效‘0’
    _checkMaxStrlen(0);
    _copy_str();
    if (maxNumberCheck(s_dispStr) == 0)
    {
        _isDecimal();
    }
    calcul_draw_str(my_scene, s_dispStr);
}

/*
 当 rotate 时，如果有高亮的图片、有计算结果或中间值，都须paint出来
 */
void paint_when_rotate(scene_calcul_t *my_scene)
{
    /* 画出当前高亮图片 */
    if (s_curKey != CAL_NULL)
    {
        calcul_draw_key(my_scene, s_curKey);
    }

    /* 画出当前计算结果或中间值 */
    _paintStringArea(my_scene);

    /* 如果旋转时，Memory中存储数据，画出memory标记 */
    if (s_mes_mValueExist == 1)
    {
        calcul_draw_mflag(my_scene);
    }
}

