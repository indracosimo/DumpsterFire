#pragma once
#include "graphics/renderer.h"
#include "../MessageManager.h"
#include <vector>
#include "core/camera.h"
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

	void RenderCameraControls(camera& camera, float& FOV);

    bool bInitialized = false;

    //void init()
    //{
    //    if (!bInitialized)
    //    {
    //        bInitialized = true;
    //    }
    //}
};
