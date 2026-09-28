#ifndef OPENGL_HELP_FUNCTIONS_H
#define OPENGL_HELP_FUNCTIONS_H

#include <glad.h>
#include <stdbool.h>

//program use this function a lot, and that define makes code shorter and simpler
#define uniLoc glGetUniformLocation

bool readFile(const char* file_location, const char** file_destinity);
bool writeFile(const char* fileLocation, const char* fileData);
bool createShader(GLuint* shader, const char* vertexShaderFileplace, const char* fragmentShaderFileplace);
bool createTexture2D(GLuint* texture, const char* Fileplace);
void activateTexture2D(GLuint texture, int activeTexture);

#endif
