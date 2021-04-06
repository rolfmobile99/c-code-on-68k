#!/usr/bin/perl
#
# gcc2vasm.pl - convert 'gcc assembler' for 68k style to 'vasm' style 
#
# USAGE:
#   gcc2vasm.pl < file.s > file.asm
#
# LIMITATIONS:
#   - does not attempt to convert strings and their labels - please do by hand
#
#   - it would be nice to allow filename arguments to be specified (e.g. -o file.asm)
#
# AUTHORS:
#   Rolf Widenfelt, Roderick McAllery - 2021
#
#
# DESCRIPTION:
#   this script will convert this:
#
# #NOAPP
# ; print strings only
#        .text
#        .align  2
#        .globl  print_str
# print_str:
#        subq.l #8,%sp
#        move.l 12(%sp),4(%sp)
# .L2:
#
#
#   to this:
#
# ; print strings only
# ;       .text
# ;       .align  2
# ;       .globl  print_str
# print_str:
#        subq.l #8,sp
#        move.l 12(sp),4(sp)
# .L2:
#
#
    use strict;
    
    while (<>) {
        my $line = $_;
        chomp $line;
        if ($line =~ m/^\s*\;(.)*$/) {          # ";" comment line - no change
            print $_;
        } elsif ($line =~ m/^\s+\.(.)+$/) {     # " .text" or similar directives - comment line
            print "; " . $line . "\n";
        } elsif ($line =~ m/^\#/) {            # stupid "# ..." line - remove
            ;
        } elsif ($line =~ s/\%//g) {            # remove "%" chars
            $line = convert_jumps($line);       # and convert jumps
            print $line . "\n";
        } else {                                # otherwise - just convert jumps
            $line = convert_jumps($line);
            print $line . "\n";
        }
    }


#
# convert various jumps/branches
#
#   1. branch instruction "jra" is converted to "bra".
#   2. conditional branch instructions (like jne) are converted from the form "jXX" to "bXX",
#      where XX is a 2-letter code (like eq,ne,gt,ge,lt,le,ls,mi,cc,cs,vc,cs).
#   3. the "jsr" instruction is left untouched.
#   4. the "jmp" instruction is also left untouched (note: but also ok to convert to "bra").
#
#   note: care is taken to preserve leading whitespace without change.
#
    sub convert_jumps {
        my $line = shift;

        if ($line =~ m/(^\s+)(jsr|jmp)/) {          # jsr or jmp (leave untouched)
            ;
        } elsif ($line =~ s/(^\s+)jra/$1bra/) {     # jra -> bra
            ;
        } elsif ($line =~ s/(^\s+)j(\w\w)/$1b$2/g) {    # jXX -> bXX (conditional branch)
            ;
        } else {
            ;
        }
        return $line;
    }