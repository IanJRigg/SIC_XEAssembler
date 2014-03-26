.sample program for SIC/XE architecture
.
.This program should assemble correctly
.
.

source1    start   $1000    .start of sample program
        lds     #3
        ldt     #300
        ldx     #0
addlf   lda     alpha,x   .loop control     
        add     beta,x
        sta     gamma,x                         
        addr    s,x
        compr   x,t 
        jlt     addlp 
        
testthis  
        .
. storage allocation section        
alpha   resw    100
beta    resw    100
gamma   resw    100
happy   BYTE    x'1000'  
happy2   BYTE    c'1000'        
        end     source1    .end of sample program
