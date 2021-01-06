#version 330 core

#define PI 3.1415926538

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-1,-1);

// fénytulajdonságok: ambiens, diffúz, ...
vec3 La = vec3(0.4, 0.4, 0.4);
vec3 Ld = vec3(0.7, 0.7, 0.7);

uniform int mixedTextures = 0;
uniform sampler2D texImage;
uniform sampler2D texImageGrass;
uniform sampler2D texImageMud;
uniform sampler2D texImageRock;

void main()
{

	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	
	float cosa = clamp(dot(normal, to_light), 0, 1);

	vec3 diffuse = cosa*Ld;
	
	float angle = 1 - acos(dot(normal, vec3(0, 1, 0))) / PI * 2;

	if (mixedTextures == 1)
		fs_out_col = vec4(ambient + diffuse, 1) * (texture(texImageGrass, vs_out_tex) * clamp(angle, 0, 1) + texture(texImageMud, vs_out_tex) * clamp(1 - abs(angle), 0, 1) + texture(texImageRock, vs_out_tex) * clamp(-angle, 0, 1));
	else
		fs_out_col = vec4(ambient + diffuse, 1) * texture(texImage, vs_out_tex);
}