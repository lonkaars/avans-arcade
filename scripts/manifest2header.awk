#!/bin/awk -f
BEGIN {
	offset = 0
	groups = 0
	arr = "0"
	print "#pragma once"
	print "#include <stddef.h>"
}
(offset > 0) {
	arr = arr ", " offset
}
1 {
	sub(".*/", "", $1)
	print "#define HH_TM_"toupper($1)"_OFFSET "offset
	print "#define HH_TM_"toupper($1)"_SIZE "$2
	groups += 1
	offset += $2
}
END {
	print "#define HH_TM_SIZE "offset
	print "#define HH_TM_GROUPS "groups
	print "const static uint8_t hh_palette_lut[]={"arr", NULL};"
}
