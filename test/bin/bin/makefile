SRCS := $(wildcard *.bs)
TARGETS := $(patsubst %.bs,%.bin, $(SRCS))

.PHONY: all clean

all: $(TARGETS)

%.bin: %.bs
	m4 bs.m4 < $< | xxd -r -p > $@

clean:
	$(RM) $(TARGETS)

