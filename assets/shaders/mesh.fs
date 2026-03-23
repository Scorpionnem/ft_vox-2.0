#version 330 core

in vec3 FragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4 outColor;

void main()
{
	outColor = vec4(vColor, 1.0);
}
