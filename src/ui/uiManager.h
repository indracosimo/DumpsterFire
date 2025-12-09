#pragma once
#include "graphics/renderer.h"
#include "../MessageManager.h"
#include <vector>

struct ImGuiIO;
struct GLFWwindow;



class uiManager
{
public:
    uiManager();
    ~uiManager();

    void Initialize(GLFWwindow* window);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void RenderCubeControls(std::vector<CubeTransform>& cubes, int& selectedCubeIndex, CubeTransform& newCubeTransform);
    void RenderDocking();

    bool bInitialized;

    void init()
    {
        if (!bInitialized)
        {
            bInitialized = true;
        }
    }
};
