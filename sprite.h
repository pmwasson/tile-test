// Player Data

static const int PLAYER_HEIGHT = 24;
static const int PLAYER_WIDTH = 16;

static const int COLLISION_OFFSET_Y = PLAYER_HEIGHT/2;
static const int COLLISION_OFFSET_X = PLAYER_WIDTH/2;

const unsigned char PROGMEM lrm_plus_mask[] =
{
// width, height,
16, 24,
// FRAME 00
0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xf8, 0xf8, 0xc6, 0xfe, 0xf2, 0xfe, 0xf2, 0xfe, 
0xf2, 0xfe, 0xf2, 0xfe, 0xc6, 0xfe, 0xf8, 0xf8, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x80, 0x00, 0x00, 
0xbf, 0xff, 0x7c, 0xff, 0x01, 0x03, 0x02, 0x03, 0xdd, 0xff, 0x9b, 0xff, 0xdb, 0xff, 0x9b, 0xff, 
0x9b, 0xff, 0xdb, 0xff, 0x9b, 0xff, 0xdd, 0xff, 0x02, 0x03, 0x01, 0x03, 0x7c, 0xff, 0xbf, 0xff, 
0x01, 0x01, 0x01, 0x01, 0x80, 0x80, 0xc0, 0xc0, 0xdf, 0xff, 0xef, 0xff, 0x01, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x01, 0xef, 0xff, 0xdf, 0xff, 0xc0, 0xc0, 0x80, 0x80, 0x01, 0x01, 0x01, 0x01, 

// FRAME 01
0x80, 0x80, 0x00, 0xc0, 0xe0, 0xe0, 0x60, 0xe0, 0xfc, 0xfc, 0xe3, 0xff, 0xf9, 0xff, 0xf9, 0xff, 
0xf9, 0xff, 0xf9, 0xff, 0xe3, 0xff, 0xfc, 0xfc, 0x60, 0xe0, 0xe0, 0xe0, 0x00, 0xc0, 0x80, 0x80, 
0xdf, 0xff, 0xbe, 0xff, 0x00, 0x01, 0x01, 0x01, 0xee, 0xff, 0xcd, 0xff, 0xed, 0xff, 0x4d, 0x7f, 
0x4d, 0x7f, 0xed, 0xff, 0xcd, 0xff, 0xee, 0xff, 0x01, 0x01, 0x00, 0x01, 0xbe, 0xff, 0xdf, 0xff, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xe0, 0xe0, 0x6f, 0x7f, 0x77, 0x7f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x77, 0x7f, 0x6f, 0x7f, 0xe0, 0xe0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 

// FRAME 02
0x00, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdc, 0xfc, 0x5f, 0xff, 0xbf, 0xff, 0xbf, 0xff, 
0xbf, 0xff, 0xbf, 0xff, 0x5f, 0xff, 0xdc, 0xfc, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0xc0, 0x00, 0x80, 
0xff, 0xff, 0xbe, 0xff, 0x01, 0x01, 0x01, 0x01, 0x7f, 0xff, 0x00, 0xff, 0xdf, 0xff, 0xaf, 0xff, 
0xef, 0xff, 0xdf, 0xff, 0x00, 0xff, 0x7f, 0xff, 0x01, 0x01, 0x01, 0x01, 0xbe, 0xff, 0xff, 0xff, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x80, 0xff, 0x3e, 0x7f, 0x2f, 0x7f, 0x1b, 0x3f, 0x1f, 0x3f, 
0x0e, 0x1f, 0x1b, 0x3f, 0x1f, 0x7f, 0x2c, 0x7f, 0xb8, 0xfe, 0xc0, 0xfc, 0x00, 0x00, 0x00, 0x00, 

// FRAME 03
0x00, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdc, 0xfc, 0x5f, 0xff, 0xbf, 0xff, 0xbf, 0xff, 
0xbf, 0xff, 0xbf, 0xff, 0x5f, 0xff, 0xdc, 0xfc, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0xc0, 0x00, 0x80, 
0xff, 0xff, 0xbe, 0xff, 0x01, 0x01, 0x01, 0x81, 0x3f, 0xff, 0xc0, 0xff, 0xdf, 0xff, 0xef, 0xff, 
0xef, 0xff, 0xdf, 0xff, 0x80, 0xff, 0x3f, 0xff, 0x01, 0x01, 0x01, 0x01, 0xbe, 0xff, 0xff, 0xff, 
0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xe0, 0xe3, 0x63, 0x7f, 0x6f, 0x7f, 0x1c, 0x3f, 0x07, 0x7f, 
0x3f, 0x7f, 0x11, 0x3f, 0x4f, 0x7f, 0x62, 0x7f, 0xe0, 0xe7, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00,
};

// Refuelling station

static const int FUEL_HEIGHT = 8;
static const int FUEL_WIDTH = 16;

