#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;

out vec4 fs_out_col;

uniform float px, py;

vec2 mult(vec2 x, vec2 y)
{
	return vec2(x[0]*y[0] - x[1]*y[1], x[0]*y[1] + x[1]*y[0]);
}

vec2 pow(vec2 x, int n)
{
	vec2 res = x;
	for (int i = 1; i < n; i++) {
		res = mult(res, x);
	}
	return res;
}

vec2 div(vec2 x, vec2 y)
{
	return mult(x, vec2(y[0], -y[1])) / dot(y, y);
}

vec2 f(vec2 x)
{
	return pow(x, 3) + vec2(px, py);
}

vec2 fd(vec2 x)
{
	return 3 * pow(x, 2);
}

vec3 hsv2rgb(float h, float s, float v)
{
    h = mod(h * 3 / 3.14159 + 3.0, 6.0);
    float c = v * s;

    vec2 cx = vec2(c, c * (1 - abs(mod(h, 2.0) - 1.0)));

    vec3 rgb = vec3(0.0);

    if(h <= 1.0) {
        rgb.rg = cx;
    } else if(h <= 2.0) {
        rgb.gr = cx;
    } else if(h <= 3.0) {
        rgb.gb = cx;
    } else if(h <= 4.0) {
        rgb.bg = cx;
    } else if(h <= 5.0) {
        rgb.br = cx;
    } else if(h <= 6.0) {
        rgb.rb = cx;
    }

    return rgb + vec3(v - c);
}


void main()
{
	vec2 x = vs_out_pos.xy;
	vec2 zi;
	vec2 zi1 = x;
	int i = 0;
	while (length(zi1- zi) > 0.01)
	{
		zi = zi1;
		zi1 = zi - div(f(zi), fd(zi));
		++i;
	}
	zi = zi1;

	float h = atan(zi[1], zi[0]);
	if (abs(zi[0]) < 0.00001)
		h = 3.14159 / 2 * sign(zi[1]);
	float s = tanh(length(zi));
	float v = tanh(15.0/i);

	fs_out_col = vec4(hsv2rgb(h, s, v), 1);
}
