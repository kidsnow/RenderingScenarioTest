#version 330

layout (location = 0) out vec4 final_color;

in vec2 UV;
uniform sampler2D u_texture;

void main(void) {
    final_color = texture(u_texture, vec2(UV.x, -UV.y));
	final_color = vec4(1.0, 1.0, 0.0, 1.0);
}
