#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;

out vec4 fs_out_col;

void main()
{
//	vec3 fs_out_pos;
//	for (int i=0; i < 32; i++) {
//		float x = gl_FragCoord.x;
//		float y = gl_FragCoord.y;
//		fs_out_pos += (cos(t/32*2*3.14159), 3, 4);
//	}
//	discard;
	if (sqrt((gl_FragCoord.x / 640 - 0.5) * (gl_FragCoord.x / 640 - 0.5) + (gl_FragCoord.y / 480 - 0.5) * (gl_FragCoord.y / 480 - 0.5)) > 0.4) {
		fs_out_col = vec4(1, 1, 1, 1);
	} else {
		//fs_out_col = vec4(0.125, 0.25, 0.5, 1);
		discard;
	}
}

// 1. feladat: rajzoljuk ki feh�rrel a t�glalapot!

// 2. feladat: uniform v�ltoz�k - az alkalmaz�s �ll�tsa be, hogy milyen sz�nnel t�lts�nk ki!

// 3. feladat: rajzoljuk ki az orig� k�z�ppont�, 1 sugar� k�rt! Mit kell tenni, ha nem a
//    k�rlapot, hanem csak a k�rvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 4. feladat: komplex sz�mok....
