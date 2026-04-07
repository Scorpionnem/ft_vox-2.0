#version 330 core

#mbatty_import "assets/shaders/utils/atlas_uv.glsl"
#mbatty_import "assets/shaders/utils/is_outline.glsl"

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4 outColor;

uniform sampler2D	atlas;

uniform float	WIDTH = 0.01;

#define BREAK_ANIM_FRAME1 240

uniform int	BREAK_ANIM_FRAME = -1;

#define TEX_SIZE 16
#define ATLAS_SIZE 256

void main()
{
	vec4	color = texture(atlas, getAtlasUV(vUv, BREAK_ANIM_FRAME1 + BREAK_ANIM_FRAME, TEX_SIZE, ATLAS_SIZE));
	if (is_outline(vFragPos, WIDTH))
		color = vec4(vec3(0), 1);
	else
		if (BREAK_ANIM_FRAME == -1)
			discard ;

	outColor = color;
}
