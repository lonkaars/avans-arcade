.PHONY: all clean

SRCS := $(wildcard *.txt)
TARGETS += $(patsubst %.txt,%.h, $(SRCS))
TARGETS += $(patsubst %.txt,%-ppu.tb.vhd, $(SRCS))
TARGETS += $(patsubst %.txt,%-spi.tb.vhd, $(SRCS))

all: $(TARGETS)

%.h: %.txt ./data2test.awk
	tr -d ':' < $< | ./data2test.awk > $@

%-ppu.tb.vhd: %.txt ./data2pputb.awk
	tr -d ':' < $< | ./data2pputb.awk > $@

%-spi.tb.vhd: %.txt ./data2spitb.awk
	(tr -d ':' < $<; echo "ffff ffff") | ./data2spitb.awk > $@

clean:
	$(RM) $(TARGETS)
