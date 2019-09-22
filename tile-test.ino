/*
Little Rocket Man
Sept. 2019
Paul Wasson
*/

#include <Arduboy2.h>
#include <Sprites.h>
#include <ArduboyTones.h>

#include "sprite.h"
#include "map.h"

Arduboy2 arduboy;
Sprites  sprites;
ArduboyTones sound(arduboy.audio.enabled);

enum  : char {
  COLLISION_NONE = 0,
  COLLISION_GROUND = 1,
  COLLISION_CEILING = 2,
  COLLISION_RIGHT = 4,
  COLLISION_LEFT = 8
};

// Globals

int player_x;
int player_y;
uint16_t background = 0;

// Setup
void setup() {
  // initiate arduboy instance
  arduboy.begin();
  arduboy.setFrameRate(60);

  player_x = 0;
  player_y = 255;
  background = 0;
}

// Game loop
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  arduboy.pollButtons();

  // Move character

  move(0);
    
  // Read map data
  
  int map_pos = (player_x >> 7) + (player_y >> 7)*MAP_WIDTH;
  
  Tile tiles[4] = {
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos)),
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos+1)),
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos+MAP_WIDTH)),
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos+MAP_WIDTH+1))
  };

  //uint8_t collision = detectCollisions(player_x, player_y, tiles);
  

  
  //----------------------
  // Render
  //----------------------

  // Draw walls draws write the whole buffer, so no
  // clear is needed.
  drawWalls(player_x&0x7f, player_y&0x7f, tiles);

  // Draw background

  drawStars(613,0);
  drawStars(3001,1);
    
  // Draw player
  
  // Draw in center of screen
  sprites.drawPlusMask((WIDTH/2)-(16/2),(HEIGHT/2)-(24/2),lrm_plus_mask,0);

  // Debug
//  arduboy.setCursor(0, 28);
//  arduboy.print(player_x);
  arduboy.setCursor(56, 0);
  arduboy.print(player_y);
//  arduboy.setCursor(8*14, 28);
  //arduboy.print(collision);
  
//  Display results
  
  arduboy.display();
}

uint8_t detectCollisions(int delx, int dely, Tile tiles[4]) {

  return COLLISION_NONE;
}

// Character movement
void move(uint8_t collision) {
  const int MAX_X = (MAP_WIDTH-1)*TILE_SIZE;
  const int MAX_Y = (MAP_HEIGHT-1)*TILE_SIZE;
  int del_x = 0;
  int del_y = 0;
  
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (player_x >= MAX_X) {
      player_x = MAX_X;
    }
    else {
      del_x = 1;
    }
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    if (player_x <= 0) {
      player_x = 0;
    }
    else {
      del_x = -1;
    }
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    if (player_y >= MAX_Y) {
      player_y = MAX_Y;
    }
    else {
      del_y = 1;
    }
  }
  if (arduboy.pressed(UP_BUTTON)) {
    if (player_y <= 0) {
      player_y = 0;
    }
    else {
      del_y = -1;
    }
  }
  player_x += del_x;
  player_y += del_y;
  background += (del_y<<8) - del_x;
}

// Calculate bits based on location within tile
uint8_t tileBits(Tile tile, int8_t x, int8_t y) {
  switch(tile) {
    case Tile::empty:       return 0x00;
    case Tile::wall:        return 0xff;
    case Tile::lowerLeft:   return x < y   ? 0xff  :
                                   x < y+8 ? 0xff << (x-y)  :
                                   0x00  ; 
    case Tile::upperRight:  return x < y   ? 0x00  :
                                   x < y+8 ? ~(0xff << (x-y))  :
                                   0xff  ; 
    case Tile::lowerRight:  return TILE_SIZE_M1 - x < y   ? 0xff  :
                                   TILE_SIZE_M1 - x < y+8 ? 0xff << (TILE_SIZE_M1-x-y)  :
                                   0x00  ; 
    case Tile::upperLeft:   return TILE_SIZE_M1 - x < y   ? 0x00  :
                                   TILE_SIZE_M1 - x < y+8 ? ~(0xff << (TILE_SIZE_M1-x-y))  :
                                   0xff  ; 
  }
  // Use dirt for undefined (doesn't work with stars)
  return 0x55<<((x^y)&1);
}

// Tile order
// 0 1
// 2 3

void drawWalls(int delx, int dely, Tile tiles[4]) {
  int xswitch = 128-delx;
  int yswitch = 128-dely;
  for (int y=0; y < 64; y += 8) {
    for (int x=0; x < 128; x += 1) {
      size_t idx = (x < xswitch ? 0 : 1) +
                   (y < yswitch ? 0 : 2) ;
      uint8_t bits = tileBits(tiles[idx],x + delx - (idx&1 ? 128 : 0), y + dely - (idx&2 ? 128 : 0));
      if ((y < yswitch) && ((y + 8) > yswitch)) {
        uint8_t mask = 0xff >> (8 - (yswitch - y));
        bits = (mask&bits) | (~mask & (tileBits(tiles[idx+2],x + delx - (idx&1 ? 128 : 0), y + dely - 128)));
      }
      arduboy.sBuffer[x+y*16] = bits;
    }
  }

  arduboy.setCursor(0, 7*8);
  arduboy.print(yswitch);
  arduboy.print(",");
  arduboy.print(dely);
  arduboy.print(",");
  arduboy.print(static_cast<uint8_t>(tiles[0]));
  arduboy.print(",");
  arduboy.print(static_cast<uint8_t>(tiles[2]));
}

void drawStars(uint16_t seed, uint8_t layer) {
  background = -((player_y >> layer) << 7) - (player_x >> layer);
  for(uint16_t i=0; i < WIDTH*HEIGHT; i+=seed) {
    background += seed;
    arduboy.drawPixel (background & 0x7f, (background >> 7) & 0x3f, WHITE);
  }
}
