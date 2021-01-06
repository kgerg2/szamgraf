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

// 1. feladat: rajzoljuk ki fehérrel a téglalapot!

// 2. feladat: uniform változók - az alkalmazás állítsa be, hogy milyen színnel töltsünk ki!

// 3. feladat: rajzoljuk ki az origó középpontú, 1 sugarú kört! Mit kell tenni, ha nem a
//    körlapot, hanem csak a körvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 4. feladat: komplex számok....