const unsigned char PROGMEM fuel_plus_mask[] =
{
// width, height,
16, 8,
// 0
0xFC, 0xFC, 0xA4, 0xE4, 0xE4, 0xE4, 0xAF, 0xEF, 0xA0, 0xE0, 
0xE0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 
0xE0, 0xE0, 0xA0, 0xE0, 0xAF, 0xEF, 0xE4, 0xE4, 0xA4, 0xE4, 
0xFC, 0xFC, 
// 1
0xFC, 0xFC, 0xE4, 0xE4, 0xE4, 0xE4, 0xAF, 0xEF, 0xA0, 0xE0, 
0xE0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 
0xE0, 0xE0, 0xE0, 0xE0, 0xEF, 0xEF, 0xE4, 0xE4, 0xA4, 0xE4, 
0xFC, 0xFC,
// 2
0xFC, 0xFC, 0xA4, 0xE4, 0xE4, 0xE4, 0xEF, 0xEF, 0xE0, 0xE0, 
0xE0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 0xA0, 0xE0, 
0xE0, 0xE0, 0xA0, 0xE0, 0xAF, 0xEF, 0xE4, 0xE4, 0xE4, 0xE4, 
0xFC, 0xFC,
// 3
0xFC, 0xFC, 0xA4, 0xE4, 0xE4, 0xE4, 0xAF, 0xEF, 0xA0, 0xE0, 
0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 
0xE0, 0xE0, 0xA0, 0xE0, 0xAF, 0xEF, 0xE4, 0xE4, 0xA4, 0xE4, 
0xFC, 0xFC
};


static const int ROCKET_HEIGHT = 40;
static const int ROCKET_WIDTH = 58;

const unsigned char PROGMEM rocket_plus_mask[] =
{
// width, height,
58, 40,
// FRAME 00
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x02, 0x1f, 0x04, 0xff, 0xfc, 0xfe, 0xfc, 0xfe, 0xfc, 0xfe, 0xfc, 0xfe, 0xf8, 0xfc, 0xf8, 0xfc, 0xf8, 0xfc, 0xf0, 0xf8, 0xf0, 0xf8, 0xe0, 0xf0, 0x40, 0xe0, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 
0x00, 0xc0, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xc0, 0x80, 0xe0, 0x40, 0xf0, 0xa0, 0xf0, 0x10, 0xf8, 0xc0, 0xff, 0xfb, 0xff, 0xfb, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfd, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x3f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1e, 0xff, 0x1e, 0xff, 0x1e, 0xff, 0x0e, 0xff, 0x24, 0xfe, 0x70, 0xfe, 0xf8, 0xfe, 0xf8, 0xfc, 0xf8, 0xfc, 0xf0, 0xf8, 0xf0, 0xf8, 0xe0, 0xf0, 0x00, 0xe0, 0x80, 0xe0, 0x00, 0xc0, 0x80, 0xc0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x00, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 
0xe7, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0x81, 0xff, 0x00, 0xff, 0x00, 0xff, 0x3c, 0xff, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e, 0xff, 0x3c, 0xff, 0x00, 0xff, 0x00, 0xff, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xff, 0x14, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x55, 0xff, 0x2a, 0xff, 0x14, 0x7e, 0x00, 0x3c, 
0x00, 0x03, 0x02, 0x07, 0x05, 0x0f, 0x02, 0x0f, 0x04, 0x1f, 0x03, 0xff, 0x17, 0xff, 0xcf, 0xff, 0x97, 0xff, 0xaf, 0xff, 0x9f, 0xff, 0xaf, 0xff, 0x5f, 0xff, 0x3f, 0xff, 0x5f, 0xff, 0x3f, 0xff, 0x5f, 0xff, 0xbf, 0xff, 0x7f, 0xff, 0xbf, 0xff, 0x7f, 0xff, 0xbf, 0xff, 0x7f, 0xff, 0xbf, 0xff, 0x7f, 0xff, 0xbf, 0xff, 0x7f, 0xff, 0xbf, 0xff, 0x7f, 0xff, 
0xbf, 0xff, 0x7f, 0xff, 0xbf, 0xff, 0x7f, 0xff, 0xbf, 0xff, 0x7e, 0xff, 0xbc, 0xff, 0x78, 0xff, 0xb8, 0xff, 0x78, 0xff, 0x38, 0xff, 0x58, 0xff, 0x30, 0xff, 0x04, 0x7f, 0x0e, 0x7f, 0x1f, 0x7f, 0x0f, 0x3f, 0x17, 0x3f, 0x0b, 0x1f, 0x07, 0x1f, 0x03, 0x0f, 0x00, 0x07, 0x02, 0x07, 0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0xe0, 0x40, 0xf8, 0x20, 0xff, 0x3c, 0x7f, 0x3f, 0x7f, 0x3f, 0x7f, 0x3f, 0x7f, 0x1f, 0x3f, 0x1f, 0x3f, 0x1f, 0x3f, 0x0f, 0x1f, 0x0f, 0x1f, 0x07, 0x0f, 0x02, 0x07, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 
0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};







    
