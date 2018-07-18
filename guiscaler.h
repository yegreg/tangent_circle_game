#ifndef GUISCALER_H
#define GUISCALER_H

#include <QPointF>
#include <QPainter>
#include "circle.h"

/**
 * @brief GUIScaler::GUIScaler
 * Keeps track of zooming and translation of the board
 */
class GUIScaler
{
public:
    GUIScaler();
    GUIScaler(double scale, QPointF origin);
    /** Compute true position of a given point based on the
     * current zoom
     * @brief getTruePosition
     * @param point
     * @return true position
     */
    QPointF getTruePosition(QPoint point) const;
    void drawCircle(const Circle &c, QPainter* painter) const;
    void rescale(double ratio, QPointF mousePosition);
    void translate(const QPointF &offset);
    void setSize(double width, double height);

private:
    double scale;
    QPointF origin;
    // size of object
    double width;
    double height;
};

#endif // GUISCALER_H
