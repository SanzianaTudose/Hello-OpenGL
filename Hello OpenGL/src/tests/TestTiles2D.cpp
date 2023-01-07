#include "TestTiles2D.h"

#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>

const float WINDOW_WIDTH = 800.0f, WINDOW_HEIGHT = 600.0f; // BAD PRACTICE! this is already set in Application.cpp

namespace test 
{

    TestTiles2D::TestTiles2D() :
          _proj(glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, -1.0f, 1.0f)),
          _view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
          _size(3),
          _gridTranslate(glm::vec3(0.0f, 0.0f, 0.0f))
    {
        // Enable alpha blending and specify behavior
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Index data set-up
        // TODO: Make indices depend on {_size} 
      
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
       
        // Vertex Array & Vertex Buffer
        _VAO = std::make_unique<VertexArray>();
        _VBO = std::make_unique<VertexBuffer>(nullptr, _size * _size * sizeof(Vertex), GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(3); // Position element (posX, posY, posZ)
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

   static std::array<Vertex, 4> CreateQuad(float x, float y) {
       float size = 100.0f;

       Vertex v0 = {
           { x, y, 0.0f }, // Position
           { 1.0f, 1.0f, 1.0f, 1.0f } // Color
       };

       Vertex v1 = {
           { x + size, y, 0.0f }, // Position
           { 1.0f, 1.0f, 1.0f, 1.0f } // Color
       };

       Vertex v2 = {
           { x + size,  y + size, 0.0f }, // Position
           { 1.0f, 1.0f, 1.0f, 1.0f } // Color
       };

       Vertex v3 = {
           { x, y + size, 0.0f }, // Position
           { 1.0f, 1.0f, 1.0f, 1.0f } // Color
       };

       return { v0, v1, v2, v3 };
   }
    
    void TestTiles2D::OnUpdate(float deltaTime) {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // Set dynamic Vertex Buffer
        // TODO: Make vertexData depend on {_size} 
        auto q0 = CreateQuad(-50.0f, -50.0f);
        
        Vertex vertexData[4];
        memcpy(vertexData, q0.data(), q0.size() * sizeof(Vertex));
            
        _VBO->Bind();
        _VBO->AddData(0, sizeof(vertexData), vertexData);
    }

	void TestTiles2D::OnRender() {
        Renderer renderer;

        // Batch draw grid
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), _gridTranslate); // Model Matrix = model transform (Translation/Rotation/Scale)
            glm::mat4 mvp = _proj * _view * model;
            _shader->Bind();
            _shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*_VAO, *_IBO, *_shader);
        }
	}

	void TestTiles2D::OnImGuiRender() {
        ImGui::SliderFloat3("Grid Translate", &_gridTranslate.x, 0.0f, 600.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}