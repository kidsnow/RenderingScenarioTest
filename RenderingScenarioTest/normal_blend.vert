#version 330

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec2 a_texcoord;

out vec2 UV;
out vec3 pos;

uniform mat4 MVP;

void main(void) {
    gl_Position = MVP * a_position;
	pos = gl_Position.xyz;
	UV = a_texcoord;
}
