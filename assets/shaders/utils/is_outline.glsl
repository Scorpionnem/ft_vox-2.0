bool	is_outline(vec3 pos, float width)
{
	float w = width;

	vec3 edgeLow  = step(pos, vec3(w));
	vec3 edgeHigh = step(vec3(1.0 - w), pos);

	vec3 edge = max(edgeLow, edgeHigh);

	return ((edge.x + edge.y + edge.z) >= 2.0);
}
