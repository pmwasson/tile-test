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
static const int COLLISION_SLANT = 16;
static const int COLLISION_FUEL = 32;

static const int MOTION_NONE = 0;
static const int MOTION_UP = 1;
static const int MOTION_DOWN = 2;
static const int MOTION_RIGHT = 4;
static const int MOTION_LEFT = 8;

static const int STARFIELD0 = 613;
static const int STARFIELD1 = 3001;

static const int MAX_FUEL = 100 << 4;

// Globals

int     player_x;
int     player_y;
int8_t  motion_x;
int8_t  motion_y;
uint8_t collision;
uint8_t motion;
bool    draw_fuel;
int     fuel_x;
int     fuel_y;
int     fuel;

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
  fuel = 0;
}

// Game loop
void loop() {

  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // Move character
  movePlayer();
  detectCollisions();

  //----------------------
  // Render
  //----------------------

  // Draw walls draws write the whole buffer, so no
  // clear is needed.
  drawWalls();

  // Draw background

  drawStars(STARFIELD0,0);
  drawStars(STARFIELD1,1);
     
  // Draw in center of screen
  drawPlayer();
  drawFuel();

  arduboy.setCursor(0, 0);
  arduboy.print("FUEL: ");
  arduboy.print(fuel>>4);
  arduboy.print("%");

  // Debug
//  arduboy.setCursor(0, 28);
//  arduboy.print(player_x%TILE_SIZE);
//  arduboy.print(",");
//  arduboy.print(motion_x);
//  arduboy.setCursor(56, 0);
//  arduboy.print(player_y%TILE_SIZE);
//  arduboy.print(",");
//  arduboy.print(motion_y);
//  arduboy.setCursor(8*14, 28);
//  arduboy.print(collision);
  
  // Display results
  arduboy.display();
}

// Player movement
void movePlayer() {
  motion = 0;
  
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (!(collision & COLLISION_RIGHT)|| (collision & COLLISION_SLANT)) {
      motion |= MOTION_RIGHT;  
      if (collision & COLLISION_FLOOR) {
        motion_x = min(motion_x+1,MAX_FLOOR_MOTION);
        if (collision & COLLISION_SLANT) {
          motion_y = -1;
        }
        walkSound(); 
      }
      else {
        motion_x = min(motion_x+1,MAX_MOTION);
      }
    }
  }
  else if (arduboy.pressed(LEFT_BUTTON)) {
    if (!(collision & COLLISION_LEFT) || (collision & COLLISION_SLANT)) {
      motion |= MOTION_LEFT;  
      if (collision & COLLISION_FLOOR) {
        motion_x = max(motion_x-1,-MAX_FLOOR_MOTION);
        if (collision & COLLISION_SLANT) {
          motion_y = -1;
        }
        walkSound(); 
      }
      else {
        motion_x = max(motion_x-1,-MAX_MOTION);
      }
    }
  }
  else if (collision & COLLISION_FLOOR) {
    if (collision & COLLISION_SLANT) {
      if (collision & COLLISION_LEFT) {
        motion_x = 1;
        motion |= MOTION_RIGHT;
      }
      else {
        motion_x = -1;
        motion |= MOTION_LEFT;
      }
      motion_y = 1;
      slideSound(); 
    }
    else {
      motion_x = 0;
    }
  }
  
  if (arduboy.pressed(B_BUTTON) && (fuel > 0)) {
    motion |= MOTION_UP;  
    flySound();
    motion_y = max(motion_y-1,-MAX_MOTION);
    decreaseFuel();
  }
  else if (!(collision & COLLISION_FLOOR)) {
    motion |= MOTION_DOWN;  
    motion_y = min(motion_y+1,MAX_MOTION);
  }

  player_x += motion_x;
  player_y += motion_y;

  // Hard boundaries for bad maps
  player_x = max(player_x,TILE_SIZE/2);
  player_x = min(player_x,TILE_SIZE*MAP_WIDTH-TILE_SIZE/2);
  player_y = max(player_y,TILE_SIZE/2);
  player_y = min(player_y,TILE_SIZE*MAP_HEIGHT-TILE_SIZE/2);
  
}

// Collision Dection
//---------------------------

