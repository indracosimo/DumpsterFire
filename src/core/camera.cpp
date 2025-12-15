#define GLM_ENABLE_EXPERIMENTAL
#include "camera.h"
#include "imgui.h"

glm::mat4 camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Orientation, Up);
}

glm::mat4 camera::GetProjectionMatrix(float FOVdeg, float nearPlane, float farPlane) const
{
	return glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);
}

camera::camera(int width, int height, glm::vec3 position)
{
	camera::width = width;
	camera::height = height;
	Position = position;
}

void camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

	 glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

