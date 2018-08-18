   ; file x.as

.data 55,6  , 7
		.string "lmnop"
.entry left
.entry e15
e15:		mov	r3 ,B5
left:		jmp L1(#67,r6)
mAIN:		prn #19
			bne W(r2,r3)
			sub r5, r1
			bne L3
L1:			dec r3
.entry B5
			bne left(e15,mAIN)
Final:		stop
StringLabel:		.string "abcdef"
B5:		.string "WXYZ"
.extern L3