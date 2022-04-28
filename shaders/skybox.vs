#version 400 core

uniform mat4	view;
uniform mat4	projection;

layout (location = 0) in vec3	aPos;

out vec3 TexCoords;

void main()
{
	vec4 pos = projection * view * vec4(aPos, 1.0);
	TexCoords = aPos;
	gl_Position = pos.xyww;
}
