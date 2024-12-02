#pragma once

#include <openGL/open_gl.hpp>
#include <world.hpp>
#include <viewpoint.hpp>

const double pi = 3.14159265358979323846; //3.141592653589793238462643383279502884197169399375105820974944;
const double mouseSensivity = 0.0025;
const float fov = (pi / 180.0f) * 80.0f;
const float zFar = 1000.0f;
const float zNear = 10.0f;

class Engine
{
    private:
        OpenGL *openGL;
        World world;
        Viewpoint viewpoint;
        double mX, mY, lastMX, lastMY;
        float aspectRatio, f, q;

    public:
        Engine(OpenGL *openGL)
        {
            this->openGL = openGL;
            world = World();
            viewpoint = Viewpoint(0, 0, 20, 0.0f, 50.0f);

            aspectRatio = (float)openGL->canvas->height / (float)openGL->canvas->width;
            f = 1.0f / tan(fov / 2);
            q = zFar / (zFar - zNear);

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

        void renderMap()
        {
            for (Wall wall : world.walls) {
                Point start = world.points[wall.start]
                    .add(Point(openGL->canvas->width >> 1, openGL->canvas->height >> 1));

                Point end = world.points[wall.end]
                    .add(Point(openGL->canvas->width >> 1, openGL->canvas->height >> 1));

                openGL->canvas->drawLine(start.x, start.y, end.x, end.y, ColorRGBA(128, 0, 0));
            }

            float x = viewpoint.pos.x + (openGL->canvas->width >> 1);
            float y = viewpoint.pos.y + (openGL->canvas->height >> 1);
            float dirX = sin(viewpoint.heading) * 10;
            float dirY = cos(viewpoint.heading) * 10;

            openGL->canvas->drawFilledCircle(x, y, 2, ColorRGBA(255, 0, 0));
            openGL->canvas->drawLine(x, y, x + dirX, y + dirY, ColorRGBA(255, 0, 0));
        }

        void renderTransformedMap()
        {
            for (Wall wall : world.walls) {
                Point start = world.points[wall.start]
                    .rotateAround(viewpoint.pos, viewpoint.heading)
                    .add(Point(openGL->canvas->width >> 1, openGL->canvas->height >> 1))
                    .sub(viewpoint.pos);

                Point end = world.points[wall.end]
                    .rotateAround(viewpoint.pos, viewpoint.heading)
                    .add(Point(openGL->canvas->width >> 1, openGL->canvas->height >> 1))
                    .sub(viewpoint.pos);

                openGL->canvas->drawLine(start.x, start.y, end.x, end.y, ColorRGBA(0, 128, 0));
            }

            float x = openGL->canvas->width >> 1;
            float y = openGL->canvas->height >> 1;

            openGL->canvas->drawFilledCircle(x, y, 2, ColorRGBA(0, 128, 0));
            openGL->canvas->drawLine(x, y, x, y + 10, ColorRGBA(0, 128, 0));
        }

        void render3DView()
        {
            for (Wall wall : world.walls) {

                Point start = world.points[wall.start]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading);

                Point end = world.points[wall.end]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading);

                float height = wall.height;

                float x1 = aspectRatio * this->f * start.x;
                float y1 = this->f * (height - viewpoint.height);
                float z1 = start.y * this->q - zNear * this->q;
                float sx1 = (x1 / z1 + 1) * (openGL->canvas->width >> 1);
                float sy1 = (y1 / z1 + 1) * (openGL->canvas->height >> 1);

                float x2 = aspectRatio * this->f * end.x;
                float y2 = this->f * (height - viewpoint.height);
                float z2 = end.y * this->q - zNear * this->q;
                float sx2 = (x2 / z2 + 1) * (openGL->canvas->width >> 1);
                float sy2 = (y2 / z2 + 1) * (openGL->canvas->height >> 1);

                float x3 = aspectRatio * this->f * end.x;
                float y3 = this->f * -viewpoint.height;
                float z3 = end.y * this->q - zNear * this->q;
                float sx3 = (x3 / z3 + 1) * (openGL->canvas->width >> 1);
                float sy3 = (y3 / z3 + 1) * (openGL->canvas->height >> 1);

                float x4 = aspectRatio * this->f * start.x;
                float y4 = this->f * -viewpoint.height;
                float z4 = start.y * this->q - zNear * this->q;
                float sx4 = (x4 / z4 + 1) * (openGL->canvas->width >> 1);
                float sy4 = (y4 / z4 + 1) * (openGL->canvas->height >> 1);

                openGL->canvas->drawLine(sx1, sy1, sx2, sy2, ColorRGBA(128, 128, 128));
                openGL->canvas->drawLine(sx2, sy2, sx3, sy3, ColorRGBA(128, 128, 128));
                openGL->canvas->drawLine(sx3, sy3, sx4, sy4, ColorRGBA(128, 128, 128));
                openGL->canvas->drawLine(sx4, sy4, sx1, sy1, ColorRGBA(128, 128, 128));
            }
        }
};
