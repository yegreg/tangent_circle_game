#ifndef CIRCLEBOARDGUI_H
#define CIRCLEBOARDGUI_H
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTransform>

#include "circle.h"
#include "circlelogic.h"
#include "guiscaler.h"

static const int MAX_PLAYERS = 3;

class CircleBoardGUI : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    CircleBoardGUI(QQuickItem *parent = 0);

    QString name() const;
    void setName(const QString &name);

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void restartGame();
    Q_INVOKABLE QString getScoreString() const;

    QColor color() const;
    void setColor(const QColor &color);

    void paint(QPainter *painter);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;
    void hoverMoveEvent(QHoverEvent *event) override;

signals:
    void refreshScoreBoard();

private:
    std::vector<circle_ptr> circles;
    CircleLogic game;

    QString m_name;
    QColor m_color;

    QBrush playerBrushes[MAX_PLAYERS];
    QBrush hoverBrushes[MAX_PLAYERS];
    QBrush blankBrush;
    void setUpBrushes();

    GUIScaler guiScaler;
    // previous position of mouse pointer
    QPointF m_prevPoint;
    // position where mouse was clicked
    QPointF m_clickPoint;
};

#endif // CIRCLEBOARDGUI_H
