#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <random>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void) {}

CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::DrawObj(const glm::mat4& world, int indCount)
{
	m_program.SetUniform("MVP", m_camera.GetViewProj() * world);
	m_program.SetUniform("world", world);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, nullptr);
}

void CMyApp::InitCircle()
{
	std::vector<Vertex> vert(N + 1);

	vert[0] = { glm::vec3(0, 0, 0), glm::vec3(0,-1, 0), glm::vec2(0, 0) };

	for (size_t i = 0; i < N; ++i) {
		float alpha = i / (float)N * 2 * M_PI;
		vert[i + 1] = { glm::vec3(cos(alpha) * pipeWidth, 0, sin(alpha) * pipeWidth), glm::vec3(0, 1, 0), glm::vec2(i / (float)N, 0) };
	}

	std::vector<int> indices(3 * N);
	for (size_t i = 0; i < N; ++i) {
		indices[3 * i] = 0;
		indices[3 * i + 1] = i + 1;
		indices[3 * i + 2] = (i + 1) % N + 1;
	}

	m_CircleVertexBuffer.BufferData(vert);

	m_CircleIndices.BufferData(indices);

	m_CircleVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_CircleVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_CircleVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CircleVertexBuffer },
		},
		m_CircleIndices
		);
}

void CMyApp::DrawCircle(const glm::mat4& world) {
	m_CircleVao.Bind();
	DrawObj(world, 3 * N);
}

void CMyApp::InitSphere()
{
	auto GetPos = [](float u, float v)
	{
		u *= float(2 * M_PI);
		v *= float(M_PI);
		float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
		float r = 2;

		return glm::vec3(r * cu * sv, r * cv, r * su * sv);
	};

	auto GetNorm = [](float u, float v)
	{
		u *= float(2 * M_PI);
		v *= float(M_PI);
		float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
		return glm::vec3(cu * sv, cv, su * sv);
	};

	auto GetTex = [](float u, float v)
	{
		return glm::vec2(1 - u, 1 - v);
	};

	std::vector<Vertex> vert((N + 1) * (N + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= N; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)N;

			vert[i + j * (N + 1)].p = GetPos(u, v);
			vert[i + j * (N + 1)].n = GetNorm(u, v);
			vert[i + j * (N + 1)].t = GetTex(u, v);
		}

	std::vector<int> indices(3 * 2 * N * N);
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}

	m_SphereVertexBuffer.BufferData(vert);

	m_SphereIndices.BufferData(indices);

	m_SphereVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer },
		},
		m_SphereIndices
		);
}

void CMyApp::DrawSphere(const glm::mat4& world) {
	m_SphereVao.Bind();
	DrawObj(world, 6 * N * N);
}

void CMyApp::InitPipe()
{
	std::vector<Vertex> vert;

	for (size_t i = 0; i < N; ++i) {
		float alpha = i / (float)N * 2 * M_PI;
		vert.push_back({ glm::vec3(cos(alpha) * pipeWidth, -0.5, sin(alpha) * pipeWidth), glm::vec3(cos(alpha), 0, sin(alpha)), glm::vec2(i / (float)N, 0) });
	}

	for (size_t i = 0; i < N; ++i) {
		float alpha = i / (float)N * 2 * M_PI;
		vert.push_back({ glm::vec3(cos(alpha) * pipeWidth, 2, sin(alpha) * pipeWidth), glm::vec3(cos(alpha), 0, sin(alpha)), glm::vec2(i / (float)N, 1) });
	}

	std::vector<int> indices;

	for (size_t i = 0; i < N; ++i) {
		indices.push_back(i);
		indices.push_back(N + i);
		indices.push_back((i + 1) % N);

		indices.push_back((i + 1) % N);
		indices.push_back(N + i);
		indices.push_back(N + (i + 1) % N);
	}

	m_PipeVertexBuffer.BufferData(vert);

	m_PipeIndices.BufferData(indices);

	m_PipeVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_PipeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_PipeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_PipeVertexBuffer },
		},
		m_PipeIndices
		);
}

void CMyApp::DrawPipe(const glm::mat4& world, bool topFilled, bool bottomFilled)
{
	m_PipeVao.Bind();
	DrawObj(world, 6 * N);

	if (topFilled)
	{
		DrawCircle(world *
			glm::rotate(glm::pi<float>(), glm::vec3(1, 0, 0)) *
			glm::translate(glm::vec3(0, -2, 0)));
	}
	if (bottomFilled)
	{
		DrawCircle(world * glm::translate(glm::vec3(0, -0.5, 0)));
	}
}

