#version 330

layout (location = 0) in vec4 a_position;


void main(void) {

gl_Position = vec4(a_position.xy, 0.5, 1.0);

}
