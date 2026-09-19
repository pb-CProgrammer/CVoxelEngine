#ifndef VOXEL_ENGINE_H
#define VOXEL_ENGINE_H

#include <stdbool.h>
#include "linmath.h"
#include "player.h"

//this is to fix race condition problem in my program
//update thread will with mutex safely write data here
//and draw thread will safely with mutex read data from here
typedef struct SafeData
{
    Player player;
    int startChunkX, startChunkZ, endChunkX, endChunkZ;
} SafeData;

void voxelEngineUpdate();
void voxelEngineDraw();
bool voxelEngineInit();
bool voxelEngineExit();
void writeToSafeData();
void readFromSafeData();
void processVoxelEngineMouseInput(double xPos, double yPos);
void processVoxelEngineKeyboardInput(int key, int action);

#endif
