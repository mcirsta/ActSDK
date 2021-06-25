// calc.cpp : Defines the entry point for the console application.
//
#include "calculErrProcess.h"
#include "string.h"
#include "float_simlib.h"

#define stack_init_size1 100
#define stack_init_size2 100
#define stackincrement1 400
#define stackincrement2 200

static float_type OperandStack_space[stack_init_size1];
static char p_stack_space[stack_init_size2];
//static int endOfInteger = 0;

int getNextOperand(char *str, int *idx, float_type *t);
int processNumberEndWithSevenZero(char *expr);
int getNextOperandStr(char *str, int *idx, char *floatstr);
int processPrefixZero(char *expr, int maxLen);
int findNextOp(const char *expr);

/*
 *
 *   操作数栈
 *
 */
typedef struct
{
    float_type *base;
    float_type *top;
    int stacksize;
} OperandStack;

/*
 *
 *  运算符栈
 *
 */

typedef struct
{
    char *base;
    char *top;
    int stacksize;
} OperatorStack;

#define CALC_NO_ERROR  0
#define CALC_STACK_OVERFLOW   1
#define CALC_STACK_BADEXPRESSION  2

typedef struct
{

    int result; //计算结果
    int errFlag; // 表达式计算错误代码
} calcObj;

/*
 *  优先级表
 */
static char priotbl[7][7] =
{
    { '>', '>', '<', '<', '<', '>', '>' },
    { '>', '>', '<', '<', '<', '>', '>' },
    { '>', '>', '>', '>', '<', '>', '>' },
    { '>', '>', '>', '>', '<', '>', '>' },
    { '<', '<', '<', '<', '<', '=', '>' },
    { '>', '>', '>', '>', '>', '>', '>' },
    { '<', '<', '<', '<', '<', '>', '=' } 
};

/*
 *    返回操作符在优先级表中的序号
 */
int in(char c)
{
    switch (c)
    {
        case '+':
        return 0;
        case '-':
        return 1;
        case '*':
        return 2;
        case '/':
        return 3;
        case '(':
        return 4;
        case ')':
        return 5;
        case '#':
        return 6;
        default:
        return 9;
    }
}
/*
 *     入栈操作数
 *
 */

void s_push(OperandStack *s, float_type e)
{
    if ((s->top - s->base) >= s->stacksize)
    {
        //printf("stack over flow !\n");
        return;
    }
    *s->top = e;
    s->top++;
}

/*
 * 入栈操作符
 *
 */
void f_push(OperatorStack *f, char e)
{
    if ((f->top - f->base) >= f->stacksize)
    {
        //printf("stack over flow !\n");
        return;
    }
    *f->top = e;
    f->top++;

}

float_type s_gettop(OperandStack *s)
{
    s->top--;
    return *s->top;
}

char f_gettop(OperatorStack *f)
{
    f->top--;
    return *f->top;
}

/*
 *
 *   将当前操作数和栈中操作数比较: 结果是 c > f->op , c<f->op, c=c->op中的一种
 *
 */
char isPrioHigher(OperatorStack *f, char c)
{
    int m, n;
    char w;
    
    m = in(c);
    w = *(f->top - 1);
    n = in(w);
    if ((m < 7) && (n < 7))
    {
        return priotbl[n][m];
    }
    else
    {
        return 0;
    }
}

int calcOperator(float_type *p, char r, float_type *q, float_type *result)
{
    int flag = 0;
    
    switch (r)
    {
        case '+':
        flag = float_add(p, q, result);
        break;
        case '-':
        flag = float_dec(p, q, result);
        break;
        case '*':
        flag = float_mul(p, q, result);
        break;
        case '/':
        flag = float_div(p, q, result);
        break;
        default:
        break;
    }
    return flag;
}

/*
 *  expr为需要计算的表达式 例如 127.0 * 456 -123 +37.8
 *
 */
