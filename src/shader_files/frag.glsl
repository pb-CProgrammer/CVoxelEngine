#version 460 core

out vec4 fragColor;
in vec3 normal;
in vec2 texCoords;

uniform sampler2D texture_atlas;

void main()
{
    fragColor = texture(texture_atlas, texCoords);//mix(texture(texture1, texCoords), texture(texture2, texCoords), 0.2);

    if(normal[1] == 1)
    {
        fragColor.rgb *= 0.86f;
    }
    else if(normal[2] != 0)
    {
        fragColor.rgb *= 0.80f;
    }
}