void checkDown(int x, int y) {

  Tile tile = readTile(x,y);
  if (isFloor(tile) && nearMin(y)) {
    player_y -= y % TILE_SIZE;
    collision |= COLLISION_FLOOR;
    motion_y = 0;
  }
}

void checkUp(int x, int y) {
  Tile tile = readTile(x,y);
  if (isCeiling(tile) && nearMax(y)) {
    player_y += (TILE_SIZE - 1) - (y % TILE_SIZE);
    collision |= COLLISION_CEILING;
    motion_y = 0;
  }
}

void checkRight(int x, int y) {
  Tile tile = readTile(x,y);
  if (isRight(tile) && nearMin(x)) {
    player_x -= x % TILE_SIZE;
    collision |= COLLISION_RIGHT;
    motion_x = 0;
  }
}

void checkLeft(int x, int y) {
  Tile tile = readTile(x,y);
  if (isLeft(tile) && nearMax(x)) {
    player_x += (TILE_SIZE - 1) - (x % TILE_SIZE);
    collision |= COLLISION_LEFT;
    motion_x = 0;
  }
}

void checkLowerLeft(int x, int y) {
  Tile tile = readTile(x,y);
  int diff = y%TILE_SIZE - x%TILE_SIZE;
  if (isLowerLeft(tile) && (diff >= 0)) {
    player_x += diff;
    collision |= COLLISION_LEFT;
    collision |= COLLISION_FLOOR;
    collision |= COLLISION_SLANT;
    motion_x = 0;
    motion_y = 0;
  }
}

void checkLowerRight(int x, int y) {
  Tile tile = readTile(x,y);
  int diff = y%TILE_SIZE - (TILE_SIZE - 1 - x%TILE_SIZE);
  if (isLowerRight(tile) && (diff >= 0)) {
    player_x -= diff;
    collision |= COLLISION_RIGHT;
    collision |= COLLISION_FLOOR;
    collision |= COLLISION_SLANT;
    motion_x = 0;
    motion_y = 0;
  }
}

void checkUpperRight(int x, int y) {
  Tile tile = readTile(x,y);
  int diff = y%TILE_SIZE - x%TILE_SIZE;
  if (isUpperRight(tile) && (diff <= 0)) {
    player_x += diff-1;
    collision |= COLLISION_RIGHT;
    collision |= COLLISION_CEILING;
    collision |= COLLISION_SLANT;
    motion_x = 0;
  }
}

void checkUpperLeft(int x, int y) {
  Tile tile = readTile(x,y);
  int diff = y%TILE_SIZE - (TILE_SIZE - 1 - x%TILE_SIZE);
  if (isUpperLeft(tile) && (diff <= 0)) {
    player_x -= diff-1;
    collision |= COLLISION_LEFT;
    collision |= COLLISION_CEILING;
    collision |= COLLISION_SLANT;
    motion_x = 0;
  }
}

void checkFuel(int x, int y) {
  Tile tile = readTile(x,y);
  if (isFuel(tile) && nearMax(y) && nearCenter(x)) {
    collision |= COLLISION_FUEL;
  }
  
}

int playerUp() {
  return player_y - PLAYER_HEIGHT/2 - 1;
}

int playerDown() {
  return player_y + PLAYER_HEIGHT/2;
}

int playerLeft() {
  return player_x - PLAYER_WIDTH/2 - 1;
}

int playerRight() {
  return player_x + PLAYER_WIDTH/2;
}

// Create 4 points outside but bounding player
void detectCollisions() {

  collision = COLLISION_NONE;
  
  checkUp(        playerLeft()+1,   playerUp());
  checkUp(        playerRight()-1,  playerUp());
  checkDown(      playerLeft()+1,   playerDown());
  checkDown(      playerRight()-1,  playerDown());
  checkLeft(      playerLeft(),     playerUp()+1);
  checkLeft(      playerLeft(),     playerDown()-1);
  checkRight(     playerRight(),    playerUp()+1);
  checkRight(     playerRight(),    playerDown()-1);
  checkLowerLeft( playerLeft()+1,   playerDown()-1);
  checkLowerRight(playerRight()-1,  playerDown()-1);
  checkUpperRight(playerRight()-1,  playerUp()+1);
  checkUpperLeft( playerLeft()+1,   playerUp()+1);
  checkFuel(      player_x,         playerDown()-1);
}


