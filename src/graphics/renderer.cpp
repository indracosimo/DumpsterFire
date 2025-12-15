#include "geometry.h"
#include "renderer.h"
#include "textureLoader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../MeshManager.h"



renderer::renderer(unsigned int width, unsigned int height)
	: SCR_WIDTH(width), SCR_HEIGHT(height)
{
	glEnable(GL_DEPTH_TEST);
	mainShader = new Shader("src/shaders/vertexShader.vs", "src/shaders/fragmentShader.fs");

	setupCube();
	setupQuad();
	setupFramebuffer();
	loadTextures();

	std::cout << "texColorBuffer ID: " << texColorBuffer << std::endl;
	std::cout << "texture1 ID: " << texture1 << std::endl;
}

renderer::~renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete mainShader;
	//delete crtShader;
}

void renderer::setupCube()
{
	float* vertices = getCubeVertices();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, getCubeVertexCount() * sizeof(float), vertices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texcoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void renderer::setupQuad()
{
	float* vertices = getQuadVertices();
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, getQuadVertexCount() * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void renderer::setupFramebuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderer::loadTextures()
{
	texture1 = loadTexture("assets/images/concrete.jpg");
}

void renderer::render(const std::vector<CubeTransform>& cubes) 
{
	Mesh* planeMesh = MeshManager::GetInstance().GetMesh("assets/obj/quadPlane.obj");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, bEnableCRT ? framebuffer : 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainShader->use();
	mainShader->setInt("texture1", 0);

	glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, texture1);
	//glBindVertexArray(VAO);

	for (const auto& cube : cubes)
	{
		glBindTexture(GL_TEXTURE_2D, cube.textureID);
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			float(SCR_WIDTH) / SCR_HEIGHT,
			0.1f, 9999.0f);
		model = glm::translate(model, cube.position);
		model = glm::rotate(model, glm::radians(cube.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(cube.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(cube.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cube.scale);

		mainShader->setMat4("projection", projection);
		mainShader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		Mesh* meshToDraw = nullptr;
		if (!cube.meshPath.empty())
		{
			meshToDraw = MeshManager::GetInstance().GetMesh(cube.meshPath);
		}

		if (meshToDraw && meshToDraw->vertexCount > 0)
		{
			meshToDraw->DrawMesh();
		}
		else
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	//planeMesh->DrawMesh();


	glBindVertexArray(0);

	// CRT post-process pass
	//if (bEnableCRT)
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	glDisable(GL_DEPTH_TEST);
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	crtShader->use();
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	//	crtShader->setInt("iChannel0", 0);
	//	crtShader->setVec2("iResolution", glm::vec2(SCR_WIDTH, SCR_HEIGHT));
	//	crtShader->setFloat("iTime", glfwGetTime());

	//	glBindVertexArray(quadVAO);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glBindVertexArray(0);
	//}
}
