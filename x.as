   ; file x.as

;.data 123456789,1234567891,2345678912,3456789123,4567891234,5678912345,6789123456,7891234567,89
;.data 55,6  , 7
		;.string "lmnop"
.entry left
.entry e15
e15:		mov	r3 ,B5
left:		jmp L1(#67,r6)
mAIN:		prn #19
			bne Final(r2,r3)
			sub r5, r1
			bne L3
L1:			dec r3
.entry B5
			bne left(e15,mAIN)
Final:		stop
StringLabel:		.string "abcdef"
.data 7,,8
B5:		.string "WXYZ"
.extern L3
