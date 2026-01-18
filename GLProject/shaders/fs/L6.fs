#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D textureSample;
uniform vec4 objectColor;

void main()
{
    // 1. Sample the color from the texture using the coordinates
    vec4 texColor = texture(textureSample, texCoord);

    // 2. Multiply the texture color by the objectColor
    // This allows you to "tint" the texture. 
    // If objectColor is White (1,1,1,1), you see the original texture.
    FragColor = texColor * objectColor;
}
