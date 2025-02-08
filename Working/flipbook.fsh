#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

uniform int frames;

void main()
{
	float newXCoord = TexCoord.x / frames;
	vec2 testTexCoord = vec2(newXCoord, TexCoord.y);
    FragColor = texture(ourTexture, testTexCoord);
} 