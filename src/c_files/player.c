#include "player.h"

#include <stdbool.h>
#include <linmath.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "settings.h"
#include "math_help_functions.h"

//small help function
void updatePos(Player* player, bool key, float* dir, float speed)
{
    if(key)
    {
        vec3 add_pos;
        vec3_scale(add_pos, dir, speed);
        vec3_add(player->camera.pos, player->camera.pos, add_pos);
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
    xOffset *= MOUSE_INPUT_SENSITIVITY;
    yOffset *= MOUSE_INPUT_SENSITIVITY;

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

    //every axis is seperated for easier transform of that function
    updatePos(player, player->W, forwardDir, PLAYER_SPEED);
    updatePos(player, player->S, forwardDir, -PLAYER_SPEED);
    updatePos(player, player->A, player->right, PLAYER_SPEED);
    updatePos(player, player->D, player->right, -PLAYER_SPEED);
    updatePos(player, player->SPACE, absoluteUp, PLAYER_SPEED);
    updatePos(player, player->SHIFT, absoluteUp, -PLAYER_SPEED);

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
