#pragma once

#include "ppusim/work.h"

/** @brief calculate color for pixel (`x`,`y`), and put into `unsigned short s[3]` */
void hh_ppusim_pixel(uint8_t* s, unsigned x, unsigned y);

