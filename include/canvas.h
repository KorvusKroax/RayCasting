#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <iostream>
#include <color_rgba.h>

class Canvas
{
    public:
        unsigned int width;
        unsigned int height;
        int *pixels;

        Canvas() { }

        Canvas(unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            this->pixels = new int[this->width * this->height];
        }

        ~Canvas() { delete[] pixels; }

        void clearCanvas()
        {
            memset(pixels, 0, width * height * sizeof(int));
        }

        void fillCanvas(ColorRGBA color)
        {
            for (int i = 0; i < width * height; i++) {
                pixels[i] = color.value;
            }
        }

        void setPixel(int x, int y, ColorRGBA color)
        {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                if (color.getAlpha() == 255) {
                    pixels[x + y * width] = color.value;
                } else {
                    if (color.getAlpha() == 0) {
                        return;
                    }

                    ColorRGBA pixel = ColorRGBA(pixels[x + y * width]);

                    float weight = color.getAlpha() / 255.0f;

                    int r = int(pixel.getRed()   * (1.0f - weight) + color.getRed()   * weight);
                    int g = int(pixel.getGreen() * (1.0f - weight) + color.getGreen() * weight);
                    int b = int(pixel.getBlue()  * (1.0f - weight) + color.getBlue()  * weight);
                    int a = (pixel.getAlpha() + color.getAlpha()) >> 1;

                    pixels[x + y * width] = ColorRGBA(r, g, b, a).value;
                }
            }
        }

        void setPixels(int x, int y, Canvas *canvas)
        {
            for (int i = 0; i < canvas->width; i++) {
                for (int j = 0; j < canvas->height; j++) {
                    setPixel(x + i, y + j, canvas->pixels[i + j * canvas->width]);
                }
            }
        }

        bool getPixel(int x, int y, ColorRGBA *color)
        {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                *(color) = ColorRGBA(pixels[x + y * width]);
                return true;
            }

            *(color) = 0;
            return false;
        }

        Canvas getPixels(int x, int y, unsigned int w, unsigned int h)
        {
            ColorRGBA color;
            Canvas canvas = Canvas(w, h);
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    getPixel(x + i, y + j, &color);
                    canvas.pixels[i + j * w] = color.value;
                }
            }

            return canvas;
        }

        void drawRectangle(int x, int y, int w, int h, ColorRGBA color)
        {
            for (int i = 0; i < w; i++) {
                setPixel(x + i, y        , color);
                setPixel(x + i, y + h - 1, color);
            }
            for (int i = 1; i < h - 1; i++) {
                setPixel(x        , y + i, color);
                setPixel(x + w - 1, y + i, color);
            }
        }

        void drawFilledRectangle(int x, int y, int w, int h, ColorRGBA color)
        {
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++)  {
                    setPixel(x + j, y + i, color);
                }
            }
        }

        void drawAALine(int x1, int y1, int x2, int y2, ColorRGBA color)
        {
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;
            int err = dx - dy, ee, xx;
            int ed = dx + dy == 0 ? 1 : sqrt((float)dx * dx + (float)dy * dy);

            while (true) {
                setPixel(x1, y1, ColorRGBA(color.value, 255 - 255 * abs(err - dx + dy) / ed));
                ee = err;
                xx = x1;
                if (ee * 2 >= -dx) {
                    if (x1 == x2) break;
                    if (ee + dy < ed) setPixel(x1, y1 + sy, ColorRGBA(color.value, 255 - 255 * (ee + dy) / ed));
                    err -= dy;
                    x1 += sx;
                }
                if (ee * 2 <= dy) {
                    if (y1 == y2) break;
                    if (dx - ee < ed) setPixel(xx + sx, y1, ColorRGBA(color.value, 255 - 255 * (dx - ee) / ed));
                    err += dx;
                    y1 += sy;
                }
            }
        }

        void drawLine(int x1, int y1, int x2, int y2, ColorRGBA color)
        {
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = x1 < x2 ? 1 : -1;
            int sy = y1 < y2 ? 1 : -1;

            if (dx > dy) {
                int y = y1;
                int d = dy * 2 - dx;
                for (int x = x1; x != x2; x += sx) {
                    setPixel(x, y, color);
                    if (d > 0) {
                        y += sy;
                        d -= dx * 2;
                    }
                    d += dy * 2;
                }
            } else {
                int x = x1;
                int d = dx * 2 - dy;
                for (int y = y1; y != y2; y += sy) {
                    setPixel(x, y, color);
                    if (d > 0) {
                        x += sx;
                        d -= dy * 2;
                    }
                    d += dx * 2;
                }
            }
            setPixel(x2, y2, color);
        }

        void drawBezierCurve_quadratic(int x1, int y1, int xc, int yc, int x2, int y2, float resolution, ColorRGBA color)
        {
            float xA, yA, xB, yB;

            float xp, x = x1;
            float yp, y = y1;
            float step = 1.0f / resolution;
            for (float t = step; t < 1 + step; t += step) {
                xp = x;
                yp = y;

                xA = x1 + (xc - x1) * t;
                yA = y1 + (yc - y1) * t;
                xB = xc + (x2 - xc) * t;
                yB = yc + (y2 - yc) * t;

                x = xA + (xB - xA) * t;
                y = yA + (yB - yA) * t;
                drawLine(xp, yp, x, y, color);
            }
            drawLine(x, y, x2, y2, color);
        }

        void drawBezierCurve_cubic(int x1, int y1, int x1c, int y1c, int x2c, int y2c, int x2, int y2, float resolution, ColorRGBA color)
        {
            float xA, yA, xB, yB, xC, yC;
            float xAB, yAB, xBC, yBC;

            float xp, x = x1;
            float yp, y = y1;
            float step = 1.0f / resolution;
            for (float t = step; t < 1 + step; t += step) {
                xp = x;
                yp = y;

                xA = x1 + (x1c - x1) * t;
                yA = y1 + (y1c - y1) * t;
                xB = x1c + (x2c - x1c) * t;
                yB = y1c + (y2c - y1c) * t;
                xC = x2c + (x2 - x2c) * t;
                yC = y2c + (y2 - y2c) * t;

                xAB = xA + (xB - xA) * t;
                yAB = yA + (yB - yA) * t;
                xBC = xB + (xC - xB) * t;
                yBC = yB + (yC - yB) * t;

                x = xAB + (xBC - xAB) * t;
                y = yAB + (yBC - yAB) * t;
                drawLine(xp, yp, x, y, color);
            }
            drawLine(x, y, x2, y2, color);
        }

        void drawMidpointCircle(int cx, int cy, int r, ColorRGBA color)
        {
            int x = 0;
            int y = -r;
            int p = -r;

            while (x < -y) {
                if (p > 0) p += 2 * (x + ++y) + 1;
                else p += 2 * x + 1;
                setPixel(cx + x, cy + y, color);
                setPixel(cx - x, cy + y, color);
                setPixel(cx + x, cy - y, color);
                setPixel(cx - x, cy - y, color);
                setPixel(cx + y, cy + x, color);
                setPixel(cx - y, cy + x, color);
                setPixel(cx + y, cy - x, color);
                setPixel(cx - y, cy - x, color);
                x++;
            }
        }

        void drawCircle(int cx, int cy, int r, ColorRGBA color)
        {
            int x = 0;
            int y = -r;
            int p = 2 - 2 * r;

            while (y < 0) {
                setPixel(cx - x, cy + y, color);
                setPixel(cx - y, cy - x, color);
                setPixel(cx + x, cy - y, color);
                setPixel(cx + y, cy + x, color);
                r = p;
                if (r <= x) p += ++x * 2 + 1;
                if (r > y || p > x) p += ++y * 2 + 1;
            }
        }

        void drawFilledCircle(int cx, int cy, int r, ColorRGBA color)
        {
            int rr = r;
            int x = -r;
            int y = 0;
            int p = 2 - 2 * r;

            for (int i = cx + x; i <= cx - x; i++) setPixel(i, cy + y, color);
            while (y < rr) {
                r = p;
                if (r > x || p > y) p += ++x * 2 + 1;
                if (r > y) continue;
                p += ++y * 2 + 1;
                for (int i = cx + x; i <= cx - x; i++) setPixel(i, cy + y, color);
                for (int i = cx + x; i <= cx - x; i++) setPixel(i, cy - y, color);
            }
        }

        void floodFill(int x, int y, ColorRGBA color)
        {
            ColorRGBA targetColor, pixel;
            if (!getPixel(x, y, &targetColor) || targetColor.value == color.value) return;

            int *next = new int[width * height * 2];
            int index = 0;

            int dir[] = {0,1, 1,0, 0,-1, -1,0};

            setPixel(x, y, color);
            while (true) {
                for (int i = 0; i < 8; i += 2) {
                    if (getPixel(x + dir[i], y + dir[i + 1], &pixel) && pixel.value == targetColor.value) {
                        setPixel(x + dir[i], y + dir[i + 1], color);
                        next[index] = x + dir[i];
                        next[index + 1] = y + dir[i + 1];
                        index += 2;
                    }
                }

                if (index == 0) break;
                index -= 2;
                x = next[index];
                y = next[index + 1];
            }

            delete[] next;
        }

        void spanFill(int x, int y, ColorRGBA color)
        {
            ColorRGBA targetColor, pixel;
            if (!getPixel(x, y, &targetColor) || targetColor.value == color.value) return;

            int *next = new int[width * height * 2];
            int index = 0;

            next[index] = x;
            next[index + 1] = x;
            next[index + 2] = y;
            next[index + 3] = 1;
            index += 4;

            int x1 = x;
            int x2 = x;
                y  = y - 1;
            int dy = -1;

            while (true) {
                x = x1;
                if (getPixel(x, y, &pixel) && pixel.value == targetColor.value) {
                    while (getPixel(x - 1, y, &pixel) && pixel.value == targetColor.value) {
                        setPixel(x - 1, y, color);
                        x--;
                    }
                    if (x < x1) {
                        next[index] = x;
                        next[index + 1] = x1 - 1;
                        next[index + 2] = y - dy;
                        next[index + 3] = -dy;
                        index += 4;
                    }
                }

                while (x1 <= x2) {
                    while (getPixel(x1, y, &pixel) && pixel.value == targetColor.value) {
                        setPixel(x1, y, color);
                        x1++;
                    }
                    if (x1 > x) {
                        next[index] = x;
                        next[index + 1] = x1 - 1;
                        next[index + 2] = y + dy;
                        next[index + 3] = dy;
                        index += 4;
                    }
                    if (x1 - 1 > x2) {
                        next[index] = x2 + 1;
                        next[index + 1] = x1 - 1;
                        next[index + 2] = y - dy;
                        next[index + 3] = -dy;
                        index += 4;
                    }
                    x1++;
                    while (x1 < x2 && getPixel(x1, y, &pixel) && pixel.value != targetColor.value) x1++;
                    x = x1;
                }
                if (index == 0) break;
                index -= 4;
                x1 = next[index];
                x2 = next[index + 1];
                y  = next[index + 2];
                dy = next[index + 3];
            }

            delete[] next;
        }

        int cross(int x1, int y1, int x2, int y2, int x, int y)
        {
            return (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
            // n > 0: p is left
            // n = 0: p is on line
            // n < 0: p is right
        }

        bool pointInTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)//, bool withBias = false)
        {
            return
                cross(x1, y1, x2, y2, x, y) <= 0 && // + (!withBias || (y2 - y1 < 0 || (y2 - y1 == 0 && x2 - x1 > 0)) ? 0 : -1) <= 0 &&
                cross(x2, y2, x3, y3, x, y) <= 0 && // + (!withBias || (y3 - y2 < 0 || (y3 - y2 == 0 && x3 - x2 > 0)) ? 0 : -1) <= 0 &&
                cross(x3, y3, x1, y1, x, y) <= 0;   // + (!withBias || (y1 - y3 < 0 || (y1 - y3 == 0 && x1 - x3 > 0)) ? 0 : -1) <= 0;
        }

        void drawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorRGBA color)//, bool withBias = false)
        {
            float minX = x1;
            if (minX > x2) minX = x2;
            if (minX > x3) minX = x3;
            float minY = y1;
            if (minY > y2) minY = y2;
            if (minY > y3) minY = y3;

            float maxX = x1;
            if (maxX < x2) maxX = x2;
            if (maxX < x3) maxX = x3;
            float maxY = y1;
            if (maxY < y2) maxY = y2;
            if (maxY < y3) maxY = y3;

            for (int x = minX; x < maxX; x++) {
                for (int y = minY; y < maxY; y++) {
                    if (pointInTriangle(x1, y1, x2, y2, x3, y3, x, y)) {//, withBias)) {
                        setPixel(x, y, color);
                    }
                }
            }
        }

        void drawImage(int x, int y, const char *fileName, bool flipImageVertically = true)
        {
            int w, h, channelCount;
            stbi_set_flip_vertically_on_load(flipImageVertically);
            unsigned char *image = stbi_load(fileName, &w, &h, &channelCount, 0);

            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {

                    int color =
                        image[(i * channelCount + 0) + j * w * channelCount] |
                        image[(i * channelCount + 1) + j * w * channelCount] << 8 |
                        image[(i * channelCount + 2) + j * w * channelCount] << 16 |
                        255 << 24;

                    setPixel(x + i, y + j, color);
                }
            }
        }
};
