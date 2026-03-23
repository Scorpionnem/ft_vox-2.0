#version 330 core

in vec3 FragPos;
in vec3 vNormal;
in vec2 vUv;

out vec4 outColor;

void main()
{
	outColor = vec4(abs(vNormal) * vec3(vUv.x + vUv.y), 1.0);
}
