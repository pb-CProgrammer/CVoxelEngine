#ifndef CONSTANTS_H
#define CONSTANTS_H

//window constants
#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 6
#define TARGET_UPS 200
#define TARGET_FPS 60

//voxel engine constants
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 128
#define CHUNK_AREA (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_VOLUME (CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE)
#define BLOCK_SIDE_DATA 23 // 4 * 3 int for positions + 4 * 2 int for texturePos + 1 * 3 int for normal
#define DRAW_DATA_STRIDE 8
#define BLOCK_SIDES 6
#define BLOCKS 8
#define TRANSPARENT_BLOCKS 4 //how much firsts blocks have transparency
#define TEXTURES_IN_ATLAS_WIDTH 3
#define TEXTURES_IN_ATLAS_HEIGHT 3
#define TEXTURES_IN_ATLAS (TEXTURES_IN_ATLAS_WIDTH * TEXTURES_IN_ATLAS_HEIGHT)
#define HITBOX_VERTICES 8

#endif