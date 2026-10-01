#include "chunk_manager.h"

#include "constants.h"
#include "game_data.h"
#include "settings.h"
#include "math_help_functions.h"
#include "chunk.h"
#include <math.h>
#include <osn-noise.h>
#include <khash.h>

//hash map for chunks
KHASH_MAP_INIT_INT64(chunk_map, Chunk*)
khash_t(chunk_map) *chunkMap;

int startChunkX, startChunkZ, endChunkX, endChunkZ;

void calculateDrawArea(float playerPosX, float playerPosZ, int* startChunkX, int* startChunkZ, int* endChunkX, int* endChunkZ)
{   
    //calculate player pos in chunks
    float currentChunkX = playerPosX / (float)CHUNK_SIZE;
    float currentChunkZ = playerPosZ / (float)CHUNK_SIZE;

    if(drawAreaInChunks % 2 != 0)
    {
        //if draw area is odd, program just subtract 1 from it, and add to player pos in each direction
        *startChunkX = (int)floor(currentChunkX) - (drawAreaInChunks - 1) / 2;
        *startChunkZ = (int)floor(currentChunkZ) - (drawAreaInChunks - 1) / 2;

        *endChunkX = (int)floor(currentChunkX) + (drawAreaInChunks - 1) / 2;
        *endChunkZ = (int)floor(currentChunkZ) + (drawAreaInChunks - 1) / 2;
    }
    else
    {
        //else program first calculates square of chunks which for sure will be drawn
        *startChunkX = (int)floor(currentChunkX) - (drawAreaInChunks - 2) / 2;
        *startChunkZ = (int)floor(currentChunkZ) - (drawAreaInChunks - 2) / 2;

        *endChunkX = (int)floor(currentChunkX) + (drawAreaInChunks - 2) / 2;
        *endChunkZ = (int)floor(currentChunkZ) + (drawAreaInChunks - 2) / 2;

        //and based on player pos in chunk, program add 1 to side that is closer to player
        float offsetInChunkX, offsetInChunkZ;

        if(currentChunkX >= 0)
        {
            offsetInChunkX = currentChunkX - floor(currentChunkX);
            offsetInChunkZ = currentChunkZ - floor(currentChunkZ);
        }
        else
        {
            offsetInChunkX = floor(currentChunkX) * -1 - currentChunkX * -1;
            offsetInChunkZ = floor(currentChunkZ) * -1 - currentChunkZ * -1;
        }

        if(offsetInChunkX < 0.5f)
        {
            *startChunkX -= 1;
        }
        else
        {
            *endChunkX += 1;
        }

        if(offsetInChunkZ < 0.5f)
        {
            *startChunkZ -= 1;
        }
        else
        {
            *endChunkZ += 1;
        }
    }
};

void generateChunkArea(int startChunkX, int startChunkZ, int endChunkX, int endChunkZ)
{
    //generates given square of chunks if they dont exists
    for(int chunkX = startChunkX; chunkX <= endChunkX; chunkX++)
    {
        for(int chunkZ = startChunkZ; chunkZ <= endChunkZ; chunkZ++)
        {
            uint64_t key;
            vec2ToHashKey(chunkX, chunkZ, &key);

            int ret;
            khint_t foundChunk = kh_put(chunk_map, chunkMap, key, &ret);

            if(ret)
            {
                //if chunk doesnt exists we malloc it
                Chunk* chunk = malloc(sizeof(Chunk));
                chunk->pos[0] = chunkX;
                chunk->pos[1] = chunkZ;
                chunk->generatedDrawData = false;
                chunk->compiledDrawData = false;

                kh_value(chunkMap, foundChunk) = chunk;

                generateChunk(chunk);
            }
        }
    }
};

