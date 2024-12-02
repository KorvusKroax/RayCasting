#pragma once

struct Point
{
    public:
        float x, y;

        Point(float x = 0, float y = 0) {
            this->x = x;
            this->y = y;
        }

        Point add(Point p)
        {
            return Point(x + p.x, y + p.y);
        }

        Point sub(Point p)
        {
            return Point(x - p.x, y - p.y);
        }

        Point rotate(float angle)
        {
            return Point(
                x * cos(angle) - y * sin(angle),
                y * cos(angle) + x * sin(angle)
            );
        }
};
