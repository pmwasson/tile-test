static const int TILE_SIZE    = 64;
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

bool isEdge(int d) {
  return (d % TILE_SIZE == 0) || (d % TILE_SIZE == TILE_SIZE_M1);
}

static const int MAP_WIDTH    = 8;
static const int MAP_HEIGHT   = 10;

static const int START_X      = 2*TILE_SIZE + TILE_SIZE/2;
static const int START_Y      = 7*TILE_SIZE + TILE_SIZE/2;

const uint8_t PROGMEM tile_map[] =
{
  7,7,7,7,7,7,7,7,
  7,1,1,1,1,1,1,7,
  7,1,0,0,0,0,1,7,
  7,1,0,5,4,0,1,7,
  7,1,0,3,2,5,1,7,
  7,1,0,0,0,3,1,7,
  7,1,0,1,1,0,1,7,
  7,1,0,0,0,0,1,7,
  7,1,1,1,1,1,1,7,
  7,7,7,7,7,7,7,7
};