void chunkManagerUpdate(float playerPosX, float playerPosZ)
{
    calculateDrawArea(playerPosX, playerPosZ, &startChunkX, &startChunkZ, &endChunkX, &endChunkZ);

    //firstly program generates larger square of chunks, its because generateChunkDrawData() needs 4 surrounding chunks
    generateChunkArea(startChunkX -1, startChunkZ -1, endChunkX + 1, endChunkZ + 1);

    //in smaller chunk program firstly gets all surrounding chunks, then if chunk hasnt generated draw data, program generates them and compile them
    //and when everything is done, program draws chunk
    for(int chunkX = startChunkX; chunkX <= endChunkX; chunkX++)
    {
        for(int chunkZ = startChunkZ; chunkZ <= endChunkZ; chunkZ++)
        {
            uint64_t key;
            vec2ToHashKey(chunkX, chunkZ, &key);

            khint_t foundChunk = kh_get(chunk_map, chunkMap, key);

            if(foundChunk != kh_end(chunkMap))
            {
                Chunk* chunk = kh_value(chunkMap, foundChunk);

                vec2ToHashKey(chunkX - 1, chunkZ, &key);
                foundChunk = kh_get(chunk_map, chunkMap, key);
                Chunk* westChunk = kh_value(chunkMap, foundChunk);

                vec2ToHashKey(chunkX + 1, chunkZ, &key);
                foundChunk = kh_get(chunk_map, chunkMap, key);
                Chunk* eastChunk = kh_value(chunkMap, foundChunk);

                vec2ToHashKey(chunkX, chunkZ + 1, &key);
                foundChunk = kh_get(chunk_map, chunkMap, key);
                Chunk* southChunk = kh_value(chunkMap, foundChunk);

                vec2ToHashKey(chunkX, chunkZ - 1, &key);
                foundChunk = kh_get(chunk_map, chunkMap, key);
                Chunk* northChunk = kh_value(chunkMap, foundChunk);

                if(!chunk->generatedDrawData)
                {
                    generateChunkDrawData(chunk, westChunk, eastChunk, southChunk, northChunk);
                }
            }
        }
    }
};

//this function calculates which chunks to draw and draws them
void chunkManagerDraw(int startChunkX, int startChunkZ, int endChunkX, int endChunkZ)
{
    for(int chunkX = startChunkX; chunkX <= endChunkX; chunkX++)
    {
        for(int chunkZ = startChunkZ; chunkZ <= endChunkZ; chunkZ++)
        {
            uint64_t key;
            vec2ToHashKey(chunkX, chunkZ, &key);

            khint_t foundChunk = kh_get(chunk_map, chunkMap, key);

            if(foundChunk != kh_end(chunkMap))
            {
                Chunk* chunk = kh_value(chunkMap, foundChunk);

                if(!chunk->compiledDrawData)
                {
                    compileChunkDrawData(chunk);
                }

                chunkDraw(chunk);
            }
        }
    }
};

//function inits hash map and osn noise contex
void chunkManagerInit()
{
    chunkMap = kh_init(chunk_map);
    open_simplex_noise(worldSeed, &osn_ctx);
};

void chunkManagerExit()
{
    for(khint_t i = kh_begin(chunkp); i != kh_end(chunkMap); ++i)
    {
        if(kh_exist(chunkMap, i))
        {
            Chunk* chunk = kh_value(chunkMap, i);

            //free opengl stuff
            glDeleteBuffers(1, &chunk->VBO);
            glDeleteVertexArrays(1, &chunk->VAO);

            //free chunk itself
            free(chunk);

            //program dont free chunk draw data, because it was set free earlier
        }
    }

    //destroy hash map itself
    kh_destroy(chunk_map, chunkMap);

    open_simplex_noise_free(osn_ctx);
};

bool isInsideBlock(float xPos, float yPos, float zPos)
{
    int chunkX = floorf(xPos / CHUNK_SIZE);
    int chunkZ = floorf(zPos / CHUNK_SIZE);

    int chunkXPos = 0;
    int chunkZPos = 0;

    if(chunkX >= 0)
    {
        chunkXPos = (int)xPos % CHUNK_SIZE;
    }
    else
    {
        chunkXPos = abs((int)floorf(xPos) % CHUNK_SIZE);
        chunkXPos = CHUNK_SIZE - chunkXPos - 1;
    }

    if(chunkZ >= 0)
    {
        chunkZPos = (int)zPos % CHUNK_SIZE;
    }
    else
    {
        chunkZPos = abs((int)floorf(zPos) % CHUNK_SIZE);
        chunkZPos = CHUNK_SIZE - chunkZPos - 1;
    }

    uint64_t key;
    vec2ToHashKey(chunkX, chunkZ, &key);

    khint_t foundChunk = kh_get(chunk_map, chunkMap, key);

    if(foundChunk != kh_end(chunkMap))
    {
        Chunk* chunk = kh_value(chunkMap, foundChunk);

        int index;
        posToIndex((vec3){ chunkXPos, (int)yPos, chunkZPos }, &index);
        if (chunk->chunkData[index] == AIR_BLOCK)
        {
            return false;
        }
    }
    
    return true;
}
