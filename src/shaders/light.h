#pragma once
#include <glm/glm.hpp>
#include <string>

enum class LightType { POINT = 0, DIRECTIONAL = 1, SPOT = 2};

struct light
{
    std::string name = "Light";
    LightType type = LightType::POINT;
    glm::vec3 position = glm::vec3(10.0f, 25.0f, 15.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

    glm::vec4 diffuse = glm::vec4(1.0f, 0.9f, 0.8f, 0.0f);
    glm::vec4 specular = glm::vec4(0.9f, 0.8f, 0.7f, 1.0f);

    glm::vec3 attenuation = glm::vec3(1.0f, 0.1f, 0.01f);

    float spotCutoff = 0.95f;
    int spotExponent = 32;

    bool bEnabled = true;
};
