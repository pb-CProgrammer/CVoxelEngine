#include "game_data.h"
#include <stdint.h>

//all data importang to draw single block
float blockData[BLOCK_SIDES * BLOCK_SIDE_DATA] = {
    // WEST
    //pos                //texCoords
    0, 0, 0, 0, 0,
    0, 0, 1, 1, 0,
    0, 1, 1, 1, 1,
    0, 1, 0, 0, 1,

    // normal
    -1, 0, 0,

    // SOUTH
    //pos                //texCoords
    0, 0, 1, 0, 0,
    1, 0, 1, 1, 0,
    1, 1, 1, 1, 1,
    0, 1, 1, 0, 1,

    // normal
    0, 0, 1,
    
    // EAST
    //pos                //texCoords
    1, 0, 1, 0, 0,
    1, 0, 0, 1, 0,   
    1, 1, 0, 1, 1,
    1, 1, 1, 0, 1,

    // normal
    1, 0, 0,

    // NORTH
    //pos                //texCoords
    1, 0, 0, 0, 0,
    0, 0, 0, 1, 0,
    0, 1, 0, 1, 1,
    1, 1, 0, 0, 1,

    // normal
    0, 0, -1,

    // TOP
    //pos                //texCoords
    1, 1, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 1,
    1, 1, 1, 0, 1,

    // normal
    0, 1, 0,

    // BOTTOM
    //pos                //texCoords
    0, 0, 0, 0, 0,
    1, 0, 0, 1, 0, 
    1, 0, 1, 1, 1,
    0, 0, 1, 0, 1,

    // normal
    0, -1, 0
};

//first in line are block which have transparency and later are those which are opaque
//indeces/positions on texture atlas
vec2 texPosis[TEXTURES_IN_ATLAS] = {
    { 0, 0 }, //FLOWER_GRASS_TOP_TEXTURE
    { 1, 0 }, //GRASS_SIDE_TEXTURE
    { 2, 0 }, //DIRT_TEXTURE
    { 0, 1 }, //STONE_TEXTURE
    { 1, 1 }, //GRASS_TOP_TEXTURE
    { 2, 1 }, //SAND_TEXTURE
    { 0, 2 }, //WATER_TEXTURE
    { 1, 2 }, //GLASS_TEXTURE
    { 2, 2 }, //AQUA_GLASS_TEXTURE
};

//what texture is on given block side, based on block id
unsigned int blockSidesTextures[BLOCKS][6] = {
    { WATER_TEXTURE, WATER_TEXTURE, WATER_TEXTURE, WATER_TEXTURE, WATER_TEXTURE, WATER_TEXTURE }, //water_block
    { GLASS_TEXTURE, GLASS_TEXTURE, GLASS_TEXTURE, GLASS_TEXTURE, GLASS_TEXTURE, GLASS_TEXTURE }, //glass_block
    { AQUA_GLASS_TEXTURE, AQUA_GLASS_TEXTURE, AQUA_GLASS_TEXTURE, AQUA_GLASS_TEXTURE, AQUA_GLASS_TEXTURE, AQUA_GLASS_TEXTURE }, //aqua_glass_block
    { DIRT_TEXTURE, DIRT_TEXTURE, DIRT_TEXTURE, DIRT_TEXTURE, DIRT_TEXTURE, DIRT_TEXTURE }, //dirt_block
    { GRASS_SIDE_TEXTURE, GRASS_SIDE_TEXTURE, GRASS_SIDE_TEXTURE, GRASS_SIDE_TEXTURE, FLOWER_GRASS_TOP_TEXTURE, DIRT_TEXTURE }, //flower_grass_block
    { STONE_TEXTURE, STONE_TEXTURE, STONE_TEXTURE, STONE_TEXTURE, STONE_TEXTURE, STONE_TEXTURE }, //stone_block
    { GRASS_SIDE_TEXTURE, GRASS_SIDE_TEXTURE, GRASS_SIDE_TEXTURE, GRASS_SIDE_TEXTURE, GRASS_TOP_TEXTURE, DIRT_TEXTURE }, //grass_block
    { SAND_TEXTURE, SAND_TEXTURE, SAND_TEXTURE, SAND_TEXTURE, SAND_TEXTURE, SAND_TEXTURE }, //sand_block
};
