#include <openGL/open_gl.h>
#include <engine.h>

int main()
{
    OpenGL openGL = OpenGL(WINDOWED, 3, 400, 250, "RayCasting");
    // OpenGL openGL = OpenGL(FULLSCREEN, 4);//, 320, 200);
    // glfwSetInputMode(openGL.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(openGL.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    Engine engine = Engine(&openGL);

    while (!openGL.shouldClose())
    {
        openGL.canvas->clearCanvas();
        engine.update();

        // engine.renderMap();
        engine.renderTransformedMap();
        // engine.renderView_wireframe_v1();
        engine.renderView_wireframe_v2();

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
