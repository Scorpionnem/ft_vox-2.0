#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4 outColor;

uniform sampler2D	atlas;

void main()
{
	//texture(atlas, vUv)
	vec4	color = vec4(vec3(1), 0.2);

	outColor = color;
}
