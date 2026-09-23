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

        GUI();

    public:
        virtual ~GUI();

        static GUI& getInstancia();

        void inicializar(GLFWwindow* window);
        void refrescar();
        void liberar();
    };

}

#endif //PAG_PRAC1_GUI_H