#version 330

layout (location = 0) out vec4 final_color;

in vec2 UV;
in vec3 pos;

uniform int blendMode;

uniform sampler2D baseTexture;
uniform sampler2D detailTexture;

void main(void)
{
	if (blendMode == 0)
	{
		final_color = texture(baseTexture, vec2(UV.x, -UV.y));
	}
    else if (blendMode == 1)
	{
		final_color = texture(detailTexture, vec2(UV.x, -UV.y));
	}
	//final_color = vec4(1.0, 1.0, 0.0, 1.0);
}
