#!/bin/awk -f
BEGIN {
	targets=""
	print "tiled_dir:\n\tmkdir -p tiled"
}
1 {
	for (i = 0; i < $2; i++) {
		target = sprintf("tiled/%s_%02d.png", $1,i)
		printf target" "
		targets = targets" "target
	}
	printf "&: %s.bin\n",$1
	printf "\t$(TILEMAP2TILED) $<\n\n"
}
END { print "tiled: tiled_dir"targets }

