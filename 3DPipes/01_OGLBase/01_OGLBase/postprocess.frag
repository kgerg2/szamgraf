#version 130

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D frameTex;
uniform float t;
uniform mat3 color_matrix = mat3( 0.393f, 0.349f, 0.272f, 
								  0.769f, 0.686f, 0.534f,
								  0.189f, 0.168f, 0.131f );

void main()
{
	vec4 originalColor = texture(frameTex, vs_out_tex);
	vec4 sepiaColored = vec4(
								color_matrix*originalColor.xyz, 
								1
							);
	fs_out_col = mix(originalColor, sepiaColored, t);
}