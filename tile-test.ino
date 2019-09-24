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

static const int MOTION_NONE = 0;
static const int MOTION_UP = 1;
static const int MOTION_DOWN = 2;
static const int MOTION_RIGHT = 4;
static const int MOTION_LEFT = 8;

static const int STARFIELD0 = 613;
static const int STARFIELD1 = 3001;

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

  // Move character
  movePlayer();
  //detectCollisions();

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
  
  // Debug
//  arduboy.setCursor(0, 28);
//  arduboy.print(player_x&0x7f);
//  arduboy.print(",");
//  arduboy.print(motion_x);
//  arduboy.setCursor(56, 0);
//  arduboy.print(player_y&0x7f);
//  arduboy.print(",");
//  arduboy.print(motion_y);
//  arduboy.setCursor(8*14, 28);
//  arduboy.print(collision);
  
  // Display results
  arduboy.display();
}

//// Player coordinate is for the upper-left corner of the screen,
//// so must offset for detection
//
//// coordinate 0: floor = 64/2+24/2 = 32+12 = 44
//// So want to check when player = 128 - 44 = 84
//
//void detectCollisions(Tile tiles[4]) {
//  uint8_t delx = player_x & (TILE_SIZE-1);
//  uint8_t dely = player_y & (TILE_SIZE-1);
//
//  collision = 0;  
//  
//  // Flat floor
//  if ((dely >= CHECK_FLOOR) && (dely < CHECK_FLOOR+CHECK_BUFFER)  &&  
//     (((delx < CHECK_OFFSET_RIGHT) && isFloor(tiles[2]))  ||
//      ((delx > CHECK_OFFSET_LEFT)  && isFloor(tiles[3])))) {
//    collision |= COLLISION_FLOOR;
//    player_y = (player_y & ~0x7f) + CHECK_FLOOR;
//    dely = CHECK_FLOOR;
//    motion_y = 0;
//  }
//  // Flat ceiling
//  if ((dely <= CHECK_CEILING) && (dely > CHECK_CEILING-CHECK_BUFFER) && 
//     (((delx < CHECK_OFFSET_RIGHT) && isCeiling(tiles[0]))     ||
//      ((delx > CHECK_OFFSET_LEFT)  && isCeiling(tiles[1]))))    {
//    collision |= COLLISION_CEILING;
//    player_y = (player_y & ~0x7f) + CHECK_CEILING;
//    dely = CHECK_CEILING;
//    motion_y = 0;
//  }
//  // Flat Right
//  if ((delx >= CHECK_RIGHT) && (delx < CHECK_RIGHT+CHECK_BUFFER) && 
//      (((dely < CHECK_CEILING) && isRight(tiles[1])) ||
//       ((dely > CHECK_FLOOR)   && isRight(tiles[3])))) {
//    collision |= COLLISION_RIGHT;
//    player_x = (player_x & ~0x7f) + CHECK_RIGHT;
//    delx = CHECK_RIGHT;
//    motion_x = 0;
//  }
//  // Flat Left
//  if ((delx <= CHECK_LEFT) && (delx > CHECK_LEFT-CHECK_BUFFER) && 
//      (((dely < CHECK_CEILING) && isLeft(tiles[0])) ||
//       ((dely > CHECK_FLOOR)   && isLeft(tiles[2])))) {
//    collision |= COLLISION_LEFT;
//    player_x = (player_x & ~0x7f) + CHECK_LEFT;
//    delx = CHECK_LEFT;
//    motion_x = 0;    
//  }
//
//  // Lower Left
//  if (((dely >= (CHECK_FLOOR-CHECK_LEFT) + delx) && (delx >= CHECK_LEFT) && (delx < 128-(CHECK_FLOOR-CHECK_LEFT)) && isLowerLeft(tiles[3])) ||
//      ((dely >= (CHECK_FLOOR-CHECK_LEFT) + delx - 128) && (delx >= 128-(CHECK_FLOOR-CHECK_LEFT)) && isLowerLeft(tiles[1])) ||
//      ((dely >= (CHECK_FLOOR-CHECK_LEFT) + delx) && (delx < CHECK_LEFT) && isLowerLeft(tiles[0]))) {
//    collision |= COLLISION_FLOOR | COLLISION_LEFT | COLLISION_SLANT;
//    player_y = (player_y & ~0x7f) + (((CHECK_FLOOR-CHECK_LEFT) + delx) & 0x7f);
//    dely = ((CHECK_FLOOR-CHECK_LEFT) + delx) & 0x7f;
//    motion_y = 0;
//    motion_x = 0;
//  }
//  // Lower Right
//  if (((dely >= (CHECK_FLOOR+CHECK_RIGHT) - delx) && (delx <= CHECK_RIGHT) && isLowerRight(tiles[2])) ||
//      ((dely >= (CHECK_FLOOR+CHECK_RIGHT) - (delx + 128)) && (delx <= (CHECK_FLOOR+CHECK_RIGHT)-128) && isLowerRight(tiles[0])) ||
//      ((dely >= (CHECK_FLOOR+CHECK_RIGHT) - delx) && (delx > CHECK_RIGHT) && isLowerRight(tiles[1]))) {
//    collision |= COLLISION_FLOOR | COLLISION_RIGHT | COLLISION_SLANT;
//    player_y = (player_y & ~0x7f) + (((CHECK_FLOOR+CHECK_RIGHT) - delx) & 0x7f);
//    dely = ((CHECK_FLOOR-CHECK_LEFT) - delx) & 0x7f;
//    motion_y = 0;
//    motion_x = 0;
//  }
//}

