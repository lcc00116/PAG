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
        //Inicia un nuevo frame de Dear ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //---------- Ventana "Fondo": selector de color ----------

        //Posición y tamaño iniciales (solo la primera vez que se abre)
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(280, 380), ImGuiCond_Once);

        //Crea la ventana; si está contraída, Begin devuelve false y no dibujamos nada dentro
        if (ImGui::Begin("Fondo")) {
            ImGui::SetWindowFontScale(1.5f); //Escala el texto para pantallas de alta resolución

            //Dibuja la rueda de color; devuelve true solo en el frame en que cambia el valor
            if (ImGui::ColorPicker3("Color", _bgColor, ImGuiColorEditFlags_PickerHueWheel)) {
                //Si hubo cambio, se lo comunicamos al Renderer
                Renderer::getInstancia().cambiarColorFondo(_bgColor[0], _bgColor[1], _bgColor[2]);
            }
        }
        ImGui::End(); //Siempre hay que llamarlo

        //---------- Ventana "Mensajes": histórico de mensajes ----------

        ImGui::SetNextWindowPos(ImVec2(300, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);

        if (ImGui::Begin("Mensajes")) {
            ImGui::SetWindowFontScale(1.0f);

            //Pinta cada mensaje guardado, uno por línea
            for (const auto& msg : _mensajes) {
                ImGui::TextWrapped("- %s", msg.c_str());
            }

            //Si el usuario ya estaba viendo el final, seguimos bajando con cada mensaje nuevo
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::End();

        //Renderiza todos los controles dibujados en este frame y los manda a OpenGL
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

    void GUI::agregarMensaje(const std::string& mensaje) {
        _mensajes.push_back(mensaje);
    }

}