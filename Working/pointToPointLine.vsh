#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
	mat4 vp = projectionMat * viewMat;
    gl_Position = vp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoord = aTexCoord;
}