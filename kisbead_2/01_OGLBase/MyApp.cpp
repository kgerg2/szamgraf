#include "MyApp.h"

#include <vector>

#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_mesh = nullptr;
	backgroundChange = false;
}

CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		});

	m_program.LinkProgram();
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();

	// egyéb textúrák betöltése
	m_suzanneTexture.FromFile("assets/marron.jpg");
	m_grassTexture.FromFile("assets/grass_texture.jpg");
	m_mudTexture.FromFile("assets/mud_texture.jpg");
	m_rockTexture.FromFile("assets/rock_texture.jpg");

	// mesh betoltese
	m_mesh = ObjParser::parse("assets/Suzanne.obj");
	m_mesh->initBuffers();

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::Clean()
{
	delete m_mesh;
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	//Suzanne
	glm::mat4 suzanneWorld = glm::mat4(1.0f) * glm::scale(glm::vec3(3, 3, 3));
	m_program.Use();
	m_program.SetTexture("texImage", 0, m_suzanneTexture);
	m_program.SetTexture("texImageGrass", 1, m_grassTexture);
	m_program.SetTexture("texImageMud", 2, m_mudTexture);
	m_program.SetTexture("texImageRock", 3, m_rockTexture);
	m_program.SetUniform("MVP", viewProj * suzanneWorld);
	m_program.SetUniform("world", suzanneWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(suzanneWorld)));
	m_program.SetUniform("mixedTextures", mixedTextures);
	m_mesh->draw();

	ImGui::Text("Udvozollek!");

	if (ImGui::Button("Bezier gorbe kovetes")) {
		cameraMovable = !cameraMovable;
		cameraMoving = cameraMovable;
		if (cameraMoving)
			cameraLastTick = SDL_GetTicks();
	}

	if (cameraMovable && cameraMoving) {
		cameraTicks += SDL_GetTicks() - cameraLastTick;
		cameraLastTick = SDL_GetTicks();
		float t = (sinf(cameraTicks / 2000.0f) + 1) / 2;
		m_camera.SetView((1 - t) * (1 - t) * cameraP1 + 2 * (1 - t) * t * cameraP2 + t * t * cameraP3, glm::vec3(0, 0, 0), m_camera.GetUp());
	}

	if (ImGui::Button("Textura interpolalas")) {
		mixedTextures = 1 - mixedTextures;
	}

	if (ImGui::Button("Hatterszin modositasa")) {
		backgroundChange = !backgroundChange;
	}

	if (backgroundChange) {
		ImGui::SliderFloat3("Szin: ", backgroundColor, 0, 1);
		glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
	}

	if (ImGui::Button("Kilepes")) {
		SDL_Event* ev = new SDL_Event;
		ev->type = SDL_QUIT;
		SDL_PushEvent(ev);
	}
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
	if (key.keysym.sym == SDLK_SPACE) {
		cameraMovable = !cameraMovable;
		cameraLastTick = SDL_GetTicks();
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
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

	m_camera.Resize(_w, _h);
}
