XXD := xxd
TILEPACK := ../../scripts/tilepack
MANIFEST2HEADER := ../../scripts/manifest2header.awk

INPUT += air.hex \
			shop.hex \
			slime.hex \
			world.hex
USER_META += world.h

OUT_BIN := $(patsubst %.hex,%.bin, $(INPUT))
OUT_META := $(patsubst %.hex,%.meta, $(INPUT))

.PHONY: all clean

all: tilemap.bin tilemap.h

%.bin: %.hex
	sed 's/;.*//g' $< | $(XXD) -r | $(TILEPACK) > $@

%.meta: %.bin
	wc -c $< | tr '.' ' ' | awk '1 { print $$2" "$$1 / 104 }' > $@

manifest.txt: $(OUT_BIN) $(OUT_META)
	cat $(OUT_META) > $@

tilemap.bin: manifest.txt $(OUT_BIN)
	awk '1 { print $$1".bin" }' $< | xargs cat > $@

tilemap.h: manifest.txt
	$(MANIFEST2HEADER) $< > $@
	cat $(USER_META) >> $@

clean:
	$(RM) $(OUT_BIN) $(OUT_META) manifest.txt tilemap.h tilemap.bin
