;ErrorExamples.as - Shows all errors except for not entering a file name to the command line and entering a non-existing file

;Syntax Error (line 4):
.data 7,,8

;Illegal label name (line 7):
3label: .string "abcd"

;Line Length Error (line 10):
LineTooLong: .string "mzaNHI7zlDIwsXsVcBJEh7VFgUXoeBv9ISSAgr6bwI5tQTaZB8kdJKpgWkmm"

;Illegal Destination for jmp/bne/jsr Error (line 13):
jsr IllegalJSRLabel

;Illegal Parameter 1 Error (line 16):
j: .data 25
jmp j(jmp,#3)

;Illegal Parameter 2 Error (line 20):
bne j(#-25,r8)

;Expected Comma Error (line 23):
Left: .data 5 -9

;Illegal Destination Error (line 26):
sub 3,r8

;Illegal Source Error (line 29):
add r3,8

;Expected Parenthesis Error (line 33):
jmpLabel: .data 5, 6, 7
jmp jmpLabel(#-6,r4

;Expected End-of-Line Error (line 36):
rts a

;Entry Error (line 39):
.entry UnusedEntry

;Undefined Used Labels (line 42):
add UndefinesLabel, #5

;External Error (line 45):
.extern UnusedExtern