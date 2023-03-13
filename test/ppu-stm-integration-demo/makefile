data.h: data.txt
	echo "#pragma once" > $@
	echo "#include <stdint.h>" >> $@
	(printf "#define HH_PPUINTDEMO_LENGTH "; wc -l < $<) >> $@
	tr -d ':' < $< | awk 'BEGIN { printf "const uint16_t HH_PPUINTDEMO_ADDR[] = { " } 1 { printf "0x"$$1", " } END { print "};" }' >> $@
	tr -d ':' < $< | awk 'BEGIN { printf "const uint16_t HH_PPUINTDEMO_DATA[] = { " } 1 { printf "0x"$$2", " } END { print "};" }' >> $@
