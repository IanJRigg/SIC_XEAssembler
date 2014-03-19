.sample program for SIC/XE architecture
.
.Test comment
prog    start   0    .start of sample program
test    lds     #3 .Test comment
label
        ldt     #300
        ldx     #0
addlf   lda     alpha,x   .loop control 
        add     beta,x
        sta     gamma,x   .1qqqqqqqqqqqqqqq                  
        addr    s,x	  .2qqqqqqqqqqqqqqq     
        compr   x,t 	  .3qqqqqqqqqqqqqqq     
        jlt     add   .lf
	jmp		.testing if only opcode and comment
labeltest

 
 
   
test    test   test     .testing with number in line        .
. storage allocation section        
alpha   resw    100
beta    resw    100
gamma   resw    100   
        end     prog    .end of sample program   
