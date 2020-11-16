#version 330

layout (location = 0) out vec4 final_color;

in vec2 UV;
in vec3 pos;

uniform mat3 toDetailTexCoord;

uniform int blendMode;

uniform sampler2D baseTexture;
uniform sampler2D detailTexture;

float overlay(float x, float y)
{
	if (x < 0.5)
		return 2.0*x*y;
	else
		return 1.0 - 2.0*(1.0 - x)*(1.0 - y);
}

bool hits(float s, float t)
{
	if (s < 0.0 || s > 1.0)
		return false;
	if (t < 0.0 || t > 1.0)
		return false;
	return true;
}

void main(void)
{
	vec3 result;

	vec3 detailTexCoord;
	//detailTexCoord = toDetailTexCoord * UV;
	detailTexCoord = toDetailTexCoord * vec3(UV.x - 0.5, UV.y - 0.5, 1.0);
	detailTexCoord += vec3(0.5, 0.5, 0.0);

	if (!hits(detailTexCoord.x, detailTexCoord.y))
	{
		result = texture2D(baseTexture, vec2(UV.x, -UV.y)).xyz;
		final_color = vec4(result, 1.0);
		return;
	}

	vec3 base = texture2D(baseTexture, vec2(UV.x, -UV.y)).xyz;
	vec3 detail = texture2D(detailTexture, vec2(detailTexCoord.x, -detailTexCoord.y)).xyz;
	base = base*2.0 - 1.0;
	detail = detail*2.0 - 1.0;

	if (blendMode == 0) // LINEAR
	{
		result = base + detail;
	}
	else if (blendMode == 1) // OVERLAY
	{
		base = texture2D(baseTexture, vec2(UV.x, -UV.y)).xyz;
		detail = texture2D(detailTexture, vec2(UV.x, -UV.y)).xyz;
		result.x = overlay(base.x, detail.x);
		result.y = overlay(base.y, detail.y);
		result.z = overlay(base.z, detail.z);
		result = result*2.0 - 1.0;
	}
	else if (blendMode == 2) // PARTIAL_DERIVATIVE
	{
		result = normalize(vec3(base.xy*detail.z + detail.xy*base.z, base.z*detail.z));
	}
	else if (blendMode == 3) // WHITEOUT
	{
		result = vec3(base.xy + detail.xy, base.z*detail.z);
	}
    else if (blendMode == 4) // UDN
	{
		result = normalize(vec3(base.xy + detail.xy, base.z));
	}
	else if (blendMode == 5) // REORIENTED
	{
		float a = 1.0/(1.0 + base.z);
		float b = -base.x*base.y*a;
		vec3 basis0 = vec3(1.0 - base.x*base.x*a, b, -base.x);
		vec3 basis1 = vec3(b, 1.0 - base.y*base.y*a, -base.y);
		
		if (base.z < -0.9999999)
		{
			basis0 = vec3(0.0, -1.0, 0.0);
			basis1 = vec3(-1.0, 0.0, 0.0);
		}

		result = detail.x*basis0 + detail.y*basis1 + detail.z*base;
	}
	else if (blendMode == 6) // UNITY
	{
		vec3 basis0 = vec3(base.zy, -base.x);
		vec3 basis1 = vec3(base.xz, -base.y);
		result = detail.x*basis0 + detail.y*basis1 + detail.z*base;
	}

	result = (result + 1.0)*0.5;
	final_color = vec4(result, 1.0);
}
