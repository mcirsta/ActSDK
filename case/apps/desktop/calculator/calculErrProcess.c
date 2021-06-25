#include "calcul_play.h"
#include "Float_simlib.h"

int stripleFloatPrecision(char *str, int num);
static int isop(char c);
int maxNumberCheck(const char *expstr);



void dealInputError(int errNo)
{
    switch (errNo)
    {
        case ERR_DOUBLE_OPERATOR:
        break;
        case ERR_MISSING_OPERATOR:
        //print_info(" 缺少运算符\n");
        break;
        case ERR_MISSING_DECIAML:
        //print_info("小数点后面小数部分缺失\n");
        break;
        case ERR_MSVALUE_NOT_VALID:
        //print_info("该值无法放入寄存器\n");
        break;
        default:
        //print_info("未知错误\n");
        break;
    }
}

//检查输入的表达式
//如果无错，返回1，否则返回0
int expressionCheck(char *expstr)
{
    int idx = 0;
    char c;
    char dststr[MAX_STR_LEN];
    char *p;

    if ((!expstr) || (strlen(expstr) > MAX_STR_LEN - 1))
    {
        return 0;
    }

    memset(dststr, 0, sizeof(dststr));
    p = expstr;
    //先除去所有空白字符
    c = *p;
    while (c != '\0')
    {
        if (c != ' ')
        {
            dststr[idx] = c;
            idx++;
        }
        p++;
        c = *p;
    }
    //扫描有无语法错误

    //去除多于小数位数，最多只能保留三位
    if(idx > MAX_STR_LEN)
    {
        return 0;	
    }
    if ((idx + 1) < MAX_STR_LEN)
    {
        memcpy(expstr, dststr, (unsigned int)(idx + 1));
    }
    stripleFloatPrecision(expstr, MAX_DECIMAL_LEN);
    //print_info("after expresssion check:  %s \n", expstr);

    if (maxNumberCheck(expstr) == 0) //表达式错误
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 *
 *检查表达式，对于小数点后小数超过7位的截断
 */
int stripleFloatPrecision(char *str, int num)
{
    char *p = str;
    char *p2;
    char *tmpP;
    char c;
    char modifiedFlag = 0;
    int count = 0;
    char afterDecimal = 0; //遇到了小数点

    c = *p;
    while (c != '\0')
    {
        if (c == '.')
        {
            count = 0;
            afterDecimal = 1;
            p++;
            //continue;
            goto c_p_add;
        }
        
        if (afterDecimal == 1) //对小数点后整数个数计数
        {
            if ((c == '0') || (c == '1') || (c == '2') || (c == '3') || (c == '4') ||  
                (c == '5') || (c == '6') || (c == '7') || (c == '8') || (c == '9') 
               )
            {
                count++;
            }
            else
            {
                afterDecimal = 0;
            }
        }
        
        if (count > num)
        {
            modifiedFlag = 1;
            p2 = p;
            tmpP = p;
            c = *p2;
            while (c != '\0')
            {
                if ((c == '+') || (c == '-') || (c == '*') || (c == '/'))
                {
                    break;
                }
                p2++;
                c = *p2;
            }
            
            while (*p2 != '\0')
            {
                *p = *p2;
                p++;
                p2++;
            }
            *p = '\0';
            
            //恢复p
            p = tmpP;
            afterDecimal = 0;
            count = 0;
            p--;
        }
        
        p++;
        
        c_p_add:    
        c = *p;
    }
    //print_info("after processing str:     %s \n", str);
    return modifiedFlag;
}

/**
 *
 *  检查有无超出计算范围的数，如果有，返回0
 *  没有则返回1
 *
 */
int maxNumberCheck(const char *expstr)
{
    int startCount = 0;
    char c; 
      
    if (NULL == expstr)
    {
        return 0;
    }

    c = *expstr;
    while (c != '\0')
    {
        if (isop(c) == 1)
        {
            if (startCount > MAX_NUM_SUPPORT)
            {
                return 0;
            }
            else
            {
                startCount = 0;
                //continue;
                goto c_add;
            }
        }

        if (startCount > MAX_NUM_SUPPORT)
        {
            return 0;
        }
        if (c != '.')
        {
            startCount++;
        }
    
        c_add:    
        expstr++;
        c = *expstr;
    }
    
    if (startCount > MAX_NUM_SUPPORT)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//如果是运算符，返回1
static int isop(char c)
{
    if ((c == '+') || (c == '-') || (c == '*') || (c == '/'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

