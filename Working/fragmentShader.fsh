#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform int isHighlighted;

void main()
{
	vec4 highlightColour = vec4(0.0, 0.0, 0.0, 0.0);
	
	vec4 sampledColour = texture(ourTexture, TexCoord);
	
	if(isHighlighted == 1 && sampledColour.a != 0) {
		highlightColour = vec4(1.0, 0.9, 0.6, 1.0)*0.3;
	}
	
    FragColor = sampledColour + highlightColour;
} 