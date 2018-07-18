#include "guiscaler.h"
#include <QtDebug>

// not allowed to zoom out more than this
static const double MIN_ZOOM = 1.0;
static const double MAX_ZOOM = 10.0;
static const QPointF ORIGIN = QPointF(0.0, 0.0);

GUIScaler::GUIScaler() : GUIScaler(1.0, QPointF(0.0, 0.0)) {}

GUIScaler::GUIScaler(double scale, QPointF origin)
{
    this->scale = scale;
    this->origin = origin;
}

QPointF GUIScaler::getTruePosition(QPoint point) const
{
    return origin + point / scale;
}

void GUIScaler::drawCircle(const Circle& c, QPainter *painter) const
{
    double r = c.getRadius();
    QPointF center = c.getCenter();
    QPointF newCenter = scale * (center - origin);
    double newR = scale * r;
    painter->drawEllipse(newCenter, newR, newR);
}

void GUIScaler::rescale(double ratio, QPointF mousePosition)
{
    if (this->scale > MAX_ZOOM && ratio > 1.0) return;
    this->scale *= ratio;
    QPointF newOrigin;
    if (ratio > 1.0) {
        newOrigin = ((ratio - 1) * mousePosition + this->origin * this->scale) / this->scale;
    } else if (ratio < 1.0) {
        newOrigin = (this->scale - 1) / (this->scale - ratio) * this->origin;
    }
    this->origin = newOrigin;
    if (this->scale < MIN_ZOOM) {
        this->scale = MIN_ZOOM;
        this->origin = ORIGIN;
    }
}

void GUIScaler::translate(const QPointF &offset)
{
    this->origin -= offset / this->scale;
    if (this->origin.x() + width / this->scale > width) {
        this->origin.setX(width - width / this->scale);
    }
    if (this->origin.x() < 0.0) {
        this->origin.setX(0.0);
    }
    if (this->origin.y() + height / this->scale > height) {
        this->origin.setY(height - height / this->scale);
    }
    if (this->origin.y() < 0.0) {
        this->origin.setY(0.0);
    }
}

void GUIScaler::setSize(double width, double height)
{
    this->width = width;
    this->height = height;
}
