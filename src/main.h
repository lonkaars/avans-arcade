#pragma once

#include <stdbool.h>

extern bool g_hh_run;

/** @brief exec entrypoint */
int main();

/** @brief setup i/o interfaces */
void hh_setup();
/** @brief main loop (sim only) */
void hh_loop();
/** @brief stop main loop and cleanup/deinitialize */
void hh_exit();
