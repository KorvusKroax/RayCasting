#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <canvas.h>
#include <iostream>

class SpriteSheet
{
    public:
        int width;
        int height;
        int channelCount;
        unsigned char* pixels;

        SpriteSheet(const char *fileName)
        {
            stbi_set_flip_vertically_on_load(true);
            pixels = stbi_load(fileName, &width, &height, &channelCount, 0);
            if (!pixels) {
                std::cout << "Failed to load image" << std::endl;
            }
        }

        ~SpriteSheet() { delete[] pixels; }

        Canvas getSprite(int x, int y, int w, int h)
        {
            Canvas sprite = Canvas(w, h);
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    int r = pixels[((x + i) * channelCount + 0) + (y + j) * width * channelCount];
                    int g = pixels[((x + i) * channelCount + 1) + (y + j) * width * channelCount];
                    int b = pixels[((x + i) * channelCount + 2) + (y + j) * width * channelCount];
                    int a = 255;
                    sprite.pixels[i + j * w] = r | (g << 8) | (b << 16) | (a << 24);
                }
            }

            return sprite;
        }
};
