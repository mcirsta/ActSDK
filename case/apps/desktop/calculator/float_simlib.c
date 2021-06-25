/*
 ********************************************************************************
 *                         uSDK130
 *
 *
 *
 *                     (c) Copyright 2002-2003, Actions Co,Ld.
 *                                       All Right Reserved
 *
 * File   :  float_simlib.c
 * By     :  ywwang
 * Purpose:   ���ö���ģ�⸡������
 *
 * Version:       first version     2006-12-5
 ********************************************************************************
 ********************************************************************************
 */

/*******************************************************************************
 *
 *    ģ���ṩ�����б�:
 *
 ********************************************************************************/

/*
 *     ------------------------------------------------------
 *	|  ����λ|  ��������|С������|
 *	------------------------------------------------------
 *
 *
 */

//#include "stdafx.h"


#include "float_simlib.h"
#include "string.h"

static int setFloatPrecision(float_type *x, int n);
int float_cmp_unsigned(const float_type *x, const float_type *y);




/************************************
 *   ���������  x*y
 *  �ؼ���С�����ֵĶ���
 * **********************************
 */

int float_add(float_type *x, float_type *y, float_type *result)
{

    float_type x_cpy;
    float_type y_cpy;
    int cycleLeftLen = 0;
    int resultDecLen = 0;
    memcpy(&x_cpy, x, sizeof(float_type));
    memcpy(&y_cpy, y, sizeof(float_type));

    if (x_cpy.declen > y_cpy.declen)
    {
        resultDecLen = x_cpy.declen;
        cycleLeftLen = x_cpy.declen - y_cpy.declen;
        BigInteger_shiftleft(&(y_cpy.bInteger), cycleLeftLen);
    }
    else if (x_cpy.declen < y_cpy.declen)
    {
        resultDecLen = y_cpy.declen;
        cycleLeftLen = y_cpy.declen - x_cpy.declen;
        BigInteger_shiftleft(&(x_cpy.bInteger), cycleLeftLen);
    }
    else
        resultDecLen = x_cpy.declen;

    BigInteger_add(&x_cpy.bInteger, &y_cpy.bInteger, &result->bInteger);
    result->declen = resultDecLen;
    return 1;
}

/** ���������
 *
 *    x-y  = x+ (-y)
 */
int float_dec(float_type *x, float_type *y, float_type *result)
{
    float_type x_cpy;
    float_type y_cpy;
    
    memcpy(&x_cpy, x, sizeof(float_type));
    memcpy(&y_cpy, y, sizeof(float_type));
    //printf("in sub  calculation .................\n");
    if (y_cpy.bInteger.sbit == 0)
    {
        y_cpy.bInteger.sbit = 1;
    }
    else
    {
        y_cpy.bInteger.sbit = 0;
    }
    
    return float_add(&x_cpy, &y_cpy, result);
}

/**
 *   ���������  x*y
 *
 */
int float_mul(float_type *x, float_type *y, float_type *result)
{
    int resultDecLen = 0;
    resultDecLen = x->declen + y->declen;
    BigInteger_mul(&x->bInteger, &y->bInteger, &result->bInteger);
    if (resultDecLen > MAX_DECIMAL_LEN) //����С����󸡵�λ��
    {
        BigInteger_shiftright(&result->bInteger, resultDecLen - MAX_DECIMAL_LEN);
        resultDecLen = MAX_DECIMAL_LEN;
    }
    result->declen = resultDecLen;
    return 1;
}

/**
 *
 *    ���������x/y
 *
 */

