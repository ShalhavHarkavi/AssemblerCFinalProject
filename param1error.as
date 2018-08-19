h: lea a,r1
i: inc h
j: dec a
a: .string "aaa"
jmp j(jmp,i)
