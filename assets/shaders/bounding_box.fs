#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4 outColor;

uniform sampler2D	atlas;

uniform vec3	COLOR;
uniform float	WIDTH = 0.01;

bool	is_outline(vec3 pos)
{
	float w = WIDTH;

	vec3 edgeLow  = step(pos, vec3(w));
	vec3 edgeHigh = step(vec3(1.0 - w), pos);

	vec3 edge = max(edgeLow, edgeHigh);

	return ((edge.x + edge.y + edge.z) >= 2.0);
}

void main()
{
	vec4	color = vec4(COLOR, 1.0);

	if (is_outline(vFragPos))
		outColor = color;
	else
		discard ;
}
