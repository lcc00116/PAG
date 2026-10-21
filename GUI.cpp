//
// Created by lucar on 23/09/2026.
//

#include "GUI.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <glm/glm.hpp>

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
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);   //Empieza plegada

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

        ImGui::SetNextWindowPos(ImVec2(10, 446), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(420, 120), ImGuiCond_Once);

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


        //---------- Ventana "Shader Program": nombre base de los shaders ----------

        ImGui::SetNextWindowPos(ImVec2(10, 50), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(280, 110), ImGuiCond_Once);

        if (ImGui::Begin("Shader Program")) {
            ImGui::SetWindowFontScale(1.5f);
            ImGui::InputText("##", &_nombreShader, ImGuiInputTextFlags_AutoSelectAll);

            if (ImGui::Button("Load")) {
                _cargaShaderPendiente = true;   //main.cpp se encargará de cargarlo
            }
        }
        ImGui::End();


        //---------- Ventana "Camera": movimiento de cámara ----------

        ImGui::SetNextWindowPos(ImVec2(714, 390), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 176), ImGuiCond_Once);

        if (ImGui::Begin("Camera")) {
            ImGui::SetWindowFontScale(1.5f);

            //Mismo orden que el enum MovimientoCamara
            const char* nombres[] = { "Pan", "Tilt", "Dolly", "Crane", "Orbit", "Zoom" };
            int actual = static_cast<int>(Renderer::getInstancia().getMovimiento());

            //Combo devuelve true solo en el frame en que el usuario cambia la opción
            if (ImGui::Combo("Movement", &actual, nombres, IM_ARRAYSIZE(nombres))) {
                Renderer::getInstancia().seleccionarMovimiento(static_cast<MovimientoCamara>(actual));
            }

            Renderer& r = Renderer::getInstancia();
            const float paso = 2.0f;   //Grados por clic
            const float lineal = 0.1f;   //Unidades de escena por clic

            switch (r.getMovimiento()) {
                case MovimientoCamara::Zoom: {
                    //Leemos el ángulo cada frame: así el slider sigue al zoom hecho con el ratón
                    float fov = r.getFovCamara();
                    if (ImGui::SliderFloat("Angle", &fov, 5.0f, 120.0f)) {
                        r.setFovCamara(fov);
                    }
                    break;
                }
                case MovimientoCamara::Pan:
                    ImGui::Text("Direction");
                    if (ImGui::Button("<- Left"))  r.moverCamara(MovimientoCamara::Pan, paso);
                    ImGui::SameLine();
                    if (ImGui::Button("Right ->")) r.moverCamara(MovimientoCamara::Pan, -paso);
                    break;

                case MovimientoCamara::Tilt:
                    ImGui::Text("Direction");
                    if (ImGui::Button("^ Up ^"))   r.moverCamara(MovimientoCamara::Tilt, paso);
                    ImGui::SameLine();
                    if (ImGui::Button("v Down v")) r.moverCamara(MovimientoCamara::Tilt, -paso);
                    break;

                case MovimientoCamara::Dolly:
                    ImGui::Text("Direction");
                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.3f);
                    if (ImGui::Button("^ Back ^"))  r.moverCamara(MovimientoCamara::Dolly, 0.0f, -lineal);
                    if (ImGui::Button("<- Left"))   r.moverCamara(MovimientoCamara::Dolly, -lineal, 0.0f);
                    ImGui::SameLine();
                    if (ImGui::Button("Right ->"))  r.moverCamara(MovimientoCamara::Dolly, lineal, 0.0f);
                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.3f);
                    if (ImGui::Button("v Front v")) r.moverCamara(MovimientoCamara::Dolly, 0.0f, lineal);
                    break;

                case MovimientoCamara::Crane:
                    ImGui::Text("Direction");
                    if (ImGui::Button("^ Up ^"))    r.moverCamara(MovimientoCamara::Crane, lineal);
                    ImGui::SameLine();
                    if (ImGui::Button("v Down v"))  r.moverCamara(MovimientoCamara::Crane, -lineal);
                    break;

                case MovimientoCamara::Orbit:
                    ImGui::Text("Latitude");
                    if (ImGui::Button("^ North ^")) r.moverCamara(MovimientoCamara::Orbit, paso, 0.0f);
                    ImGui::SameLine();
                    if (ImGui::Button("v South v")) r.moverCamara(MovimientoCamara::Orbit, -paso, 0.0f);
                    ImGui::Text("Longitude");
                    if (ImGui::Button("<- West"))   r.moverCamara(MovimientoCamara::Orbit, 0.0f, -paso);
                    ImGui::SameLine();
                    if (ImGui::Button("East ->"))   r.moverCamara(MovimientoCamara::Orbit, 0.0f, paso);
                    break;
            }
        }
        ImGui::End();


        //---------- Ventana "Models": añadir y eliminar modelos ----------

        ImGui::SetNextWindowPos(ImVec2(714, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(300, 160), ImGuiCond_Once);

        if (ImGui::Begin("Models")) {
            ImGui::SetWindowFontScale(1.5f);
            Renderer& r = Renderer::getInstancia();

            if (ImGui::Button("Add triangle")) {
                r.creaModelo();
            }

            ImGui::Separator();

            //Una línea por modelo; al hacer clic en ella, queda seleccionado
            for (std::size_t i = 0; i < r.numModelos(); ++i) {
                std::string etiqueta = "Model " + std::to_string(i + 1);
                if (ImGui::Selectable(etiqueta.c_str(), _modeloSeleccionado == static_cast<int>(i))) {
                    _modeloSeleccionado = static_cast<int>(i);
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Delete selected")) {
                //Solo borramos si hay un modelo seleccionado y existe
                if (_modeloSeleccionado >= 0 && _modeloSeleccionado < static_cast<int>(r.numModelos())) {
                    r.eliminarModelo(static_cast<std::size_t>(_modeloSeleccionado));
                    _modeloSeleccionado = -1;   //Ya no queda ninguno seleccionado
                }
            }
        }
        ImGui::End();


        //---------- Ventana "Transform": transformar el modelo seleccionado ----------

        ImGui::SetNextWindowPos(ImVec2(714, 180), ImGuiCond_Once);

        if (ImGui::Begin("Transform", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowFontScale(1.0f);
            Renderer& r = Renderer::getInstancia();
            const float desplaza = 0.1f;   //Unidades de escena por clic

            //Solo se puede transformar si hay un modelo seleccionado y existe
            if (_modeloSeleccionado < 0 || _modeloSeleccionado >= static_cast<int>(r.numModelos())) {
                ImGui::Text("Select a model first");
            } else {
                std::size_t i = static_cast<std::size_t>(_modeloSeleccionado);
                ImGui::Text("Model %d", _modeloSeleccionado + 1);

                ImGui::Text("Move");
                if (ImGui::Button("- X")) r.trasladarModelo(i, -desplaza, 0.0f, 0.0f);
                ImGui::SameLine();
                if (ImGui::Button("+ X")) r.trasladarModelo(i,  desplaza, 0.0f, 0.0f);
                ImGui::SameLine();
                if (ImGui::Button("- Y")) r.trasladarModelo(i, 0.0f, -desplaza, 0.0f);
                ImGui::SameLine();
                if (ImGui::Button("+ Y")) r.trasladarModelo(i, 0.0f,  desplaza, 0.0f);
                ImGui::SameLine();
                if (ImGui::Button("- Z")) r.trasladarModelo(i, 0.0f, 0.0f, -desplaza);
                ImGui::SameLine();
                if (ImGui::Button("+ Z")) r.trasladarModelo(i, 0.0f, 0.0f,  desplaza);

                const float gira = 10.0f;   //Grados por clic
                ImGui::Text("Rotate");
                if (ImGui::Button("- RX")) r.rotarModelo(i, -gira, glm::vec3(1.0f, 0.0f, 0.0f));
                ImGui::SameLine();
                if (ImGui::Button("+ RX")) r.rotarModelo(i,  gira, glm::vec3(1.0f, 0.0f, 0.0f));
                ImGui::SameLine();
                if (ImGui::Button("- RY")) r.rotarModelo(i, -gira, glm::vec3(0.0f, 1.0f, 0.0f));
                ImGui::SameLine();
                if (ImGui::Button("+ RY")) r.rotarModelo(i,  gira, glm::vec3(0.0f, 1.0f, 0.0f));
                ImGui::SameLine();
                if (ImGui::Button("- RZ")) r.rotarModelo(i, -gira, glm::vec3(0.0f, 0.0f, 1.0f));
                ImGui::SameLine();
                if (ImGui::Button("+ RZ")) r.rotarModelo(i,  gira, glm::vec3(0.0f, 0.0f, 1.0f));

                ImGui::Text("Scale");
                if (ImGui::Button("Smaller")) r.escalarModelo(i, 0.9f);
                ImGui::SameLine();
                if (ImGui::Button("Bigger"))  r.escalarModelo(i, 1.1f);
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

    bool GUI::capturaTeclado() const {
        return ImGui::GetIO().WantCaptureKeyboard;
    }

    bool GUI::capturaRaton() const {
        return ImGui::GetIO().WantCaptureMouse;
    }

    bool GUI::consumirPeticionShader(std::string& nombre) {
        if (!_cargaShaderPendiente) {
            return false;
        }
        _cargaShaderPendiente = false;
        nombre = _nombreShader;
        return true;
    }

}