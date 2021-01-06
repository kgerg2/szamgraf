#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_pos = vec3(0, 5, 5);
uniform vec3 eye_pos = vec3(1, 1, 1);

// fénytulajdonságok: ambiens, diffúz, spekuláris
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.4, 0.6, 0.6);
uniform vec3 Ls = vec3(0.4, 0.0, 0.0);

// anyagtulajdonságok: ambiens, diffúz, spekuláris
uniform vec3 Ka = vec3(0.2, 0.4, 0.6);
uniform vec3 Kd = vec3(0.2, 0.4, 0.6);
uniform vec3 Ks = vec3(0.2, 0.4, 0.6);

uniform sampler2D texImage;

void main()
{	
	//
	// ambiens szín számítása
	//

	vec3 ambient = La * Ka;

	//
	// diffúz szín számítása
	//	
	/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/
	vec3 normal = normalize(vs_out_norm);
	vec3 toLight = normalize(light_pos - vs_out_pos);
	float di = clamp(dot(normal, toLight), 0, 1); // diffúz intenzitás
	vec3 diffuse = Ld * Kd * di;

	//
	// fényfoltképzõ szín
	//
	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/

	vec3 toEye = normalize(eye_pos - vs_out_pos);
	vec3 h = normalize(toLight * toEye); // ?
	float si = pow(clamp(dot(h, normal), 0, 1), 25); // intenzitás
	vec3 specular = Ls * Ks * si;
	
	//
	// a fragment végsõ színének meghatározása
	//

	//fs_out_col = vec4(ambient + diffuse + specular, 1);

	// felületi normális
	//fs_out_col = texture(texImage, vs_out_tex);
	fs_out_col = vec4(ambient + diffuse + specular, 1) * texture(texImage, vs_out_tex);



	// textúrával
	//vec4 textureColor = texture(texImage, vs_out_tex);
	//fs_out_col = vec4(ambient + diffuse + specular, 1) * textureColor;
}

// Feladatok

// 1) Fényszámítás
// - ambiens
// - diffúz
// - spekuláris

// 2) Textúra
