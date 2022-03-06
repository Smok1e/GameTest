uniform vec2  position;
uniform float radius;
uniform vec4  center_color;
uniform vec4  border_color;
uniform float y_bound;
uniform float shift = .7;

//------------------------

vec4 interpolate (vec4 lft, vec4 rgt, float t)
{
	return vec4 (
		lft.x + (rgt.x-lft.x) * t,
		lft.y + (rgt.y-lft.y) * t,
		lft.z + (rgt.z-lft.z) * t,
		lft.w + (rgt.w-lft.w) * t
	);
}

//------------------------

void main ()
{
	vec2 coord  = vec2 (gl_FragCoord.x, y_bound - gl_FragCoord.y);
	vec2 center = position + vec2 (radius, radius);

	vec2  diff = coord - center;
	float dist = sqrt (diff.x*diff.x + diff.y*diff.y);
	float t    = dist/radius - shift;

	if (t < 0) t = 0;
	t = t / (1 - shift);

	gl_FragColor = interpolate (center_color, border_color, t);
}

//------------------------