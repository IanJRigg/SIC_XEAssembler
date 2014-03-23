.sample program for SIC/XE architecture
.test program for proj1-4
.This file should assemble without errors

source6    start   0    .start of sample program
bsrch   EQU     14
OFFB    EQU     8
        BASE    delta
        LDB     #delta
. storage allocation section
test    byte    C'12345'
test2   byte    x'007b'
zeta    BYTE    C'EOF'
eta     byte    X'FFFFFE'
foo     word    bsrch
theta   RESB    1000
delta   word    8
        end     source6    .end of sample program
        
