#include "TestTiles2D.h"

#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test 
{

    TestTiles2D::TestTiles2D()
        : _size(3),
          _gridTopLeft(glm::vec3(0.0f, 0.0f, 0.0f)),
          _proj(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f)),
          _view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
    {
        // Enable alpha blending and specify behavior
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Vertex data set-up
        // posX, posY, texX, texY
        float vertexData[] = {
            -50.0f, -50.0f,     1.0f, 1.0f, 1.0f, 1.0f,
             50.0f, -50.0f,     1.0f, 1.0f, 1.0f, 1.0f,
             50.0f,  50.0f,     1.0f, 1.0f, 1.0f, 1.0f,
            -50.0f,  50.0f,     1.0f, 1.0f, 1.0f, 1.0f,
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
       
        // Vertex Array & Vertex Buffer
        _VAO = std::make_unique<VertexArray>();
        _VBO = std::make_unique<VertexBuffer>(vertexData, 4 * 6 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2); // Position element (posX, posY)
        layout.Push<float>(4); // Color element (colR, colG, colB, colAlph)
        _VAO->AddBuffer(*_VBO, layout);

        // Index Buffer
        _IBO = std::make_unique<IndexBuffer>(indices, 6);
        IndexBuffer ib();

        // Shader
        _shader = std::make_unique<Shader>("res/shaders/Tile.shader");
        _shader->Bind();
	}
	
	TestTiles2D::~TestTiles2D() {}

	void TestTiles2D::OnUpdate(float deltaTime) {}

	void TestTiles2D::OnRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        // Batch draw grid
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), _gridTopLeft); // Model Matrix = model transform (Translation/Rotation/Scale)
            glm::mat4 mvp = _proj * _view * model;
            _shader->Bind();
            _shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*_VAO, *_IBO, *_shader);
        }
	}

	void TestTiles2D::OnImGuiRender() {
        ImGui::SliderFloat3("Grid Origin", &_gridTopLeft.x, 0.0f, 600.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}