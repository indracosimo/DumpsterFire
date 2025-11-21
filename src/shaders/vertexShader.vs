#version 330 core
layout (location = 0) in vec3 aPos;      
layout (location = 1) in vec2 aTexCoord; // use location 1 for texture coordinates
layout (location = 2) in vec3 aColor;

out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 camMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}
