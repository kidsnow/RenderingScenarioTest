#version 330

layout (location = 0) out vec4 final_color;

in vec2 UV;
in vec3 pos;

uniform int blendMode;

uniform sampler2D baseTexture;
uniform sampler2D detailTexture;

void main(void)
{
	vec3 result;
	vec3 base = texture2D(baseTexture, vec2(UV.x, -UV.y)).xyz;
	vec3 detail = texture2D(detailTexture, vec2(UV.x, -UV.y)).xyz;
	base.xy = base.xy * 2.0 - 1.0;
	detail.xy = detail.xy * 2.0 - 1.0;

	if (blendMode == 0)
	{
		result = normalize(vec3(base.xy * detail.z + detail.xy * base.z, base.z * detail.z));
	}
    else if (blendMode == 1)
	{
		result = normalize(vec3(base.xy + detail.xy, base.z));
	}

	result.xy = (result.xy + 1.0) * 0.5;
	final_color = vec4(result, 1.0);
}
