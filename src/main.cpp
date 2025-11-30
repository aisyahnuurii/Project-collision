#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Vec2.h" 
#include "Circle.h"

sf::Color getRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(50, 255);
    return sf::Color(dis(gen), dis(gen), dis(gen));
}

Circle createRandomCircle(float width, float height, float radius) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> posX(radius, width - radius);
    std::uniform_real_distribution<> posY(radius, height - radius);
    std::uniform_real_distribution<> vel(-200, 200);

    Vec2 pos(posX(gen), posY(gen));
    Vec2 velocity(vel(gen), vel(gen));
    sf::Color color = getRandomColor();

    return Circle(pos, velocity, radius, color);
}

Circle createCircleAt(Vec2 position, float radius) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> vel(-150, 150);
    
    Vec2 velocity(vel(gen), vel(gen));
    sf::Color color = getRandomColor();
    
    return Circle(position, velocity, radius, color);
}
