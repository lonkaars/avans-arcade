#include "engine/level_const.h"


hh_g_all_levels hh_init_game_levels(){
	hh_g_all_levels levels;
	levels.currentLevel=0;

	levels.level[0].x=40;
	levels.level[0].y=60;
	levels.level[0].hh_level_completed=false;
	FILE *fp = fopen("../test/bin/level1_test.bin", "rb");
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp) / sizeof(int);
	fseek(fp, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_level1 = malloc(size * sizeof(int));
	fread(hh_game_level1, sizeof(int), size, fp);
	fclose(fp);
	levels.level[0].place = hh_game_level1;

	return levels;
}