// Player movement
void movePlayer() {
  motion = 0;
  
  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (!(collision & COLLISION_RIGHT)) {
      motion |= MOTION_RIGHT;  
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
    if (!(collision & COLLISION_LEFT)) {
      motion |= MOTION_LEFT;  
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
    if (collision & COLLISION_SLANT) {
      motion_x = ((collision & COLLISION_LEFT) ? 1 : 0) + ((collision & COLLISION_RIGHT) ? -1 : 0);
      if (motion_x) slideSound(); 
    }
    else {
      motion_x = 0;
    }
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
  //player_x += motion_x;
  //player_y += motion_y;

  // Hard boundaries for bad maps
  //player_x = min(player_x,TILE_SIZE/2);
  //player_x = max(player_x,TILE_SIZE*MAP_WIDTH-TILE_SIZE/2);
  //player_y = min(player_y,TILE_SIZE/2);
  //player_y = max(player_y,TILE_SIZE*MAP_HEIGHT-TILE_SIZE/2);
  
}

// Drawing
//---------------------------

Tile readTile(int x, int y) {
  return static_cast<Tile>(pgm_read_byte(tile_map + (x/TILE_SIZE) + (y/TILE_SIZE)*MAP_WIDTH));
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

void drawWalls() {  
  arduboy.clear();

  // Align to top of screen, but then increment by tile size
  for (int ty = player_y - HEIGHT/2; ty < player_y + HEIGHT/2; ty += TILE_SIZE - ty % TILE_SIZE) {

    int ymax = min(ty + TILE_SIZE - (ty % TILE_SIZE),player_y + HEIGHT/2);

    // Convert to screen coordinates
    int sy = ty - (player_y - HEIGHT/2);
    int symax = ymax - (player_y - HEIGHT/2);
    
    for (int tx = player_x - WIDTH/2; tx < player_x + WIDTH/2; tx += TILE_SIZE - tx % TILE_SIZE) {
      int xmax = min(tx + TILE_SIZE - (tx % TILE_SIZE),player_x + WIDTH/2);

      Tile tile = readTile(tx,ty);

      // Convert to screen coordinates
      int sx = tx - (player_x - WIDTH/2);
      int sxmax = xmax - (player_x - WIDTH/2);

//      arduboy.setCursor(sx,sy);
//      arduboy.print(static_cast<uint8_t>(tile));
//
//      for (int x = sx; x < sx+8; x++) {
//        int offset = x+(sy/8)*WIDTH;
//        if (sy%8==0) {
//          arduboy.sBuffer[offset] |= 0xff; 
//        }
//        else {
//          arduboy.sBuffer[offset] |= 0xff<<(sy%8);           
//          arduboy.sBuffer[offset+WIDTH] |= 0xff>>(8-sy%8);           
//        }
//      }


      // Get y aligned to screen bytes
      for (int y = sy; y < symax; y += 8 - y % 8) {
        
        bool mergeTop    = (y % TILE_SIZE > 0) && (y % TILE_SIZE < 8);
        // Don't need a merge bottom, since extra bits will get overwritten

        for (int x = sx; x < sxmax; x += 1) {
          // Get tile bit pattern
          uint8_t bits = tileBits(tile,(x+player_x) % TILE_SIZE, (y+player_y) % TILE_SIZE);
          
          int offset = x + (y/8)*16;
          
          if (mergeTop) {
            uint8_t mask = 0xff >> y % 8;
            bits = (arduboy.sBuffer[offset] & mask) | (bits & ~mask);
          }
          arduboy.sBuffer[offset] = bits;
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

void slideSound() {
  if (!sound.playing()) {
    sound.tone(NOTE_D2,30, NOTE_A1,30); 
  }
}
