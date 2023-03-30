all: ppu_tb.vhd spi_tb.vhd

ppu_tb.vhd: ppu_tb.vhd.m4 ../../test/ppu-stm-integration-demo/test-image-ppu.tb.vhd
spi_tb.vhd: spi_tb.vhd.m4 ../../test/ppu-stm-integration-demo/test-image-spi.tb.vhd

%.vhd: %.vhd.m4
	m4 -I../../test/ppu-stm-integration-demo/ $< > $@

