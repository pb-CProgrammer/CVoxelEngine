#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec2 texCoords;

uniform mat4 trans;
uniform mat4 view;
uniform mat4 perspe;

void main()
{
    gl_Position = perspe * view * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    normal = aNormal;
    texCoords = aTexCoords;
}
