#!/bin/awk -f
BEGIN {
	printf "\n"
	printf "\t\tWEN <= '1';\n"
	printf "\n"
}
1 {
	printf "\t\tADDR <= x\""$1"\";\n"
	printf "\t\tDATA <= x\""$2"\";\n"
	printf "\t\twait for 10 ns;\n"
	printf "\n"
}
END {
	printf "\t\tWEN <= '0';\n"
	printf "\n"
}
