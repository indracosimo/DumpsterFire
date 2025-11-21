#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "uiManager.h"
#include <GLFW/glfw3.h>
#include <string>

uiManager::uiManager() 
{
	return;
}

uiManager::~uiManager() 
{
	if (bInitialized) 
	{
		Shutdown(); 
	}
}

void uiManager::Initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.0f;
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bInitialized = true;
}

void uiManager::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    bInitialized = false;
}

void uiManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void uiManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#pragma region Cube Controls
void uiManager::RenderCubeControls(std::vector<CubeTransform>& cubes, int& selectedCubeIndex,
    CubeTransform& newCubeTransform)
{
    ImGui::Begin("Cube Controls");
    ImGui::Text("Total Cubes: %d", static_cast<int>(cubes.size()));
    ImGui::Separator();

    ImGui::Text("New Cube Transform:");
    ImGui::Text("          x                y                z     ");
    ImGui::DragFloat3("Position##new", &newCubeTransform.position.x, 0.1f, -10.0f, 10.0f);
    ImGui::DragFloat3("Rotation##new", &newCubeTransform.rotation.x, 1.0f, 0.0f, 360.0f);
    ImGui::DragFloat3("Scale##new", &newCubeTransform.scale.x, 0.1f, 0.1f, 10.0f);

    if (ImGui::Button("Spawn Cube"))
    {
        if (newCubeTransform.name.empty() || newCubeTransform.name == "Nameless Cube" || newCubeTransform.name.rfind("Nameless Cube", 0) == 0)
        {
            newCubeTransform.name = "Nameless Cube " + std::to_string(cubes.size());
        }

        cubes.push_back(newCubeTransform);
        selectedCubeIndex = static_cast<int>(cubes.size()) - 1;

        newCubeTransform.name = "Nameless Cube";
    }

    if (ImGui::Button("Reset Cube##new"))
    {
        newCubeTransform.position = { 0.0f, 0.0f, 0.0f };
        newCubeTransform.rotation = { 0.0f, 0.0f, 0.0f };
        newCubeTransform.scale = { 1.0f, 1.0f, 1.0f };
        newCubeTransform.name = "Nameless Cube";
    }


    ImGui::Separator();

    if (!cubes.empty())
    {
        ImGui::Text("Edit Existing Cubes:");

        if (ImGui::BeginCombo("Select Cube", selectedCubeIndex >= 0 ?
            cubes[selectedCubeIndex].name.c_str() : "None"))
        {
            for (int i = 0; i < cubes.size(); i++)
            {
                bool isSelected = (selectedCubeIndex == i);
                if (ImGui::Selectable(cubes[i].name.c_str(), isSelected))
                    selectedCubeIndex = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (selectedCubeIndex >= 0 && selectedCubeIndex < cubes.size())
        {
            CubeTransform& cube = cubes[selectedCubeIndex];
            std::vector<char> cubeNameBuffer(cube.name.begin(), cube.name.end());
            cubeNameBuffer.resize(256);

            ImGui::Text("Cube %d Transform:", selectedCubeIndex, cubeNameBuffer.data());
            ImGui::Text("       x                y                z        ");


            if (ImGui::InputText("Cube Name", cubeNameBuffer.data(), cubeNameBuffer.size()))
            {
                cube.name = std::string(cubeNameBuffer.data()); // Update std::string after editing
            }
            ImGui::DragFloat3("Position", &cube.position.x, 0.1f, -10.0f, 999.0f);
            ImGui::DragFloat3("Rotation", &cube.rotation.x, 1.0f, 0.0f, 360.0f);
            ImGui::DragFloat3("Scale", &cube.scale.x, 0.1f, 0.1f, 999.0f);

            if (ImGui::Button("Reset Cube"))
            {
                cube.position = { 0.0f, 0.0f, 0.0f };
                cube.rotation = { 0.0f, 0.0f, 0.0f };
                cube.scale = { 1.0f, 1.0f, 1.0f };
            }

            if (ImGui::Button("Delete Cube"))
            {
                cubes.erase(cubes.begin() + selectedCubeIndex);
                selectedCubeIndex = -1;

                for (int j = 0; j < (int)cubes.size(); ++j)
                {
                    if (cubes[j].name.rfind("Nameless Cube", 0) == 0)
                        cubes[j].name = "Nameless Cube " + std::to_string(j);
                }
            }
        }
    }

    if (ImGui::Button("Delete All Cubes"))
    {
        cubes.clear();
        selectedCubeIndex = -1;
    }

    ImGui::End();
}
#pragma endregion