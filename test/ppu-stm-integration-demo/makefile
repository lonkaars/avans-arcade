.PHONY: all clean

SRCS := $(wildcard *.txt)
SRCS += $(patsubst %.pvc,%.txt, $(wildcard *.pvc))
TARGETS += $(patsubst %.txt,%.h, $(SRCS))
TARGETS += $(patsubst %.txt,%-ppu.tb.vhd, $(SRCS))
TARGETS += $(patsubst %.txt,%-spi.tb.vhd, $(SRCS))

.PRECIOUS: $(SRCS)

all: $(TARGETS)

%.txt: %.pvc
	sed 's/.*://g' $< | tr -d ' \n' | sed 's/\(.\{4\}\)\(.\{4\}\)/\L\1: \2\n/g' > $@

%.h: %.txt ./data2test.awk
	tr -d ':' < $< | ./data2test.awk > $@

%-ppu.tb.vhd: %.txt ./data2pputb.awk
	tr -d ':' < $< | ./data2pputb.awk > $@

%-spi.tb.vhd: %.txt ./data2spitb.awk
	(tr -d ':' < $<; echo "ffff ffff") | ./data2spitb.awk > $@

clean:
	$(RM) $(TARGETS)
