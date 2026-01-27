#include "LightManager.h"

LightManager* LightManager::instance = nullptr;

LightManager& LightManager::GetInstance()
{
    if (!instance) {instance = new LightManager();}
    return *instance;
}

LightManager::LightManager()
{
    light defaultLight;
    defaultLight.name = "Default Light";
    Lights.push_back(defaultLight);
}

light* LightManager::AddLight(light& Light)
{
    Lights.push_back(Light);
    return &Lights.back();
}

void LightManager::RemoveLight(int index)
{
    if (index >= 0 && index < Lights.size()) {Lights.erase(Lights.begin() + index);}
}

light* LightManager::GetLight(int index)
{
    if (index >= 0 && index < Lights.size()) {return &Lights[index];}
    return nullptr;
}

int LightManager::GetLightCount() const
{
    return Lights.size();
}

const std::vector<light>& LightManager::GetAllLights() const
{
    return Lights;
}