// Drawing
//---------------------------

Tile readTile(int x, int y) {
  return static_cast<Tile>(pgm_read_byte(tile_map + (x/TILE_SIZE) + (y/TILE_SIZE)*MAP_WIDTH));
}

// Calculate bits based on location within tile
uint8_t tileBits(Tile tile, int8_t x, int8_t y) {
  switch(tile) {
    case Tile::empty:
    case Tile::fuel:        return 0x00;
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

void drawWalls() {
      
  int xoffset, yoffset;  
  int mxoffset = player_x - WIDTH/2;
  int myoffset = player_y - HEIGHT/2;

  draw_fuel = false;
  
  for (int ty = 0; ty < HEIGHT; ty += yoffset) {
    yoffset = ty ? TILE_SIZE : TILE_SIZE - myoffset % TILE_SIZE;
    
    for (int tx = 0; tx < WIDTH; tx += tx ? TILE_SIZE : xoffset) {
      xoffset = tx ? TILE_SIZE : TILE_SIZE - mxoffset % TILE_SIZE;

      // Convert to map coordinates
      Tile tile = readTile(tx+mxoffset,ty+myoffset);

      // Check for fuel (but draw later)
      // map constrained to only have 1 visible at a time.
      if (tile == Tile::fuel) {
        draw_fuel = true;
        fuel_x = ((tx+ mxoffset)/TILE_SIZE)*TILE_SIZE;
        fuel_y = ((ty+ myoffset)/TILE_SIZE)*TILE_SIZE;
      }
            
      for (int y = ty; y < min(HEIGHT,ty+yoffset); y += 8-y%8) {
        for (int x = tx; x < min(WIDTH,tx+xoffset); x += 1) {
          int offset = (y/8)*WIDTH+x;
          int mask = 0xff << y %8;
          int bits = tileBits(tile,(x+mxoffset)%TILE_SIZE,(y+myoffset)%TILE_SIZE) << y%8;
          arduboy.sBuffer[offset] = arduboy.sBuffer[offset]&~mask | (bits&mask);
        }
      }
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
  sprites.drawPlusMask((WIDTH/2)-(PLAYER_WIDTH/2),(HEIGHT/2)-(PLAYER_HEIGHT/2),lrm_plus_mask,frame);
}

void drawStars(uint16_t seed, uint8_t layer) {
  uint16_t background = -((player_y >> layer) << 7) - (player_x >> layer);
  for(uint16_t i=0; i < WIDTH*HEIGHT; i+=seed) {
    background += seed;
    arduboy.drawPixel (background & 0x7f, (background >> 7) & 0x3f, WHITE);
  }
}

void drawFuel() {
  int x = fuel_x - player_x + (WIDTH/2 + TILE_SIZE/2 - FUEL_WIDTH/2); // Horizontal center of tile
  int y = fuel_y - player_y + (HEIGHT/2 + TILE_SIZE - FUEL_HEIGHT);   // Bottom of tile
  
  if (draw_fuel) {
    if (collision & COLLISION_FUEL) {
      sprites.drawPlusMask(x,y,fuel_plus_mask,(arduboy.frameCount>>3) & 3);
      if (fuel < MAX_FUEL) {
        increaseFuel();
        fuelSound();
      }
    }
    else {
      sprites.drawPlusMask(x,y,fuel_plus_mask,0);      
    }
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
    if (fuel > (10<<4)) {
      sound.tone(NOTE_C2H,60, NOTE_A2H,30);
    }
    else { 
      sound.tone(NOTE_D2H,60, NOTE_REST,30);
    }
  }
}

void slideSound() {
  if (!sound.playing()) {
    sound.tone(NOTE_D2,30, NOTE_A1,30); 
  }
}

void fuelSound() {
  if (!sound.playing()) {
    sound.tone(NOTE_C3H,150, NOTE_E3H,150, NOTE_G3H,150); 
  }
}

void increaseFuel() {
  fuel = min(fuel+10,MAX_FUEL);
}

void decreaseFuel() {
  fuel = max(fuel-2,0);
}
