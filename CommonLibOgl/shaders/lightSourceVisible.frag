#version 440 core
#pragma debug(on)
#pragma optimize(off)

layout(location = 1) uniform vec3 lightColor;

out vec4 outColor;

void main()
{
	outColor = vec4(lightColor, 1.0);
}
