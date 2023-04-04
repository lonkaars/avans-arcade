#pragma once

#include "ppu/types.h"

/** @brief vblank interrupt (game main loop) */
void hh_ppu_vblank_interrupt();
/** @brief initialize ppu interface */
void hh_ppu_init();
/** @brief deinitialize ppu interface */
void hh_ppu_deinit();
/** @brief flush ppu buffer */
void hh_ppu_flush();

/** @brief update single foreground sprite */
void hh_ppu_update_foreground(unsigned index, hh_s_ppu_loc_fam_entry e);
/** @brief update single background sprite */
void hh_ppu_update_background(unsigned index, hh_s_ppu_loc_bam_entry e);
/** @brief update background shift */
void hh_ppu_update_background_pos(uint16_t shift_x, uint16_t shift_y);
/** @brief update single sprite */
void hh_ppu_update_sprite(unsigned tilemap_index, const hh_s_ppu_loc_sprite sprite);
/** @brief update entire palette table */
void hh_ppu_update_palette_table(hh_ppu_loc_palette_table_t table);
/** @brief update single palette */
void hh_ppu_update_palette(unsigned palette_index, hh_ppu_loc_palette_data_t palette);
/** @brief update single color in palette */
void hh_ppu_update_color(unsigned palette_index, unsigned color_index, hh_ppu_rgb_color_t color);
