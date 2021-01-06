#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader

	gCamera				m_camera;

	Texture2D			m_suzanneTexture;
	Texture2D			m_grassTexture;
	Texture2D			m_mudTexture;
	Texture2D			m_rockTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	bool backgroundChange;
	float backgroundColor[3]{ 1, 1, 1 };
	
	bool cameraMovable = false;
	bool cameraMoving = false;
	int cameraTicks = 0;
	int cameraLastTick = 0;
	glm::vec3 cameraP1{ -12, 20, 0 };
	glm::vec3 cameraP2{ 0, 20, -15 };
	glm::vec3 cameraP3{ 12, 20, 0 };

	int mixedTextures = 0;

	// mesh adatok
	Mesh *m_mesh;

	// a jobb olvashatóság kedvéért
	void InitShaders();
};

