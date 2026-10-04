#include "math_help_functions.h"

#include <stdio.h>
#include <stdint.h>
#include <linmath.h>
#include "constants.h"

//this is up vector, for view matrix
vec3 absoluteUp = (vec3){0.0f, 1.0f, 0.0f};

void print_vec3(float* target)
{
    printf("%f %f %f\n", target[0], target[1], target[2]);
};

void vec3_set(float* origin, float* target)
{
    //that function is important, because sometimes (vec)a + (vec)b doesnt work
    target[0] = origin[0];
    target[1] = origin[1];
    target[2] = origin[2];
}

void indexToPos(int index, float* dest)
{
    //safety check
    if(index < 0 || index > CHUNK_VOLUME)
    {
        printf("chunk block index is out of range\n");
        return;
    }

    //its hard to explain z, you have to understand it by yourself
    dest[0] = (float)(index % CHUNK_SIZE);
    dest[1] = floor((float)index / CHUNK_AREA);
    dest[2] = (float)((index - index % CHUNK_SIZE) % CHUNK_AREA) / CHUNK_SIZE;
};

void posToIndex(float* pos, int* index)
{
    //also safety check
    if(pos[0] < 0 || pos[0] >= CHUNK_SIZE || pos[1] < 0 || pos[1] >= CHUNK_HEIGHT || pos[2] < 0 || pos[2] >= CHUNK_SIZE)
    {
        printf("chunk block pos is out of chunk (%f, %f, %f)\n", pos[0], pos[1], pos[2]);
        return;
    }

    //hard to explain, but easy to understand
    *index = pos[0];
    *index += pos[1] * CHUNK_AREA;
    *index += pos[2] * CHUNK_SIZE;
}

float degToRad(float deg)
{
    return deg * (M_PI / 180.0f);
};

void vec2ToHashKey(int x, int z, uint64_t* key)
{
    //this function just takes two 32 bit ints, and combine them into one 64 bit int
    //simplier saying if this function will take 12 and 34, it will make key 1234
    *key = ((uint64_t)(uint32_t)x << 32) | (uint32_t)z;
}

void posToChunkData(float* pos, int* chunkX, int* chunkZ, int* chunkXPos, int* chunkYPos, int* chunkZPos, bool* isChunkYPosOk)
{
    *chunkX = (int)floorf(pos[0] / CHUNK_SIZE);
    *chunkZ = (int)floorf(pos[2] / CHUNK_SIZE);

    int worldBlockX = (int)floorf(pos[0]);
    int worldBlockZ = (int)floorf(pos[2]);

    *chunkXPos = worldBlockX - (*chunkX * CHUNK_SIZE);
    *chunkZPos = worldBlockZ - (*chunkZ * CHUNK_SIZE);
    
    *chunkYPos = (int)floorf(pos[1]);

    if(*chunkYPos < 0 || *chunkYPos >= CHUNK_HEIGHT)
    {
        *isChunkYPosOk = false;
    }
    else
    {
        *isChunkYPosOk = true;
    }
}