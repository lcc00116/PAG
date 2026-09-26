//
// Created by lucar on 23/09/2026.
//

#include "GUI.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace PAG {

    GUI* GUI::instancia = nullptr;

    GUI::GUI() : _bgColor{0.6f, 0.6f, 0.6f} {
    }

    GUI::~GUI() {
    }

    GUI& GUI::getInstancia() {
        if (!instancia) {
            instancia = new GUI();
        }
        return *instancia;
    }

    void GUI::inicializar(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
    }

    void GUI::refrescar() {
        // Inicia un nuevo frame de Dear ImGui (obligatorio antes de dibujar nada)
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Fija la posición inicial de la ventana flotante (solo la primera vez)
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(280, 380), ImGuiCond_Once);

        // Crea la ventana "Fondo"; si está contraída, Begin devuelve false
        // y no se dibuja nada dentro
        if (ImGui::Begin("Fondo")) {
            ImGui::SetWindowFontScale(1.5f); // Escala el texto para pantallas de alta resolución

            // Dibuja el selector de color; devuelve true solo en el frame
            // en que el usuario cambia el valor
            if (ImGui::ColorPicker3("Color", _bgColor, ImGuiColorEditFlags_PickerHueWheel)) {
                Renderer::getInstancia().cambiarColorFondo(_bgColor[0], _bgColor[1], _bgColor[2]);
            }
        }
        ImGui::End(); // Cierra la ventana (siempre hay que llamarlo, incluso si Begin devolvió false)

        // Renderiza todos los controles dibujados en este frame y los manda a OpenGL
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GUI::liberar() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GUI::notificarBotonRaton(int boton, bool pulsado) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(boton, pulsado);
    }

}