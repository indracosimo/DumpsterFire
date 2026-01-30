#version 330 core

uniform mat4 modelMatrix = mat4(1);
uniform mat4 viewMatrix = mat4(1);
uniform mat4 projectionMatrix = mat4(1);

layout (location = 0) in vec3 position;

void main()
{
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(position, 1.0);
}
