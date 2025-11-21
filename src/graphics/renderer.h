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
};

