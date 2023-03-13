#include "stdlib.h"
#include "stdio.h"
#include "ppu/consts.h"
#include "entity.h"

#include "demo.c"



//int hTiles = HH_PPU_BG_CANVAS_TILES_H;
//int vTiles = HH_PPU_BG_CANVAS_TILES_V;
//int 

/** Visable tiles X and Y*/
int visibleTilesX = HH_PPU_SCREEN_WIDTH / HH_PPU_BG_CANVAS_TILES_H;
int visibleTilesY = HH_PPU_SCREEN_HEIGHT / HH_PPU_BG_CANVAS_TILES_V;

/** Camera position on player*/
int cameraPosX;
int cameraPosY;


int offsetX;
int offsetY;

int levelWidth;
int levelHeight;


void playerCamera()
{
    /** Set camra pos op player tijdelijk g_hh_pos voor de sim*/
    cameraPosX = g_hh_pos_x;
    cameraPosY = g_hh_pos_y;

   /** Top left most visibale tile*/
   offsetX = cameraPosX - visibleTilesX / 2;
   offsetY = cameraPosY - visibleTilesX / 2;

   /** Clamp camra so that it wont go out of screen*/
   if(offsetX < 0)
   {
    offsetX = 0;
   }
   if(offsetY < 0)
   {
     offsetY = 0;
   }
   if(offsetX > levelWidth -visibleTilesX )
   {
     offsetX = levelWidth -visibleTilesX;
   }
      if(offsetX > levelWidth -visibleTilesX )
   {
     offsetX = levelWidth -visibleTilesX;
   }

 
}
