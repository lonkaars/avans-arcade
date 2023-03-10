#include <stdlib.h>

#include "main.h"
#include "demo.h"

bool g_hh_run = true;

static int8_t buttonDPAD[] = {0,0,0,0}; //1left 2right 3down 4up

struct playerData{
	uint16_t posX;
	uint16_t posY;
	uint8_t radius;
	uint8_t rotation; //45 degrees steps 0 == right 2 == down 4 == left 6 == up
	uint8_t directionX; //direction where its looking at in case no input;
	int8_t speed;	//10 default L/R MODifier
	bool inAir;

};

struct playerData player1;

uint8_t tileMap[30][40] =
  {
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 },
		  {1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1,1 }



  };

void buttonRead();
void playerMovement();
void sendData(uint8_t, uint16_t);

  uint16_t pos_x; //0b0000 0001 0011 0110
  uint16_t pos_y;

  uint8_t left = 0;
  uint8_t right = 0;
  uint8_t up = 0;
  uint8_t down = 0;



  uint8_t pos_x_bit[2];
  uint8_t pos_y_bit[2];
  uint8_t data_send[3];

  int tileX;
  int tileY;
//  struct playerData player1;
 //int buttons[] = {GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_8}; // 1 left // 2 right // 3 up // 4 down


int main() {
	hh_setup();

/// init struct
  player1.posX = 31000; //0b0000 0001 0011 0110
  player1.posY = 21000;
  player1.radius = 8;
  player1.speed = 1;
  player1.directionX = 1;
  player1.rotation = 8;
  player1.inAir = false;

	hh_loop();
	hh_exit();
	return 0;
}

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;
	frame++;

	buttonRead();
	playerMovement();


	// send data via SPI //adjust map size
	pos_x = player1.posX / 100;
	pos_y = player1.posY / 100;

	sendData(0b01000000,pos_x);
	sendData(0b00000000,pos_y);

	// hh_demo_loop(frame);
}

void sendData(uint8_t address, uint16_t data)
{
			uint8_t bitData[3];
			bitData[2] = data & 0xff;
			bitData[1] = (data >> 8);
			bitData[0] = address; // first byte is address



			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			HAL_SPI_Transmit(&hspi1, bitData, 3, 100); //2*8 bit data
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

}

void buttonRead()
{
	//int buttons[] = {GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_8}; // 1 left // 2 right // 3 down // 4 up  8-6-4-5
	uint16_t buttons[] = {0x0100U,0x0040U,0x0010U,0x0020U}; // 1 left // 2 right // 3 down // 4 up

	for(int i = 0; i < 4; i++)
	{
		if(HAL_GPIO_ReadPin(GPIOB, buttons[i]) == 1)
		{
			if((i+2) % 2 == 0)
			{
				buttonDPAD[i] = 1;
			}
			else
			{
				buttonDPAD[i] = -1;
			}
		}
		else
		{
			buttonDPAD[i] = 0;
		}
	}
}

void playerMovement()
{

	int8_t directionX = 0; // -1 = L || 1 == R
	int8_t directionY = 0; // -1 = D || 1 == U

	uint8_t i,j;
	uint8_t rotation = 0; // 0-7
	//temp var for testing



	// direction calc
	directionX = buttonDPAD[0] + buttonDPAD[1];
	directionY = buttonDPAD[2] + buttonDPAD[3];

	//rotation calc
	for(i = -1; i < 2;i++)
	{
		for(j = -1; j < 2; j++)
		{
			if(directionX == i)
			{
				if(directionY == j)
				{
					if(i != 0 && j != 0) //dont update when player idle
					{
						player1.rotation = rotation;
					}
				}
			}
		rotation++;
		}
	}
	//direction calc
	if(directionX != 0) //update direction if player is not idle
	{
		player1.directionX = directionX;
	}
	//collision map x-axis

	//tile calc including radius and direction for background coliision

	uint16_t tileColX;
	uint16_t tileColY = ( player1.posY / 100) / 16; ;

	// remaining space between grid and exact
	uint8_t modTileX;
	uint8_t modTileY;



	if(player1.inAir == false && directionX != 0)
	{
		if(directionX == 1)
		{
			tileColX = ( ( player1.posX / 100) + player1.radius ) / 20;
			modTileX = ( player1.posX + ( 100 * player1.radius ) ) % 2000;
		}
		else if(directionX == -1)
		{
			tileColX = ( ( player1.posX / 100) - player1.radius ) / 20;
			modTileX = ( player1.posX - ( 100 * player1.radius ) ) % 2000;
		}

			if(tileMap[tileColY][tileColX + directionX] != 1)
			{
				player1.posX = player1.posX + (directionX * player1.speed); // NEW x set
			}

			else if(tileMap[tileColY][tileColX + directionX] == 1)
			{
				if(modTileX < player1.speed)
				{
					player1.posX = player1.posX + (directionX * modTileX); // NEW x set
				}
				else
				{
					player1.posX = player1.posX + (directionX * player1.speed); // NEW x set
				}
			}

	}
	else //if in air different all borders have to be checked
	{

	}

	//collision map floor (y-axis) (falling)
	// if falling no jump press (implement)
	/*
	tileColY = (( player1.posY / 100) + player1.radius) / 16; //bottom of player box
	modTileY = 1;
	if(tileMap[tileColY+1][tileColX] != 1) //rework after jumping
	{
		player1.posY = player1.posY + 5 ;// NEW y set //makew var gravity
		//playerStat = falling; //for later use of graphics/sound
	}
	*/
	//else if(tileMap[])





}
