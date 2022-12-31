#version 330

layout (location = 0) out vec4 final_color;

in vec2 UV;

void main(void)
{
	final_color = vec4(1.0, 0.0, 0.0, 1.0);
}
