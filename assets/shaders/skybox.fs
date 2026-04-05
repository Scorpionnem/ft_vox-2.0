#version 330 core

out vec4 outColor;

in vec3 vFragPos;

uniform vec3	SKYBOX_UP_COLOR = vec3(1, 0, 0);
uniform vec3	SKYBOX_DOWN_COLOR = vec3(0, 0, 1);
uniform float	SKYBOX_UP_HEIGHT = 0.0;
uniform float	SKYBOX_MIX_DISTANCE = 4;

void main()
{
	vec3	pos = normalize(vFragPos);
	pos = clamp(pos, -1.0, 1.0);

	float	mix_factor = smoothstep(0.0, 1.0, (pos.y - SKYBOX_UP_HEIGHT) * SKYBOX_MIX_DISTANCE);

	vec3	color = mix(SKYBOX_DOWN_COLOR, SKYBOX_UP_COLOR, mix_factor);
	outColor = vec4(color, 1.0);
}