void CMyApp::DrawPipes(int x, int y, int z, glm::vec3 color, bool selected, bool onlyLast)
{
	Obj pipe;
	int dx, dy, dz;
	bool first = true;
	m_program.SetUniform("color", color);
	m_program.SetUniform("selected", selected ? 1 : 0);

	do
	{
		pipe = field[x][y][z];

		if (pipe == S)
		{
			DrawSphere(glm::translate(glm::vec3(2 * x, 2 * y, 2 * z)));

			dx = dy = dz = 0;
			if (x < size - 1 && field[x + 1][y][z] == XN)
				dx = 1;
			else if (y < size - 1 && field[x][y + 1][z] == YN)
				dy = 1;
			else if (z < size - 1 && field[x][y][z + 1] == ZN)
				dz = 1;
			else if (x > 0 && field[x - 1][y][z] == XP)
				dx = -1;
			else if (y > 0 && field[x][y - 1][z] == YP)
				dy = -1;
			else if (z > 0 && field[x][y][z - 1] == ZP)
				dz = -1;
		}
		else
		{
			glm::mat4 rotate = glm::mat4(1.0f);

			if (pipe == XP || pipe == XN)
				rotate = glm::rotate(glm::pi<float>() / 2, glm::vec3(0, 0, -1));
			else if (pipe == ZP || pipe == ZN)
				rotate = glm::rotate(glm::pi<float>() / 2, glm::vec3(1, 0, 0));

			bool up = pipe == XP || pipe == YP || pipe == ZP;
			DrawPipe(glm::translate(glm::vec3(2 * x, 2 * y, 2 * z)) * rotate, first && up, first && !up);

			dx = pipe == XP ? -1 : pipe == XN ? 1 : 0;
			dy = pipe == YP ? -1 : pipe == YN ? 1 : 0;
			dz = pipe == ZP ? -1 : pipe == ZN ? 1 : 0;
		}

		x += dx;
		y += dy;
		z += dz;

		first = false;
	} while (!onlyLast && (dx || dy || dz));
}

void CMyApp::InitShaders()
{
	m_program.Init(
		{
			{ GL_VERTEX_SHADER, "myVert.vert"},
			{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
			{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		});

	// postprocess shader - a geometria a kódban van
	m_programPostprocess.Init(
		{
			{ GL_VERTEX_SHADER,		"postprocess.vert" },
			{ GL_FRAGMENT_SHADER,	"postprocess.frag" }
		});
}

bool CMyApp::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafelé néző lapok eldobását
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();
	InitSphere();
	InitPipe();
	InitCircle();

	m_metalTexture.FromFile("assets/metal.jpg");

	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);
	m_camera.SetView(glm::vec3(-10, -10, -10), glm::vec3(0, 0, 0), m_camera.GetUp());

	// FBO - kezdeti
	CreateFrameBuffer(640, 480);

	std::vector<std::vector<Obj>> temp;
	for (int i = 0; i < size; i++)
		temp.push_back(std::vector<Obj>(size, No));

	for (int i = 0; i < size; i++)
		field.push_back(temp);

	for (int i = 0; i < pipeCount; i++)
		pipeEnds.push_back(NewPipe());

	return true;
}

void CMyApp::Clean()
{
	if (m_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &m_depthBuffer);
		glDeleteTextures(1, &m_colorBuffer);
		glDeleteFramebuffers(1, &m_frameBuffer);
	}
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	int time = SDL_GetTicks();
	if (time > lastGenTime + genInterval)
	{
		if (!isFull)
		{
			try
			{
				GenPipe();
			}
			catch (std::exception)
			{
				isFull = true;
			}
		}
		lastGenTime = time;
	}
}

std::tuple<int, int, int, glm::vec3> CMyApp::NewPipe()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> rnd(0, size - 1);

	int x, y, z, count = 0;

	do
	{
		x = rnd(gen);
		y = rnd(gen);
		z = rnd(gen);
	} while (field[x][y][z] != No && ++count < maxRetryCount);

	if (count == maxRetryCount)
	{
		int x, y, z;
		x = 0;
		while (x < size && field[x][y = 0][z = 0] != No)
		{
			y = 0;
			while (y < size && field[x][y][z = 0] != No)
			{
				z = 0;
				while (z < size && field[x][y][z] != No)
				{
					z++;
				}
				y++;
			}
			x++;
		}

		if (x == size && y == size && z == size)
		{
			std::cout << "Cannot generate new pipe, available space full.\n";
			throw std::exception("Cannot generate new pipe, available space full.\n");
		}
	}

	field[x][y][z] = S;

	std::uniform_real_distribution<> color(0, 1);

	return std::tuple<int, int, int, glm::vec3>(x, y, z, glm::vec3(color(gen), color(gen), color(gen)));
}

