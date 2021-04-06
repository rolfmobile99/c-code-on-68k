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

// copied from my_itoa.c (based on itoa.c from K&R C manual - 2nd edition)
// -rolf mar 2021
//
// note: porting to Tricky68k sim
//
//  - printf replaced with minprintf
//


//#include <stdio.h>
//#include <string.h>
#include <stdarg.h>     /* needed for varargs use below */


#define USE_SIM 1

// printf and putchar normally defined in stdio.h
//#define NULL 0
void minprintf(char *fmt, ...);
int putchar(int c);


#ifdef NOTDEF

/* strcmp:  return <0 if s<t, 0 if s==t, >0 if s>t */
/*  sec 5.5, p.78 */
/*  note: type char * changed to const char *  */

int knr_strcmp(const char *s, const char *t)
{
    for ( ; *s == *t; s++, t++)
        if (*s == '\0')
            return 0;
    return *s - *t;
}

/* strcpy:  copy t to s; pointer version 2 */
/*  sec 5.5, p.78 */
/*  note: type char * changed to const char *, the copy direction is sort of backwards! */

void knr_strcpy(char *s, const char *t)
{
    while ((*s++ = *t++) != '\0')
        ;
}
#endif

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


/* test itoa function */
int main(int argc, char *argv[])
{
    int tests[5] = {3, 99, 256, -100, 65536};
    char str[16];
    int i;
    
    for (i = 0; i < sizeof(tests)/sizeof(int); i++) {
        knr_itoa(tests[i], str);
        minprintf("%s\n", str);         // behaves like a minimal printf()
    }

    return 0;
}


/*** below are library functions in C ***/

#if USE_SIM
#define PUTCHAR_NEWLINE_CHAR 0xa
#endif

/*
    minprintf() - like printf but only allows %s in the format string.
    
    note: originally based on "minprintf()" as described in K&R C Prog Lang, section 7.2
    
    note: on the simulator this also converts the string "\n" to a LF char
*/
void minprintf(char *fmt, ...)
{
	va_list ap;     // pointer to args, if any
	char *p, *sval;
	char c;
	int slashflag = 0;
	//int ival;
	//double dval;

	va_start(ap, fmt);
	for (p = fmt; (c = *p); p++) {

#if USE_SIM
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
#if 0
		case 'd':
			ival = va_arg(ap, int);     // extract arg of 'int' type
			printf("%d", ival);
			break;
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


#if USE_SIM

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
