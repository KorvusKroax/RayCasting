#pragma once

#include <openGL/open_gl.h>
#include <world.h>
#include <viewpoint.h>

const double pi = 3.14159265358979323846; //3.141592653589793238462643383279502884197169399375105820974944;
const double mouseSensivity = 0.0025;

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
            viewpoint = Viewpoint();

            glfwGetCursorPos(openGL->window, &mX, &mY);
            lastMX = mX;
            lastMY = mY;
        }

        void update()
        {
            float dx = sin(viewpoint.heading) * viewpoint.speed * openGL->deltaTime;
            float dy = cos(viewpoint.heading) * viewpoint.speed * openGL->deltaTime;

            if (glfwGetKey(openGL->window, GLFW_KEY_W) == GLFW_PRESS) {
                viewpoint.pos.x += dx;
                viewpoint.pos.y += dy;
            }
            if (glfwGetKey(openGL->window, GLFW_KEY_S) == GLFW_PRESS) {
                viewpoint.pos.x -= dx;
                viewpoint.pos.y -= dy;
            }
            if (glfwGetKey(openGL->window, GLFW_KEY_A) == GLFW_PRESS) {
                viewpoint.pos.x -= dy;
                viewpoint.pos.y += dx;
            }
            if (glfwGetKey(openGL->window, GLFW_KEY_D) == GLFW_PRESS) {
                viewpoint.pos.x += dy;
                viewpoint.pos.y -= dx;
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

            float fov = 80 / 180.0f * pi;
            float fov_x1 = sin(viewpoint.heading - fov * .5f) * 100;
            float fov_y1 = cos(viewpoint.heading - fov * .5f) * 100;
            float fov_x2 = sin(viewpoint.heading + fov * .5f) * 100;
            float fov_y2 = cos(viewpoint.heading + fov * .5f) * 100;
            openGL->canvas->drawLine(x, y, x + fov_x1, y + fov_y1, ColorRGBA(64, 0, 0));
            openGL->canvas->drawLine(x, y, x + fov_x2, y + fov_y2, ColorRGBA(64, 0, 0));
        }

        void renderTransformedMap()
        {
            for (Wall wall : world.walls) {
                Point start = world.points[wall.start]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading)
                    .add(Point(openGL->canvas->width >> 1, openGL->canvas->height >> 1));

                Point end = world.points[wall.end]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading)
                    .add(Point(openGL->canvas->width >> 1, openGL->canvas->height >> 1));

                openGL->canvas->drawLine(start.x, start.y, end.x, end.y, ColorRGBA(0, 128, 0));
            }

            float x = openGL->canvas->width >> 1;
            float y = openGL->canvas->height >> 1;

            openGL->canvas->drawFilledCircle(x, y, 2, ColorRGBA(0, 128, 0));
            openGL->canvas->drawLine(x, y, x, y + 10, ColorRGBA(0, 128, 0));

            float fov = 80 / 180.0f * pi;
            float fov_x1 = sin(-fov * .5f) * 100;
            float fov_y1 = cos(-fov * .5f) * 100;
            float fov_x2 = sin(fov * .5f) * 100;
            float fov_y2 = cos(fov * .5f) * 100;
            openGL->canvas->drawLine(x, y, x + fov_x1, y + fov_y1, ColorRGBA(0, 64, 0));
            openGL->canvas->drawLine(x, y, x + fov_x2, y + fov_y2, ColorRGBA(0, 64, 0));
        }

