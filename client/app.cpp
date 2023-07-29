#include "app.h"

#include <GLFW/glfw3.h>

#include <cfloat>
#include <cstring>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void App::imguiStyle() {
  auto textColor = ImVec4(236.f / 255.f, 240.f / 255.f, 241.f / 255.f, 1.0f);
  auto bodyColor = ImVec4(44.f / 255.f, 62.f / 255.f, 80.f / 255.f, 1.0f);
  auto areaColor = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.Colors[ImGuiCol_Text] = textColor;
  style.Colors[ImGuiCol_Border] = bodyColor;
  style.Colors[ImGuiCol_FrameBg] = areaColor;
}

void App::start() {
  client->messages = {
      "Hola que pasa chaval un place estar por aquio",
      "lo mismo digo mola mogollon xdd",
      "sin duda este foro esta lleno de cracks",
      "guys remember this is an English room do not write in Spanish please"};

  if (!running) setup();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set dimensions
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(width, height));

    imguiStyle();

    ImGui::Begin("##", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    // Messages list
    ImGui::Text("Messages");
    ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
    if (ImGui::BeginListBox(
            "##", ImVec2(contentRegionAvail.x, contentRegionAvail.y - 30))) {
      for (const auto& msg : client->messages) {
        ImGui::Selectable(msg.c_str(), false);
      }
      ImGui::EndListBox();
    }

    // Input Text for sending messages
    static char buf1[64] = "";
    if (ImGui::InputText("Send", buf1, 64,
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
      std::cout << "Enter key pressed" << std::endl;
      client->SendMsg(buf1);
      strncpy(buf1, "", 64);
    }

    ImGui::End();

    // Rendering
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }
}

App::~App() { glfwDestroyWindow(window); }

void App::clean() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::stop() {}
static void glfwErrorCallback(int error, const char* description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void App::setup() {
  glfwSetErrorCallback(glfwErrorCallback);
  if (!glfwInit()) {
    std::cerr << "Unable to init glfw" << std::endl;
    running = false;
    return;
  }

  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(1280, 720, "Chat", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Unable to create glfw window" << std::endl;
    running = false;
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  running = true;
}
