#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
	mat4 mvp = projectionMat * viewMat * modelMat;
    gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
	//gl_Position = vec4(1.0, 1.0, 0, 1.0);
	TexCoord = aTexCoord;
}