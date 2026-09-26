//
// Created by lucar on 23/09/2026.
//

#ifndef PAG_PRAC1_GUI_H
#define PAG_PRAC1_GUI_H

struct GLFWwindow;

namespace PAG {

    class GUI {
    private:
        static GUI* instancia;

        float _bgColor[3];

        GUI();

    public:
        virtual ~GUI();

        static GUI& getInstancia();

        void inicializar(GLFWwindow* window);
        void refrescar();
        void liberar();
        void notificarBotonRaton(int boton, bool pulsado);
    };

}

#endif //PAG_PRAC1_GUI_H