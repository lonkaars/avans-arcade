#!/bin/awk -f
BEGIN {
	print "#pragma once"
	print "#include <stdint.h>"
	printf("const uint8_t HH_PPUINTDEMO_ARR[] = { ")
}

length($1) == 4 && length($2) == 4 {
	printf "0x"substr($1,1,2)", 0x"substr($1,3,2)", 0x"substr($2,1,2)", 0x"substr($2,3,2)", "
}

END {
	printf("};\n") # close array
	printf("#define HH_PPUINTDEMO_LENGTH %d\n", NR*4) # calculate array length
}
