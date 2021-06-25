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
 *   ������ջ
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
 *  �����ջ
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

    int result; //������
    int errFlag; // ���ʽ����������
} calcObj;

/*
 *  ���ȼ���
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
 *    ���ز����������ȼ����е����
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
 *     ��ջ������
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
 * ��ջ������
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
 *   ����ǰ��������ջ�в������Ƚ�: ����� c > f->op , c<f->op, c=c->op�е�һ��
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
 *  exprΪ��Ҫ����ı��ʽ ���� 127.0 * 456 -123 +37.8
 *
 */
int calc(char *expr, float_type *retVar)
{
    float_type result;
    OperandStack s;
    OperatorStack f;
    char tmpStr[MAX_NUMBER_COUNT];
    char c;
    int meetOperator; // ǰһ���ǲ��ǲ�����

    int idx = 0; //ָʾ��ǰ�����˱��ʽ�ַ����еڼ����ַ�

    /// ��ʼ���������
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
    //��ʼ����*/
    c = *(expr + idx);
    while (c != '\0') //���ʽ������\0��Ϊ������
    {
        float_type t;
        float_type p, q;
        char r;

        if (in(c) == 9) //�����˲�������һ���֣�ȡ���ӵ�������ջ
        {
            getNextOperand(expr, &idx, &t);
            s_push(&s, t);
            meetOperator = 0;
        }
        else if ((in(c) == 1) && meetOperator) //����
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
                case '<': //ջ���ȵ�ǰ���������ȼ��ͣ�������ջ
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

                case '>': //ջ�����������ȼ��߼���
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
    
    //while ((c == '\0') && (*(f.top - 1) != '#'))  //QAC ��ͨ��
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

// ȡ��һ��������
int getNextOperand(char *str, int *idx, float_type *t)
{
    char digital;
    char floatstr[32]; //���渡������ָ��
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

//��src�б��ʽת��Ϊ������ʾ
//���ת����������1�����򷵻�0
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

    //����ɨ�������ַ��������Ƿ���������
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
    //src��һ������������ת��Ϊ��ѧ������
    //����˼·�����ҵ�С���㣬����ȷ������
    //��С�����ֵĳ���

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

    if (*src == '-') //������
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
    //printf(" ��ѧ������ת��������: %s \n", dst);
    float_striple_pend_zero(dst);

    idx = (int)strlen(dst);
    *(idx + dst) = 'E';
    idx++;
    *(idx + dst) = '+';
    idx++;
    memcpy(dst + idx, expr, strlen(expr) + 1);
    //printf("��ѧ������ĩβ0�����: %s \n", dst);
    return 1;
}

/*
 *      �ж�ĳ�����ʽ�Ƿ�ֻ��һ�����������������κ�
 *     �����,����ǵ���������������1,���򷵻�0
 *
 */
int isStringSingleOperand(const char *src)
{
    char c;
    int retFlag = 1;
    int idx = 0;
    
    //����ɨ�������ַ��������Ƿ���������
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
 *     ���ĳ�����Ƿ����С����(С��)������з���1,���򷵻�0
 *
 *   ע��!!!:  src�����ǺϷ��������ַ���
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

//����0ת��ΪС��0������0.0000000  ->  0
//ע��number�����ǺϷ��ĸ�����

int floatZeroToZero(char * number)
{
    /*
     int retFlag=0;
     char floatZero[]="0.0000000";
     if(!memcmp(floatZero, number, 9))
     {

     memset(number, 0, strlen(number));
     *number='0';
     //printf("����0!\n");
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
 *      ���ʽ�Ϸ��Լ��,Ŀǰ��������:
 *      1. ����10*005  =>  10*5��ת��
 *      2. ���ڸ���0��ת����0.0000000 =>  0
 *
 */

static char exprCheckBuf[30 * 2]; //��������
static char retValStr[640];

int exprValidate(char *expr)
{
    char c;
    int idx = 0;
    int dstIndex = 0;
    //int firstOp = 0;

    memset(retValStr, 0, sizeof(retValStr));

    c = *(expr + idx);
    while (c != '\0') //���ʽ������\0��Ϊ������
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

            //�����һ��λ�û��������
            //��ô��ǰ���Ϳ��Դ�����
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
                if (isStringSingleOperand(retValStr) == 1) //��һ��������
                {
                    //printf("����Ϊ���ǵ�һ��������\n");
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
 *      ȡ��һ��������(�ַ�����ʽ)
 **    ����ֵ:     0  �����ַ�����β
 *                             1  ȡ��һ��������
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

//��ȥǰ��0, ����0133==>133, 00005 ==>5
//���ܰ�������λ!

int processPrefixZero(char *expr, int maxLen)
{
    char outStr[MAX_NUMBER_COUNT];
    int len = (int)strlen(expr);
    int idx = 0;
    int dstIndex = 0;
    
    memset(outStr, 0, sizeof(outStr));
    if (len == 0) //�մ�
    {
        return 0;
    }
        
    if (1 == isStringContainDot(expr)) //С��  �������С������
    {
        if (stripleFloatPrecision(expr, 7) == 1)
        {
            processNumberEndWithSevenZero(expr);
        }

        return 0;
    }
    else //����
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
    //printf(" ��ȥǰ��0 :  %s \n",  expr);
    return 1;
}

/*
 *    �ӵ�ǰλ����Ѱ�¸�������,����ҵ�,����1
 *    ���򷵻�0
 *
 */
int findNextOp(const char *expr)
{
    //���ǵ���-�����
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
 *      end with seven zero ,����0.0000000
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
