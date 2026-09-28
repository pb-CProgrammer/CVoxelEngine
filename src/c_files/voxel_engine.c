#include <glad.h>
#include "voxel_engine.h"
#include <stdio.h>
#include <stdbool.h>
#include "window.h"
#include "opengl_help_functions.h"
#include "math_help_functions.h"
#include "player.h"
#include "chunk_manager.h"


/*
 * TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 finish settings.json add file creating iof not exists
 make constants const int targetUPS = 200;
const int targetFPS = 60;
 blockc textures and simiral data in json
  * realistic movement, i mean if i press w and a, i should be the same speeed as i would press w, not times sqrt(2) + simple physics
 * player speed in settings shouldnt be a magic number
 *
 *
 *
 *
 *
 *
 */

//variables important to run engine
GLuint shaderProgram;
GLuint textureAtlas;

Player player;

//update thread will write data here
SafeData safeWriteData;
//draw thread will safely read data from here
SafeData safeReadData;

void voxelEngineUpdate()
{
    playerUpdate(&player);
    chunkManagerUpdate(player.camera.pos[0], player.camera.pos[2]);
};

void voxelEngineDraw()
{
    //all draw matrices, thre isnt trans matrix, because program dont use it
    mat4x4 perspe;
    mat4x4_perspective(perspe, degToRad(player.camera.FOV), (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);

    mat4x4 view;
    mat4x4_look_at(view, safeReadData.player.camera.pos, safeReadData.player.target, absoluteUp);

    glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(uniLoc(shaderProgram, "view"), 1, GL_FALSE, view[0]);
    glUniformMatrix4fv(uniLoc(shaderProgram, "perspe"), 1, GL_FALSE, perspe[0]);

    chunkManagerDraw(safeReadData.startChunkX, safeReadData.startChunkZ, safeReadData.endChunkX, safeReadData.endChunkZ);
};

bool voxelEngineInit()
{
    //opengl handles not drawing faces player dont see
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    //safety checks on creating important pieces
    if(!createShader(&shaderProgram, "src/shader_files/vert.glsl", "src/shader_files/frag.glsl"))
    {
        printf("failed to create shader\n");
        return false;
    }

    if(!createTexture2D(&textureAtlas, "res/texture_atlas.png"))
    {
        printf("failed to create texture\n");
        return false;
    }

    activateTexture2D(textureAtlas, GL_TEXTURE0);

    glUseProgram(shaderProgram);

    glUniform1i(uniLoc(shaderProgram, "texture_atlas"), 0);

    playerInit(&player, (vec3){0.0f, 100.0f, 0.0f}, 90.0f, 0.0f, 90.0f);
    calculatePlayerData(&player);

    chunkManagerInit();

    return true;
};

bool voxelEngineExit()
{
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &textureAtlas);

    chunkManagerExit();

    return true;
};

//should be used with mutex
void writeToSafeData()
{
    safeWriteData.player = player;
    safeWriteData.startChunkX = startChunkX;
    safeWriteData.startChunkZ = startChunkZ;
    safeWriteData.endChunkX = endChunkX;
    safeWriteData.endChunkZ = endChunkZ;
}

void readFromSafeData()
{
    safeReadData.player = safeWriteData.player;
    safeReadData.startChunkX = safeWriteData.startChunkX;
    safeReadData.startChunkZ = safeWriteData.startChunkZ;
    safeReadData.endChunkX = safeWriteData.endChunkX;
    safeReadData.endChunkZ = safeWriteData.endChunkZ;
}

void processVoxelEngineMouseInput(double xPos, double yPos)
{
    processPlayerMouseInput(&player, xPos, yPos);
};

void processVoxelEngineKeyboardInput(int key, int action)
{
    processPlayerKeyboardInput(&player, key, action);
};
