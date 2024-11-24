#include <openGL/open_gl.hpp>
#include <engine.hpp>

int main()
{
    OpenGL openGL = OpenGL(FULLSCREEN, 4);//, 320, 200);

    Engine engine = Engine(&openGL);

    glfwSetInputMode(openGL.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(openGL.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    while (!openGL.shouldClose())
    {
        openGL.canvas->clearCanvas();
        engine.update();
        engine.render();

        if (glfwGetKey(openGL.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(openGL.window, true);
        }

        // // mouse
        // openGL.canvas->drawCircle(openGL.mouseX, openGL.mouseY, 3, ColorRGBA(0, 255, 255));
        // openGL.canvas->setPixel(openGL.mouseX, openGL.mouseY, ColorRGBA(255, 255, 255));

        openGL.update();
    }

    return 0;
}
