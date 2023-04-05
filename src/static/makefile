XXD := xxd
TILEPACK := ../../scripts/tilepack
MANIFEST2HEADER := ../../scripts/manifest2header.awk
TILEDMK := ../../scripts/tiled.mk.awk
TILEMAP2TILED := ../../scripts/tilemap2tiled

INPUT += air.hex \
			background/bricks.hex \
			background/crates.hex \
			background/shop_stall.hex \
			background/shop.hex \
			background/title_screen_icon.hex \
			foreground/title_screen_letteres_large.hex\
			foreground/bullet.hex \
			foreground/gozer.hex \
			foreground/slime.hex \
			foreground/font.hex 
			# world.hex
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

tiled.mk: manifest.txt
	$(TILEDMK) < $< > $@

include tiled.mk

clean:
	$(RM) -r $(OUT_BIN) $(OUT_META) manifest.txt tilemap.h tilemap.bin tiled.mk tiled
