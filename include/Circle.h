#ifndef CIRCLE_H
#define CIRCLE_H

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class Circle {
public:
    Vec2 position;
    Vec2 velocity;
    float radius;
    sf::Color color;
    float mass;
    float collisionCooldown;

    Circle(Vec2 pos, Vec2 vel, float r, sf::Color c)
        : position(pos), velocity(vel), radius(r), color(c), collisionCooldown(0.0f) {
        mass = r * r;
    }

    void update(float dt) {
        position += velocity * dt;
        
        if (collisionCooldown > 0) {
            collisionCooldown -= dt;
        }
    }

    bool isColliding(const Circle& other) const {
        float distance = Vec2::distance(position, other.position);
        return distance < (radius + other.radius);
    }

    bool resolveCollision(Circle& other) {
        Vec2 delta = other.position - position;
        float distance = delta.length();
        
        if (distance == 0) return false;
        
        Vec2 collisionNormal = delta / distance;
        
        Vec2 relativeVelocity = other.velocity - velocity;
        
        float velocityAlongNormal = relativeVelocity.dot(collisionNormal);
        
        if (velocityAlongNormal > 0) return false;
        
        float restitution = 1.0f; 
        float impulseScalar = -(1 + restitution) * velocityAlongNormal;
        impulseScalar /= (1 / mass + 1 / other.mass);
        
        Vec2 impulse = collisionNormal * impulseScalar;
        velocity -= impulse / mass;
        other.velocity += impulse / other.mass;
        
        float overlap = (radius + other.radius) - distance;
        Vec2 separation = collisionNormal * (overlap / 2.0f);
        position -= separation;
        other.position += separation;
        
        collisionCooldown = 0.3f;
        other.collisionCooldown = 0.3f;
        
        return true; 
    }

    void checkWallCollision(float width, float height) {
        if (position.x - radius < 0) {
            position.x = radius;
            velocity.x = -velocity.x;
        }
        if (position.x + radius > width) {
            position.x = width - radius;
            velocity.x = -velocity.x;
        }
        if (position.y - radius < 0) {
            position.y = radius;
            velocity.y = -velocity.y;
        }
        if (position.y + radius > height) {
            position.y = height - radius;
            velocity.y = -velocity.y;
        }
    }

    void draw(sf::RenderWindow& window) const {
        sf::CircleShape shape(radius);
        shape.setPosition(position.x - radius, position.y - radius);
        shape.setFillColor(color);
        shape.setOutlineThickness(1);
        shape.setOutlineColor(sf::Color::White);
        window.draw(shape);
    }
};

#endif