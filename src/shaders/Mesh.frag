#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertColor;

uniform sampler2D texture0;
uniform int type;

void main()
{
    if(type > 0)
        FragColor = texture(texture0, TexCoord);
    else
        FragColor = VertColor;
}