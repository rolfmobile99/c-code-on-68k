//
// hello2.c - improved hello world in C, for the Tricky68k sim
//
// this version uses a minimal version of printf().
// it handles the \n issue mentioned below inside this function.
//
// -rolf mar 2021
//
// DEPENDENCY:
//  - putchar()
//
// ISSUES:
// - '\n' char in a string is not treated correctly by the ASM code when targeting Tricky68k sim.
//  (it should be converted to a LF (0xa), although CR (0xd) works too)
//


//#include <stdio.h>
//#include <string.h>
#include <stdarg.h>     /* needed for varargs use below */


#define USE_SIM 1

//#define NULL 0
int putchar(int c);     // normally defined in stdio.h


#if USE_SIM
#define PUTCHAR_NEWLINE_CHAR 0xa
#else
#define PUTCHAR_NEWLINE_CHAR '\n'
#endif


int touched = 0;

/*
    minprintf() - like printf but only allows %s in the format string.
    
    note: originally based on "minprintf()" as described in K&R C Prog Lang, section 7.2
    
    note: this also converts \n to a LF char
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
	        touched = 1;
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


/* test print function */
int main(int argc, char *argv[])
{
    minprintf("hello!\n");

    minprintf("%s\n", "world");

    if (touched)
        minprintf("touched\n");     // indicates that special code above was run

    return 0;
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
