#version 450 core


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoordinates;
layout(location = 3) in vec4 colorr;

uniform mat4 projection;

out vec2 TextureCoordinates;
out vec3 fNormal;

void main()
{
    fNormal = normal;    	
    gl_Position = projection * vec4(position, 10);//savethis shit 0.5
	TextureCoordinates=textureCoordinates;
   
};