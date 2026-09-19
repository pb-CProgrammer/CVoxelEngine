#include "camera.h"

#include <linmath.h>
#include "math_help_functions.h"

//calculates camera direction from yaw and pitch, formula is from learnopengl
void cameraDir(Camera* camera, float* cameraDir)
{
    cameraDir[0] = cos(degToRad(camera->yaw)) * cos(degToRad(camera->pitch));
    cameraDir[1] = sin(degToRad(camera->pitch));
    cameraDir[2] = sin(degToRad(camera->yaw)) * cos(degToRad(camera->pitch));
    vec3_norm(cameraDir, cameraDir);
};

//calculates camera right, formula is from learnopengl
void cameraRight(float* cameraDir, float* cameraRight)
{
    vec3_mul_cross(cameraRight, absoluteUp,cameraDir);
    vec3_norm(cameraRight, cameraRight);
};

//calculates camera up
void cameraUp(float* cameraDir, float* cameraRight, float* cameraUp)
{
    vec3_mul_cross(cameraUp, cameraDir, cameraRight);
};

//calculates camera target - pos where camera is looking,
void cameraTarget(float* cameraPos, float* cameraDir, float* cameraTarget)
{
    vec3_add(cameraTarget, cameraPos, cameraDir);
};
