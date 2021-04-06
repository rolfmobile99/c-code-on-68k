/*
  factor.c - calculate and print factorials up to 13!
  
  This code is the starting point for running a compiled C program on the 68k.
  
  Note: code is loosely based on the factorial demo from Jeff Tranter:
    http://jefftranter.blogspot.com/2017/05/building-68000-single-board-computer-c.html
  He also includes some really helpful hints on setting up gcc for the 68k.

  3/22/2021 - rolf
  - simplified. now based on printf and putchar directly.  (rolf)
  - fixed typo in printf (this generates n^3, not n^4 as was stated).
  - increased loop to 13 (13! is about 1.9B, still within a signed 32-bit value)
  
  AUTHORS: Rolf W, Roderick M
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
