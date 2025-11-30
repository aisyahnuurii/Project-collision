#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Vec2.h"
#include "Circle.h"

class Rectangle {
public:
    float x, y, width, height;

    Rectangle() : x(0), y(0), width(0), height(0) {}
    Rectangle(float x, float y, float w, float h)
        : x(x), y(y), width(w), height(h) {}

    bool contains(const Vec2& point) const {
        return (point.x >= x && point.x <= x + width &&
                point.y >= y && point.y <= y + height);
    }

    bool intersects(const Rectangle& range) const {
        return !(range.x > x + width ||
                 range.x + range.width < x ||
                 range.y > y + height ||
                 range.y + range.height < y);
    }

    bool containsCircle(const Circle& circle) const {
        return (circle.position.x - circle.radius >= x &&
                circle.position.x + circle.radius <= x + width &&
                circle.position.y - circle.radius >= y &&
                circle.position.y + circle.radius <= y + height);
    }

    bool intersectsCircle(const Circle& circle) const {
        float closestX = std::max(x, std::min(circle.position.x, x + width));
        float closestY = std::max(y, std::min(circle.position.y, y + height));
        
        float distanceX = circle.position.x - closestX;
        float distanceY = circle.position.y - closestY;
        
        return (distanceX * distanceX + distanceY * distanceY) < (circle.radius * circle.radius);
    }
};

#endif