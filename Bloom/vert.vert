#version 460 core
out vec4 vTexCoord;
void main(void)
{
    gl_Position = vec4( gl_Vertex.xy, 0.0, 1.0 ); 
    //transform quad coords from [-1,1] -> [0,1] point = (vec2(1)+point.xy)/2
    vec2 point = vec2((gl_Position.x+1.0)/2.0,(gl_Position.y+1.0)/2.0);
    vTexCoord = vec4(point.xy,0.0,1.0);
}