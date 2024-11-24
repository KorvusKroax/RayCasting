#pragma once

#include <openGL/open_gl.hpp>
#include <world.hpp>
#include <viewpoint.hpp>

const double pi = 3.14159265358979323846; //3.141592653589793238462643383279502884197169399375105820974944;
const double mouseSensivity = .0025;

class Engine
{
    private:
        OpenGL *openGL;
        World world;
        Viewpoint viewpoint;
        double mX, mY, lastMX, lastMY;

    public:
        Engine(OpenGL *openGL)
        {
            this->openGL = openGL;
            world = World();
            viewpoint = Viewpoint(0.0f, 0.0f, 40.0f, 0.0f, 50.0f);

            glfwGetCursorPos(openGL->window, &mX, &mY);
            lastMX = mX;
            lastMY = mY;
        }

        void update()
        {
            if (glfwGetKey(openGL->window, GLFW_KEY_W) == GLFW_PRESS) {
                viewpoint.pos.x += sin(viewpoint.heading) * viewpoint.speed * openGL->deltaTime;
                viewpoint.pos.y += cos(viewpoint.heading) * viewpoint.speed * openGL->deltaTime;
            }
            if (glfwGetKey(openGL->window, GLFW_KEY_S) == GLFW_PRESS) {
                viewpoint.pos.x -= sin(viewpoint.heading) * viewpoint.speed * openGL->deltaTime;
                viewpoint.pos.y -= cos(viewpoint.heading) * viewpoint.speed * openGL->deltaTime;
            }
            if (glfwGetKey(openGL->window, GLFW_KEY_A) == GLFW_PRESS) {
                viewpoint.pos.x -= sin(viewpoint.heading + pi * 0.5f) * viewpoint.speed * openGL->deltaTime;
                viewpoint.pos.y -= cos(viewpoint.heading + pi * 0.5f) * viewpoint.speed * openGL->deltaTime;
            }
            if (glfwGetKey(openGL->window, GLFW_KEY_D) == GLFW_PRESS) {
                viewpoint.pos.x += sin(viewpoint.heading + pi * 0.5f) * viewpoint.speed * openGL->deltaTime;
                viewpoint.pos.y += cos(viewpoint.heading + pi * 0.5f) * viewpoint.speed * openGL->deltaTime;
            }

            glfwGetCursorPos(openGL->window, &mX, &mY);
            viewpoint.heading += (mX - lastMX) * mouseSensivity;

            lastMX = mX;
            lastMY = mY;
        }

        void render()
        {
            renderWalls();
            renderRotatedViewpoint();
        }

        void renderWalls()
        {
            for (Wall wall : world.walls) {
                Point start = world.points[wall.start];
                Point end = world.points[wall.end];

                start.x += (openGL->canvas->width >> 1);
                start.y += (openGL->canvas->height >> 1);
                end.x += (openGL->canvas->width >> 1);
                end.y += (openGL->canvas->height >> 1);

                openGL->canvas->drawLine(start.x, start.y, end.x, end.y, ColorRGBA(128, 128, 128));
            }
        }

        void renderRotatedViewpoint()
        {
            float x = viewpoint.pos.x;
            float y = viewpoint.pos.y;
            float dirX = sin(viewpoint.heading) * 10;
            float dirY = cos(viewpoint.heading) * 10;
            x += openGL->canvas->width >> 1;
            y += openGL->canvas->height >> 1;

            openGL->canvas->drawFilledCircle(x, y, 2, ColorRGBA(255, 0, 0));
            openGL->canvas->drawLine(x, y, x + dirX, y + dirY, ColorRGBA(255, 128, 128));
        }
};
