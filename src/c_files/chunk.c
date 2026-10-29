#include "chunk.h"

#include "constants.h"
#include "game_data.h"
#include "math_help_functions.h"
#include <osn-noise.h>
#include <linmath.h>
#include <stb/stb_ds.h>

struct osn_context *osn_ctx;

//function that draws chunk
void chunkDraw(Chunk* chunk)
{
    glBindVertexArray(chunk->VAO);
    glDrawElements(GL_TRIANGLES,chunk->numOfIndices, GL_UNSIGNED_INT, 0);
}

void chunkDrawTransparent(Chunk* chunk)
{
    glBindVertexArray(chunk->tVAO);
    glDrawElements(GL_TRIANGLES,chunk->numOfTIndices, GL_UNSIGNED_INT, 0);
}


//this function push Block face data to chunk.drawData, those are data to create VBO, VAO and draw chunk
void addBlockSide(Chunk* chunk, Side side, float* blockPos, int index)
{
    Face** drawData; //save faces to normal draw data or transparent draw data
    if(chunk->chunkData[index] < TRANSPARENT_BLOCKS)
    {
        drawData = &chunk->tDrawData;
    }
    else
    {
        drawData = &chunk->drawData;
    }

    Face face;
    int currentStride = side * BLOCK_SIDE_DATA;

    //tex coords
    int blockIndex = chunk->chunkData[index] - 1; // - 1 because index = 0 is AIR_BLOCK, but block index = 0 is NOT air
    int textureIndex = blockSidesTextures[blockIndex][side];

    for (int i = 0; i < 4; i++)
    {
        vec3_set((vec3){blockData[currentStride + i * 5] + chunk->pos[0] * CHUNK_SIZE + blockPos[0]
            , blockData[currentStride + i * 5 + 1] + blockPos[1],
             blockData[currentStride + i * 5 + 2]  + chunk->pos[1] * CHUNK_SIZE + blockPos[2]
            }, face.pos[i]);

        int texturePosX = blockData[currentStride + i * 5 + 3] + texPosis[textureIndex][0];
        float normalizedTexturePosX = (float)texturePosX / (float)TEXTURES_IN_ATLAS_WIDTH;

        float textureCorrection = 0.0001f;
        if(blockData[currentStride + i * 5 + 3] > 0.0f)
        {
            textureCorrection *= -1.0f;
        }

        face.texCoords[i][0] = normalizedTexturePosX + textureCorrection;

        int texturePosY = blockData[currentStride + i * 5 + 4] + TEXTURES_IN_ATLAS_HEIGHT - 1 - texPosis[textureIndex][1];
        float normalizedTexturePosY = (float)texturePosY / (float)TEXTURES_IN_ATLAS_HEIGHT;
        textureCorrection = 0.0001f;
        if(blockData[currentStride + i * 5 + 4] > 0.0f)
        {
            textureCorrection *= -1.0f;
        }

        face.texCoords[i][1] = normalizedTexturePosY + textureCorrection;
    }

    vec3_set((vec3){blockData[currentStride + 20], blockData[currentStride + 21], blockData[currentStride + 22]}, face.normal);

    arrput(*drawData, face);
};

//function that generates chunk
void generateChunk(Chunk* chunk)
{
    for(int x = 0; x < CHUNK_SIZE; x++)
    {
        for(int z = 0; z < CHUNK_SIZE; z++)
        {
            //program calculates height of block pillar in chunk, and select blocks based on it
            float multiplyer = 0.01f;
            int height = 0.1f * CHUNK_HEIGHT * (open_simplex_noise2(osn_ctx, (x + chunk->pos[0] * CHUNK_SIZE) * multiplyer, (z + chunk->pos[1] * CHUNK_SIZE) * multiplyer) * 0.5f + 0.5f);
            height *= 4;

            for(int y = 0; y < CHUNK_HEIGHT; y++)
            {
                int index;
                posToIndex((vec3){x, y, z}, &index);

                if(y > height)
                {
                    chunk->chunkData[index] = AIR_BLOCK;
                }
                else if(y == height)
                {
                    chunk->chunkData[index] = GRASS_BLOCK;
                }
                else if(y < height && y > height - 3)
                {
                    chunk->chunkData[index] = DIRT_BLOCK;
                }
                else
                {
                    chunk->chunkData[index] = STONE_BLOCK;
                }

                if(y < 30 && chunk->chunkData[index] == AIR_BLOCK)
                {
                    chunk->chunkData[index] = WATER_BLOCK;
                }

                if(x + chunk->pos[0] * CHUNK_SIZE > 10 && x + chunk->pos[0] * CHUNK_SIZE < 20 && z + chunk->pos[1] * CHUNK_SIZE > 10 && z + chunk->pos[1] * CHUNK_SIZE < 20 && y == 70)
                {
                    chunk->chunkData[index] = GLASS_BLOCK;
                }
                if(x + chunk->pos[0] * CHUNK_SIZE >= 20 && x + chunk->pos[0] * CHUNK_SIZE < 30 && z + chunk->pos[1] * CHUNK_SIZE > 10 && z + chunk->pos[1] * CHUNK_SIZE < 20 && y == 70)
                {
                    chunk->chunkData[index] = AQUA_GLASS_BLOCK;
                }
            }
        }
    }
};

