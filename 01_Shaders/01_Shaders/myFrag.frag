#version 130

// bemeneti v�ltoz� - most a vertex shader-b�l (vagyis ottani out)
in vec4 vs_out_col;

// kimeneti v�ltoz� - a fragment sz�ne
out vec4 fs_out_col;

void main()
{
	//				  R, G, B, A
	fs_out_col = vs_out_col;

	//				  R, G, B, A
	//fs_out_col = vec4(1, 1, 1, 1);
}