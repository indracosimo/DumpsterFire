#include "Viewport.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <iostream>

Viewport::Viewport(int width, int height, const glm::vec3& initialCameraPos) : width(width), height(height), mCamera(width, height, initialCameraPos)
{
    //  framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    //  color texture
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    // depth renderbuffer
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    mRenderer = std::make_unique<renderer>(width, height);
}

Viewport::~Viewport()
{
    if (colorTexture)
        glDeleteTextures(1, &colorTexture);
    if (depthRenderbuffer)
        glDeleteRenderbuffers(1, &depthRenderbuffer);
    if (framebuffer)
        glDeleteFramebuffers(1, &framebuffer);
}

void Viewport::Resize(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    mCamera = camera(width, height, mCamera.Position);
}

void Viewport::Update(float deltaTime, GLFWwindow* window)
{
    mCamera.Inputs(window, deltaTime);
}

void Viewport::Render(const std::vector<CubeTransform>& cubes)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (mRenderer)
    {
        mRenderer->mainShader->use();
        mCamera.Matrix(90.0f, 0.1f, 100.0f, *mRenderer->mainShader, "camMatrix");
        mRenderer->render(cubes);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewport::RenderScene(const std::vector<CubeTransform>& cubes, float FOV)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mRenderer)
    {
        mRenderer->mainShader->use();
        mCamera.Matrix(FOV, 0.1f, 100.0f, *mRenderer->mainShader, "camMatrix");
        mRenderer->render(cubes);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
