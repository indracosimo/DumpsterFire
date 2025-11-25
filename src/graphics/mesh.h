#pragma once
#include <glad/glad.h>
#include <vector>

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

