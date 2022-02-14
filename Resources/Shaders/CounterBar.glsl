uniform vec4  lft_color;
uniform vec4  rgt_color;
uniform vec4  background_color;
uniform float t;
uniform vec2  position;
uniform vec2  size;

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
	float current_t = (gl_FragCoord.x - position.x) / size.x;

	if (current_t < t) gl_FragColor = interpolate (lft_color, rgt_color, current_t);
	else gl_FragColor = background_color;
}

//------------------------