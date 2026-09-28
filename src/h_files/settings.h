#ifndef SETTINGS_H
#define SETTINGS_H

//window opengl stuff
extern int startWindowWidth;
extern int startWindowHeight;
extern int openglVersionMajor;
extern int openglVersionMinor;
extern const char* windowTitle;
//FPS UPS

//player stuff
extern float mouseInputSensitivity;
extern float playerSpeed;

//game stuff
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 128
#define CHUNK_AREA (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_VOLUME (CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE)
#define BLOCK_DATA_STRIDE 9
#define BLOCK_SIDE_DATA (6 * BLOCK_DATA_STRIDE)
#define BLOCK_SIDES 6
#define BLOCKS 5
#define TEXTURES_IN_ATLAS_WIDTH 3
#define TEXTURES_IN_ATLAS_HEIGHT 2
#define TEXTURES_IN_ATLAS (TEXTURES_IN_ATLAS_WIDTH * TEXTURES_IN_ATLAS_HEIGHT)
extern int drawAreaInChunks;
extern int worldSeed;

void initSettings();

#endif
