#include "Camera.h"
#include <math.h>

#include <iostream>

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, std::unique_ptr<Shader>& shader, const char* uniform, glm::mat4 model)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + orientation, up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)_width / (float)_height, nearPlane, farPlane);
	shader->SetUniformMat4f(uniform, projection * view * model);
}

void Camera::Inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += speed * orientation;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= speed * orientation;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position += speed * glm::normalize(glm::cross(up, orientation));

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position -= speed * glm::normalize(glm::cross(up, orientation));

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		position += speed * up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		position -= speed * up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = fast_speed;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		speed = regular_speed;

	if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// used in order to bring cursor to the middle of the screen every frame
		float prevX = _width / 2;
		float prevY = _height / 2;

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (firstFocus)
		{
			prevX = mouseX;
			prevY = mouseY;
			firstFocus = false;
		}

		float offsetX = mouseX - prevX;
		float offsetY = mouseY - prevY;

		yaw += sensitivity * offsetX;
		pitch += sensitivity * offsetY;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		float x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		float y = -sin(glm::radians(pitch));
		float z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		// change to this if y-axis movement is inverted
		//orientation = glm::normalize(glm::vec3(x, -y, z));

		orientation = glm::normalize(glm::vec3(x, y, z));

		glfwSetCursorPos(window, _width / 2, _height / 2);
	}
	else
	{
		firstFocus = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}