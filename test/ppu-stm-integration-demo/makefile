.PHONY: all clean

TARGETS := test-background-color.h test-image.h test-image-spi.tb.vhd test-image-ppu.tb.vhd test-background-color-spi.tb.vhd

all: $(TARGETS)

%.h: %.txt ./data2test.awk
	tr -d ':' < $< | ./data2test.awk > $@

test-%-ppu.tb.vhd: test-%.txt ./data2pputb.awk
	tr -d ':' < $< | ./data2pputb.awk > $@

test-%-spi.tb.vhd: test-%.txt ./data2spitb.awk
	(tr -d ':' < $<; echo "ffff ffff") | ./data2spitb.awk > $@

clean:
	$(RM) $(TARGETS)
