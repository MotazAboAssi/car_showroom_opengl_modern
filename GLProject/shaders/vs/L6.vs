#version 460 core

// Location 0: 3D Position (x, y, z)
layout (location = 0) in vec3 aPos;
// Location 1: 2D Texture Coordinate (u, v)
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Pass the input texture coordinate to the fragment shader
    texCoord = aTexCoord;
}
