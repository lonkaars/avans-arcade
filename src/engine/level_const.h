#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>`

typedef enum {
	hh_e_STATE_startingScreen,
	hh_e_STATE_Shop,
	hh_e_STATE_Gameplay,
	hh_e_STATE_GameOver,
	hh_e_STATE_HighScore
} hh_e_GameState;

typedef struct  {
	int x;
	int y;
	int hh_total_enemies;
	int* place;
	bool hh_level_completed;
}hh_level_entity;

typedef struct  {
	hh_level_entity level[1];
	int currentLevel;


}hh_g_all_levels;

hh_g_all_levels hh_init_game_levels();
