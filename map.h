static const int MAP_WIDTH    = 8;
static const int MAP_HEIGHT   = 8;
static const int TILE_SIZE    = 128;
static const int TILE_SIZE_M1 = (TILE_SIZE-1);
static const int START_X      = 4*TILE_SIZE;
static const int START_Y      = 1*TILE_SIZE;

enum class Tile : char {
  empty = 0,
  wall = 1,
  upperLeft = 2,
  upperRight = 3,
  lowerLeft = 4,
  lowerRight = 5,
  dirt = 6
};

const uint8_t PROGMEM tile_map[] =
{
  1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,1,
  1,1,1,1,1,1,0,1,
  1,0,0,0,0,0,0,1,
  1,0,4,0,0,5,0,1,
  1,0,3,1,1,2,0,1,
  1,4,0,0,0,0,5,1,
  1,1,1,1,1,1,1,1
};
