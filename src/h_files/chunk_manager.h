#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <stdbool.h>

extern int startChunkX, startChunkZ, endChunkX, endChunkZ;

void chunkManagerUpdate(float playerPosX, float playerPosZ);
void chunkManagerDraw(int startChunkX, int startChunkZ, int endChunkX, int endChunkZ);
void chunkManagerInit();
void chunkManagerExit();
bool isInsideBlock(float xPos, float yPos, float zPos);

#endif
