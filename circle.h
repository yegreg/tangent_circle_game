#ifndef CIRCLE_H
#define CIRCLE_H
#include <QPointF>
#include <QString>

class Circle
{
public:
    Circle();
    Circle(double radius, double centerX, double centerY);
    Circle(double radius, QPointF center);
    operator QString() const {
        std::string toString = "Circle(radius=" + std::to_string(this->radius)
                + ", center=(" + std::to_string(this->centerX)
                + ", " + std::to_string(this->centerY) + ")";
        return QString::fromStdString(toString);
    }

    void setRadius(double radius);
    double getRadius() const;
    void setCenter(double x, double y);
    QPointF getCenter() const;
    void setPlayer(int player);
    int getPlayer() const;
    bool contains(QPointF point) const;
    bool isHovered() const;
    void setHovered(bool hovered);

    static Circle findInnerSoddyCircle(const Circle& c1, const Circle& c2, const Circle& c3, bool contains = false);

private:
    double radius;
    double centerX;
    double centerY;
    int player;
    bool hovered;
    double distance(QPointF a, QPointF b) const;
};

#endif // CIRCLE_H
