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

void main()
{
	vec3	fogFactor = pow(length(VIEW_POS - vWorldPos) / RENDER_DISTANCE, vec3(FOG_POWER));
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec3	color = vColor;
	color = mix(color, FOG_COLOR, fogFactor);
	color = mix(color, FOG_COLOR, 1.0 - spawn_fade);

	outColor = vec4(color, 1.0);
}
