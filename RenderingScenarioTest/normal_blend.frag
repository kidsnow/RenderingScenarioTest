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
	base.xy = base.xy*2 - 1;
	detail.xy = detail.xy*2 - 1;

	if (blendMode == 0) // LINEAR
	{
		result = base + detail;
	}
	else if (blendMode == 1) // OVERLAY
	{
		base = texture2D(baseTexture, vec2(UV.x, -UV.y)).xyz;
		detail = texture2D(detailTexture, vec2(UV.x, -UV.y)).xyz;
		if (base.x < 0.5)
		{
			result.x = 2*base.x*detail.x;
		}
		else
		{
			result.x = 1 - 2*(1 - base.x)*(1 - detail.x);
		}
		if (base.y < 0.5)
		{
			result.y = 2*base.y*detail.y;
		}
		else
		{
			result.y = 1 - 2*(1 - base.y)*(1 - detail.y);
		}
		if (base.z < 0.5)
		{
			result.z = 2*base.z*detail.z;
		}
		else
		{
			result.z = 1 - 2*(1 - base.z)*(1 - detail.z);
		}
		result.xy = result.xy*2 - 1;
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
		float a = 1/(1 + base.z);
		float b = -base.x*base.y*a;
		vec3 basis0 = vec3(1 - base.x*base.x*a, b, -base.x);
		vec3 basis1 = vec3(b, 1 - base.y*base.y*a, -base.y);
		
		if (base.z < -0.9999999)
		{
			basis0 = vec3(0, -1, 0);
			basis1 = vec3(-1, 0, 0);
		}

		result = detail.x*basis0 + detail.y*basis1 + detail.z*base;
	}
	else if (blendMode == 6) // UNITY
	{
		vec3 basis0 = vec3(base.zy, -base.x);
		vec3 basis1 = vec3(base.xz, -base.y);
		result = detail.x*basis0 + detail.y*basis1 + detail.z*base;
	}

	result.xy = (result.xy + 1.0)*0.5;
	final_color = vec4(result, 1.0);
}
