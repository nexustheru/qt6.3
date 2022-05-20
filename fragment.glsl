#version 450 core

in vec3 fNormal;
in vec2 TextureCoordinates;

out vec4 FragColor;
uniform sampler2D pic;

void main()
{     
    FragColor = texture2D(pic, TextureCoordinates);
	
}