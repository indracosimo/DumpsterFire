#pragma once
#include "shaders/shader.h"
#include "geometry.h"
#include <glm/glm.hpp>
#include <vector>

struct CubeTransform
{
    std::string name = "Nameless Cube";
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f };
    glm::vec3 scale{ 1.0f };
};

class renderer
{
public:
    renderer(unsigned int width, unsigned int height);
    ~renderer();
    void render(const std::vector<CubeTransform>& cubes);

	Shader* mainShader = nullptr;

private:
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    unsigned int VAO = 0, VBO = 0, quadVAO = 0, quadVBO = 0;
    unsigned int framebuffer = 0, texColorBuffer = 0, RBO = 0;
    unsigned int texture1 = 0;

    void setupCube();
    void setupQuad();
    void setupFramebuffer();
    void loadTextures();

    void DrawMesh();
};

class mesh 
{
public:
	unsigned int VAO, VBO;
    int vertexCount;

	mesh(const std::vector<float>& vertices)
	{
		vertexCount = vertices.size() / 8;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texcoords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void DrawMesh()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glBindVertexArray(0);

	}
};

