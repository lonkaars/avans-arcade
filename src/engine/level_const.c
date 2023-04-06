#include "engine/level_const.h"

#include <stdio.h>

hh_g_all_levels hh_init_game_levels(){
	hh_g_all_levels levels;
	levels.current_level=1;

	levels.shop.size.x=40;
	levels.shop.size.y=15;
	levels.shop.hh_level_completed=false;
	
	levels.level[0].size.x=40;
	levels.level[0].size.y=100;
	levels.level[0].hh_level_completed=false;

	levels.level[1].size.x=100;
	levels.level[1].size.y=28;
	levels.level[1].hh_level_completed=false;

	FILE *fp = fopen("static/shop_new.bin", "rb");
	if (fp == NULL) {
		printf("shop_new.bin not found!\n");
		return levels;
	} 
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp) / sizeof(int);
	fseek(fp, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_level1 = malloc(size * sizeof(int));
	fread(hh_game_level1, sizeof(int), size, fp);
	fclose(fp);

	FILE *lvl2 = fopen("../test/bin/level2_test.bin", "rb");
	if (lvl2 == NULL) {
		printf("level2_test.bin not found!\n");
		return levels;
	}
	fseek(lvl2, 0, SEEK_END);
	size = ftell(lvl2) / sizeof(int);
	fseek(lvl2, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_level2 = malloc(size * sizeof(int));
	fread(hh_game_level2, sizeof(int), size, lvl2);
	fclose(lvl2);

	FILE *shop = fopen("static/shop_new.bin", "rb");
	if (shop == NULL) {
		printf("shop.bin not found!\n");
		return levels;
	}
	fseek(shop, 0, SEEK_END);
	size = ftell(shop) / sizeof(int);
	fseek(shop, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_shop = malloc(size * sizeof(int));
	fread(hh_game_shop, sizeof(int), size, shop);
	fclose(shop);

	levels.shop.place = hh_game_shop;
	levels.level[0].place = hh_game_level1;
	levels.level[1].place = hh_game_level2;

	// free(hh_game_level1);
	// free(hh_game_level2);
	return levels;
}

