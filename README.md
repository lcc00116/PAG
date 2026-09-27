
# Práctica 2 - Lucía Cano Cubillo

## Explicación de los cambios realizados

### PAG::Renderer (patrón Singleton)

Toda la lógica que antes llamaba directamente a funciones de OpenGL desde
`main.cpp` (`glClear`, `glClearColor`, `glViewport`, `glEnable`,
`glGetString`) se ha movido a una clase `PAG::Renderer`, implementada
como Singleton (atributo estático privado, constructor privado, acceso
único mediante `getInstancia()` con inicialización perezosa).

Como resultado, `main.cpp` ya no contiene ninguna llamada directa a
funciones de OpenGL: solo llama a GLFW, GLAD, y a los métodos públicos de
`PAG::Renderer` y `PAG::GUI`.

### PAG::GUI (patrón Singleton)

Siguiendo la Figura 4 del guion, se ha creado una segunda clase
Singleton, `PAG::GUI`, que encapsula toda la comunicación con Dear ImGui.
Ni `main.cpp` ni `PAG::Renderer` incluyen ninguna cabecera de ImGui.
Cuando el usuario cambia el color en el selector, `GUI` delega en
`Renderer::cambiarColorFondo(...)` en vez de tocar OpenGL directamente.

### Controles de Dear ImGui

- **Ventana "Fondo"**: un `ColorPicker3` con rueda de tono para cambiar
  el color de fondo de la escena de forma interactiva.
- **Ventana "Mensajes"**: sustituye la consola como salida de mensajes
  de la aplicación (arranque, info de OpenGL, teclas, botones del ratón,
  scroll, errores de GLFW).

### Reenvío de eventos de ratón a Dear ImGui

Como GLFW solo permite un callback por tipo de evento y ventana, al
registrar `mouse_button_callback` propio se sobrescribía el callback que
Dear ImGui instala automáticamente. Se soluciona notificando el evento
manualmente a `PAG::GUI`, que reenvía el evento a Dear ImGui.

## Diagrama de clases

![Diagrama de clases](img/diagrama-clases.png)

`main.cpp` inicializa y usa tanto `PAG::Renderer` como `PAG::GUI` (ambos
Singleton). `PAG::GUI` depende de `PAG::Renderer` únicamente para
comunicarle el nuevo color de fondo; ninguna de las dos clases conoce la
biblioteca de la otra por dentro.

## Nota sobre dependencias

Dear ImGui no se incluye en el repositorio (ver `.gitignore`); es
necesario descargarlo desde https://github.com/ocornut/imgui.

