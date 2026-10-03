#ifndef MATH_HELP_FUNCTIONS_H
#define MATH_HELP_FUNCTIONS_H

#include <stdbool.h>
#include <stdint.h>
#include <linmath.h>

extern vec3 absoluteUp;

void print_vec3(float* target);
void vec3_set(float* origin, float* target);
void indexToPos(int index, float* dest);
void posToIndex(float* pos, int* index);
float degToRad(float deg);
void vec2ToHashKey(int x, int z, uint64_t* key);
void posToChunkData(float* pos, int* chunkX, int* chunkZ, int* chunkXPos, int* chunkYPos, int* chunkZPos, bool* isChunkYPosOk);

#endif
