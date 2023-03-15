#pragma once
#include "player_controller.h"

typedef struct {
    int x;
    int y;
    int velocity;
    int isActive;
} Bullet;

//Bullet* createBullet(float x, float y, float velocity, float direction);
void shootBullet(vec2 playerPos, Bullet* bullet);
void updateBullet(Bullet* bullet, int deltaTime);
void drawBullet(Bullet* bullet);
