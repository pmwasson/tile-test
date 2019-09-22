static const int MAP_WIDTH  = 8;
static const int MAP_HEIGHT = 8;
static const int TILE_SIZE  = 128;
static const int TILE_SIZE_M1  = (TILE_SIZE-1);

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
  1,1,0,0,0,0,1,1,
  1,0,5,4,5,4,0,1,
  0,5,1,1,1,1,4,0,
  0,3,1,1,1,1,2,0,
  6,0,3,1,1,2,0,1,
  6,6,0,3,2,0,6,1,
  6,6,6,0,0,6,6,1,
  6,6,6,6,6,6,6,1
};