int calc(char *expr, float_type *retVar)
{
    float_type result;
    OperandStack s;
    OperatorStack f;
    char tmpStr[MAX_NUMBER_COUNT];
    char c;
    int meetOperator; // 前一个是不是操作符

    int idx = 0; //指示当前处理了表达式字符创中第几个字符

    /// 初始化相关数据
    int errNO = 0;
    
    s.base = OperandStack_space;
    s.top = s.base;
    s.stacksize = stack_init_size1;
    f.base = p_stack_space;
    f.top = f.base;
    f.stacksize = stack_init_size2;
    *f.top = '#';
    f.top++;
    c = *(expr + idx);
    if (in(c) == 1)
    {
        meetOperator = 1;
    }
    else
    {
        meetOperator = 0;
    }
    //开始计算*/
    c = *(expr + idx);
    while (c != '\0') //表达式必须以\0作为结束符
    {
        float_type t;
        float_type p, q;
        char r;

        if (in(c) == 9) //遇到了操作数的一部分，取出加到操作数栈
        {
            getNextOperand(expr, &idx, &t);
            s_push(&s, t);
            meetOperator = 0;
        }
        else if ((in(c) == 1) && meetOperator) //负数
        {
            idx++;
            getNextOperand(expr, &idx, &t);
            t.bInteger.sbit = 1;
            getStrFromFloat(tmpStr, &t);

            s_push(&s, t);
            meetOperator = 0;
        }
        else
        {
            switch (isPrioHigher(&f, c))
            {
                case '<': //栈顶比当前操作符优先级低，继续入栈
                f_push(&f, c);
                idx++;
                meetOperator = 1;
                //printf("push operator :   %c \n", c);
                break;
                
                case '=':
                f.top--;
                f.stacksize--;
                idx++;
                meetOperator = 1;
                break;

                case '>': //栈顶操作符优先级高计算
                p = s_gettop(&s);
                q = s_gettop(&s);
                r = f_gettop(&f);

                errNO = calcOperator(&q, r, &p, &result);
                if (errNO != 1)
                {
                    memset(retVar, 0, sizeof(float_type));
                    return errNO;
                }
                s_push(&s, result);
                meetOperator = 0;
                break;
                
                default:
                break;
            }
        }
        c = *(expr + idx);
    }
    
    //while ((c == '\0') && (*(f.top - 1) != '#'))  //QAC 不通过
    while (*(f.top - 1) != '#')
    {
        float_type p, q;
        char r;
        p = s_gettop(&s);
        q = s_gettop(&s);
        r = f_gettop(&f);
        calcOperator(&q, r, &p, &result);
        errNO = calcOperator(&q, r, &p, &result);
        if (errNO != 1)
        {
            memset(retVar, 0, sizeof(float_type));
            return errNO;
        }
        s_push(&s, result);
    }
    *retVar = s_gettop(&s);
    getStrFromFloat(tmpStr, retVar);
    //printf(" result = %s \n", tmpStr);
    return 1;
}
/*
 int main(int argc, char *argv[])

 {
 calc("1111+444444*2.3");
 return 1;
 }
*/

// 取下一个操作符
int getNextOperand(char *str, int *idx, float_type *t)
{
    char digital;
    char floatstr[32]; //保存浮点数的指针
    int i = 0; 
       
    memset(floatstr, 0, sizeof(floatstr));
    
    digital = *(str + *idx);
    while (digital != '\0')
    {
        if (in(digital) == 9)
        {
            floatstr[i] = digital;
            i++;
        }
        else
        {
            break;
        }

        (*idx)++;
        digital = *(str + *idx);
    }
    getFloatFromStr(floatstr, t);

    return 1;
}

