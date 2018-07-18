#include "circlelogic.h"
#include <QDebug>

CircleLogic::CircleLogic(int numPlayers)
{
    this->numPlayers = numPlayers;
    this->currentPlayer = 1;
}

void CircleLogic::selectCircle(circle_ptr circle)
{
    if (circle->getPlayer() < 0) {
        circle->setPlayer(this->currentPlayer);
        updateNeighbors(circle);
        runNextRound();
    } else {
        // clicked circle already occupied
        return;
    }

}

void CircleLogic::setUpBoard(double radius, QPointF center)
{
    qInfo("Setting up board");
    this->board = CircleBoard(radius, center);
    initNeighborCounts();
}

const CircleBoard &CircleLogic::getBoard() const
{
    return this->board;
}

int CircleLogic::getCurrentPlayer() const
{
    return this->currentPlayer;
}

void CircleLogic::runNextRound()
{
    this->currentPlayer = (this->currentPlayer + 1) % this->numPlayers;
}

void CircleLogic::initNeighborCounts()
{
    for (const auto &level: this->board.getLevels()) {
        for (const circle_ptr &circle: level) {
            for (int i = 0; i < this->numPlayers; ++i) {
                this->neighborCounts[circle][i] = 0;
            }
        }
    }
}

void CircleLogic::updateNeighbors(circle_ptr circle)
{
    const std::vector<circle_ptr> &neighbors = this->board.getGameNeighbors().at(circle);
    for (const circle_ptr &nb: neighbors) {
        this->neighborCounts[nb][this->currentPlayer]++;
    }
    for (const circle_ptr &nb: neighbors) {
        if (nb->getPlayer() < 0
                && this->neighborCounts[nb][this->currentPlayer] >= CONVERSION_MINIMUM) {
            nb->setPlayer(this->currentPlayer);
            updateNeighbors(nb);
        }
    }

}