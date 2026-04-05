#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4 outColor;

uniform sampler2D	atlas;

#define TEX_SIZE 16
#define ATLAS_SIZE 256
vec2	getAtlasUV(vec2 uv, int textureId)
{
	vec2	baseUV = uv;

	int row = (TEX_SIZE - 1) - (textureId / TEX_SIZE);;
	int col = textureId % TEX_SIZE;

	vec2 cellSize = vec2(float(TEX_SIZE) / float(ATLAS_SIZE));
	vec2 atlasOffset;
	atlasOffset.x = float(col) * cellSize.x;
	atlasOffset.y = float(row) * cellSize.y;
	vec2 atlasUV;
	atlasUV.x = atlasOffset.x + baseUV.x * cellSize.x;
	atlasUV.y = atlasOffset.y + baseUV.y * cellSize.y;

	return (atlasUV);
}

bool	is_outline(vec3 pos)
{
	float w = 0.01;

	vec3 edgeLow  = step(pos, vec3(w));
	vec3 edgeHigh = step(vec3(1.0 - w), pos);

	vec3 edge = max(edgeLow, edgeHigh);

	return ((edge.x + edge.y + edge.z) >= 2.0);
}

#define BREAK_ANIM_FRAME1 240

uniform int	BREAK_ANIM_FRAME = -1;

void main()
{
	vec4	color = texture(atlas, getAtlasUV(vUv, BREAK_ANIM_FRAME1 + BREAK_ANIM_FRAME));
	if (is_outline(vFragPos))
		color = vec4(vec3(0), 1);
	else
		if (BREAK_ANIM_FRAME == -1)
			discard ;

	outColor = color;
}
