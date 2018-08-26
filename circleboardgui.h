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

class CircleBoardGUI : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    CircleBoardGUI(QQuickItem *parent = 0);

    QString name() const;
    void setName(const QString &name);

    Q_INVOKABLE void initialize(int numPlayers=CircleLogic::DEFAULT_NUM_PLAYERS,
                                int boardDepth=CircleLogic::DEFAULT_BOARD_DEPTH,
                                int boardSymmetry=CircleLogic::DEFAULT_BOARD_SYMMETRY,
                                int botPlayer=CircleLogic::DEFAULT_BOT_PLAYER,
                                int botDifficulty=CircleBot::DEFAULT_BOT_DIFFICULTY);
    Q_INVOKABLE void restartGame(int numPlayers,
                                 int boardDepth,
                                 int boardSymmetry,
                                 int botPlayer,
                                 int botDifficulty);
    Q_INVOKABLE QString getScoreString() const;
    Q_INVOKABLE QString getWinnerString() const;
    Q_INVOKABLE QString getRemainingRoundString() const;

    QColor color() const;
    void setColor(const QColor &color);

    static const int MAX_PLAYERS = 3;

    void paint(QPainter *painter);
    static const int ANIMATION_PAUSE = 200;
    static void delay(int millisecs);
    static void botPause();

    static std::unordered_map<int, QBrush> playerBrushes;
    static std::unordered_map<int, QBrush> hoverBrushes;
    static QBrush blankBrush;

    Q_INVOKABLE static QColor getPlayerColor(int player);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;
    void hoverMoveEvent(QHoverEvent *event) override;

signals:
    void refreshScoreBoard();
    void gameOver();
    void newGame();

private:
    std::vector<circle_ptr> circles;
    CircleLogic game;

    QString m_name;
    QColor m_color;

    void setUpBrushes();
    void endGame();

    GUIScaler guiScaler;
    // previous position of mouse pointer
    QPointF m_prevPoint;
    // position where mouse was clicked
    QPointF m_clickPoint;

};

#endif // CIRCLEBOARDGUI_H