/*
        bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
        {
            float s1_x = p1_x - p0_x;
            float s1_y = p1_y - p0_y;
            float s2_x = p3_x - p2_x;
            float s2_y = p3_y - p2_y;

            float den = (-s2_x * s1_y + s1_x * s2_y)
            float s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / den;
            float t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / den;

            if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
            {
                *i_x = p0_x + (t * s1_x);
                *i_y = p0_y + (t * s1_y);
                return true;
            }
            return false;
        }
*/

        char lineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float *px = 0, float *py = 0)
        {
            float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
            float u = -(((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den);

            *px = x1 + t * (x2 - x1);
            *py = y1 + t * (y2 - y1);

            if (0 <= t && t <= 1 && 0 <= u && u <= 1) {
                return 3; // crossing on both line
            }
            if (0 <= u && u <= 1) {
                return 2; // crossing on second line only
            }
            if (0 <= t && t <= 1) {
                return 1; // crossing on first line only
            }
            return 0; // no crossing
        }

        bool rayIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float *px = 0, float *py = 0)
        {
            float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;

            if (0 <= t && t <= 1) {
                float u = -(((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den);
                if (0 <= u) {
                    *px = x1 + t * (x2 - x1);
                    *py = y1 + t * (y2 - y1);
                    return true; // ray hit the line
                }
            }
            return false; // no crossing
        }

        void clipBehind(float *x1, float *y1, float x2, float y2)
        {
            float d = *y1 - y2; if (d == 0) d = 1;
            float s = *y1 / (*y1 - y2);

            *x1 = *x1 + s * (x2 - (*x1));
            *y1 = *y1 + s * (y2 - (*y1)); if (*y1 == 0) *y1 = 1;
        }

        void renderView_wireframe_v1()
        {
            float fov = 80 / 180.0f * pi;
            float zFar = 1000.0f;
            float zNear = 10.0f;

            float aspectRatio = (float)openGL->canvas->height / (float)openGL->canvas->width;
            float f = 1.0f / tan(fov / 2);
            float q = zFar / (zFar - zNear);

            float fov_x1 = sin(-fov * .5f) * 100;
            float fov_y1 = cos(-fov * .5f) * 100;
            float fov_x2 = sin(fov * .5f) * 100;
            float fov_y2 = cos(fov * .5f) * 100;

            for (Wall wall : world.walls) {

                // transform wall
                Point start = world.points[wall.start]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading);
                Point end = world.points[wall.end]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading);

                // wall's both points is left from leftClip
                if (openGL->canvas->cross(0, 0, fov_x1, fov_y1, start.x, start.y) > 0 &&
                    openGL->canvas->cross(0, 0, fov_x1, fov_y1, end.x, end.y) > 0) {
                        return;
                }
                // wall's both points is right from rightClip
                if (openGL->canvas->cross(0, 0, fov_x2, fov_y2, start.x, start.y) < 0 &&
                    openGL->canvas->cross(0, 0, fov_x2, fov_y2, end.x, end.y) < 0) {
                        return;
                }

                // cut wall to view
                float x, y;
                if (rayIntersection(start.x, start.y, end.x, end.y, 0, 0, fov_x1, fov_y1, &x, &y)) {
                    start.x = x;
                    start.y = y;
                }
                if (rayIntersection(start.x, start.y, end.x, end.y, 0, 0, fov_x2, fov_y2, &x, &y)) {
                    end.x = x;
                    end.y = y;
                }
                if (start.y <= 0 && end.y <= 0) {
                    return;
                }

                // calculate wall vertices
                float x1 = aspectRatio * f * start.x;
                float y1 = f * (wall.top - viewpoint.height);
                float z1 = start.y * q - zNear * q;

                float x2 = aspectRatio * f * end.x;
                float y2 = f * (wall.top - viewpoint.height);
                float z2 = end.y * q - zNear * q;

                float x3 = aspectRatio * f * end.x;
                float y3 = f * (wall.bottom - viewpoint.height);
                float z3 = end.y * q - zNear * q;

                float x4 = aspectRatio * f * start.x;
                float y4 = f * (wall.bottom - viewpoint.height);
                float z4 = start.y * q - zNear * q;

                // calculate vertices' screen coords
                float px1 = (x1 / z1 + 1) * (openGL->canvas->width >> 1);
                float py1 = (y1 / z1 + 1) * (openGL->canvas->height >> 1);

                float px2 = (x2 / z2 + 1) * (openGL->canvas->width >> 1);
                float py2 = (y2 / z2 + 1) * (openGL->canvas->height >> 1);

                float px3 = (x3 / z3 + 1) * (openGL->canvas->width >> 1);
                float py3 = (y3 / z3 + 1) * (openGL->canvas->height >> 1);

                float px4 = (x4 / z4 + 1) * (openGL->canvas->width >> 1);
                float py4 = (y4 / z4 + 1) * (openGL->canvas->height >> 1);

                // draw wall
                openGL->canvas->drawLine(px1, py1, px2, py2, ColorRGBA(128, 128, 0));
                openGL->canvas->drawLine(px2, py2, px3, py3, ColorRGBA(128, 128, 0));
                openGL->canvas->drawLine(px3, py3, px4, py4, ColorRGBA(128, 128, 0));
                openGL->canvas->drawLine(px4, py4, px1, py1, ColorRGBA(128, 128, 0));
            }
        }

        float px_min = std::numeric_limits<float>::max();
        float py_min = std::numeric_limits<float>::max();
        float px_max = std::numeric_limits<float>::min();
        float py_max = std::numeric_limits<float>::min();
        void renderView_wireframe_v2()
        {
            float fov = 80 / 180.0f * pi;
            float nearClip = (openGL->canvas->width >> 1) / tan(fov * .5f); // (320 / 2) / tan(fov / 2) = 277;

            float fov_x1 = sin(-fov * .5f) * 100;
            float fov_y1 = cos(-fov * .5f) * 100;
            float fov_x2 = sin(fov * .5f) * 100;
            float fov_y2 = cos(fov * .5f) * 100;

            for (Wall wall : world.walls) {

                // transform walls
                Point start = world.points[wall.start]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading);
                Point end = world.points[wall.end]
                    .sub(viewpoint.pos)
                    .rotate(viewpoint.heading);

                if (start.y < 1 && end.y < 1) return;
                // if (start.y < 1) clipBehind(&start.x, &start.y, end.x, end.y);
                // if (end.y < 1) clipBehind(&end.x, &end.y, start.x, start.y);

                // cut wall to view
                float x, y;
                if (rayIntersection(start.x, start.y, end.x, end.y, 0, 0, fov_x1, fov_y1, &x, &y)) {
                    start.x = x;
                    start.y = y;
                }
                if (rayIntersection(start.x, start.y, end.x, end.y, 0, 0, fov_x2, fov_y2, &x, &y)) {
                    end.x = x;
                    end.y = y;
                }

                // calculate wall vertices
                float x1 = start.x;
                float y1 = wall.top - viewpoint.height;
                float z1 = start.y;

                float x2 = end.x;
                float y2 = wall.top - viewpoint.height;
                float z2 = end.y;

                float x3 = end.x;
                float y3 = wall.bottom - viewpoint.height;
                float z3 = end.y;

                float x4 = start.x;
                float y4 = wall.bottom - viewpoint.height;
                float z4 = start.y;

                // calculate vertices' screen coords
                float px1 = (x1 / z1) * nearClip + (openGL->canvas->width >> 1);
                float py1 = (y1 / z1) * nearClip + (openGL->canvas->height >> 1);

                float px2 = (x2 / z2) * nearClip + (openGL->canvas->width >> 1);
                float py2 = (y2 / z2) * nearClip + (openGL->canvas->height >> 1);

                float px3 = (x3 / z3) * nearClip + (openGL->canvas->width >> 1);
                float py3 = (y3 / z3) * nearClip + (openGL->canvas->height >> 1);

                float px4 = (x4 / z4) * nearClip + (openGL->canvas->width >> 1);
                float py4 = (y4 / z4) * nearClip + (openGL->canvas->height >> 1);

                // draw wall
                openGL->canvas->drawLine(px1, py1, px2, py2, ColorRGBA(0, 128, 128));
                openGL->canvas->drawLine(px2, py2, px3, py3, ColorRGBA(0, 128, 128));
                openGL->canvas->drawLine(px3, py3, px4, py4, ColorRGBA(0, 128, 128));
                openGL->canvas->drawLine(px4, py4, px1, py1, ColorRGBA(0, 128, 128));



                px_min = px1 < px_min ? px1 : px_min;
                py_min = py1 < py_min ? py1 : py_min;
                px_max = px1 > px_max ? px1 : px_max;
                py_max = py1 > py_max ? py1 : py_max;
                std::cout << "px_min: " << px_min << std::endl;
                std::cout << "py_min: " << py_min << std::endl;
                std::cout << "px_max: " << px_max << std::endl;
                std::cout << "py_max: " << py_max << std::endl;
                std::cout << std::endl;
            }
        }
};