int float_div(float_type *x, float_type *y, float_type *result)
{
    int resultDecLen;
    float_type left;
    float_type right;
    float_type zero;
    int flag = 0;
    int sbit;
    
    if (x->bInteger.sbit == y->bInteger.sbit)
    {
        sbit = 0;
    }
    else
    {
        sbit = 1;
    }
    getFloatFromStr("00", &zero);

    flag = float_cmp_unsigned(&zero, y);
    if (flag == 0)
    {
        memset(result, 0, sizeof(float_type));
        return 0;
    }

    resultDecLen = x->declen - y->declen;
    memcpy(&left, x, sizeof(float_type));
    memcpy(&right, y, sizeof(float_type));
    //��߼���ľ���
    if (left.declen < MAX_DECIMAL_LEN)
    {
        BigInteger_shiftleft(&left.bInteger, MAX_DECIMAL_LEN - left.declen);
        left.declen = MAX_DECIMAL_LEN;
    }
    if (right.declen < MAX_DECIMAL_LEN)
    {
        BigInteger_shiftleft(&right.bInteger, MAX_DECIMAL_LEN - right.declen);
        right.declen = MAX_DECIMAL_LEN;
    }
    BigInteger_shiftleft(&left.bInteger, MAX_DECIMAL_LEN);
    BigInteger_div(&left.bInteger, &right.bInteger, &result->bInteger);

    result->declen = MAX_DECIMAL_LEN;
    result->bInteger.sbit = (char)sbit;
    return 1;
}

/*******************************************
 *    �����ַ������ɸ������ڲ���ʾ��ʽ
 *
 *******************************************
 */

int getFloatFromStr(char *str, float_type *value)
{
    int i = 0;
    int j = 0;
    char cChar;    
    int len;
    
    if (str == 0)
    {
        return 0;
    }    
    
    len = (int)strlen(str);    
    memset(value, 0, sizeof(float_type));

    while (i < len)
    {
        if (*(str + i) == '.')
        {
            break;
        }
        i++;
    }
    if (i == len) //������
    {
        value->declen = 0;
        getBigIntegerFromStr(str, &value->bInteger);
    }
    else //������
    {
        value->declen = (len - i) - 1; //С����λ��
        i = 0;
        if (*(str + i) == '-') //ȷ������λ
        {
            value->bInteger.sbit = 1;
            i++;
        }
        else if (*(str + i) == '+')
        {
            value->bInteger.sbit = 0;
            i++;
        }
        else
        {
            value->bInteger.sbit = 0;
        }

        len = len - 1; //�����һ�����ֿ�ʼ�������
        j = MAX_NUMBER_COUNT - 1; //���λ������
        while ((len >= 0) && (j >= 0))
        {
            cChar = *(str + len);
            if (cChar == '-')
            {
                len--;
                continue;
            }
            if (cChar != '.')
            {
                value->bInteger.bytesArray[j] = cChar - '0';
                j--;
            }
            len--;
        }
    }

    return 1;
}

/*******************************************
 *    ���ݸ��������ɸ��������ַ���
 *
 *******************************************
 */
int getStrFromFloat(char *str, float_type *value)
{
    int idx = 0;
    int i;
    char cChar;    
    int declen = value->declen;

    while (idx < (MAX_NUMBER_COUNT - declen)) //��ȥ���λ��0
    {
        cChar = value->bInteger.bytesArray[idx];
        if (cChar != 0)
        {
            break;
        }
        idx++;
    }
    
    i = 0;
    if (value->bInteger.sbit == 1) //���������
    {
        *(str + i) = '-';
        i++;
    }

    if (idx == (MAX_NUMBER_COUNT - declen)) //��������Ϊ0
    {
        str[i] = '0';
        i++;
    }
    else
    {
        while (idx < (MAX_NUMBER_COUNT - declen)) //��������
        {
            cChar = value->bInteger.bytesArray[idx];
            cChar += '0';
            *(str + i) = cChar;
            i++;
            idx++;
        }
    }
    if (value->declen > 0)
    {
        *(str + i) = '.';
        i++;
    }
    while (idx < MAX_NUMBER_COUNT) //С������
    {
        *(str + i) = value->bInteger.bytesArray[idx] + '0';
        idx++;
        i++;
    }
    *(str + i) = '\0';

    //printf("output of getStrFromFloat  %s   \n", str);
    float_striple_pend_zero(str);
    
    return 1;
}


/**
 *      �Ƚ�x,y����ֵ��С��
 *      ����ֵ��x>y, ����1�� x==y,����0�����򷵻�-1
 *
 */
