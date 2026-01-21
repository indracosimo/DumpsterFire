#pragma once
#include "graphics/renderer.h"
#include "../MessageManager.h"
#include <vector>
#include "core/camera.h"
#include <memory>

struct ImGuiIO;
struct GLFWwindow;

class Viewport;

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

    //docking
    void RenderDockingLayout();
    void AddViewport(const glm::vec3& cameraPos);
    void RenderViewports(const std::vector<CubeTransform>& cubes, float FOV, GLFWwindow* window);
    
    std::vector<std::shared_ptr<Viewport>>& GetViewports() { return mViewports; }

private:
    std::vector<std::shared_ptr<Viewport>> mViewports;
    bool bInitialized = false;

    //void init()
    //{
    //    if (!bInitialized)
    //    {
    //        bInitialized = true;
    //    }
    //}
};
