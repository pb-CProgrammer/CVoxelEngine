#include "player.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <linmath.h>
#include "chunk_manager.h"
#include <GLFW/glfw3.h>
#include "game_data.h"
#include "camera.h"
#include "settings.h"
#include "math_help_functions.h"

//small help function
void updatePos(vec3 pos, bool key, float* dir, float speed)
{
    if(key)
    {
        vec3 add_pos;
        vec3_scale(add_pos, dir, speed);
        vec3_add(pos, pos, add_pos);
    }
};

void playerUpdate(Player* player)
{
    //mouse update
    //firstly function calculates mouse pos offset since last move
    //for y its flipped
    float xOffset = player->mouseX - player->mouseLastX;
    float yOffset = player->mouseLastY - player->mouseY;

    //then it reset last pos
    player->mouseLastX = player->mouseX;
    player->mouseLastY = player->mouseY;

    //offset is multiplied by sensivity
    xOffset *= mouseInputSensitivity;
    yOffset *= mouseInputSensitivity;

    //camera direction values are updated
    player->camera.yaw += xOffset;
    player->camera.pitch += yOffset;

    //limit on pitch, because if player could look up infinitly
    if(player->camera.pitch >= 90.0f)
    {
        player->camera.pitch = 89.9f;
    }

    if(player->camera.pitch <= -90.0f)
    {
        player->camera.pitch = -89.9f;
    }

    calculatePlayerData(player);

    //forward dir doesnt include Y
    vec3 forwardDir;
    forwardDir[0] = player->dir[0];
    forwardDir[1] = 0.0f;
    forwardDir[2] = player->dir[2];
    vec3_norm(forwardDir, forwardDir);

    vec3 addPos = { 0.0f, 0.0f, 0.0f };
    //every axis is seperated for easier transform of that function
    updatePos(addPos, player->W, forwardDir, playerSpeed);
    updatePos(addPos, player->S, forwardDir, -playerSpeed);
    updatePos(addPos, player->A, player->right, playerSpeed);
    updatePos(addPos, player->D, player->right, -playerSpeed);
    updatePos(addPos, player->SPACE, absoluteUp, playerSpeed);
    updatePos(addPos, player->SHIFT, absoluteUp, -playerSpeed);

    vec3 tempPos;
    vec3_add(tempPos, player->camera.pos, addPos);

    if(canGoThere(tempPos))
    {
        vec3_add(player->camera.pos, player->camera.pos, addPos);
    }
    
    calculatePlayerData(player);
};

//this function inits player struct with starting values
bool playerInit(Player* player, float* pos, float yaw, float pitch, float FOV)
{
    player->camera.pos[0] = pos[0];
    player->camera.pos[1] = pos[1];
    player->camera.pos[2] = pos[2];

    player->camera.yaw = yaw;
    player->camera.pitch = pitch;
    player->camera.FOV = FOV;

    player->mouseFirstMove = true;

    return true;
};

//this function calls all camera function and calculate each direction vector
void calculatePlayerData(Player* player)
{
    cameraDir(&player->camera, player->dir);
    cameraRight(player->dir, player->right);
    cameraTarget(player->camera.pos, player->dir, player->target);
};

void processPlayerMouseInput(Player* player, float xPos, float yPos)
{
    //this is here, because if not, there will be sudden jump when you move a mouse,
    //instead at first move, last pos is set to current pos, which eliminates that bug
    if(player->mouseFirstMove)
    {
        player->mouseLastX = xPos;
        player->mouseLastY = yPos;
        player->mouseFirstMove = false;
    }

    player->mouseX = xPos;
    player->mouseY = yPos;
};

//another small help function
void setKeyBool(bool* keyBool, int action)
{
    if(action == GLFW_PRESS)
    {
        *keyBool = true;
    }
    else if(action == GLFW_RELEASE)
    {
        *keyBool = false;
    }
};

void processPlayerKeyboardInput(Player* player, int key, int action)
{
    //this function checks which key had event (was pressed or released)
    //and and set key bool accordind to action
    switch(key)
    {
        case GLFW_KEY_W: setKeyBool(&player->W, action);
        break;
        case GLFW_KEY_S: setKeyBool(&player->S, action);
        break;
        case GLFW_KEY_A: setKeyBool(&player->A, action);
        break;
        case GLFW_KEY_D: setKeyBool(&player->D, action);
        break;
        case GLFW_KEY_SPACE: setKeyBool(&player->SPACE, action);
        break;
        case GLFW_KEY_LEFT_SHIFT: setKeyBool(&player->SHIFT, action);
        break;
    }
};

bool canGoThere(float* pos)
{
    int chunkX, chunkZ;
    int chunkXPos, chunkYPos, chunkZPos;
    bool isChunkYPosOk;

    posToChunkData(pos, &chunkX, &chunkZ, &chunkXPos, &chunkYPos, &chunkZPos, &isChunkYPosOk);

    if(!isChunkYPosOk)
    {
        return false;
    }

    Chunk* chunk = getChunk(chunkX, chunkZ);

    if(chunk != NULL)
    {
        int index;
        posToIndex((vec3){ chunkXPos, chunkYPos, chunkZPos }, &index);
        if (chunk->chunkData[index] == AIR_BLOCK)
        {
            return true;
        }
    }
    
    return false;
}