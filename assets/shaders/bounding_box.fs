#version 330 core

#mbatty_import "assets/shaders/utils/is_outline.glsl"

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4 outColor;

uniform sampler2D	atlas;

uniform vec3	COLOR;
uniform float	WIDTH = 0.01;

void main()
{
	vec4	color = vec4(COLOR, 1.0);

	if (is_outline(vFragPos, WIDTH))
		outColor = color;
	else
		discard ;
}
