#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <stdbool.h>
#include <chunk.h>

extern int startChunkX, startChunkZ, endChunkX, endChunkZ;

void chunkManagerUpdate(float playerPosX, float playerPosZ);
void chunkManagerDraw(int startChunkX, int startChunkZ, int endChunkX, int endChunkZ);
void chunkManagerInit();
void chunkManagerExit();
Chunk* getChunk(int xChunk, int zChunk);

#endif
