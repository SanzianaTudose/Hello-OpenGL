#include "TestTiles2D.h"

#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>

namespace test 
{
    // TODO: Make {_gridSize} dynamic 

    const float WINDOW_WIDTH = 800.0f, WINDOW_HEIGHT = 600.0f; // BAD PRACTICE! this is already set in Application.cpp
    const unsigned int MAX_SIZE = 10;

    static void SetUpIndexBufferQuads(unsigned int quadCount, unsigned int indices[]) {
        for (int quad = 0; quad < quadCount; quad++) {
            // A quad is made up of vertices 0, 1, 2, 2, 3, 0
            int first = 6 * quad;
            indices[first] = 4 * quad + 0;
            indices[first + 1] = 4 * quad + 1;
            indices[first + 2] = 4 * quad + 2;
            indices[first + 3] = 4 * quad + 2;
            indices[first + 4] = 4 * quad + 3;
            indices[first + 5] = 4 * quad + 0;
        }
    }

    TestTiles2D::TestTiles2D() :
          _proj(glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, -1.0f, 1.0f)),
          _view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
          _gridSize(3), _gridOrigin(glm::vec3(0.0f, 0.0f, 0.0f)),
          _tileSize(10.0f), _tileSpacing(10.0f)
    {
        // Enable alpha blending and specify behavior
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Index data set-up for _gridSize * _gridSize quads
        const unsigned int maxQuadCount = MAX_SIZE * MAX_SIZE;
        unsigned int indices[6 * maxQuadCount]; // Allocate maximum number of vertices
        SetUpIndexBufferQuads(maxQuadCount, indices);
       
        // Vertex Array & Vertex Buffer
        _VAO = std::make_unique<VertexArray>();
        _VBO = std::make_unique<VertexBuffer>(nullptr, 4 * maxQuadCount * sizeof(Vertex), GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(3); // Position element (X, Y, Z)
        layout.Push<float>(4); // Color element (R, G, B, alpha)
        _VAO->AddBuffer(*_VBO, layout);

        // Index Buffer
        _IBO = std::make_unique<IndexBuffer>(indices, maxQuadCount * 6);
        IndexBuffer ib();

        // Shader
        _shader = std::make_unique<Shader>("res/shaders/Tile.shader");
        _shader->Bind();
	}
	
	TestTiles2D::~TestTiles2D() {}

    // Return vertices array for a quad with bottom right corner in (x,y)
    static std::array<Vertex, 4> CreateQuad(float x, float y, float quadSize) {
       float size = quadSize;

       Vertex v0;
       v0.position = { x, y, 0.0f };
       v0.color = { 1.0f, 1.0f, 1.0f, 1.0f };

       Vertex v1;
       v1.position = { x + size, y, 0.0f };
       v1.color = { 1.0f, 1.0f, 1.0f, 1.0f };

       Vertex v2;
       v2.position = { x + size,  y + size, 0.0f };
       v2.color = { 1.0f, 1.0f, 1.0f, 1.0f };
       
       Vertex v3;
       v3.position = { x, y + size, 0.0f };
       v3.color = { 1.0f, 1.0f, 1.0f, 1.0f };

       return { v0, v1, v2, v3 };
   }
    
    // Create vertex data for a square tile grid
    void TestTiles2D::CreateGridData(Vertex vertexData[]) {
        const float spacing = _tileSpacing;
        const float quadSize = _tileSize;
        
        unsigned int dataOffset = 0;
        for (int i = 0; i < _gridSize; i++)
        for (int j = 0; j < _gridSize; j++) {
            float rowsBelow = _gridSize - i - 1;
            float colsRight = _gridSize - j - 1;
            float totalXOffset = (rowsBelow * quadSize) + (rowsBelow * spacing);
            float totalYOffset = (colsRight * quadSize) + (colsRight * spacing);

            auto quad = CreateQuad(_gridOrigin.x + totalXOffset, _gridOrigin.y + totalYOffset, quadSize);
            memcpy(vertexData + dataOffset, quad.data(), quad.size() * sizeof(Vertex));

            dataOffset += quad.size();
        }
    }

    void TestTiles2D::OnUpdate(float deltaTime) {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // Set-up dynamic Vertex Buffer
        Vertex vertexData[4 * MAX_SIZE * MAX_SIZE];
        CreateGridData(vertexData);
            
        _VBO->Bind();
        _VBO->AddData(0, sizeof(vertexData), vertexData);
    }

	void TestTiles2D::OnRender() {
        Renderer renderer;

        // Batch draw grid
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), _gridOrigin); // Model Matrix = model transform (Translation/Rotation/Scale)
            glm::mat4 mvp = _proj * _view * model;
            _shader->Bind();
            _shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*_VAO, *_IBO, *_shader);
        }
	}

	void TestTiles2D::OnImGuiRender() {
        ImGui::SliderFloat3("Grid Translate", &_gridOrigin.x, 0.0f, 300.0f);
        ImGui::SliderInt("Grid Size", &_gridSize, 0, 10);

        ImGui::SliderFloat("Tile Size", &_tileSize, 10.0f, 100.0f);
        ImGui::SliderFloat("Tile Spacing", &_tileSpacing, 0.0f, 30.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}