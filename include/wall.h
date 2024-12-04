#pragma once

struct Wall
{
    public:
        int start, end; // point indicies
        float bottom, top;

        Wall(int start, int end, float bottom, float top) {
            this->start = start;
            this->end = end;
            this->bottom = bottom;
            this->top = top;
        }
};
