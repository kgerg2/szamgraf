#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

//in vec3 vs_out_col;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_pos = vec3(0, 100, 0);
uniform vec3 eye_pos = vec3(1, 1, 1);

// fénytulajdonságok: ambiens, diffúz, ...
vec4 La = vec4(0.8);
vec4 Ld = vec4(0.8);
vec4 Ls = vec4(1);

// anyagtulajdonságok: ambiens, diffúz, ...
vec4 Ka = vec4(1);
vec4 Kd = vec4(1);
vec4 Ks = vec4(1);

uniform int selected = 0;
uniform vec3 color = vec3(1, 1, 1);
uniform sampler2D texImage;

void main()
{
	if (selected == 1)
	{
		fs_out_col = vec4(1);
	}
	else
	{
		vec4 ambient = La * Ka;

		vec3 normal = normalize(vs_out_norm);
		vec3 toLight = normalize(light_pos - vs_out_pos);
		float di = clamp(dot(normal, toLight), 0, 1);
		vec4 diffuse = Ld * Kd * di;

		vec3 toEye = normalize(eye_pos - vs_out_pos);
		vec3 h = normalize(toLight + toEye);
		float si = pow(clamp(dot(h, normal), 0, 1), 25);
		vec4 specular = Ls * Ks * si;

		fs_out_col = (ambient + diffuse + specular) * texture(texImage, vs_out_tex) * vec4(color, 1);
	}
}