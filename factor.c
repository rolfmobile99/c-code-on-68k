/*
  Example of compiling C code for 68000. Doesn't require any C library
  or other run-time code except libgcc. Relies on the Motorola TUTOR
  monitor program.

  3/22/2021 - simplified. now based on printf and putchar directly.  (rolf)
  - fixed typo in printf (this generates n^3, not n^4 as was stated).
  - increased loop to 13 (13! is about 1.9B, still within a signed 32-bit value)
*/

#include <stdio.h>


int factorial(const int n);


int main()
{
    //printf("sizeof(int) %d\n", (int)sizeof(int));

    printf("n  n^2  n^3  n!\n");
    
    for (int i = 1; i <= 13; i++) {
        printf("%d", i);
        putchar(' ');
        printf("%d", i*i);
        putchar(' ');
        printf("%d", i*i*i);
        putchar(' ');
        printf("%d\n", factorial(i));
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
