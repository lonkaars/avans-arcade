dnl library functions
define(`forloop',`pushdef(`$1',$2)_forloop(`$1',$2,$3,`$4')popdef(`$1')')dnl
define(`_forloop',`$4`'ifelse($1,$3,`',`define(`$1',incr($1))_forloop(`$1',$2,$3,`$4')')')dnl
define(`repeat',`forloop(`i',1,verbose_hex($1),`$2 ')')dnl
define(`hexc2dec',`eval(10 + translit(`$1',`a-f',`0-5'))')dnl
define(`hex2dec',`patsubst(translit(`$1',`A-F',`a-f'),`\([a-z]\)',`hexc2dec(\1)')')dnl
define(`_hex',`ifelse(len($2),0,$3,`_hex(incr($1),substr($2,0,decr(len($2))),eval($3 + (hex2dec(substr($2,decr(len($2)))) * (16 ** $1))))')')dnl
define(`verbose_hex',`patsubst(`$1',`\(0x\)\([0-9a-fA-F]*\)',`hex(\2)')')dnl
define(`hex',`_hex(0,patsubst(`$1',`0x',`'),`0')')dnl
dnl
dnl main
define(`INPUT',`include(`/dev/stdin')')dnl
define(`NOCOMMENTS',`patsubst(`$1', `;.*', `')')dnl
define(`ONELINE',`patsubst(`$1', `[ \t
]', `')')dnl
define(`BYTES',`patsubst(`$1',`\(..\)',`\1
')')dnl
define(`OUTPUT',`BYTES(ONELINE(NOCOMMENTS($1)))')dnl
OUTPUT(INPUT())dnl
