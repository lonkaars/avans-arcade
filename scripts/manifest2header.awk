#!/bin/awk -f
BEGIN {
	offset = 0
	print "#pragma once"
}
1 {
	print "#define HH_TM_"toupper($1)"_OFFSET "offset
	print "#define HH_TM_"toupper($1)"_SIZE "$2
	offset += $2
}
END {
	print "#define HH_TM_SIZE "offset
}
