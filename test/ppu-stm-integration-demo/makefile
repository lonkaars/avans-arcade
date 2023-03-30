.PHONY: all clean

TARGETS := test-background-color.h test-image.h test-image.tb.vhd

all: $(TARGETS)

%.h: %.txt ./data2test.awk
	tr -d ':' < $< | ./data2test.awk > $@

test-%.tb.vhd: test-%.txt ./data2vhdltb.awk
	tr -d ':' < $< | ./data2vhdltb.awk > $@

clean:
	$(RM) $(TARGETS)
