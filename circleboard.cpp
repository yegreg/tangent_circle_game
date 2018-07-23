#include "circleboard.h"
#include <QDebug>

const int NUM_LEVELS = 5;
const int FIRST_LEVEL_SIZE = 4;
const double RHO = std::sin(M_PI / FIRST_LEVEL_SIZE);

CircleBoard::CircleBoard()
{
}

CircleBoard::CircleBoard(double outer_radius, QPointF center) {
    qInfo("Generating board");
    generateBoard(outer_radius, center);
    initGameNeighbors();
}

QPointF CircleBoard::rotate(double x, double y, double angle) {
    /**
      Rotates the point (x, y) around the origin by angle
    */
    double newX = x * cos(angle) - y * sin(angle);
    double newY = x * sin(angle) + y * cos(angle);
    return QPointF(newX, newY);
}

void CircleBoard::initGameNeighbors()
{
    for (const auto &level: this->levels) {
        for (const circle_ptr &circle: level) {
            if (circle != this->baseCircle) {
                this->gameNeighbors[circle] = std::vector<circle_ptr>();
                for (const circle_ptr &nb: this->neighbors.at(circle)) {
                    if (nb != this->baseCircle) {
                        this->gameNeighbors[circle].push_back(nb);
                    }
                }
            }
        }
    }
}

void CircleBoard::generateBoard(double outer_radius, QPointF center)
{
    // base circle (level 0)
    this->baseCircle = circle_ptr(new Circle(outer_radius, center));
    std::vector<circle_ptr> level0;
    level0.push_back(baseCircle);
    this->levels.push_back(level0);
    this->neighbors[baseCircle] = std::vector<circle_ptr>();

    double newRadius = outer_radius / (1.0 + 1.0 / RHO);
    double cx = newRadius / RHO;
    double cy = 0;

    // level 1 circles
    circle_ptr init_circles[FIRST_LEVEL_SIZE];

    for (int i = 0; i < FIRST_LEVEL_SIZE; ++i) {
        init_circles[i] = circle_ptr(new Circle(newRadius, center + rotate(cx, cy, i * 2 * M_PI / FIRST_LEVEL_SIZE)));
    }

    std::vector<circle_ptr> level1;
    for (int i = 0; i < FIRST_LEVEL_SIZE; ++i) {
        level1.push_back(init_circles[i]);
        this->neighbors[init_circles[i]] = std::vector<circle_ptr>();
        this->neighbors[init_circles[i]].push_back(init_circles[(i + 1) % FIRST_LEVEL_SIZE]);
        int prev = (i - 1) % FIRST_LEVEL_SIZE;
        prev = prev < 0 ? prev + FIRST_LEVEL_SIZE : prev;
        this->neighbors[init_circles[i]].push_back(init_circles[prev]);
        this->neighbors[init_circles[i]].push_back(baseCircle);
        this->neighbors[baseCircle].push_back(init_circles[i]);
    }

    this->levels.push_back(level1);

    // level 2 circles
    std::vector<circle_ptr> level2;
    QPointF centerSum = QPointF(0, 0);
    for (int i = 0; i < FIRST_LEVEL_SIZE; ++i) {
        circle_ptr c2 = init_circles[i];
        circle_ptr c3 = init_circles[(i + 1) % FIRST_LEVEL_SIZE];
        Circle newCircle = Circle::findInnerSoddyCircle(
                    *this->baseCircle,
                    *c2,
                    *c3,
                true);
        circle_ptr new_circle = std::make_shared<Circle>(newCircle);
        level2.push_back(new_circle);
        this->neighbors[this->baseCircle].push_back(new_circle);
        this->neighbors[c2].push_back(new_circle);
        this->neighbors[c3].push_back(new_circle);
        this->neighbors[new_circle] = {this->baseCircle, c2, c3};

        centerSum += init_circles[i]->getCenter();
    }
    // center circle
    double centerRadius = newRadius * (1 / RHO - 1);
    QPointF centerCenter = centerSum / FIRST_LEVEL_SIZE;
    circle_ptr centerCircle = circle_ptr(new Circle(centerRadius, centerCenter));
    level2.push_back(centerCircle);
    this->neighbors[centerCircle] = std::vector<circle_ptr>(
                init_circles,
                init_circles + sizeof(init_circles) / sizeof(init_circles[0]));
    for (int i = 0; i < FIRST_LEVEL_SIZE; ++i) {
        this->neighbors[init_circles[i]].push_back(centerCircle);
    }

    this->levels.push_back(level2);

    // remaining circles
    for (int i = 3; i < NUM_LEVELS; ++i) {
        std::vector<circle_ptr> newLevel;
        std::vector<circle_ptr> lastLevel = levels[i - 1];
        for (circle_ptr circle: lastLevel) {
            int num_neighbors = this->neighbors[circle].size();
            for (int j = 0; j < num_neighbors; ++j) {
                circle_ptr c2 = this->neighbors[circle][j];
                circle_ptr c3 = this->neighbors[circle][(j + 1) % num_neighbors];

                Circle newCircle;
                if (c2 == this->baseCircle) {
                    newCircle = Circle::findInnerSoddyCircle(*c2, *circle, *c3, true);
                } else if (c3 == this->baseCircle) {
                    newCircle = Circle::findInnerSoddyCircle(*c3, *circle, *c2, true);
                } else {
                    newCircle = Circle::findInnerSoddyCircle(*c2, *circle, *c3);
                }

                circle_ptr new_circle = std::make_shared<Circle>(newCircle);
                newLevel.push_back(new_circle);

                this->neighbors[circle].push_back(new_circle);
                this->neighbors[c2].push_back(new_circle);
                this->neighbors[c3].push_back(new_circle);
                this->neighbors[new_circle] = {circle, c2, c3};

            }
        }
    this->levels.push_back(newLevel);
    }
//    std::vector<Circle> level2;
//    Circle c012 = Circle::findInnerSoddyCircle(baseCircle, c1, c2, true);
//    level2.push_back(c012);
//    Circle c023 = Circle::findInnerSoddyCircle(baseCircle, c2, c3, true);
//    level2.push_back(c023);
//    Circle c031 = Circle::findInnerSoddyCircle(baseCircle, c3, c1, true);
//    level2.push_back(c031);
//    Circle c123 = Circle::findInnerSoddyCircle(c1, c2, c3);
//    level2.push_back(c123);
//    this->levels.push_back(level2);
}

const circle_ptr& CircleBoard::getBaseCircle() const
{
    return this->baseCircle;
}

const std::vector<std::vector<circle_ptr>>& CircleBoard::getLevels() const
{
    return this->levels;
}

const std::unordered_map<circle_ptr, std::vector<circle_ptr> > &CircleBoard::getGameNeighbors() const
{
    return this->gameNeighbors;
}


