#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4 outColor;

uniform sampler2D	atlas;

void main()
{
	vec4	color = texture(atlas, vUv);

	outColor = color;
}
