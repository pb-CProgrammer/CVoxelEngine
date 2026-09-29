#ifndef GAME_DATA_C
#define GAME_DATA_C

#include "constants.h"
#include <linmath.h>

//there is every data needed to run game, load everything.
//generate everything

//program gives id to each block side
typedef enum
{
    WEST_SIDE = 0,
    SOUTH_SIDE = 1,
    EAST_SIDE = 2,
    NORTH_SIDE = 3,
    TOP_SIDE = 4,
    BOTTOM_SIDE = 5,
} Side;

//every texture with coresponding id
typedef enum
{
    FLOWER_GRASS_TOP_TEXTURE = 0,
    GRASS_SIDE_TEXTURE = 1,
    DIRT_TEXTURE = 2,
    STONE_TEXTURE = 3,
    GRASS_TOP_TEXTURE = 4,
    SAND_TEXTURE = 5
} Texture;

//every block with coresponding id
typedef enum
{
    AIR_BLOCK = 0,
    DIRT_BLOCK = 1,
    FLOWER_GRASS_BLOCK = 2,
    STONE_BLOCK = 3,
    GRASS_BLOCK = 4,
    SAND_BLOCK = 5
} Block;

//program declares data arrays
extern float blockData[BLOCK_SIDES * BLOCK_SIDE_DATA];
extern vec2 texPosis[TEXTURES_IN_ATLAS];
extern unsigned int blockSidesTextures[BLOCKS][6];

#endif
