#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = texture(texture2texture, TexCoord);
	
	vec4 basicColor = texture(texture1, TexCoord);
	vec4 shadowColor = texture(texture2, TexCoord);
	FragColor = basicColor * shadowColor;
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
} 