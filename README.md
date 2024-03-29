## Simple C code examples for 68k target

**hello1.c** - only uses `putchar` for output

**hello2.c** - prints `hello! world`.

This uses `minprintf`, a rather minimal version of `printf`.
(`minprintf` is borrowed from K&R, section 7.2)


**itoa1.c** - implements a version of `itoa` (integer to ascii conversion) also taken from K&R.

The function `itoa` will be needed by our minimal printf to handle `%d` (decimal numbers).
So, it is good to ensure this works before proceeding further.
For the 68k target, gcc generated calls to `__modsi3` and `__divsi3``, 
so ASM version of those had to be tracked down.
This demo also introduces some simple versions of classic `string.h` functions, like `strlen`.

**itoa2.c** - excuse the mess here. Introduces `minprintf` with just `%s` support.

**itoa3.c** - improves `minprintf` with `%x` (unsigned hex) support.


**factor.c** - prints factorials up to 13!. This version should compile on "real" unix/linux.

**factor1.c** - based on factor.c

This tests 32-bit mult, and uses a `minprintf` that now supports `%d` format.


