#pragma once

// C++ includes
#include <memory>
#include <tuple>

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
	ProgramObject		m_program;				// main shader
	ProgramObject		m_programPostprocess;	// postprocess shader

	VertexArrayObject	m_SphereVao;
	IndexBuffer			m_SphereIndices;
	ArrayBuffer			m_SphereVertexBuffer;
	VertexArrayObject	m_PipeVao;
	IndexBuffer			m_PipeIndices;
	ArrayBuffer			m_PipeVertexBuffer;
	VertexArrayObject	m_CircleVao;
	IndexBuffer			m_CircleIndices;
	ArrayBuffer			m_CircleVertexBuffer;

	float m_filterWeight{};
	bool m_frameBufferCreated{ false };
	GLuint m_depthBuffer;               	//FBO Mélységbuffer textúra azonosító
	GLuint m_colorBuffer;               	//FBO Színbuffer textúra azonosító
	GLuint m_frameBuffer;               	//FBO azonosító

	gCamera				m_camera;

	Texture2D			m_metalTexture;

	enum Obj { No, XP, XN, YP, YN, ZP, ZN, S };
	std::vector<std::vector<std::vector<Obj>>> field;
	std::vector<std::tuple<int, int, int, glm::vec3>> pipeEnds;
	std::vector<std::tuple<int, int, int, glm::vec3>> finishedPipeEnds;
	static const int maxRetryCount{ 1000 };
	static const int N{ 20 };
	static constexpr float pipeWidth{ 0.8f };
	static constexpr float randomTurnChance{ 0.1f };

	int selectedBranch{ 0 };
	int genInterval{ 100 };
	int size{ 64 };
	int newSize{ 64 };
	int pipeCount{ 5 };
	int lastGenTime{ 0 };
	bool needsRedraw{ true };
	bool isFull{ false };
	glm::mat4 lastViewMatrix;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	void InitShaders();
	void DrawObj(const glm::mat4& world, int indCount);
	void InitSphere();
	void DrawSphere(const glm::mat4& world);
	void InitPipe();
	void DrawPipe(const glm::mat4& world, bool topFilled, bool bottomFilled);
	void DrawPipes(int x, int y, int z, glm::vec3 color, bool selected, bool onlyLast);
	void InitCircle();
	void DrawCircle(const glm::mat4& world);
	void GenPipe();
	std::tuple<int, int, int, glm::vec3> NewPipe();
	void CreateFrameBuffer(int width, int height);
};

