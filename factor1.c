/*
  Example of compiling C code for 68000. Doesn't require any C library
  or other run-time code except libgcc. Relies on the Motorola TUTOR
  monitor program.

  3/22/2021 - simplified. now based on printf and putchar directly.  (rolf)
  - fixed typo in printf (this generates n^3, not n^4 as was stated).
  - increased loop to 13 (13! is about 1.9B, still within a signed 32-bit value)
*/

#define USE_SIM 1       /* comment this out for non-simulator build */

#ifndef USE_SIM
#include <stdio.h>
#endif

#include <stdarg.h>

// printf and putchar normally defined in stdio.h
//#define NULL 0
void minprintf(char *fmt, ...);

#ifdef USE_SIM
//#define NULL 0
int putchar(int c);     // normally defined in stdio.h
#endif


int factorial(const int n);


int main()
{
    //printf("sizeof(int) %d\n", (int)sizeof(int));

    minprintf("n  n^2  n^3  n!\n");
    
    for (int i = 1; i <= 13; i++) {      // note: 1..13 should work!
        minprintf("%d", i);
        putchar(' ');
        minprintf("%d", i*i);
        putchar(' ');
        minprintf("%d", i*i*i);
        putchar(' ');
        minprintf("%d\n", factorial(i));
    }
}


//
// Calculate factorial of a number, recursively.
// Note: the definition of factorial states that 0! = 1
//
int factorial(const int n)
{
    if (n <= 0) {
        return 1;
    } else {
       return(n * factorial(n - 1));
    }
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


#if USE_SIM
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
	int slashflag = 0;
	int ival;
	//double dval;
	char sbuf[12];

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
		case 'd':
			ival = va_arg(ap, int);     // extract arg of 'int' type
			//printf("%d", ival);
			sbuf[0] = '\0';             // be safe
			knr_itoa(ival, sbuf);
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


#ifdef USE_SIM

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
