#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "ppu/types.h"

/** @brief generate bitmask with `n` bits set to one from LSB */
#define HH_MASK(n) ((1 << (n))-1)
/** @brief resize `in` to `upper downto lower` like in vhdl */
#define HH_RESIZE(in, upper, lower) ((((hh_ppu_data_t)(in)) & (HH_MASK(upper+1-lower) << lower)) >> lower)

/** @brief memory word(s) that can be placed into vram */
typedef struct {
	hh_ppu_addr_t offset;
	hh_ppu_addr_t size;
	hh_ppu_data_t* data;
} hh_s_ppu_vram_data;

/** @brief check if `addr` is a valid PPU address */
bool hh_ppu_vram_valid_address(hh_ppu_addr_t addr);
/** @brief write data block into vram */
void hh_ppu_vram_write(hh_s_ppu_vram_data data);

/** @brief add raw SPI data to ppu command buffer */
void hh_ppu_vram_buffer(uint8_t data[4]);
/** @brief propagate command buffer (also handles SPI reset) */
void hh_ppu_vram_flush();
/** @brief write raw spi bytes in ppu format */
void hh_ppu_vram_dwrite(uint8_t* data, size_t size);

/** @brief convert local background attribute memory entry to PPU format */
hh_s_ppu_vram_data hh_ppu_2nat_bam(hh_s_ppu_loc_bam_entry);
/** @brief convert local foreground attribute memory entry to PPU format */
hh_s_ppu_vram_data hh_ppu_2nat_fam(hh_s_ppu_loc_fam_entry);
/** @brief convert local AUX memory copy to PPU format */
hh_s_ppu_vram_data hh_ppu_2nat_aux(hh_s_ppu_loc_aux);
/** @brief convert local sprite to PPU format */
hh_s_ppu_vram_data hh_ppu_2nat_sprite(const hh_ppu_loc_sprite_data_t);
/** @brief convert local RGB color to PPU format */
hh_s_ppu_vram_data hh_ppu_2nat_color(hh_ppu_rgb_color_t);

