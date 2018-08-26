#include "circleboardgui.h"
#include <math.h>
#include <sstream>
#include <QTime>

static const double MARGIN = 0.95;
static const double SCROLL_SPEED = 1.0 / 1200;
/** how far the mouse can move between press and release
* to count as a click */
static const double MOUSE_SENSITIVITY = 3;
static const int HOVER_DARKNESS = 150;

static const int BOT_PAUSE = 1000;

std::unordered_map<int, QBrush> CircleBoardGUI::playerBrushes;
std::unordered_map<int, QBrush> CircleBoardGUI::hoverBrushes;
QBrush CircleBoardGUI::blankBrush;


CircleBoardGUI::CircleBoardGUI(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    setUpBrushes();
}


void CircleBoardGUI::mousePressEvent(QMouseEvent *event)
{
    m_prevPoint = event->pos();
    m_clickPoint = event->pos();
}

void CircleBoardGUI::mouseMoveEvent(QMouseEvent *event)
{
    QPointF currPos = event->pos();
    QPointF offset = currPos - m_prevPoint;
    this->guiScaler.translate(offset);
    m_prevPoint = currPos;
    this->update();
}

void CircleBoardGUI::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->pos() - this->m_clickPoint).manhattanLength() < MOUSE_SENSITIVITY) {
        QPointF truePos = this->guiScaler.getTruePosition(event->pos());
        for (circle_ptr c: this->circles) {
            if (c->contains(truePos)) {
                if (!this->game.selectCircle(c)) {
                    endGame();
                }
            }
        }
        this->update();
        emit refreshScoreBoard();
    }
}

void CircleBoardGUI::wheelEvent(QWheelEvent *event)
{
    guiScaler.rescale(1 + (float(event->delta()) * SCROLL_SPEED), event->pos());
    update();

}

void CircleBoardGUI::hoverMoveEvent(QHoverEvent* event) {
        QQuickItem::hoverMoveEvent(event);
        QPointF truePos = this->guiScaler.getTruePosition(event->pos());
        for (circle_ptr circle: this->circles) {
            if (circle->contains(truePos)) {
                circle->setHovered(true);
            } else {
                circle->setHovered(false);
            }
        }
        this->update();
}

void CircleBoardGUI::setUpBrushes()
{
    blankBrush = QBrush(QColor(255, 0, 0, 0));

    hoverBrushes[CircleLogic::PLAYER_ONE] = QBrush(QColor(255, 0, 50, 100));
    playerBrushes[CircleLogic::PLAYER_ONE] = QBrush(QColor(255, 0, 50, 200));

    hoverBrushes[CircleLogic::PLAYER_TWO] = QBrush(QColor(0, 50, 255, 100));
    playerBrushes[CircleLogic::PLAYER_TWO] = QBrush(QColor(0, 50, 255, 200));

    hoverBrushes[CircleLogic::PLAYER_THREE] = QBrush(QColor(50, 255, 0, 100));
    playerBrushes[CircleLogic::PLAYER_THREE] = QBrush(QColor(50, 255, 0, 200));

    hoverBrushes[CircleLogic::GAME_OVER] = QBrush(QColor(100, 100, 100, 100));
    playerBrushes[CircleLogic::GAME_OVER] = QBrush(QColor(100, 100, 100, 200));
}

void CircleBoardGUI::endGame()
{
    emit gameOver();
}

void CircleBoardGUI::delay(int millisecs)
{
    QTime dieTime= QTime::currentTime().addMSecs(millisecs);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, millisecs / 2);
    }

}

void CircleBoardGUI::botPause()
{
    delay(BOT_PAUSE);
}

QColor CircleBoardGUI::getPlayerColor(int player)
{
    QColor toReturn = playerBrushes[player].color();
    return toReturn;
}

QString CircleBoardGUI::name() const
{
    return m_name;
}

void CircleBoardGUI::setName(const QString &name)
{
    m_name = name;
}

QColor CircleBoardGUI::color() const
{
    return m_color;
}

void CircleBoardGUI::setColor(const QColor &color)
{
    m_color = color;
}

void CircleBoardGUI::initialize(int numPlayers,
                                int boardDepth,
                                int boardSymmetry,
                                int botPlayer,
                                int botDifficulty)
{
    qInfo("Init GUI");
    this->game = CircleLogic(this, numPlayers, boardDepth, boardSymmetry, botPlayer, botDifficulty);
    this->guiScaler = GUIScaler();

    m_prevPoint = QPoint(0, 0);
    m_clickPoint = QPoint(0, 0);

    double width = this->width();
    double height = this->height();
    this->guiScaler.setSize(width, height);

    double radius = std::min(width, height) * MARGIN / 2.0;
    QPointF center = QPointF(width / 2.0, height / 2.0);
    this->game.setUpBoard(radius, center);
    this->circles.clear();
    std::vector<std::vector<circle_ptr>> levels = this->game.getBoard().getLevels();
    for (uint i = 1; i < levels.size(); ++i) {
        for (circle_ptr c: levels[i]) {
            this->circles.push_back(c);
        }
    }

    emit newGame();
    emit refreshScoreBoard();
}

void CircleBoardGUI::restartGame(int numPlayers,
                                 int boardDepth,
                                 int boardSymmetry,
                                 int botPlayer,
                                 int botDifficulty)
{
    qInfo("Restarting game");
    initialize(numPlayers, boardDepth, boardSymmetry, botPlayer, botDifficulty);
    if (botPlayer == CircleLogic::PLAYER_ONE) {
        this->game.botStep();
    }
    update();
}

QString CircleBoardGUI::getScoreString() const
{
    std::stringstream ss;
    int numPlayers = this->game.getNumPlayers();
    for (int player = 0; player < numPlayers; ++player) {
        ss << "Player " << player + 1 << ": " << this->game.getScore(player) << "        ";
    }
    return QString::fromStdString(ss.str());
}

QString CircleBoardGUI::getWinnerString() const
{
    std::vector<int> winners = game.getWinners();
    std::stringstream ss;
    ss << "Game Over." << std::endl;
    if (winners.size() == 1) {
        ss << "Player " << winners.at(0) + 1 << " wins." << std::endl;
    } else {
        ss << "Draw between players ";
        ss << winners.at(0) + 1;
        for (uint i = 1; i < winners.size(); ++i) {
            ss << " and " << winners.at(i) + 1;
        }
        ss << "." << std::endl;
    }
    ss << "Well played, congratulations!";
    return QString::fromStdString(ss.str());
}

QString CircleBoardGUI::getRemainingRoundString() const
{
    std::stringstream ss;
    int numPlayers = this->game.getNumPlayers();
    for (int player = 0; player < numPlayers; ++player) {
        ss << "Player " << player + 1 << ": " << this->game.getRemainingSteps(player) << "        ";
    }
    return QString::fromStdString(ss.str());
}



void CircleBoardGUI::paint(QPainter *painter)
{
    QPen pen(m_color, 2);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    // draw base circle
    guiScaler.drawCircle(*this->game.getBoard().getBaseCircle(), painter);
    for (circle_ptr circle: this->circles) {
        int player = circle->getPlayer();
        if (player >= 0) {
            painter->setBrush(this->playerBrushes[player]);
            if (circle->isHovered()) {
                painter->setBrush(QBrush(this->playerBrushes[player].color().darker(HOVER_DARKNESS)));
            }
        } else if (circle->isHovered()) {
            painter->setBrush(hoverBrushes[this->game.getCurrentPlayer()]);
        } else {
            painter->setBrush(blankBrush);
        }
        guiScaler.drawCircle(*circle, painter);
    }

}
