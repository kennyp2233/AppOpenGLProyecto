#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    ourColor = aColor;
    texCoord = vec2((aPos.x+1)/2, (aPos.y+1)/2);
}