//将src中表达式转换为大数表示
//如果转化过，返回1，否则返回0
int convertExprtoScience(char *dst, const char *src, int maxlen)
{
    int idx = 0;
    int dotPos = 0;
    int retFlag = 0;
    int intergerLen = 0;
    int decimalLen = 0;
    int cpyCount = 0;
    char c;
    char expr[10];
    
    memset(expr, 0, sizeof(expr));

    //首先扫描所有字符串，看是否包含运算符
    c = *(src + idx);
    while ((c != '\0') && (idx < maxlen))
    {
        switch (c)
        {
            case '+':
            case '*':
            case '/':
            retFlag = 1;
            break;
            
            case '-':
            if (idx > 0)
            {
                retFlag = 1;
            }
            break;
            
            default:
            break;
        }
        
        if (retFlag == 1)
        {
            break;
        }
        
        idx++;
        c = *(src + idx);
    }

    if (retFlag == 1)
    {
        memcpy(dst, src, strlen(src) + 1);
        return 0;
    }

    if ((*src == '-') && (strlen(src) == 1))
    {
        memcpy(dst, src, strlen(src) + 1);
        return 0;
    }
    //src是一个大数，将其转换为科学计数法
    //基本思路是先找到小数点，并且确定整数
    //和小数部分的长度

    if (*src == '-')
    {
        idx = 1;
    }
    else
    {
        idx = 0;
    }

    dotPos = -1;
    c = *(src + idx);
    while ((c != '\0') && (idx < maxlen))
    {
        switch (c)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            if (dotPos > 0)
            {
                decimalLen++;
            }
            else
            {
                intergerLen++;
            }
            break;
            case '.':
            dotPos = idx;
            break;
            default:
            break;
        }
        idx++;
        c = *(src + idx);
    }

    if (intergerLen < 10)
    {
        memcpy(dst, src, strlen(src) + 1);
        return 0;
    }

    if (*src == '-') //处理负数
    {
        *dst = '-';
        idx = 1;
    }
    else
    {
        idx = 0;
    }
    *(dst + idx) = *(src + idx);
    *(dst + idx + 1) = '.';
    idx++;
    while ((cpyCount < MAX_DECIMAL_LEN) && (idx < intergerLen))
    {

        *((dst + idx) + 1) = *(src + idx);
        idx++;
        cpyCount++;
    }

    sprintf(expr, "%d", intergerLen - 1);
    *((dst + idx) + 1) = '\0';
    //printf(" 科学计数法转换后数据: %s \n", dst);
    float_striple_pend_zero(dst);

    idx = (int)strlen(dst);
    *(idx + dst) = 'E';
    idx++;
    *(idx + dst) = '+';
    idx++;
    memcpy(dst + idx, expr, strlen(expr) + 1);
    //printf("科学计数法末尾0处理后: %s \n", dst);
    return 1;
}

/*
 *      判断某个表达式是否只有一个操作数，不包含任何
 *     运算符,如果是单个操作数，返回1,否则返回0
 *
 */
int isStringSingleOperand(const char *src)
{
    char c;
    int retFlag = 1;
    int idx = 0;
    
    //首先扫描所有字符串，看是否包含运算符
    c = *(src + idx);
    while (c != '\0')
    {
        switch (c)
        {
            case '+':
            case '*':
            case '/':
            retFlag = 0;
            break;
            case '-':
            if (idx > 0)
            {
                retFlag = 0;
            }
            break;
            default:
            break;
        }
        if (retFlag == 0)
        {
            break;
        }
        idx++;
        c = *(src + idx);
    }

    return retFlag;
}

/*
 *     检查某个数是否包含小数点(小数)，如果有返回1,否则返回0
 *
 *   注意!!!:  src必须是合法的数据字符串
 */
int isStringContainDot(const char *src)
{
    char c;
    int idx = 0;
    
    c = *(src + idx);
    while (c != '\0')
    {
        switch (c)
        {
            case '.':
            return 1;
            default:
            break;
        }
        idx++;
        c = *(src + idx);
    }
    return 0;
}

//浮点0转化为小数0，例如0.0000000  ->  0
//注意number必须是合法的浮点数

int floatZeroToZero(char * number)
{
    /*
     int retFlag=0;
     char floatZero[]="0.0000000";
     if(!memcmp(floatZero, number, 9))
     {

     memset(number, 0, strlen(number));
     *number='0';
     //printf("浮点0!\n");
     return 1;
     }
     */

    float_striple_pend_zero(number);
    //if (!memcmp(number, "0.0", 4) || !memcmp(number, "-0.0", 5))
    if ((memcmp(number, "0.0", 4)==0) || (0==memcmp(number, "-0.0", 5)))
    {
        memset(number, 0, strlen(number));
        *number = '0';
    }
    return 0;
}

