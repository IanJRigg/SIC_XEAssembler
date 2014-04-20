
source1    start   $1000    .start of sample program
test equ 6000
testt equ test
.Valid Format 3 4 instructions

    +ldb #alpha
    base alpha
test1 lda 1028
test2 lda 1028,x
test3 lda @1028
test4 lda #1028
test5 +lda #1028
test6 lda test1
test65 lda badone
test7 +lda test1
test8 lda test1,x
test85 lda badone,x
test9 +lda test1,x
test10 lda @test1
test105 lda @badone
test11 +lda @test1
test12 lda #test1
test125 lda #badone
test13 +lda #test1
 +jsub test
 +jsub testt

.bad cases next

.test14 lda #100,x
.test15 lda @100,x
.test16 lda 100,l
.test17 lda 100,
.test18 lda #@test1
.test19 lda @#test1
.test20 lda #
.test21 lda @
.test22 lda #test1,x
.test23 lda @test1,x
.test24 lda #-2049
.test25 lda #2048

alpha    resw 3000
badone  word 1
    end source1
