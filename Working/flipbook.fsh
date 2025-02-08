#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

uniform int frames;
uniform int currentFrame;

void main()
{
	float currentFrameFloat = currentFrame;
	float offset = currentFrameFloat/frames;
	float newXCoord = TexCoord.x / frames;
	
	
	vec2 testTexCoord = vec2(newXCoord + offset, TexCoord.y);
    FragColor = texture(ourTexture, testTexCoord);
} 