void CMyApp::GenPipe()
{
	int x, y, z;
	glm::vec3 color;

	for (auto& t : pipeEnds)
	{
		std::tie(x, y, z, color) = t;

		Obj pipe = field[x][y][z];

		if (pipe == S)
		{
			std::vector<std::tuple<int, int, int>> free;
			if (x < size - 2 && field[x + 1][y][z] == No && field[x + 2][y][z] == No && (x == 0 || field[x - 1][y][z] != XP))
				free.emplace_back(1, 0, 0);
			if (y < size - 2 && field[x][y + 1][z] == No && field[x][y + 2][z] == No && (y == 0 || field[x][y - 1][z] != YP))
				free.emplace_back(0, 1, 0);
			if (z < size - 2 && field[x][y][z + 1] == No && field[x][y][z + 2] == No && (z == 0 || field[x][y][z - 1] != ZP))
				free.emplace_back(0, 0, 1);
			if (x > 1 && field[x - 1][y][z] == No && field[x - 2][y][z] == No && (x == size - 1 || field[x + 1][y][z] != XN))
				free.emplace_back(-1, 0, 0);
			if (y > 1 && field[x][y - 1][z] == No && field[x][y - 2][z] == No && (y == size - 1 || field[x][y + 1][z] != YN))
				free.emplace_back(0, -1, 0);
			if (z > 1 && field[x][y][z - 1] == No && field[x][y][z - 2] == No && (z == size - 1 || field[x][y][z + 1] != ZN))
				free.emplace_back(0, 0, -1);

			if (free.empty())
			{
				finishedPipeEnds.push_back(t);
				t = NewPipe();
			}
			else
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> rnd(0, free.size() - 1);
				int dx, dy, dz;
				std::tie(dx, dy, dz) = free[rnd(gen)];
				Obj pipe =
					dx == 1 ? XP :
					dx == -1 ? XN :
					dy == 1 ? YP :
					dy == -1 ? YN :
					dz == 1 ? ZP :
					ZN;
				field[x + dx][y + dy][z + dz] = pipe;
				t = std::tuple<int, int, int, glm::vec3>(x + dx, y + dy, z + dz, color);
			}
		}
		else
		{
			int dx = pipe == XP ? 1 : pipe == XN ? -1 : 0;
			int dy = pipe == YP ? 1 : pipe == YN ? -1 : 0;
			int dz = pipe == ZP ? 1 : pipe == ZN ? -1 : 0;
			bool prevS =
				x - dx < 0 || x - dx >= size ||
				y - dy < 0 || y - dy >= size ||
				z - dz < 0 || z - dz >= size ||
				field[x - dx][y - dy][z - dz] == S;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> rnd(0, 1);

			if (x + dx < 0 || x + dx >= size ||
				y + dy < 0 || y + dy >= size ||
				z + dz < 0 || z + dz >= size ||
				field[x + dx][y + dy][z + dz] != No)
			{
				finishedPipeEnds.push_back(t);
				t = NewPipe();
			}
			else if (x + 2 * dx < 0 || x + 2 * dx >= size ||
				y + 2 * dy < 0 || y + 2 * dy >= size ||
				z + 2 * dz < 0 || z + 2 * dz >= size ||
				field[x + 2 * dx][y + 2 * dy][z + 2 * dz] != No ||
				rnd(gen) < randomTurnChance)
			{
				field[x + dx][y + dy][z + dz] = S;
				t = std::tuple<int, int, int, glm::vec3>(x + dx, y + dy, z + dz, color);
			}
			else
			{
				field[x + dx][y + dy][z + dz] = pipe;
				t = std::tuple<int, int, int, glm::vec3>(x + dx, y + dy, z + dz, color);
			}
		}
	}
}

