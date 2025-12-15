#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include "application.h"
#include "core/camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "input/input.h"
#include "graphics/objectLoader.h"
#include "ui/uiManager.h"
#include <iostream>
#include <vector>
#include "../MessageManager.h"
#include "../MessageQueue.h"
#include "../../MemoryChecker.h"
#include "../../MeshManager.h"

extern GLFWwindow* window;


application::application(const Resolution& res, const char* title)
    : SCR_WIDTH(res.width), SCR_HEIGHT(res.height), window(nullptr)
{
    initGLFW();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    initGLAD();
    setupCallbacks();
}

application::~application()
{
	glfwTerminate();
}

void application::initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void application::initGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    std::cout << "GL_VENDOR:   " << (const char*)glGetString(GL_VENDOR) << '\n'
        << "GL_RENDERER: " << (const char*)glGetString(GL_RENDERER) << '\n'
        << "GL_VERSION:  " << (const char*)glGetString(GL_VERSION) << std::endl;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

void application::setupCallbacks()
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void application::Run()
{
    MeshManager::Allocate();
    camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0, 0, 2));
    renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    uiManager uiManager;
	float FOV = 45.0f;

    uiManager.Initialize(window);

    std::vector<CubeTransform> cubes;
    CubeTransform newCube;
    int selectedCubeIndex = -1;

    float lastFrame = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
        lastFrame = currentFrame;

        processInput(window);
        camera.Inputs(window, deltaTime);

    #pragma region Memory Status Check
        //check memory status
        MemoryChecker status = queryMemoryStatus();
        const uint64_t safetyMarginBytes = 1024 * 1024 * 1024; // 1 GB
        //Physical Memory Check
        //if (status.availablePhysicalMemoryBytes < safetyMarginBytes)
        //{
        //    std::cerr << "Warning: Low physical memory available!" << std::endl;
        //    std::cerr << "Total Physical Memory: " << status.totalPhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //    std::cerr << "Available Physical Memory: " << status.availablePhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //}
        //else
        //{
        //    std::cout << "Memory Status: " << std::endl;
        //    std::cout << "Total Physical Memory: " << status.totalPhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //    std::cout << "Available Physical Memory: " << status.availablePhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //}
        ////Virtual Memory Check
        //if (status.availableVirtualMemoryBytes < safetyMarginBytes)
        //{
        //    std::cerr << "Warning: Low virtual memory available!" << std::endl;
        //    std::cerr << "Total Virtual Memory: " << status.totalVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //    std::cerr << "Available Virtual Memory: " << status.availableVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //}
        //else
        //{
        //    std::cout << "Memory Status: " << std::endl;
        //    std::cout << "Total Virtual Memory: " << status.totalVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //    std::cout << "Available Virtual Memory: " << status.availableVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        //}
#pragma endregion



        uiManager.BeginFrame();

	//process messages
		g_messageQueue.processMessage();

        //uiManager.RenderDocking();

        uiManager.RenderCubeControls(cubes, selectedCubeIndex, newCube);
		uiManager.RenderCameraControls(camera, FOV);
        {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
            ImGui::Text("Frame: %.2f ms", deltaTime * 1000.0f);
            ImGui::End();
        }

        renderer.mainShader->use();
        camera.Matrix(FOV, 0.1f, 100.0f, *renderer.mainShader, "camMatrix");

        //planeMesh.DrawMesh();

        renderer.render(cubes);
        uiManager.EndFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    uiManager.Shutdown();
}