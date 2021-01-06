#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec2 vs_out_tex0;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

uniform float t = 0.0;
uniform sampler2D texImage0;
uniform sampler2D texImage1;

void main()
{
	fs_out_col = mix(texture(texImage0, vs_out_tex0), texture(texImage1, vs_out_tex0), t);
}

// procedurális textúra...
