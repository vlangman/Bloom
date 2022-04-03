#version 460 core
uniform sampler2D img_output;
uniform vec2[]speciesList;

in vec4 vTexCoord;
out vec4 fragColour;


void main(void)
{
    fragColour = 
    fragColour = texture2D(img_output, vTexCoord.xy).rgba;
}