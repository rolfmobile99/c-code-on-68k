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
// note: porting to 68k nano board
//
//  - printf doesn't exist, so comment it for now
//


//#include <stdio.h>
//#include <string.h>


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
        //printf("%s\n", str);
    }

    return 0;
}