int float_cmp_unsigned(const float_type *x, const float_type *y)
{
    int shiftCycle = 0;
    BigInteger a;
    BigInteger b;
    
    memcpy(&a, &x->bInteger, sizeof(BigInteger));
    memcpy(&b, &y->bInteger, sizeof(BigInteger));
    //����λ������С��λ
    if (x->declen > y->declen)
    {
        shiftCycle = x->declen - y->declen;
        BigInteger_shiftleft(&b, shiftCycle);
    }
    
    if (x->declen < y->declen)
    {
        shiftCycle = y->declen - x->declen;
        BigInteger_shiftleft(&a, shiftCycle);
    }

    return BigInteger_cmp_unsigned(&a, &b);
    //�ٱȽ��������ֵĴ�С
}

/**
 *      ����㴮�ж���0��ȥ
 *
 *
 */
int float_striple_pend_zero(char * floatStr)
{
    int len;
    int isFloat = 0;
    char *tmp = floatStr;

    //print_info("input of float_striple_pend_zero =%s \n", floatStr);
    while (*tmp != '\0')
    {
        if (*tmp == '.')
        {
            isFloat = 1;
        }
        tmp++;
    }
    
    if (isFloat == 1)
    {
        len = (int)strlen(floatStr) - 1;
        while (len > 0)
        {
            if ((*(floatStr + len) == '0') && (*(floatStr + len - 1) != '.'))
            {
                *(floatStr + len) = '\0';
            }
            else
            {
                break;
            }

            len--;
        }
    }
    //print_info("output of float_striple_pend_zero =%s \n", floatStr);
    return 1;
}


int float_sqrt(const float_type *x_arg, float_type *result)
{
    float_type min;
    float_type max;
    float_type mid;
    float_type gap;
    float_type tmp;
    float_type delta;
    float_type c;
    float_type adjust;
    float_type *x;
    float_type xStatck;
    int flag;
    int magLattitude = 0;
    int mag = 0; //�Ƿ�Ŵ��� 
    char resultstr[100];  
    
      
    x = &xStatck;
    memcpy(x, x_arg, sizeof(float_type));

    if (float_isZero(x) == 1)
    {
        //print_info("return because of 0 \n");
        memset(result, 0, sizeof(float_type));
        return 1;
    }

    getFloatFromStr("9999999.9999999", &max); //��֧�ֵ���󿪷���
    flag = float_cmp_unsigned(x, &max);
    if (flag == 1)
    {
        memset(result, 0, sizeof(float_type));
        return 0;
    }

    getFloatFromStr("0.0001", &min); //min=1;
    flag = float_cmp_unsigned(&min, x);

    //����С�����ȷŴ�100000000��
    if (flag == 1)
    {
        mag = 1;
        magLattitude = 1;
        getFloatFromStr("100000000", &tmp);
        float_mul(&tmp, x, &c);
        memcpy(x, &c, sizeof(float_type));
        getStrFromFloat(resultstr, &c);
    }
    else
    {
        getFloatFromStr("1.000000", &min); //min=1;
        //����С�����ȷŴ�10000��
        flag = float_cmp_unsigned(&min, x);
        if (flag == 1)
        {
            mag = 1;
            magLattitude = 2;
            getFloatFromStr("10000", &tmp);
            float_mul(&tmp, x, &c);
            memcpy(x, &c, sizeof(float_type));
            getStrFromFloat(resultstr, &c);
            //print_info("�Ŵ�10^4��x = %s \n", resultstr);
        }
    }

    getFloatFromStr("0.001", &delta);
    getFloatFromStr("0.0005", &adjust); //���ھ���֮�����΢��
    getFloatFromStr("2.000000", &c);
    getFloatFromStr("1.000000", &min); //min=1;
    memcpy(&max, x, sizeof(float_type)); //max=x;
    memcpy(&mid, &min, sizeof(float_type)); //mid=min;
    float_mul(&min, &min, &tmp);
    float_dec(x, &tmp, &gap);
    flag = float_cmp_unsigned(&gap, &delta);
    while (flag == 1)
    {
        float_add(&min, &max, &tmp);
        //		getStrFromFloat(minstr, &tmp);
        //		printf(" a+b= %s \n",minstr);
        float_div(&tmp, &c, &mid);

        //		getStrFromFloat(minstr, &min);
        //		getStrFromFloat(maxstr, &max);
        //		getStrFromFloat(resultstr, &mid);
        //		printf("(%s +%s) /2=%s \n", minstr, maxstr, resultstr);
        float_mul(&mid, &mid, &tmp); //tmp=mid*mid

        //		getStrFromFloat(minstr, &tmp);
        //		printf(" %s * %s =%s \n", resultstr, resultstr, minstr);

        flag = float_cmp_unsigned(&tmp, x); // mid*mid > x  ?

        if (flag == 1)
        {
            memcpy(&max, &mid, sizeof(float_type)); //max=mid
        }
        
        if (flag == -1)
        {
            memcpy(&min, &mid, sizeof(mid)); //min=mid
        }
        float_dec(&tmp, x, &gap); //gap=mid*mid-x
        flag = float_cmp_unsigned(&gap, &delta);

        //       getStrFromFloat(resultstr, &mid);
        //	   getStrFromFloat(resultstr, &gap);
        //	   printf("gap=%s \n", resultstr);
    }
    float_add(&mid, &adjust, &mid);

    //����Ŵ��ˣ���Ҫ��С
    if (mag == 1 )
    {
        if (magLattitude == 1)
        {
            getFloatFromStr("10000", &tmp);
        }
        else
        {
            getFloatFromStr("100", &tmp);
        }
        
        float_div(&mid, &tmp, &mid);
        getStrFromFloat(resultstr, &mid);
        //print_info("result x = %s \n", resultstr);
    }
    setFloatPrecision(&mid, 3);
    memcpy(result, &mid, sizeof(float_type));
    result->bInteger.sbit = 0;
    //	getStrFromFloat(resultstr, &mid);

    //	printf("square root is %s \n\n", resultstr);
    return 1;
}

