# binary tests

this folder contains binary source (.bs) files, that can be converted to .bin
files using the provided makefile. these files can contain comments, and are
processed using `m4` and `xxd`.

example syntax:
```
00 00 00 00         ; this is an example binary source file
repeat(5,8f)        ; 5 0x8f bytes
repeat(hex(a),bb)   ; 10 0xbb bytes
repeat(0x40,00)     ; 64 0x00 bytes
;      ^ 0x prefix works too
ff                  ; single byte
00000000            ; 32-bit number
;                   ^ this is the comment character
```

if compilation hangs, you probably broke the syntax
