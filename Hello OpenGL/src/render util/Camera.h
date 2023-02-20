#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <memory>

// generic camera class
class Camera
{
public:
	Camera(int width, int height, glm::vec3 position) :
		_width(width), _height(height), position(position) {}

	// sets the mvp uniform in the vertex shader, given the current camera position,
	// its orientation, and the first 3 params of this function
	// by default, the model matrix is the identity matrix, but a different model matrix may be passed as a parameter
	void Matrix(float FOVdeg, float nearPlane, float farPlane, std::unique_ptr<Shader>& shader, const char* uniform, glm::mat4 model = glm::mat4(1.0f));

	// handles camera controls
	void Inputs(GLFWwindow* window);

private:
	glm::vec3 position;										// position of the camera
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);	// (normalised) orientation of the camera
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);		// (normalised) up vector

	int _width = 0, _height = 0;							// width and height of the window

	float speed = 0.1f;										// current speed of the camera
	const float fast_speed = 0.1f;							// speed of the camra while holding shift
	const float regular_speed = 0.005f;						// speed of the camera while not holding shift
	float sensitivity = 0.1f;								// sensitivity of the camera rotation

	// yaw and pitch are used to determine the camera orientation
	float yaw = 270.0f;										// yaw position (degrees)
	float pitch = 0.0f;										// pitch position (degrees)

	bool firstFocus = true;									// used to prevent camera jumping when re-focusing on the window
};
