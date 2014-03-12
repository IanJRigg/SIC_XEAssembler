.sample program for SIC/XE architecture
.
test    lds     #3'checking this out' .Test comment
prog    start   0    .start of sample program
test    lds     #3'checking this out' .Test comment
        ldt     #300
        ldx     #0
addlf   lda     alpha,x   .loop control     xxxxxxx
        add     beta,x
        sta     gamma,x   .1qqqqqqqqqqqqqqq     .bad line here                
        addr    s,x	  .2qqqqqqqqqqqqqqq     .bad line here
        compr   x,t 	  .3qqqqqqqqqqqqqqq     .bad line here
        jlt     addlf   
1test    test   test     .testing with number in line        .
. storage allocation section        
alpha   resw    100
beta    resw    100
gamma   resw    100   
        end     prog    .end of sample program   
