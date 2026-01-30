#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "light.h"
#include "shaders/shader.h"

class ShadowMapper
{
public:
    ShadowMapper();
    ~ShadowMapper();

    void initializeShadowMap(light& lightSource);

    void beginShadowPass(light& lightSource, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void endShadowPass();

    void bindShadowMap(light& lightSource, Shader* shader, int textureUnit = 0);

    void cleanUp();
    
    Shader* shadowDepthShader;
private:

    GLint previousFramebuffer = 0;
    glm::ivec4 previousViewport;

    void createShadowFramebuffer(light& lightSource);
    void createShadowDepthTexture(light& lightSource);
    glm::mat4 calculateLightViewMatrix(const light& lightSource);
    glm::mat4 calculateLightProjectionMatrix(const light& lightSource);
};