/***
 *
 *
 *      表达式合法性检查,目前工作包括:
 *      1. 对于10*005  =>  10*5的转换
 *      2. 对于浮点0的转换即0.0000000 =>  0
 *
 */

static char exprCheckBuf[30 * 2]; //最大操作数
static char retValStr[640];

int exprValidate(char *expr)
{
    char c;
    int idx = 0;
    int dstIndex = 0;
    //int firstOp = 0;

    memset(retValStr, 0, sizeof(retValStr));

    c = *(expr + idx);
    while (c != '\0') //表达式必须以\0作为结束符
    {
        switch (c)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            memset(exprCheckBuf, 0, sizeof(exprCheckBuf));
            getNextOperandStr(expr, &idx, exprCheckBuf);
            processPrefixZero(exprCheckBuf, sizeof(exprCheckBuf));

            //如果下一个位置还有运算符
            //那么当前数就可以处理了
            if (1 == findNextOp(expr + idx))
            {
                floatZeroToZero(exprCheckBuf);
            }

            memcpy(&retValStr[dstIndex], exprCheckBuf, strlen(exprCheckBuf));
            dstIndex += (int)strlen(exprCheckBuf);
            break;
            
            default:
            if (dstIndex > 0)
            {
                if (isStringSingleOperand(retValStr) == 1) //第一个操作数
                {
                    //printf("我认为这是第一个操作数\n");
                    floatZeroToZero(retValStr);
                }
            }
            dstIndex = (int)strlen(retValStr);
            retValStr[dstIndex] = c;
            dstIndex++;
            idx++;
            break;
        }
        
        c = *(expr + idx);
    }

    memcpy(expr, retValStr, strlen(retValStr) + 1);
    return 0;
}

/*
 *      取下一个操作符(字符串形式)
 **    返回值:     0  遇到字符串结尾
 *                             1  取到一个操作数
 */

int getNextOperandStr(char *str, int *idx, char *floatstr)
{
    char digital;
    int retVal = 0;
    int i = 0;
    
    digital = *(str + *idx);
    while (digital != '\0')
    {
        if (in(digital) == 9)
        {
            floatstr[i] = digital;
            i++;
        }
        else
        {
            retVal = 1;
            break;
        }

        (*idx)++;
        digital = *(str + *idx);
    }

    floatstr[i] = '\0';

    return retVal;
}

//除去前导0, 形如0133==>133, 00005 ==>5
//不能包含符号位!

int processPrefixZero(char *expr, int maxLen)
{
    char outStr[MAX_NUMBER_COUNT];
    int len = (int)strlen(expr);
    int idx = 0;
    int dstIndex = 0;
    
    memset(outStr, 0, sizeof(outStr));
    if (len == 0) //空串
    {
        return 0;
    }
        
    if (1 == isStringContainDot(expr)) //小数  处理多余小数问题
    {
        if (stripleFloatPrecision(expr, 7) == 1)
        {
            processNumberEndWithSevenZero(expr);
        }

        return 0;
    }
    else //整数
    {
        while ((idx < len) && (expr[idx] == '0'))
        {
            idx++;
        }
        if (idx == len) // 0
        {
            outStr[dstIndex] = '0';
            dstIndex++;
        }
        else
        {
            while (idx < len)
            {
                outStr[dstIndex] = expr[idx];
                dstIndex++;
                idx++;
            }
        }
    }

    memcpy(expr, outStr, strlen(outStr) + 1);
    //printf(" 除去前导0 :  %s \n",  expr);
    return 1;
}

/*
 *    从当前位置搜寻下个操作符,如果找到,返回1
 *    否则返回0
 *
 */
int findNextOp(const char *expr)
{
    //考虑单个-的情况
    if ((*expr == '-') && (*(expr + 1) == '\0'))
    {
        return 1;
    }

    if (isStringSingleOperand(expr) == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
 *      end with seven zero ,形如0.0000000
 *
 */
int processNumberEndWithSevenZero(char *expr)
{
    if (strlen(expr) > 7 + 1 + 1)
    {
        if (0 == memcmp( ((expr + strlen(expr)) - 1) - 7, ".0000000", 8))
        {
            floatZeroToZero(expr);
        }
    }
    return 0;
}
