#include "bullet.h"
#include "engine/sprite_controller.h"


void shootBullet(vec2 playerPos, Bullet* bullet){
	// Set bullet's x and y coordinates to player's coordinates
    bullet->x = playerPos.x;
    bullet->y = playerPos.y;
    // Set bullet's velocity to a fixed value
    bullet->velocity = 1;
    // Set bullet's status to active
    bullet->isActive = true;
}
void updateBullet(Bullet* bullet, int deltaTime){
    // Only update bullet if it is active   
	 static int latestLocationBullet = 0;
    if (bullet->isActive) {
        // Move bullet based on velocity and deltaTime
        bullet->x += bullet->velocity * deltaTime;
		drawBullet(bullet);
        // Check if bullet has moved 16 pixels
        if (bullet->x - latestLocationBullet > 32) {
            // Set bullet's status to inactive
            bullet->isActive = false;
            drawBullet(&(Bullet){.x = -16,.y = -16.    });
        }
    }
	 else{
		latestLocationBullet = bullet->x;
	 }
}
void drawBullet(Bullet* bullet){


	hh_ppu_update_foreground(10, (hh_s_ppu_loc_fam_entry)
			{
                .position_x = bullet->x,
                .position_y = bullet->y,
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 7,
				.tilemap_index = 84, // change tilemap to the correct foreground index;
			});
}
