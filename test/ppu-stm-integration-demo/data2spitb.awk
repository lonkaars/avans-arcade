#!/bin/awk -f
BEGIN { }

{
	data = strtonum("0x"$1$2)
	print "-- 0x"$1": "$2

	for (i = 0; i < 32; i++) {
		print "SPI_DATA <= '"and(rshift(data, 31), 1)"';"
		print "wait for 50 ns;"
		print "SPI_CLK <= '1';"
		print "wait for 50 ns;"
		print "SPI_CLK <= '0';"
		if (i == 15) print "--"
		else print ""
		data = lshift(data, 1)
	}
}

END { }
