#version 330 core

out vec4 outColor;

in vec3 vFragPos;

uniform vec3	SKYBOX_UP_COLOR = vec3(1, 0, 0);
uniform vec3	SKYBOX_DOWN_COLOR = vec3(0, 0, 1);
uniform float	SKYBOX_UP_HEIGHT = 0.0;
uniform float	SKYBOX_MIX_DISTANCE = 4;

uniform float	TIME = 0;

void main()
{
	vec3	pos = normalize(vFragPos);
	pos = clamp(pos, -1.0, 1.0);

	float	mix_factor = smoothstep(0.0, 1.0, (pos.y - SKYBOX_UP_HEIGHT) * SKYBOX_MIX_DISTANCE);

	vec3	color = mix(SKYBOX_DOWN_COLOR, SKYBOX_UP_COLOR, mix_factor);

	// float	sunset_line = pos.x - 0.8;
	// float	dist = pos.y - sunset_line;

	// float	sunset_mask = 1.0 - smoothstep(0.0, 1.0, dist * 4);
	// sunset_mask = clamp(sunset_mask, 0.0, 0.9);

	// vec3		SKY_SUNSET_COLOR = vec3(1.0, 122.0 / 255.0, 42.0 / 255.0);
	// color = mix(color, SKY_SUNSET_COLOR, sunset_mask);

	outColor = vec4(color, 1.0);
}
