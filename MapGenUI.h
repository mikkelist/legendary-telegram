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
	bool show_ca_window = true;
	bool my_pixel = true; // remove this later
	
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
			ImGui::Text(build_str);
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
			ImGui::Checkbox("ImGui Demo Window", &show_demo_window);       
			ImGui::Checkbox("Simulation  Window", &show_ca_window);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			std::string text = "Cell (2,3) is ";
			text += std::to_string(my_pixel);
			ImGui::Checkbox(text.c_str(), &my_pixel);
			if (my_pixel) gridWindowTex->SetTexelColor(2, 3, GT_RED); 
			else gridWindowTex->SetTexelColor(2, 3, GT_GREEN);
		}

		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_FirstUseEver);
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		if (show_ca_window)
		{
			ImGui::SetNextWindowPos(ImVec2(600, 20), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Generation Window", &show_ca_window,
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
			{ 
				ImGui::Image(reinterpret_cast<void*>(gridWindowTex->Update()), ImVec2(800.f, 800.f));
				ImGui::End();
			}
		}
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