static int setFloatPrecision(float_type *x, int n)
{

    if (n >= MAX_DECIMAL_LEN)
    {
        return 0;
    }
    
    if (x->declen < n)
    {
        BigInteger_shiftleft(&x->bInteger, n - x->declen);
    }
    else
    {
        BigInteger_shiftright(&x->bInteger, x->declen - n);
    }
    x->declen = n;
    
    return 1;
}

/**
 *
 *      ����,�����������0,���򷵻�1
 */
int float_reverse(float_type *x, float_type *result)
{

    float_type one;
    getFloatFromStr("1", &one);
    return float_div(&one, x, result);
}

/**
 *      x%y
 *      ����������, �������������С�����֣���С�����ֱ��ض�
 */

int float_mod(float_type *x, float_type *y, float_type *result)
{

    float_type tmp;
    
    //���Ƚ�ȥ�������е�С������
    if (x->declen > 0)
    {
        BigInteger_shiftright(&x->bInteger, x->declen);
        x->declen = 0;
    }
    if (y->declen > 0)
    {
        BigInteger_shiftright(&y->bInteger, y->declen);
        y->declen = 0;
    }
    float_div(x, y, &tmp);

    if (tmp.declen > 0) //��С������,��ʾ������
    {
        BigInteger_shiftright(&tmp.bInteger, tmp.declen);
        tmp.declen = 0;
    }
    BigInteger_mul(&tmp.bInteger, &y->bInteger, &result->bInteger);
    BigInteger_sub(&x->bInteger, &result->bInteger, &tmp.bInteger);
    memcpy(result, &tmp, sizeof(float_type));
    result->declen = 0;
    return 1;
}

/**
 *      �����0������1�����򷵻�0
 *
 */
int float_isZero(const float_type *x)
{
    int flag;
    float_type zero;
    
    getFloatFromStr("00", &zero);
    flag = float_cmp_unsigned(&zero, x);
    if (flag == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