void checkNegativeSideOfBlock(Chunk* currentChunk, Chunk* neighbourChunk, float* blockPos, int index, Side side, int axis, int discardBlocksIndex)
{
    //discardBlocksIndex is a block index above which nearby faces aren't draw 

    //program makes temporary version of variables
    vec3 tempPos;
    vec3_set(blockPos, tempPos);

    int tempIndex = index;

    //at first program checks if it will be checking face in direction inside or outside of chunk
    if(tempPos[axis] - 1 < 0)
    {
        //outside of the chunk,
        //there program gets pos of touching block of neightbour chunk and if its the air block program adds face to be drawn
        tempPos[axis] = CHUNK_SIZE - 1;
        posToIndex(tempPos, &tempIndex);

        if(neighbourChunk->chunkData[tempIndex] <= discardBlocksIndex)
        {
            addBlockSide(currentChunk, side, blockPos, index);
        }

        tempPos[axis] = 0.0f;
    }
    else
    {
        //inside the chunk program do the same as the outside
        tempPos[axis]--;
        posToIndex(tempPos, &tempIndex);

        if(currentChunk->chunkData[tempIndex] <= discardBlocksIndex)
        {
            addBlockSide(currentChunk, side, blockPos, index);
        }

        tempPos[axis]++;
    }
}

void checkPositiveSideOfBlock(Chunk* currentChunk, Chunk* neighbourChunk, float* blockPos, int index, Side side, int axis, int discardBlocksIndex)
{
    //the program do the same as in the method above, but in opposite way
    vec3 tempPos;
    vec3_set(blockPos, tempPos);

    int tempIndex = index;

    if(tempPos[axis] + 1 >= CHUNK_SIZE)
    {
        tempPos[axis] = 0.0f;
        posToIndex(tempPos, &tempIndex);

        if(neighbourChunk->chunkData[tempIndex] <= discardBlocksIndex)
        {
            addBlockSide(currentChunk, side, blockPos, index);
        }

        tempPos[axis] = CHUNK_SIZE - 1;
    }
    else
    {
        tempPos[axis]++;
        posToIndex(tempPos, &tempIndex);

        if(currentChunk->chunkData[tempIndex] <= discardBlocksIndex)
        {
            addBlockSide(currentChunk, side, blockPos, index);
        }

        tempPos[axis]--;
    }
}

void generateChunkDrawData(Chunk* chunk, Chunk* westChunk, Chunk* eastChunk, Chunk* southChunk, Chunk* northChunk)
{
    chunk->drawData = NULL;
    chunk->tDrawData = NULL;
    for(int index = 0; index < CHUNK_VOLUME; index++)
    {
        vec3 blockPos;
        indexToPos(index, blockPos);

        if(chunk->chunkData[index] > 0)
        {
            int discardBlocksIndex;
            if(chunk->chunkData[index] > TRANSPARENT_BLOCKS - 1)
            {
                discardBlocksIndex = WATER_BLOCK;
            }
            else
            {
                discardBlocksIndex = AIR_BLOCK;
            }

            //WEST
            checkNegativeSideOfBlock(chunk, westChunk, blockPos, index, WEST_SIDE, 0, discardBlocksIndex);

            //SOUTH
            checkPositiveSideOfBlock(chunk, southChunk, blockPos, index, SOUTH_SIDE, 2, discardBlocksIndex);

            //EAST
            checkPositiveSideOfBlock(chunk, eastChunk, blockPos, index, EAST_SIDE, 0, discardBlocksIndex);

            //NORTH
            checkNegativeSideOfBlock(chunk, northChunk, blockPos, index, NORTH_SIDE, 2, discardBlocksIndex);

            //we make temporary version of variables
            vec3 tempPos;
            int tempIndex;
            tempPos[0] = blockPos[0];
            tempPos[1] = blockPos[1];
            tempPos[2] = blockPos[2];

            //TOP
            //there everything is same like above, but program dont check up and down chunk because they dont exist
            if(tempPos[1] + 1 >= CHUNK_HEIGHT)
            {
                addBlockSide(chunk, TOP_SIDE, blockPos, index);
            }
            else
            {
                tempPos[1]++;
                posToIndex(tempPos, &tempIndex);

                if(chunk->chunkData[tempIndex] <= discardBlocksIndex)
                {
                    addBlockSide(chunk, TOP_SIDE, blockPos, index);
                }

                tempPos[1]--;
            }

            //BOTTOM
            if(tempPos[1] - 1 < 0)
            {
                addBlockSide(chunk, BOTTOM_SIDE, blockPos, index);
            }
            else
            {
                tempPos[1]--;
                posToIndex(tempPos, &tempIndex);

                if(chunk->chunkData[tempIndex] <= discardBlocksIndex)
                {
                    addBlockSide(chunk, BOTTOM_SIDE, blockPos, index);
                }

                tempPos[1]++;
            }
        }
    }


    //and program pass number of vertices to chunk, for more optimized rendering, and program marks chunk as draw data generated
    chunk->generatedDrawData = true;
};

