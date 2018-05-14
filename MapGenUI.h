#pragma once
#include <string>
#include <GLFW\glfw3.h>

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"  

#include "GridTexture.h"
#include "CellGrid.h"



const char *build_str = "Build date: " __DATE__ " " __TIME__;

class MapGenUI
{
private:
  GLFWwindow* chosenWindow;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  GridTexture *gridWindowTex;

  bool show_demo_window = false;
  bool show_control_window = true;
  bool show_ca_texture_window = true;
  bool my_pixel = true; // remove this later

  void ConstructMainMenu()
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("Map File"))
      {
        if (ImGui::MenuItem("New map...", "CTRL+N")) {}
        if (ImGui::MenuItem("Quit"), "ALT+F4"){} // no implementation needed on windows
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Editing"))
      {
        if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}  // Disabled item
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item 
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View"))
      {
        if (ImGui::MenuItem("Window: Parameters Control", NULL, &show_control_window, &show_control_window)) {}
        if (ImGui::MenuItem("Window: Generated Map Tile", NULL, &show_ca_texture_window, &show_ca_texture_window)) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Window: ImGui Demo", NULL, &show_demo_window, &show_demo_window)) {}
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    } 
  }

  void ConstructControlWindow(int x, int y)
  {
    if (show_control_window)
    {
      ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
      if (ImGui::Begin("Generation Parameters Control", &show_control_window))
      {
        ImGui::Text(build_str);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Separator();

        
        std::string text = "Cell (2,3) is ";
        text += std::to_string(my_pixel);
        ImGui::Checkbox(text.c_str(), &my_pixel);
        if (my_pixel) gridWindowTex->SetTexelColor(2, 3, GT_RED);
        else gridWindowTex->SetTexelColor(2, 3, GT_GREEN);


        ImGui::End();
      }
    }
  }

  void ConstructTilePresentationWindow(int x, int y)
  {
    if (show_ca_texture_window)
    {
      ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
      if (ImGui::Begin("Generated Map Tile", &show_ca_texture_window,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
      {
        ImGui::Image(reinterpret_cast<void*>(gridWindowTex->Update()), ImVec2(600.f, 600.f));
        ImGui::Separator();
        static int step = 5;
        ImGui::SliderInt("Step", &step, 1, 10);
        ImGui::End();
      }
    }
  }

  void ConstructImguiDemoWindow(int x, int y)
  {
    if (show_demo_window)
    {
      ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
      ImGui::ShowDemoWindow(&show_demo_window);
    }
  }

public:
  MapGenUI(GLFWwindow* window)
  {
    chosenWindow = window;
    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(chosenWindow, true);
    ImGui::StyleColorsDark();
    gridWindowTex = new GridTexture(25, 25);
  }

  ~MapGenUI()
  {
    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
  }

  void RebindTexture()
  {


    CellGrid* a = new CellGrid(4, 4);
    CellGrid* b = a->NextStep();
  }

  void Update() {
    ImGui_ImplGlfwGL3_NewFrame();
    {
      ImGui::ColorEdit3("clear color", (float*)&clear_color);
    }
    // System Window top: 
    ConstructMainMenu();
    // System Window contents:
    ConstructControlWindow(10, 10); 
    ConstructTilePresentationWindow(300, 50);
    // Imgui demo for reference
    ConstructImguiDemoWindow(10, 150);
  }

  void Render() {
    // Rendering
    int display_w, display_h;
    glfwGetFramebufferSize(chosenWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    // Render gui
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
  }
};

