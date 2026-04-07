vec2	getAtlasUV(vec2 uv, int textureId, int texture_size, int atlas_size)
{
	vec2	baseUV = uv;

	int row = (texture_size - 1) - (textureId / texture_size);;
	int col = textureId % texture_size;

	vec2 cellSize = vec2(float(texture_size) / float(atlas_size));
	vec2 atlasOffset;
	atlasOffset.x = float(col) * cellSize.x;
	atlasOffset.y = float(row) * cellSize.y;
	vec2 atlasUV;
	atlasUV.x = atlasOffset.x + baseUV.x * cellSize.x;
	atlasUV.y = atlasOffset.y + baseUV.y * cellSize.y;

	return (atlasUV);
}
