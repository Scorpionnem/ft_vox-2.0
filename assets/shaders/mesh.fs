#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

in vec3 vWorldPos;

out vec4 outColor;

uniform float spawn_fade;

uniform vec3	FOG_COLOR = vec3(0);
uniform float	FOG_POWER = 4;
uniform vec3	RENDER_DISTANCE;
uniform vec3	VIEW_POS;

uniform sampler2D	atlas;

void main()
{
	vec3	fogFactor = pow(length(VIEW_POS - vWorldPos) / RENDER_DISTANCE, vec3(FOG_POWER));
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec4	color = texture(atlas, vUv);

	color.rgb *= vColor;

	float shadowForce = dot(vNormal, vec3(1, 1, 0.5));
	shadowForce = clamp(shadowForce, 0.2, 1.0);
	color.rgb *= shadowForce;

	color.rgb = mix(color.rgb, FOG_COLOR, fogFactor);
	color.rgb = mix(color.rgb, FOG_COLOR, 1.0 - spawn_fade);

	outColor = color;
}
