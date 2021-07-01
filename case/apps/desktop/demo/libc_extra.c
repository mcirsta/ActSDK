#include "libc_extra.h"

struct param {
    int width; /**< field width */
    char lz;            /**< Leading zeros */
    char sign;          /**<  The sign to display (if any) */
    char alt;           /**< alternate form */
    char base;  /**<  number base (e.g.: 8, 10, 16) */
    char uc;            /**<  Upper case (for base16 only) */
    char *bf;           /**<  Buffer to output */
};

#ifdef PRINTF_LONG_SUPPORT

static void uli2a(unsigned long int num, struct param *p)
{
    int n = 0;
    unsigned long int d = 1;
    char *bf = p->bf;
    while (num / d >= (unsigned int)p->base)
        d *= p->base;
    while (d != 0) {
        int dgt = num / d;
        num %= d;
        d /= p->base;
        if (n || dgt > 0 || d == 0) {
            *bf++ = dgt + (dgt < 10 ? '0' : (p->uc ? 'A' : 'a') - 10);
            ++n;
        }
    }
    *bf = 0;
}

static void li2a(long num, struct param *p)
{
    if (num < 0) {
        num = -num;
        p->sign = '-';
    }
    uli2a(num, p);
}

#endif

static void ui2a(unsigned int num, struct param *p)
{
    int n = 0;
    unsigned int d = 1;
    char *bf = p->bf;
    while (num / d >= (unsigned int)p->base)
        d *= p->base;
    while (d != 0) {
        int dgt = num / d;
        num %= d;
        d /= p->base;
        if (n || dgt > 0 || d == 0) {
            *bf++ = dgt + (dgt < 10 ? '0' : (p->uc ? 'A' : 'a') - 10);
            ++n;
        }
    }
    *bf = 0;
}

static void i2a(int num, struct param *p)
{
    if (num < 0) {
        num = -num;
        p->sign = '-';
    }
    ui2a(num, p);
}

static int a2d(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    else
        return -1;
}

static char a2i(char ch, const char **src, int base, int *nump)
{
    const char *p = *src;
    int num = 0;
    int digit;
    while ((digit = a2d(ch)) >= 0) {
        if (digit > base)
            break;
        num = num * base + digit;
        ch = *p++;
    }
    *src = p;
    *nump = num;
    return ch;
}

static int putf(FILE *putp, char c)
{
    if (fputc(c, putp) == EOF)
        return 0;
    else
        return 1;
}

static unsigned putchw(FILE *putp, struct param *p)
{
    unsigned written = 0;
    char ch;
    int n = p->width;
    char *bf = p->bf;

    /* Number of filling characters */
    while (*bf++ && n > 0)
        n--;
    if (p->sign)
        n--;
    if (p->alt && p->base == 16)
        n -= 2;
    else if (p->alt && p->base == 8)
        n--;

    /* Fill with space, before alternate or sign */
    if (!p->lz) {
        while (n-- > 0)
            written += putf(putp, ' ');
    }

    /* print sign */
    if (p->sign)
        written += putf(putp, p->sign);

    /* Alternate */
    if (p->alt && p->base == 16) {
        written += putf(putp, '0');
        written += putf(putp, (p->uc ? 'X' : 'x'));
    } else if (p->alt && p->base == 8) {
        written += putf(putp, '0');
    }

    /* Fill with zeros, after alternate or sign */
    if (p->lz) {
        while (n-- > 0)
            written += putf(putp, '0');
    }

    /* Put actual buffer */
    bf = p->bf;
    while ((ch = *bf++))
        written += putf(putp, ch);

    return written;
}


size_t tfp_format(FILE *putp, const char *fmt, va_list va)
{
    size_t written = 0;
    struct param p;
#ifdef PRINTF_LONG_SUPPORT
    char bf[23];
#else
    char bf[12];
#endif
    p.bf = bf;

    char ch;

    while ((ch = *(fmt++))) {
        if (ch != '%') {
            written += putf(putp, ch);
        } else {
            /* Init parameter struct */
            p.lz = 0;
            p.alt = 0;
            p.width = 0;
            p.sign = 0;
#ifdef PRINTF_LONG_SUPPORT
            char lng = 0;
#endif

            /* Flags */
            while ((ch = *(fmt++))) {
                switch (ch) {
                case '0':
                    p.lz = 1;
                    continue;
                case '#':
                    p.alt = 1;
                    continue;
                default:
                    break;
                }
                break;
            }

            /* Width */
            if (ch >= '0' && ch <= '9') {
                ch = a2i(ch, &fmt, 10, &(p.width));
            }
            if (ch == 'l') {
                ch = *(fmt++);
#ifdef PRINTF_LONG_SUPPORT
                lng = 1;
#endif
            }

            switch (ch) {
            case 0:
                goto abort;
            case 'u':
                p.base = 10;
#ifdef PRINTF_LONG_SUPPORT
                if (lng)
                    uli2a(va_arg(va, unsigned long int), &p);
                else
#endif
                    ui2a(va_arg(va, unsigned int), &p);
                written += putchw(putp, &p);
                break;
            case 'd':
            case 'i':
                p.base = 10;
#ifdef PRINTF_LONG_SUPPORT
                if (lng)
                    li2a(va_arg(va, unsigned long int), &p);
                else
#endif
                    i2a(va_arg(va, int), &p);
                written += putchw(putp, &p);
                break;
            case 'x':
            case 'X':
                p.base = 16;
                p.uc = (ch == 'X');
#ifdef PRINTF_LONG_SUPPORT
                if (lng)
                    uli2a(va_arg(va, unsigned long int), &p);
                else
#endif
                    ui2a(va_arg(va, unsigned int), &p);
                written += putchw(putp, &p);
                break;
            case 'o':
                p.base = 8;
                ui2a(va_arg(va, unsigned int), &p);
                written += putchw(putp, &p);
                break;
            case 'c':
                written += putf(putp, (char)(va_arg(va, int)));
                break;
            case 's':
                p.bf = va_arg(va, char *);
                written += putchw(putp, &p);
                p.bf = bf;
                break;
            case '%':
                written += putf(putp, ch);
            default:
                break;
            }
        }
    }
 abort:;

 return written;
}



int vfprintf(FILE *f, const char *fmt, va_list va)
{
    return tfp_format(f, fmt, va);
}

int fprintf(FILE *f, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int rv = vfprintf(f, fmt, va);
    va_end(va);
    return rv;
}
