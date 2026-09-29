#ifndef SETTINGS_H
#define SETTINGS_H

//window opengl settings
extern int startWindowWidth;
extern int startWindowHeight;
extern const char* windowTitle;

//player settings
extern float mouseInputSensitivity;
extern float playerSpeed;

//game settings
extern int drawAreaInChunks;
extern int worldSeed;

void readSettings();

#endif
