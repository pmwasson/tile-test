static const int TILE_SIZE    = 64;   // should be 32, 64 or 128
static const int TILE_SIZE_M1 = (TILE_SIZE-1);

enum class Tile : char {
  empty = 0,
  wall = 1,
  upperLeft = 2,
  upperRight = 3,
  lowerLeft = 4,
  lowerRight = 5,
  fuel = 6,
  dirt = 7
};


bool isFloor(Tile t) {
  return (t == Tile::wall) || (t == Tile::upperLeft) || (t == Tile::upperRight);
}

bool isCeiling(Tile t) {
  return (t == Tile::wall) || (t == Tile::lowerLeft) || (t == Tile::lowerRight);
}

bool isRight(Tile t) {
  return (t == Tile::wall) || (t == Tile::upperLeft) || (t == Tile::lowerLeft);
}

bool isLeft(Tile t) {
  return (t == Tile::wall) || (t == Tile::upperRight) || (t == Tile::lowerRight);
}

bool isUpperLeft(Tile t) {
  return (t == Tile::upperLeft);
}

bool isUpperRight(Tile t) {
  return (t == Tile::upperRight);
}

bool isLowerLeft(Tile t) {
  return (t == Tile::lowerLeft);
}

bool isLowerRight(Tile t) {
  return (t == Tile::lowerRight);
}

bool isFuel(Tile t) {
  return (t == Tile::fuel);
}

bool isEdge(int d) {
  return (d % TILE_SIZE == 0) || (d % TILE_SIZE == TILE_SIZE_M1);
}

bool nearMin(int d) {
  return (d % TILE_SIZE < 8);
}

bool nearMax(int d) {
  return (d % TILE_SIZE > (TILE_SIZE-8));
}

bool nearCenter(int d) {
  return ((d % TILE_SIZE > (TILE_SIZE/2-8)) && (d % TILE_SIZE < (TILE_SIZE/2+8)));
}

/*
static const int MAP_WIDTH    = 32;
static const int MAP_HEIGHT   = 32;

static const int START_X      = 7*TILE_SIZE + TILE_SIZE/2;
static const int START_Y      = 4*TILE_SIZE + TILE_SIZE/2;

const uint8_t PROGMEM tile_map[] =
{
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
  7,1,1,2,0,0,0,0,0,0,0,0,3,1,1,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,1,7,
  7,1,2,0,0,0,0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,7,
  7,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,3,1,2,0,0,0,0,1,7,
  7,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,5,1,0,1,1,4,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,1,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,1,1,0,6,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,3,1,1,1,1,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,3,1,1,2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,4,0,0,0,0,0,0,0,0,0,0,5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,1,4,0,0,0,0,0,0,0,0,5,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,7,
  7,1,1,1,1,1,1,0,1,1,1,1,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,1,2,0,0,0,0,0,0,0,0,3,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,7,
  7,1,2,0,0,0,0,0,0,0,0,0,0,3,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,2,5,1,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,5,1,1,4,0,0,0,0,1,6,0,1,2,5,2,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,5,1,1,1,1,4,0,0,0,1,1,1,2,5,2,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,1,1,7,7,1,1,0,0,0,0,0,0,5,2,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,1,1,7,7,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,3,1,1,1,1,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,3,1,1,2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,4,0,0,0,0,0,0,0,0,0,0,5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,
  7,1,1,4,0,0,0,6,0,0,0,0,5,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,1,7,
  7,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};
*/

static const int MAP_WIDTH    = 16;
static const int MAP_HEIGHT   = 16;

static const int START_X      = 2*TILE_SIZE + TILE_SIZE/2;
static const int START_Y      = 12*TILE_SIZE + TILE_SIZE/2;

const uint8_t PROGMEM tile_map[] =
{
  // 16 x 16
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,2,0,0,0,0,0,0,0,0,0,0,0,0,3,1,
  1,0,0,0,6,0,0,0,0,0,0,0,0,0,0,1,
  1,1,0,1,1,1,1,4,0,5,1,1,1,1,1,1,
  1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,
  1,1,0,1,1,1,1,2,0,3,1,1,0,0,0,1,
  1,1,0,1,1,1,2,0,0,0,3,1,1,1,0,1,
  1,1,0,1,1,0,0,0,0,6,0,0,0,0,0,1,
  1,1,0,1,1,4,3,1,1,1,1,1,0,1,1,1,
  1,1,0,1,1,1,4,3,1,1,1,2,5,1,1,1,
  1,1,0,1,1,1,1,4,3,1,2,5,1,1,1,1,
  1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,
  1,1,0,1,2,0,0,0,0,0,0,0,0,3,1,1,
  1,1,0,1,0,0,0,0,1,1,0,0,0,0,3,1,
  1,0,6,0,0,0,0,1,1,1,1,0,0,6,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};
