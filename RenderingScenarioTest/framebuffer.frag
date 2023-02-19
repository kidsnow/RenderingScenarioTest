#version 330

layout (location = 0) out vec4 final_color0;
layout (location = 1) out vec4 final_color1;
layout (location = 2) out vec4 final_color2;

in vec2 UV;

void main(void)
{
	final_color0 = vec4(1.0, 0.0, 0.0, 1.0);
	final_color1 = vec4(0.0, 1.0, 0.0, 1.0);
	final_color2 = vec4(0.0, 0.0, 1.0, 1.0);
}
