#include "circle.h"
#include <math.h>
#include <QPainter>

Circle::Circle() {}

Circle::Circle(double radius, double centerX, double centerY) {
    this->radius = radius;
    this->centerX = centerX;
    this->centerY = centerY;
    this->player = -1;
    this->hovered = false;
}

Circle::Circle(double radius, QPointF center) :
    Circle(radius, center.x(), center.y()) {
}

void Circle::setRadius(double radius)
{
    this->radius = radius;
}

double Circle::getRadius() const {
    return this->radius;
}

void Circle::setCenter(double x, double y) {
    this->centerX = x;
    this->centerY = y;
}

QPointF Circle::getCenter() const {
    return QPointF(this->centerX, this->centerY);
}

void Circle::setPlayer(int player) {
    this->player = player;
}

int Circle::getPlayer() const {
    return this->player;
}

double Circle::distance(QPointF a, QPointF b) const {
    double dx = a.x() - b.x();
    double dy = a.y() - b.y();
    long double distSq = dx * dx + dy * dy;
    return std::sqrt(distSq);
}

bool Circle::contains(QPointF point) const {
    double dist = distance(point, this->getCenter());
    return dist < this->radius;
}

bool Circle::isHovered() const {
    return this->hovered;
}

void Circle::setHovered(bool hovered) {
    this->hovered = hovered;
}

/**
 * @brief Circle::findInnerSoddyCircle
 * Finds inner Soddy circle to three given tangent circles.
 * That is, given three pairwise tangent circles c1, c2, c3, we find a circle
 * not containing them that's tangent to all three.
 * @param c1
 * @param c2
 * @param c3
 * @param contains: whether c1 contains the other two circles
 * @return inner Soddy circle
 */
Circle Circle::findInnerSoddyCircle(const Circle &c1, const Circle &c2, const Circle &c3, bool contains)
{
    // computing radius:
    // http://mathworld.wolfram.com/SoddyCircles.html
    double r1 = c1.getRadius();
    double r2 = c2.getRadius();
    double r3 = c3.getRadius();
    if (contains) {
        r1 *= -1;
    }
    double denom1 = r1 * r2 + r2 * r3 + r3 * r1;
    double denom2 = 2.0 * std::sqrt(r1 * r2 * r3 * (r1 + r2 + r3));
    double nom = r1 * r2 * r3;
    double r4;
    if (contains) {
        r4 = nom / (denom1 - denom2);
    } else {
        r4 = nom / (denom1 + denom2);
    }

    double radInvSum = 1 / r1 + 1 / r2 + 1 / r3 + 1 / r4;
    double radInvSumSq = 1 / (r1 * r1) + 1 / (r2 * r2) + 1 / (r3 * r3) + 1 / (r4 * r4);

    QPointF k1 = c1.getCenter();
    QPointF k2 = c2.getCenter();
    QPointF k3 = c3.getCenter();

    QPointF avgInv = (k1 / r1 + k2 / r2 + k3 / r3) / radInvSum;
    QPointF avgInvSq = (k1 / (r1 * r1) + k2 / (r2 * r2) + k3 / (r3 * r3)) / radInvSumSq;

    double alpha = 1 / (r4 * r4 * radInvSumSq) - 1 / (r4 * radInvSum);
    QPointF k4 = (avgInv - avgInvSq) / alpha;

    return Circle(r4, k4);
}
