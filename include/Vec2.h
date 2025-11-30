#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class Vec2 {
public:
    float x, y;

    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& v) const {
        return Vec2(x + v.x, y + v.y);
    }

    Vec2 operator-(const Vec2& v) const {
        return Vec2(x - v.x, y - v.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator/(float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    Vec2& operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    Vec2 normalize() const {
        float len = length();
        if (len > 0) {
            return Vec2(x / len, y / len);
        }
        return Vec2(0, 0);
    }

    float dot(const Vec2& v) const {
        return x * v.x + y * v.y;
    }

    static float distance(const Vec2& a, const Vec2& b) {
        return (a - b).length();
    }

    static float distanceSquared(const Vec2& a, const Vec2& b) {
        return (a - b).lengthSquared();
    }
};

#endif