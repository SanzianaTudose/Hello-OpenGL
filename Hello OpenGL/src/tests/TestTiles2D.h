#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

// Simple render of a square 2D tile grid (so one wall of a 3D room)

namespace test 
{
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	class TestTiles2D : public Test
	{
	public:
		TestTiles2D();
		~TestTiles2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> _VAO;
		std::unique_ptr<VertexBuffer> _VBO;
		std::unique_ptr<IndexBuffer> _IBO;
		std::unique_ptr<Shader> _shader;

		glm::mat4 _proj, _view; // View Projection Matrices

		// Grid properties
		int _gridSize;
		float _tileSize;
		float _tileSpacing;
		glm::vec3 _gridOrigin;

		void CreateGridData(Vertex vertexData[]);
	};
}