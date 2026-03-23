#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

out vec3 FragPos;
out vec3 vNormal;
out vec2 vUv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0);

	FragPos = aPos;
	vNormal = aNormal;
	vUv = aUv;
}
