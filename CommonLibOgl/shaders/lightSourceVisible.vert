#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout(location = 0) in vec4 position;

layout(location = 0) uniform mat4 mvp;

void main()
{
	gl_Position = mvp * position;
}
