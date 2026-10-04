#include "settings.h"

#include <stdlib.h>
#include <string.h>
#include <parson.h>
#include "opengl_help_functions.h"

int startWindowWidth;
int startWindowHeight;
const char* windowTitle;

float mouseInputSensitivity;
float playerSpeed;
float playerHitboxSize;
float playerHitboxHeight;
float playerCameraHeight;

int drawAreaInChunks;
int worldSeed;

void createDefaultSettings()
{
    JSON_Value *rootValue = json_value_init_object();
    JSON_Object *rootObject = json_value_get_object(rootValue);

    //window/opengl stuff
    json_object_dotset_number(rootObject, "window.startWindowWidth", 800);
    json_object_dotset_number(rootObject, "window.startWindowHeight", 600);
    json_object_dotset_string(rootObject, "window.windowTitle", "CVoxelEngine");

    //player stuff
    json_object_dotset_number(rootObject, "player.mouseInputSensitivity", 0.1f);
    json_object_dotset_number(rootObject, "player.playerSpeed", 0.1f);
    json_object_dotset_number(rootObject, "player.playerHitboxSize", 0.35f);
    json_object_dotset_number(rootObject, "player.playerHitboxHeight", 1.8f);
    json_object_dotset_number(rootObject, "player.playerCameraHeight", 1.7f);

    //voxel engine stuff
    json_object_dotset_number(rootObject, "game.drawAreaInChunks", 31);
    json_object_dotset_number(rootObject, "game.worldSeed", 283458);

    char *jsonText = json_serialize_to_string_pretty(rootValue);
    writeFile("json_files/settings.json", jsonText);

    json_free_serialized_string(jsonText);
    json_value_free(rootValue);
}

void readSettings()
{
    const char* settingsData;

    if(!readFile("json_files/settings.json", &settingsData))
    {
        createDefaultSettings();
    }

    readFile("json_files/settings.json", &settingsData);
    
    JSON_Value *rootValue = json_parse_string(settingsData);
    JSON_Object *rootObject = json_value_get_object(rootValue);

    startWindowWidth = json_object_dotget_number(rootObject, "window.startWindowWidth");
    startWindowHeight = json_object_dotget_number(rootObject, "window.startWindowHeight");
    const char* tempWindowTitle = json_object_dotget_string(rootObject, "window.windowTitle");
    windowTitle = strdup(tempWindowTitle);

    mouseInputSensitivity = json_object_dotget_number(rootObject, "player.mouseInputSensitivity");
    playerSpeed = json_object_dotget_number(rootObject, "player.playerSpeed");
    playerHitboxSize = json_object_dotget_number(rootObject, "player.playerHitboxSize");
    playerHitboxHeight = json_object_dotget_number(rootObject, "player.playerHitboxHeight");
    playerCameraHeight = json_object_dotget_number(rootObject, "player.playerCameraHeight");

    drawAreaInChunks = json_object_dotget_number(rootObject, "game.drawAreaInChunks");
    worldSeed = json_object_dotget_number(rootObject, "game.worldSeed");

    json_value_free(rootValue);

    free((void*)settingsData);
}