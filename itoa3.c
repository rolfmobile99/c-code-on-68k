//
// itoa3.c
//

// itoa - convert int to string - a naive version of libc's 'itoa'
//
//  this performs a conversion much like the "%d" format of printf().
//
// from example code in C book:
//      sec 3.6 Loops, p.48
//
// note: brackets may be added for clarity.
// note: some functions preceded with 'knr_' to avoid conflicts with standard libraries
//
// refs:
//      strlen
//
// revisions:
// take main code from itoa2, and add in minprintf that supports %d and %x
// -rolf - apr 4, 2021
//
// copied from my_itoa.c (based on itoa.c from K&R C manual - 2nd edition)
// -rolf mar 2021
//
// note: porting to Tricky68k sim
//
//  - printf replaced with minprintf
//


#define USE_SIM 1       /* comment this out for non-simulator build */

//#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */

#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#include <stdarg.h>

// printf and putchar normally defined in stdio.h
//#define NULL 0
void minprintf(char *fmt, ...);

#ifndef USE_STDIO_H
//#define NULL 0
int putchar(int c);     // normally defined in stdio.h, stub below
#endif



/* test itoa function */
int main(int argc, char *argv[])
{
    const int tests[6] = {3, 99, 256, -100, 65536, 2147483647};  /* last value is 2^31-1 */
    int i;
    
    for (i = 0; i < sizeof(tests)/sizeof(int); i++) {
        minprintf("%d (0x%x)\n", tests[i], tests[i]);         // behaves like a minimal printf()
    }

    return 0;
}


/*** below are library functions in C ***/


/* strlen:  return length of s */
/*  example code from sec 2.3, p.31 */
/*  note: ++i changed to i++ */

int knr_strlen(char s[])
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}


/* strlen:  taken from newlib - use for comparison */
int newlib_strlen(const char *str)
{
    const char *start = str;
    while (*str) {
        str++;
    }
    return str - start;
}


/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = knr_strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


/* itoa:  convert integer n to characters in s */
void knr_itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */

    i = 0;
    do {                /* generate digits in reverse order */
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
        s[i++] = '-';

    s[i] = '\0';
    reverse(s);
}


/* unsigned, base 16 (hex) version of above itoa function */
void ux_itoa(unsigned int u, char s[])
{
    int i, rem;
    const char digits[] = "0123456789ABCDEF";  /* lookup table for hex digits */

    i = 0;
    do {                /* generate digits in reverse order */
        rem = u % 16;
        s[i++] = digits[rem];
        u = u / 16;
    } while (u != 0);

    s[i] = '\0';
    reverse(s);
}


/* only signed base 10 and unsigned base 16 are supported */
void my_itoa(int n, char s[], int base)
{
    if (base == 16) {
        ux_itoa((unsigned int)n, s);
    } else {
        knr_itoa(n, s);
    }
}


#ifdef USE_SIM
#define PUTCHAR_NEWLINE_CHAR 0xa
#endif

/*
    minprintf() - like printf but only allows %s and %d in the format string.
    
    note: originally based on "minprintf()" as described in K&R C Prog Lang, section 7.2
    
    note: on the simulator this also converts the string "\n" to a LF char
*/
void minprintf(char *fmt, ...)
{
	va_list ap;     // pointer to args, if any
	char *p, *sval;
	char c;
	int ival;
	//double dval;
	char sbuf[12];
#ifdef USE_SIM
	int slashflag = 0;
#endif

	va_start(ap, fmt);
	for (p = fmt; (c = *p); p++) {

#ifdef USE_SIM
	    if (!slashflag && (c == '\\')) {    // code to handle "\n" in string, on simulator or gcc -S
	        slashflag = 1;
	        continue;
	    }
	    if (slashflag) {
			slashflag = 0;
	        if (c == 'n') {
			    putchar(PUTCHAR_NEWLINE_CHAR);
			    continue;
			} else {
			    putchar('\\');
			    putchar(c);
			    continue;
			}
	    }
#endif

		if (c != '%') {
			putchar(c);
			continue;
		}

		switch(*++p) {                  // handle char that follows '%'
		case 'd':
			ival = va_arg(ap, int);     // extract arg of 'int' type
			//printf("%d", ival);
			sbuf[0] = '\0';             // be safe
			my_itoa(ival, sbuf, 10);
			for (sval = sbuf; *sval; sval++)
				putchar(*sval);
			break;
		case 'x':
			ival = va_arg(ap, int);     // extract arg of 'int' type
			//printf("%d", ival);
			sbuf[0] = '\0';             // be safe
			my_itoa(ival, sbuf, 16);
			for (sval = sbuf; *sval; sval++)
				putchar(*sval);
			break;
#if 0
		case 'f':
			dval = va_arg(ap, double);  // extract arg of 'double' type
			printf("%f", dval);
			break;
#endif
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++)
				putchar(*sval);
			break;
		default:
			putchar(*p);
			break;
		}
	}
	va_end(ap);
}


/* taken from MIT xv6 string.c */
/* note: changed 'uint n' to 'int n' */
void*
memmove(void *dst, const void *src, int n)
{
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, const void *src, int n)
{
  return memmove(dst, src, n);
}


#ifndef USE_STDIO_H

int putchar(int c)
{
/***
    ; on simulator, can do "putchar" by writing a 16-bit int to address 0xFFE000
    move.l  4(sp),d0
    move.w  d0,$FFE000
    rts
***/
    return c;
}

#endif
