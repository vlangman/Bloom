#version 460 core
 uniform sampler2D img_output;
in vec4 vTexCoord;
out vec4 fragColour;


void main(void)
{
    fragColour = texture2D(img_output, vTexCoord.xy).rgba;
}