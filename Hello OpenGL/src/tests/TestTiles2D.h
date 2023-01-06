#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

// Simple render of a 2D tile grid 

namespace test 
{
	struct Vertex {
		float Position[3];
		float Color[4];
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
		unsigned int _size;
		glm::vec3 _gridTopLeft;
	};
}