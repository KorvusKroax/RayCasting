#pragma once

struct Wall
{
    public:
        int start, end;
        float height;

        Wall(int start, int end, float height) {
            this->start = start;
            this->end = end;
            this->height = height;
        }
};
