#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Vec2.h" 
#include "Circle.h"
#include "QuadTree.h"

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

int main() {
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    const float CIRCLE_RADIUS = 15.0f;
    const int MAX_CIRCLES = 100000;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Project Collision - Brute Force Or QuadTree");
    window.setFramerateLimit(60);

    std::vector<Circle> circles;

    sf::Font font;
    bool fontLoaded = false;
    
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cout << "Warning: Could not load font from assets/arial.ttf" << std::endl;
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load font from arial.ttf" << std::endl;
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cout << "Warning: Could not load font from system fonts" << std::endl;
            } else {
                fontLoaded = true;
            }
        } else {
            fontLoaded = true;
        }
    } else {
        fontLoaded = true;
    }

    sf::Text infoText;
    if (fontLoaded) {
        infoText.setFont(font);
    }
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 10);

    sf::Clock clock;
    sf::Clock fpsTimer;
    sf::Clock perfTimer;
    float deltaTime = 0;
    float fpsUpdateTime = 0;
    float displayFPS = 0;
    int frameCount = 0;

    bool useBruteForce = true;
    bool showQuadTree = false;
    bool spawnOnCollision = true;
    int totalCollisions = 0;
    int totalSpawned = 0;
    
    float bruteForceTime = 0;
    float quadTreeTime = 0;

    std::vector<Circle> newCircles;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    useBruteForce = !useBruteForce;
                    std::cout << "Switched to: " << (useBruteForce ? "Brute Force" : "QuadTree") << std::endl;
                }
                if (event.key.code == sf::Keyboard::Q) {
                    showQuadTree = !showQuadTree;
                }
                if (event.key.code == sf::Keyboard::S) {
                    spawnOnCollision = !spawnOnCollision;
                    std::cout << "Spawn on collision: " << (spawnOnCollision ? "ON" : "OFF") << std::endl;
                }
                if (event.key.code == sf::Keyboard::R) {
                    circles.clear();
                    totalCollisions = 0;
                    totalSpawned = 0;
                    std::cout << "Simulation reset!" << std::endl;
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    Vec2 mousePos(event.mouseButton.x, event.mouseButton.y);
                    Vec2 randomVel((rand() % 400) - 200, (rand() % 400) - 200);
                    circles.push_back(Circle(mousePos, randomVel, CIRCLE_RADIUS, getRandomColor()));
                    std::cout << "Added circle! Total: " << circles.size() << std::endl;
                }
            }
        }

        deltaTime = clock.restart().asSeconds();
        frameCount++;
        fpsUpdateTime += deltaTime;
        
        if (fpsUpdateTime >= 0.5f) {
            displayFPS = frameCount / fpsUpdateTime;
            frameCount = 0;
            fpsUpdateTime = 0;
        }

        for (Circle& c : circles) {
            c.update(deltaTime);
            c.checkWallCollision(WIDTH, HEIGHT);
        }

        newCircles.clear();

        int collisionsThisFrame = 0;
        Rectangle boundary(0, 0, WIDTH, HEIGHT);

        perfTimer.restart();

        if (useBruteForce) {
            for (size_t i = 0; i < circles.size(); i++) {
                for (size_t j = i + 1; j < circles.size(); j++) {
                    if (circles[i].isColliding(circles[j])) {
                        bool canSpawn = (circles[i].collisionCooldown <= 0 && 
                                       circles[j].collisionCooldown <= 0);
                        
                        bool collisionHappened = circles[i].resolveCollision(circles[j]);
                        
                        if (collisionHappened) {
                            collisionsThisFrame++;
                            
                            if (spawnOnCollision && canSpawn && 
                                circles.size() + newCircles.size() < MAX_CIRCLES) {
                                Vec2 midPoint = (circles[i].position + circles[j].position) / 2.0f;
                                newCircles.push_back(createCircleAt(midPoint, CIRCLE_RADIUS));
                                totalSpawned++;
                                
                                std::cout << "Collision spawn! Total circles: " 
                                          << (circles.size() + newCircles.size()) << std::endl;
                            }
                        }
                    }
                }
            }
            bruteForceTime = perfTimer.getElapsedTime().asMicroseconds() / 1000.0f;
        } else {
            QuadTree* qt = new QuadTree(boundary);
            for (Circle& c : circles) {
                qt->insert(&c);
            }

            for (size_t idx = 0; idx < circles.size(); idx++) {
                Circle& c = circles[idx];
                Rectangle range(
                    c.position.x - c.radius * 2,
                    c.position.y - c.radius * 2,
                    c.radius * 4,
                    c.radius * 4
                );

                std::vector<Circle*> nearby;
                qt->query(range, nearby);

                for (Circle* other : nearby) {
                    if (&c != other && c.isColliding(*other)) {
                        bool shouldResolve = false;
                        for (size_t i = 0; i < circles.size(); i++) {
                            if (&circles[i] == &c) {
                                shouldResolve = true;
                                break;
                            }
                            if (&circles[i] == other) {
                                break;
                            }
                        }
                        
                        if (shouldResolve) {
                            bool canSpawn = (c.collisionCooldown <= 0 && 
                                           other->collisionCooldown <= 0);
                            
                            bool collisionHappened = c.resolveCollision(*other);
                            
                            if (collisionHappened) {
                                collisionsThisFrame++;
                                
                                if (spawnOnCollision && canSpawn && 
                                    circles.size() + newCircles.size() < MAX_CIRCLES) {
                                    Vec2 midPoint = (c.position + other->position) / 2.0f;
                                    newCircles.push_back(createCircleAt(midPoint, CIRCLE_RADIUS));
                                    totalSpawned++;
                                    
                                    std::cout << "Collision spawn! Total circles: " 
                                              << (circles.size() + newCircles.size()) << std::endl;
                                }
                            }
                        }
                    }
                }
            }

            delete qt;
            quadTreeTime = perfTimer.getElapsedTime().asMicroseconds() / 1000.0f;
        }

        for (Circle& newCircle : newCircles) {
            circles.push_back(newCircle);
        }

        totalCollisions += collisionsThisFrame;

        window.clear(sf::Color(20, 20, 30));

        if (showQuadTree && !useBruteForce) {
            QuadTree* qt = new QuadTree(boundary);
            for (Circle& c : circles) {
                qt->insert(&c);
            }
            qt->draw(window);
            delete qt;
        }

        for (const Circle& c : circles) {
            c.draw(window);
        }

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1);
        ss << "FPS: " << displayFPS << "\n";
        ss << "Algorithm: " << (useBruteForce ? "Brute Force" : "QuadTree") << "\n";
        ss << "Collision Time: " << (useBruteForce ? bruteForceTime : quadTreeTime) << " ms\n";
        ss << "Circles: " << circles.size() << "\n";
        ss << "\n";
        ss << "[SPACE] Ganti Algorithm\n";
        ss << "[S] Nonaktifkan Spawn\n";
        ss << "[CLICK KIRI] Tambah Lingkaran\n";
        ss << "[R] Reset";

        infoText.setString(ss.str());
        window.draw(infoText);

        window.display();
    }

    return 0;
}