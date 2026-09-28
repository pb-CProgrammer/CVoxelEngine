#include "opengl_help_functions.h"

#include <GL/gl.h>
#include <glad.h>
#include <stdbool.h>
#include <stb/stb_image.h>
#include <string.h>

bool readFile(const char* fileLocation, const char** fileData)
{
    //opens file in read only mode
    FILE* file = fopen(fileLocation, "r");
    if(file == NULL)
    {
        printf("Failed to open file %s\n", fileLocation);
        return false;
    }

    //fseek moves file pointer to end of file, and return current pos with ftell, that pos is file size
    //and pointer go back to start of file, because program will read it
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    //program allocates memory acording to size of file plus 1 for end string char
    char* buffer = (char*)malloc(sizeof(char) * (file_size + 1));
    if(buffer == NULL)
    {
        printf("memory allocation error\n");
        fclose(file);
        return false;
    }

    //program reads file and set last char as end of string
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    //program close and returns file data
    fclose(file);

    *fileData = buffer;

    return true;
};

bool writeFile(const char* fileLocation, const char* fileData)
{
    //opens file in write mode
    FILE* file = fopen(fileLocation, "w");
    if (file == NULL)
    {
        printf("Failed to open file for writing: %s\n", fileLocation);
        return false;
    }

    // checks if data arent empty
    if (fileData == NULL)
    {
        fclose(file);
        return true; // written empty file
    }

    size_t data_size = strlen(fileData);

    size_t elements_written = fwrite(fileData, 1, data_size, file);
    
    // Sprawdzamy, czy zapisano tyle bajtów, ile planowano
    if (elements_written != data_size)
    {
        printf("Error while writing to file %s\n", fileLocation);
        fclose(file);
        return false;
    }

    // Zamykamy plik i kończymy z sukcesem
    fclose(file);
    return true;
}

bool createShader(GLuint* shader, const char* vertexShaderFileplace, const char* fragmentShaderFileplace)
{
    //this function opens shader file
    const char* vertexShaderSource;
    readFile(vertexShaderFileplace, &vertexShaderSource);

    //than creates shaders and compile source file
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("VERTEX::SHADER::COMPILATION::ERROR%s\n", infoLog);
    }

    //there this is same like like vertex shader
    const char* fragmentShaderSource;
    readFile(fragmentShaderFileplace, &fragmentShaderSource);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("FRAGMENT::SHADER::COMPILATION::ERROR%s\n", infoLog);
    }

    //after that function creates shader program
    *shader = glCreateProgram();

    //shaders compiled above are linked in shader program
    glAttachShader(*shader, vertexShader);
    glAttachShader(*shader, fragmentShader);
    glLinkProgram(*shader);

    glGetProgramiv(*shader, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(*shader, 512, NULL, infoLog);
        printf("SHADER::PROGRAM::LINK::FAILED::ERROR%s\n", infoLog);
    }

    //after that, function deletes leftovers
    free((void*)vertexShaderSource);
    free((void*)fragmentShaderSource);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
};

bool createTexture2D(GLuint* texture, const char* Fileplace)
{
    glGenTextures(1, texture);

    glBindTexture(GL_TEXTURE_2D, *texture);

    //we set GL_NEAREST there because if not, in distance, there will be weird artefacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(Fileplace, &width, &height, &nrChannels, 0);

    //function automaticly detects file format of texture
    if(data)
    {
        GLenum format;
        switch(nrChannels)
        {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
                break;
            case 4:
                format = GL_RGBA;
                break;
                break;
            default:
                format = GL_RGB;
        };

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture : %s\n", Fileplace);
        return false;
    }

    //function removes leftovers and unbinds texture
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
};

//this function exists, because code looks better with it
void activateTexture2D(GLuint texture, int activeTexture)
{
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
};
