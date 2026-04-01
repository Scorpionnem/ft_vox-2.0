#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

in vec3 vWorldPos;

out vec4 outColor;

uniform float spawn_fade;

uniform vec3	RENDER_DISTANCE;
uniform vec3	VIEW_POS;
uniform int		CHUNK_SIZE;

uniform vec3	FOG_DISTANCE;
uniform bool	FOG_TOGGLE = true;
uniform vec3	FOG_COLOR = vec3(0);
uniform float	FOG_POWER = 4;

uniform sampler2D	atlas;

void main()
{
	vec4	color = texture(atlas, vUv);

	if (color.a == 0)
		discard ;

	color.rgb *= vColor;

	float shadowForce = dot(vNormal, vec3(1, 1, 0.5));
	shadowForce = clamp(shadowForce, 0.4, 1.0);
	color.rgb *= shadowForce;

	if (FOG_TOGGLE == true)
	{
		vec3	fogFactor;
		fogFactor.x = pow(length(VIEW_POS.x - vWorldPos.x) / FOG_DISTANCE.x, FOG_POWER);
		fogFactor.y = pow(length(VIEW_POS.y - vWorldPos.y) / FOG_DISTANCE.y, FOG_POWER);
		fogFactor.z = pow(length(VIEW_POS.z - vWorldPos.z) / FOG_DISTANCE.z, FOG_POWER);
		fogFactor = clamp(fogFactor, 0.0, 1.0);

		color.rgb = mix(color.rgb, FOG_COLOR, fogFactor.x);
		color.rgb = mix(color.rgb, FOG_COLOR, fogFactor.y);
		color.rgb = mix(color.rgb, FOG_COLOR, fogFactor.z);
		color.rgb = mix(color.rgb, FOG_COLOR, 1.0 - spawn_fade);
	}

	outColor = color;
}
