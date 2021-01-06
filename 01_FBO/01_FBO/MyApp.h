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

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"

#include "Mesh_OGL3.h"
#include "gCamera.h"

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
	// FBO l�trehoz� elj�r�s
	void CreateFrameBuffer(int width, int height);

	// shaderekhez sz�ks�ges v�ltoz�k
	ProgramObject		m_program;				// shaderek alap programja
	ProgramObject		m_programPostprocess;	// posprocess shaderek programja

	Texture2D			m_textureMetal;

	std::unique_ptr<Mesh>	m_mesh;

	gCamera				m_camera;

	float	m_filterWeight{};

	// FBO-hoz sz�ks�ges dolgok
	bool m_frameBufferCreated{ false };
	GLuint m_depthBuffer;
	GLuint m_colorBuffer;
	GLuint m_frameBuffer;
};