void compileChunkDrawData(Chunk* chunk)
{
    float *vertices = NULL;
    unsigned int *indices = NULL;

    unsigned int numOfFaces = arrlen(chunk->drawData);

    int floatsPerFace = 4 * 8; //number fo vertices * floats per vertice (pos, normal, texCoords)

    arrsetcap(vertices, numOfFaces * floatsPerFace);
    arrsetcap(indices, numOfFaces * 6);

    for (int i = 0; i < numOfFaces; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            arrput(vertices, chunk->drawData[i].pos[j][0]);
            arrput(vertices, chunk->drawData[i].pos[j][1]);
            arrput(vertices, chunk->drawData[i].pos[j][2]);
            arrput(vertices, chunk->drawData[i].normal[0]);
            arrput(vertices, chunk->drawData[i].normal[1]);
            arrput(vertices, chunk->drawData[i].normal[2]);
            arrput(vertices, chunk->drawData[i].texCoords[j][0]);
            arrput(vertices, chunk->drawData[i].texCoords[j][1]);
        }

        //indices
        arrput(indices, i * 4 + 2);
        arrput(indices, i * 4 + 1);
        arrput(indices, i * 4);

        arrput(indices, i * 4 + 3);
        arrput(indices, i * 4 + 2);
        arrput(indices, i * 4);
    }

    glGenBuffers(1, &chunk->VBO);
    glGenBuffers(1, &chunk->EBO);
    glGenVertexArrays(1, &chunk->VAO);

    glBindVertexArray(chunk->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
    glBufferData(GL_ARRAY_BUFFER, arrlen(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DRAW_DATA_STRIDE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, DRAW_DATA_STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, DRAW_DATA_STRIDE * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    chunk->numOfIndices = arrlen(indices);

    arrfree(vertices);
    arrfree(indices);
    arrfree(chunk->drawData);

    vertices = NULL;
    indices = NULL;

    numOfFaces = arrlen(chunk->tDrawData);

    floatsPerFace = 4 * 8; //number fo vertices * floats per vertice (pos, normal, texCoords)

    arrsetcap(vertices, numOfFaces * floatsPerFace);
    arrsetcap(indices, numOfFaces * 6);

    for (int i = 0; i < numOfFaces; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            arrput(vertices, chunk->tDrawData[i].pos[j][0]);
            arrput(vertices, chunk->tDrawData[i].pos[j][1]);
            arrput(vertices, chunk->tDrawData[i].pos[j][2]);
            arrput(vertices, chunk->tDrawData[i].normal[0]);
            arrput(vertices, chunk->tDrawData[i].normal[1]);
            arrput(vertices, chunk->tDrawData[i].normal[2]);
            arrput(vertices, chunk->tDrawData[i].texCoords[j][0]);
            arrput(vertices, chunk->tDrawData[i].texCoords[j][1]);
        }

        //indices
        arrput(indices, i * 4 + 2);
        arrput(indices, i * 4 + 1);
        arrput(indices, i * 4);

        arrput(indices, i * 4 + 3);
        arrput(indices, i * 4 + 2);
        arrput(indices, i * 4);
    }

    glGenBuffers(1, &chunk->tVBO);
    glGenBuffers(1, &chunk->tEBO);
    glGenVertexArrays(1, &chunk->tVAO);

    glBindVertexArray(chunk->tVAO);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->tVBO);
    glBufferData(GL_ARRAY_BUFFER, arrlen(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->tEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DRAW_DATA_STRIDE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, DRAW_DATA_STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, DRAW_DATA_STRIDE * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    chunk->numOfTIndices = arrlen(indices);

    arrfree(vertices);
    arrfree(indices);
    arrfree(chunk->tDrawData);

    chunk->compiledDrawData = true;
};
