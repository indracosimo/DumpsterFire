#include "ShadowMapper.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

ShadowMapper::ShadowMapper() : shadowDepthShader(nullptr), previousFramebuffer(0)
{
    shadowDepthShader = new Shader("src/shaders/shadowDepth.vs", "src/shaders/shadowDepth.fs");
}

ShadowMapper::~ShadowMapper()
{
    cleanUp();
    if (shadowDepthShader){delete shadowDepthShader; shadowDepthShader = nullptr;}
}

void ShadowMapper::initializeShadowMap(light& lightSource)
{
    if (lightSource.shadowFrameBuffer == 0)
    {
        createShadowFramebuffer(lightSource);
        createShadowDepthTexture(lightSource);
    }
}

void ShadowMapper::createShadowFramebuffer(light& lightSource)
{
    glGenFramebuffers(1, &lightSource.shadowFrameBuffer);

    // glGenFramebuffers(1, &lightSource.shadowFrameBuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, lightSource.shadowFrameBuffer);
    //
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;}
    //
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapper::createShadowDepthTexture(light& lightSource)
{
    glGenTextures(1, &lightSource.shadowDepthTexture);
    glBindTexture(GL_TEXTURE_2D, lightSource.shadowDepthTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, lightSource.shadowMapResolution.x, lightSource.shadowMapResolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    //texture filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glBindFramebuffer(GL_FRAMEBUFFER, lightSource.shadowFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lightSource.shadowDepthTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 ShadowMapper::calculateLightViewMatrix(const light& lightSource)
{
    glm::vec3 lightPos = lightSource.shadowMapRenderPosition;
    glm::vec3 lightDir = glm::normalize(lightSource.direction);
    
    // calc look-at point
    glm::vec3 targetPos = lightPos + lightDir;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    if (glm::abs(glm::dot(up, lightDir)) > 0.99f)
    {
        up = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    
    return glm::lookAt(lightPos, targetPos, up);
}

glm::mat4 ShadowMapper::calculateLightProjectionMatrix(const light& lightSource)
{
    if (lightSource.type == LightType::DIRECTIONAL)
    {
        float d = lightSource.shadowMapZoom;
        return glm::ortho(-d, d, -d, d, 0.1f, 100.0f);
    }
    
    return glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}

void ShadowMapper::beginShadowPass(light& lightSource, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
if (lightSource.shadowFrameBuffer == 0 ||
        lightSource.shadowDepthTexture == 0)
        return;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);
    glGetIntegerv(GL_VIEWPORT, &previousViewport[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, lightSource.shadowFrameBuffer);
    glViewport(0, 0,
        lightSource.shadowMapResolution.x,
        lightSource.shadowMapResolution.y);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shadowDepthShader->use();

    glm::mat4 lightView = calculateLightViewMatrix(lightSource);
    glm::mat4 lightProj = calculateLightProjectionMatrix(lightSource);

    shadowDepthShader->setMat4("viewMatrix", lightView);
    shadowDepthShader->setMat4("projectionMatrix", lightProj);

    glm::mat4 biasMatrix(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    );

    lightSource.shadowMapMatrix = biasMatrix * lightProj * lightView;
}

void ShadowMapper::endShadowPass()
{
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
    glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
}

void ShadowMapper::bindShadowMap(light& lightSource, Shader* shader, int textureUnit)
{
    shader->setBool("bUseShadowMap", true);
    
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, lightSource.shadowDepthTexture);
    
    shader->use();
    
    shader->setInt("shadowMap", textureUnit);
    shader->setMat4("shadowMapMatrix", lightSource.shadowMapMatrix);
    shader->setVec2("shadowMapTexelSize",1.0f / lightSource.shadowMapResolution.x, 1.0f / lightSource.shadowMapResolution.y);
    shader->setFloat("shadowCalcBias", lightSource.shadowMapZoom * 0.0002f);

    glActiveTexture(GL_TEXTURE0);
}

void ShadowMapper::cleanUp()
{
}

