#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include "Circle.h"
#include "Rectangle.h"

class QuadTree {
private:
    static const int CAPACITY = 4; 
    Rectangle boundary;
    std::vector<Circle*> circles;
    bool divided;

    QuadTree* northeast;
    QuadTree* northwest;
    QuadTree* southeast;
    QuadTree* southwest;

    void subdivide() {
        float x = boundary.x;
        float y = boundary.y;
        float w = boundary.width / 2;
        float h = boundary.height / 2;

        Rectangle ne(x + w, y, w, h);
        northeast = new QuadTree(ne);

        Rectangle nw(x, y, w, h);
        northwest = new QuadTree(nw);

        Rectangle se(x + w, y + h, w, h);
        southeast = new QuadTree(se);

        Rectangle sw(x, y + h, w, h);
        southwest = new QuadTree(sw);

        divided = true;
    }

public:
    QuadTree(Rectangle boundary)
        : boundary(boundary), divided(false),
          northeast(nullptr), northwest(nullptr),
          southeast(nullptr), southwest(nullptr) {}

    ~QuadTree() {
        if (divided) {
            delete northeast;
            delete northwest;
            delete southeast;
            delete southwest;
        }
    }

    bool insert(Circle* circle) {
        if (!boundary.intersectsCircle(*circle)) {
            return false;
        }

        if (circles.size() < CAPACITY) {
            circles.push_back(circle);
            return true;
        }

        if (!divided) {
            subdivide();
        }

        if (northeast->insert(circle)) return true;
        if (northwest->insert(circle)) return true;
        if (southeast->insert(circle)) return true;
        if (southwest->insert(circle)) return true;

        return false;
    }

    void query(const Rectangle& range, std::vector<Circle*>& found) {
        if (!boundary.intersects(range)) {
            return;
        }

        for (Circle* c : circles) {
            if (range.intersectsCircle(*c)) {
                found.push_back(c);
            }
        }

        if (divided) {
            northwest->query(range, found);
            northeast->query(range, found);
            southwest->query(range, found);
            southeast->query(range, found);
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::RectangleShape rect(sf::Vector2f(boundary.width, boundary.height));
        rect.setPosition(boundary.x, boundary.y);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color(100, 100, 100, 100));
        rect.setOutlineThickness(1);
        window.draw(rect);

        if (divided) {
            northwest->draw(window);
            northeast->draw(window);
            southwest->draw(window);
            southeast->draw(window);
        }
    }
};

#endif