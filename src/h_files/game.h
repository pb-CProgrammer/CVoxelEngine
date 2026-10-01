#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <linmath.h>

bool gameInit();
bool startGameLoop();
void processGameMouseInput(double xPos, double yPos);
void processGameKeyboardInput(int key, int action);

#endif
