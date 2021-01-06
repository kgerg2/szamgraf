#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <time.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_ibID = 0;

	m_programID = 0;

	srand(time(NULL));
}

CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

	//
	// geometria létrehozasa
	//

	Vertex vert[M+1];

	for (int i = 0; i < M; i++)
	{
		float t = 2 * M_PI * i / M;
		vert[i].p = glm::vec3(2*sinf(t), 0, 2*cosf(t));
		vert[i].c = glm::normalize(vert[i].p);
	}

	vert[M].p = glm::vec3(0, 3, 0);
	vert[M].c = glm::vec3(1, 1, 1);

	GLushort indices[6*M-6];

	for (int i = 0; i < M; i++)
	{
		indices[3 * i] = i;
		indices[3 * i + 1] = (i + 1) % M;
		indices[3 * i + 2] = M;
	}
	for (int i = 0; i < M - 2; i++)
	{
		indices[3 * M + 3 * i] = M - 1;
		indices[3 * M + 3 * i + 1] = i + 1;
		indices[3 * M + 3 * i + 2] = i;
	}

	N = rand() % 7 + 4;

	for (int i = 0; i < N; i++) {
		float fi = (float)rand() / RAND_MAX * 2 * M_PI;
		float h = (float)rand() / RAND_MAX * 7;
		transforms.emplace_back(6 * sinf(fi), h, 6 * cosf(fi));
	}

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert),		// ennyi bájt nagyságban
				  vert,	// errõl a rendszermemóriabeli címrõl olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	// index puffer létrehozása
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés elõtt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonosítója, amibõl egy változóhoz szeretnénk hozzárendelést csinálni
							0,				// a VAO-beli azonosító index
							"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> error_massage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, error_massage.data());

		std::cout << "Hiba a shader letrehozasakor: " << error_massage.data() << std::endl;
	}
	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// egyéb inicializálás
	//

	// vetítési mátrix létrehozása
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation( m_programID, "MVP");

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// nézeti transzformáció beállítása
	m_matView = glm::lookAt(glm::vec3( 0,  30,  -20),		// honnan nézzük a színteret
							glm::vec3( 0,  0,  0),		// a színtér melyik pontját nézzük
							glm::vec3( 0,  1,  0));		// felfelé mutató irány a világban
}

void CMyApp::drawCone()
{
	glBindVertexArray(m_vaoID);

	glDrawElements(GL_TRIANGLES, // primitív típus
		6*M-6,		             // hany csucspontot hasznalunk a kirajzolashoz
		GL_UNSIGNED_SHORT,	     // indexek tipusa
		0);					     // indexek cime

	glBindVertexArray(0);
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	GLM transzformációs mátrixokra példák:
		glm::rotate<float>( szög, glm::vec3(tengely_x, tengely_y, tengely_z) ) <- tengely_{xyz} körüli elforgatás
		glm::translate<float>( glm::vec3(eltol_x, eltol_y, eltol_z) ) <- eltolás
		glm::scale<float>( glm::vec3(s_x, s_y, s_z) ) <- léptékezés
	*/

	glUseProgram(m_programID);

	Uint32 t = SDL_GetTicks();

	float x = 10 * sinf((float) t / 6000 * M_PI * 2);

	int vt = t % 8000 - 6000;
	float vibr = sinf(fmaxf(vt, 0) / 20) * 0.1;

	float scale = 1;
	if (grow = grow && vt > 0) {
		scale = tanf((float) vt / 2000 * M_PI / 2);
		vibr = 0;
	}

	for (int i = 0; i < N; i++)
	{
		m_matWorld = glm::translate<float>(transforms[i])
			* glm::translate<float>(glm::vec3(x, 0, 0.01 * x * x * x + 0.05 * x * x))
			* glm::translate<float>(glm::vec3(-vibr, 0, vibr))
			* glm::scale<float>(glm::vec3(scale, scale, scale));

		glm::mat4 mvp = m_matProj * m_matView * m_matWorld;

		glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &(mvp[0][0]));

		drawCone();
	}

	glUseProgram(0);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	grow = key.keysym.sym == SDLK_SPACE && SDL_GetTicks() % 8000 - 6000 > 0;
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}