
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define HH_PPU_VRAM_TMM_SPRITE_SIZE 52


void printData(uint8_t* in) {
	for (int i = 0; i < HH_PPU_VRAM_TMM_SPRITE_SIZE; i++)
	{
		printf("%02x ",in[i]);
	}
	printf("\n");
}

void hh_ppu_load_tilemap() {

	
	char* filename = "slime.bin";
	FILE* fp = fopen(filename,"rb");
	if (!fp){
		return;//error
	}

	int sprite_size = (16 * 16);
	fseek(fp, 0, SEEK_END);
	int _size = ftell(fp)/sprite_size;
	rewind(fp);
	// printf("%i",_size);
	for (int i = 0; i < _size; i++) {
		uint8_t data[sprite_size];
		fread(data,1,sprite_size,fp);
		
		printData(data);
	}
	fclose(fp);

}


int main(){
hh_ppu_load_tilemap();
    return 0;
}
