#ifndef CHUNK_H
#define CHUNK_H

#include "constants.h"

#include <glad.h>
#include <linmath.h>
#include <stdbool.h>

//that variable is here, because generateChunk() needs it
extern struct osn_context *osn_ctx;

//in chunk struct program stores position of chunk in chunk world, each block in chunk and things necesery to generate draw data and draw chunk
typedef struct Chunk
{
    vec2 pos;
    uint8_t chunkData[CHUNK_VOLUME];
    float* drawData;
    float* transparentDrawData;
    GLuint VBO, VAO;
    GLuint tVBO, tVAO;
    unsigned int vertices;
    unsigned int transparentVertices;
    bool generatedDrawData;
    bool compiledDrawData;
} Chunk;

//draw function and functions for each step of generation of chunk:
//1.generate block in chunk
//2.generate chunk draw data for drawing
//3.compile those draw data into VBO
//I seperated 2 and 3 step because compiling must be in main thread while generating chunk and draw data can be in different thread
void chunkDraw(Chunk* chunk);
void generateChunk(Chunk* chunk);
void generateChunkDrawData(Chunk* chunk, Chunk* westChunk, Chunk* eastChunk, Chunk* southChunk, Chunk* northChunk);
void compileChunkDrawData(Chunk* chunk);

#endif