void CMyApp::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	m_program.Use();
	m_program.SetTexture("texImage", 0, m_metalTexture);
	m_program.SetUniform("eye_pos", m_camera.GetEye());
	static glm::vec3 lPos{ 0.0f, 100.0f, 0.0f };
	m_program.SetUniform("light_pos", lPos);

	glm::vec3 color;

	if (needsRedraw || m_camera.GetViewMatrix() != lastViewMatrix)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < pipeCount; i++)
		{
			int x, y, z;
			std::tie(x, y, z, color) = pipeEnds[i];
			DrawPipes(x, y, z, color, i + 1 == selectedBranch, false);
		}

		for (int i = 0; i < finishedPipeEnds.size(); i++)
		{
			int x, y, z;
			std::tie(x, y, z, color) = finishedPipeEnds[i];
			DrawPipes(x, y, z, color, i + 1 == -selectedBranch, false);
		}

		lastViewMatrix = m_camera.GetViewMatrix();
		needsRedraw = false;
	}
	else
	{
		for (int i = 0; i < pipeCount; i++)
		{
			int x, y, z;
			std::tie(x, y, z, color) = pipeEnds[i];
			DrawPipes(x, y, z, color, i + 1 == selectedBranch, true);
		}
	}

	m_program.Unuse();


	// álljunk vissza a default FBO-ra (=frontbuffer)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_programPostprocess.Use();
	m_programPostprocess.SetTexture("frameTex", 0, m_colorBuffer);
	m_programPostprocess.SetUniform("t", m_filterWeight);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	m_programPostprocess.Unuse();


	if (ImGui::Button("Reset"))
	{
		if (size != newSize)
		{
			size = newSize;

			std::vector<std::vector<Obj>> temp;
			for (int i = 0; i < size; i++)
				temp.push_back(std::vector<Obj>(size, No));

			field.clear();
			for (int i = 0; i < size; i++)
				field.push_back(temp);
		}
		else
		{
			for (auto& yz : field)
				for (auto& z : yz)
					std::fill(z.begin(), z.end(), No);
		}

		pipeEnds.clear();
		finishedPipeEnds.clear();

		for (int i = 0; i < pipeCount; i++)
			pipeEnds.push_back(NewPipe());

		needsRedraw = true;
		isFull = false;
		selectedBranch = 0;
	}

	ImGui::InputInt("Field size", &newSize);
	newSize = std::max(0, newSize);

	int newCount = pipeEnds.size();
	ImGui::InputInt("Pipe count", &newCount);
	newCount = std::max(0, newCount);

	while (pipeEnds.size() < newCount)
	{
		pipeEnds.push_back(NewPipe());
	}

	while (pipeEnds.size() > newCount)
	{
		finishedPipeEnds.push_back(pipeEnds.back());
		pipeEnds.pop_back();
	}

	pipeCount = newCount;

	ImGui::InputInt("Wait time (ms)", &genInterval);
	genInterval = std::max(0, genInterval);

	for (int i = 1; i <= pipeCount; i++)
	{
		bool selected = selectedBranch == i;
		ImGui::Selectable(("branch " + std::to_string(i)).c_str(), &selected);
		if (selectedBranch == i && !selected)
		{
			selectedBranch = 0;
			needsRedraw = true;
		}
		else if (selectedBranch != i && selected)
		{
			selectedBranch = i;
			needsRedraw = true;
		}
	}

	for (int i = 1; i <= finishedPipeEnds.size(); i++)
	{
		bool selected = selectedBranch == -i;
		ImGui::Selectable(("branch " + std::to_string(i + pipeCount)).c_str(), &selected);
		if (selectedBranch == -i && !selected)
		{
			selectedBranch = 0;
			needsRedraw = true;
		}
		else if (selectedBranch != -i && selected)
		{
			selectedBranch = -i;
			needsRedraw = true;
		}
	}

	if (selectedBranch)
	{
		auto& t = selectedBranch > 0 ? pipeEnds[selectedBranch - 1] : finishedPipeEnds[-selectedBranch - 1];
		glm::vec3 colorVec = std::get<3>(t);
		float color[3]{ colorVec[0], colorVec[1], colorVec[2] };
		ImGui::SliderFloat3("Selected color", color, 0, 1);

		if (color[0] != colorVec[0] || color[1] != colorVec[1] || color[2] != colorVec[2])
		{
			std::get<3>(t) = glm::vec3(color[0], color[1], color[2]);
			//needsRedraw = true;
		}
	}

	float newLPos[3]{ lPos[0], lPos[1], lPos[2] };
	ImGui::InputFloat3("Light direction", newLPos);

	if (lPos[0] != newLPos[0] || lPos[1] != newLPos[1] || lPos[2] != newLPos[2])
	{
		lPos = glm::vec3(newLPos[0], newLPos[1], newLPos[2]);
		needsRedraw = true;
	}
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
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

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);

	CreateFrameBuffer(_w, _h);

	needsRedraw = true;
}

void CMyApp::CreateFrameBuffer(int width, int height)
{
	if (m_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &m_depthBuffer);
		glDeleteTextures(1, &m_colorBuffer);
		glDeleteFramebuffers(1, &m_frameBuffer);
	}

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "Error creating color attachment" << std::endl;
		exit(1);
	}

	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "Error creating depth attachment" << std::endl;
		exit(1);
	}

	// -- Completeness check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Incomplete framebuffer (";
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		}
		std::cout << ")" << std::endl;
		char ch;
		std::cin >> ch;
		exit(1);
	}

	// -- Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_frameBufferCreated = true;
}