#include "game.h"

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <tinycthread.h>
#include "constants.h"
#include "window.h"
#include "voxel_engine.h"

//time per frame and update in nanoseconds
const long timePerUpdate = 1000000000 / TARGET_UPS;
const long timePerFrame = 1000000000 / TARGET_FPS;

//variable that shows if game loop is active or not
bool gameRunning = true;

//those 2 variables are current time beetwen start and end of frame and update
long deltaU = 0;
long deltaF = 0;

//this is used to calculate difference in time passing
long currentNanoTime = 0;
long previousNanoTime = 0;

//this is same like above, but it is used to print UPS and FPS
long lastCheckTimeMilis = 0;

//as you guessed those are used to count UPS and FPS
long countUPS = 0;
long countFPS = 0;

//this is because update function is in update thread and draw function in main thread
bool isTimeToDraw = true;

//if you set this variable to true, program will make cleanup and close
bool gameStop = false;

mtx_t gameLoopMutex;

void update()
{
    voxelEngineUpdate();
};

void draw()
{
    voxelEngineDraw();
};

bool gameInit()
{
    //this will be changed
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //there program inits opengl and voxel engine
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load glad\n");
        return false;
    }

    voxelEngineInit();

    return true;
};

int gameLogicLoop(void* arg)
{
    previousNanoTime = glfwGetTime() * 1000000000;
    lastCheckTimeMilis = glfwGetTime() * 1000;

    while(true)
    {
        //it works like this:
        //that loop calculates time difference, then add it to deltaU,
        //and when deltaU is equal or larger than time per update, which is calculated based on UPS we set earlier,
        //update function is called, UPS counter is increased by 1, and deltaU is reset

        mtx_lock(&gameLoopMutex);
        //when its time to stop program, program breaks out of game logic loop
        if(!gameRunning)
        {
            mtx_unlock(&gameLoopMutex);
            break;
        }
        mtx_unlock(&gameLoopMutex);

        //program updates deltas with time difference
        currentNanoTime = glfwGetTime() * 1000000000;
        deltaU += currentNanoTime - previousNanoTime;
        deltaF += currentNanoTime - previousNanoTime;
        previousNanoTime = currentNanoTime;

        //if deltaU if larger or equal time for update, program updates and increases UPS
        if(deltaU >= timePerUpdate)
        {
            deltaU -= timePerUpdate;

            update();
            countUPS++;

            mtx_lock(&gameLoopMutex);
            writeToSafeData();
            mtx_unlock(&gameLoopMutex);
        }

        //same like above, but it sets isTimeToDraw to true, and in main thread draw function is called
        if(deltaF >= timePerFrame)
        {
            deltaF -= timePerFrame;

            mtx_lock(&gameLoopMutex);
            isTimeToDraw = true;
            mtx_unlock(&gameLoopMutex);
        }

        //each second it prints UPS and FPS
        if(glfwGetTime() * 1000 - lastCheckTimeMilis >= 1000)
        {
            printf("UPS : %ld | FPS : %ld\n", countUPS, countFPS);
            countUPS = 0;
            countFPS = 0;
            lastCheckTimeMilis += 1000;
        }
    }

    return 0;
};

bool startGameLoop()
{
    mtx_init(&gameLoopMutex, mtx_plain);

    //start update thread
    thrd_t gameThread;
    thrd_create(&gameThread, gameLogicLoop, NULL);

    //main/draw loop
    while(!glfwWindowShouldClose(window))
    {
        mtx_lock(&gameLoopMutex);
        if(isTimeToDraw)
        {
            readFromSafeData();
            mtx_unlock(&gameLoopMutex);

            //program unlocks variables for drawing, because its safe
            draw();
            glfwSwapBuffers(window);

            mtx_lock(&gameLoopMutex);

            //program locks them again for updating
            isTimeToDraw = false;
            countFPS++;
        }
        mtx_unlock(&gameLoopMutex);

        glfwPollEvents();
    }

    //program start exiting and cleanup
    mtx_lock(&gameLoopMutex);
    gameRunning = false;
    mtx_unlock(&gameLoopMutex);

    //exiting and destroying everything
    thrd_join(gameThread, NULL);
    voxelEngineExit();
    mtx_destroy(&gameLoopMutex);

    glfwTerminate();

    return true;
};

void processGameMouseInput(double xPos, double yPos)
{
    processVoxelEngineMouseInput(xPos, yPos);
};

void processGameKeyboardInput(int key, int action)
{
    //if user press escape button, program will stop
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    processVoxelEngineKeyboardInput(key, action);
};
