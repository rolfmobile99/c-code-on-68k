//
// hello1.c - hello world in C, for the Tricky68k sim
//
// this version uses a custom function print_str() instead of printf().
//
// -rolf mar 2021
//
// DEPENDENCY:
//  - putchar()
//
// ISSUES:
// - '\n' char in a string is not treated correctly by the ASM code when run on Tricky68k sim.
//  (it should be converted to a LF (0xa), although CR (0xd) works too)
//


//#include <stdio.h>
//#include <string.h>


#define USE_SIM 1

//#define NULL 0
int putchar(int c);     // normally defined in stdio.h



void print_str(const char s[])
{
	char c;

#if 0
    int i;
    for (i = 0; (c = s[i]) != '\0'; i++) {
        putchar(c);
    }
#elif 0
    for (const char *p = s; (c = *p); p++) {
        putchar(c);
    }
#else
    const unsigned char *p = (const unsigned char *)s;
    for (; (c = *p); p++) {
        putchar(c);
    }
#endif
}


#if 0
void minprintf(char *fmt, ...)
{
	va_list ap;     // pointer to args, if any
	char *p, *sval;
	int ival;
	double dval;

	va_start(ap, fmt);
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		switch(*++p) {
		case 'd':
			ival = va_arg(ap, int);  // extract arg of 'int' type
			printf("%d", ival);
			break;
		case 'f':
			dval = va_arg(ap, double);
			printf("%f", dval);
			break;
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
#endif



/* test print function */
int main(int argc, char *argv[])
{
    print_str("hello!\n");

    return 0;
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
