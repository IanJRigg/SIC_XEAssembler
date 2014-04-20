
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
test135 lda #-2048
test136 lda #2047
test137 +lda #-524288
test138 +lda #524287
test139 +lda 1048575
test140 lda 4095
 +jsub test
 +jsub testt
  BYTE c'Test ing'
  BYTE x'1234567890ABCDEF'
  WORD 10
  WORD 100

.bad cases next THESE SHOULD ALL FAIL!!!

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
.test26 +lda #-524289
.test27 +lda #524288
.test28 lda 4096 .<---------Not currently handled properly!!
.test29 lda -1
.test30 +lda 1048576
.test31 +lda -1

alpha    resw 3000
badone  word 1
.  BYTE x'G'
.  WORD A
    end source1
