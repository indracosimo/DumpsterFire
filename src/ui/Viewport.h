#pragma once
#include "graphics/renderer.h"
#include "core/camera.h"
#include "glm/glm.hpp"

class Viewport
{
public:
    Viewport(int width, int height, const glm::vec3& initialCameraPos);
    ~Viewport();

    void Resize(int width, int height);
    void Update(float deltaTime, GLFWwindow* window);
    void Render(const std::vector<CubeTransform>& cubes);
    void RenderScene(const std::vector<CubeTransform>& cubes, float FOV);

    GLuint GetFramebufferTexture() const { return colorTexture; }
    camera& GetCamera() { return mCamera; }

private:
    GLuint framebuffer = 0;
    GLuint colorTexture = 0;
    GLuint depthRenderbuffer = 0;
    
    camera mCamera;
    std::unique_ptr<renderer> mRenderer;

    int width = 0;
    int height = 0;
};