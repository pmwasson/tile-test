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

// Constants

static const int MAX_MOTION = 5;
static const int MAX_FLOOR_MOTION = 2;

static const int COLLISION_NONE = 0;
static const int COLLISION_FLOOR = 1;
static const int COLLISION_CEILING = 2;
static const int COLLISION_RIGHT = 4;
static const int COLLISION_LEFT = 8;
static const int COLLISION_SLANT = 8;

static const int MOTION_NONE = 0;
static const int MOTION_UP = 1;
static const int MOTION_DOWN = 2;
static const int MOTION_RIGHT = 4;
static const int MOTION_LEFT = 8;

static const int CHECK_FLOOR = TILE_SIZE - (HEIGHT/2 + PLAYER_HEIGHT/2);   // 84
static const int CHECK_CEILING = TILE_SIZE - (HEIGHT/2 - PLAYER_HEIGHT/2); // 108
static const int CHECK_OFFSET = TILE_SIZE - (WIDTH/2 - PLAYER_WIDTH/2);
static const int CHECK_BUFFER = 8;

// Remove once collision checking is working
static const int MAX_X = (MAP_WIDTH-1)*TILE_SIZE;
static const int MAX_Y = (MAP_HEIGHT-1)*TILE_SIZE;
  
// Globals

int     player_x;
int     player_y;
int8_t  motion_x;
int8_t  motion_y;
uint8_t collision;
uint8_t motion;

// Setup
void setup() {
  // initiate arduboy instance
  arduboy.begin();
  arduboy.setFrameRate(60);

  player_x = START_X;
  player_y = START_Y;
  motion_x = 0;
  motion_y = 0;
  collision = COLLISION_NONE;
  motion = MOTION_NONE;
}

// Game loop
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  //arduboy.pollButtons();

  // Move character

  move();
    
  // Read map data
  
  int map_pos = (player_x >> 7) + (player_y >> 7)*MAP_WIDTH;
  
  Tile tiles[4] = {
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos)),
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos+1)),
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos+MAP_WIDTH)),
    static_cast<Tile>(pgm_read_byte(tile_map+map_pos+MAP_WIDTH+1))
  };

  detectCollisions(tiles);

  //----------------------
  // Render
  //----------------------

  // Draw walls draws write the whole buffer, so no
  // clear is needed.
  drawWalls(tiles);

  // Draw background

  drawStars(613,0);
  drawStars(3001,1);
    
  // Draw player
  
  // Draw in center of screen
  drawPlayer();
  
  // Debug
  arduboy.setCursor(0, 28);
  arduboy.print(player_x);
  arduboy.setCursor(56, 0);
  arduboy.print(player_y);
  arduboy.setCursor(8*14, 28);
  arduboy.print(collision);
  
//  Display results
  
  arduboy.display();
}

// Player coordinate is for the upper-left corner of the screen,
// so must offset for detection

// coordinate 0: floor = 64/2+24/2 = 32+12 = 44
// So want to check when player = 128 - 44 = 84

void detectCollisions(Tile tiles[4]) {
  uint8_t delx = player_x & (TILE_SIZE-1);
  uint8_t dely = player_y & (TILE_SIZE-1);
  bool    offset = delx >= CHECK_OFFSET;
  collision = 0;  

  if ((dely >= CHECK_FLOOR) && (dely < CHECK_FLOOR+CHECK_BUFFER) && (tiles[2+offset] != Tile::empty)) {
    collision |= COLLISION_FLOOR;
    player_y = (player_y & ~0x7f) + CHECK_FLOOR;
  }
  else if ((dely <= CHECK_CEILING) && (dely > CHECK_CEILING-CHECK_BUFFER) && (tiles[0+offset] != Tile::empty)) {
    collision = COLLISION_CEILING;
    player_y = (player_y & ~0x7f) + CHECK_CEILING;
  }
}

// Character movement
void move() {
  motion = 0;
  
  if (arduboy.pressed(RIGHT_BUTTON)) {
    motion |= MOTION_RIGHT;  
    if (!(collision & COLLISION_RIGHT)) {
      if (collision & COLLISION_FLOOR) {
        motion_x = min(motion_x+1,MAX_FLOOR_MOTION);
        walkSound(); 
      }
      else {
        motion_x = min(motion_x+1,MAX_MOTION);
      }
    }
  }
  else if (arduboy.pressed(LEFT_BUTTON)) {
    motion |= MOTION_LEFT;  
    if (!(collision & COLLISION_LEFT)) {
      if (collision & COLLISION_FLOOR) {
        motion_x = max(motion_x-1,-MAX_FLOOR_MOTION);
        walkSound(); 
      }
      else {
        motion_x = max(motion_x-1,-MAX_MOTION);
      }
    }
  }
  else if (collision & COLLISION_FLOOR) {
    motion_x = 0;
  }
  
  if (arduboy.pressed(B_BUTTON)) {
    motion |= MOTION_UP;  
    flySound();
    motion_y = max(motion_y-1,-MAX_MOTION);
  }
  else if (!(collision & COLLISION_FLOOR)) {
    motion |= MOTION_DOWN;  
    motion_y = min(motion_y+1,MAX_MOTION);
  }
  else {
    motion_y = 0;
  }
  
  player_x += motion_x;
  if (player_x <= 0) {
    player_x = 0;
    motion_x = 0;
  }
  else if (player_x > MAX_X) {
    player_x = MAX_X;
    motion_x = 0;
  }

  player_y += motion_y;
  if (player_y <= 0) {
    player_y = 0;
    motion_y = 0;
  }
  else if (player_y > MAX_Y) {
    player_y = MAX_Y;
    motion_y = 0;
  }
}

// Drawing
//---------------------------

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

void drawWalls(Tile tiles[4]) {
  uint8_t delx = player_x & (TILE_SIZE-1);
  uint8_t dely = player_y & (TILE_SIZE-1);
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
}

void drawPlayer() {
  uint8_t frame = 0;
  uint8_t animation = arduboy.frameCount>>2&1;
  if (collision & COLLISION_FLOOR) {
    if (motion & (MOTION_RIGHT | MOTION_LEFT)) {
      frame = 0+animation;
    }
  }
  else {
    if (motion & MOTION_UP) {
      frame = 2+animation;
    }
  }
  sprites.drawPlusMask((WIDTH/2)-(16/2),(HEIGHT/2)-(24/2),lrm_plus_mask,frame);
}

void drawStars(uint16_t seed, uint8_t layer) {
  uint16_t background = -((player_y >> layer) << 7) - (player_x >> layer);
  for(uint16_t i=0; i < WIDTH*HEIGHT; i+=seed) {
    background += seed;
    arduboy.drawPixel (background & 0x7f, (background >> 7) & 0x3f, WHITE);
  }
}

// Sounds
//-------------------------------------------

void walkSound() {
  if (!sound.playing()) {
    sound.tone(NOTE_C3,100, NOTE_REST,80);
  }
}

void flySound() {
  if (!sound.playing()) {
    sound.tone(NOTE_C2H,60, NOTE_A2H,30); 
  }
}
