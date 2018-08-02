#include "circlelogic.h"
#include <QDebug>

const int CircleLogic::PLAYER_ONE = 0;
const int CircleLogic::PLAYER_TWO = 1;
const int CircleLogic::PLAYER_THREE = 2;
const int CircleLogic::GAME_OVER = -2;

CircleLogic::CircleLogic(int numPlayers, int boardDepth, int boardSymmetry)
{
    this->numPlayers = numPlayers;
    this->boardDepth = boardDepth;
    this->boardSymmetry = boardSymmetry;
    this->currentPlayer = 0;

    for (int player = 0; player < numPlayers; ++player) {
        this->scores[player] = 0;
    }
}

bool CircleLogic::selectCircle(circle_ptr circle)
{
    if (this->remainingSteps[currentPlayer] <= 0) {
        return false;
    }
    if (circle->getPlayer() < 0) {
        circle->setPlayer(this->currentPlayer);
        this->scores[currentPlayer]++;
        this->remainingSteps[currentPlayer]--;

        updateNeighbors(circle);
        return runNextRound();
    }
    return true;
}

void CircleLogic::setUpBoard(double radius, QPointF center)
{
    qInfo("Setting up board");
    this->board = CircleBoard(radius, center, this->boardDepth, this->boardSymmetry);
    initCircleCounts();
}

const CircleBoard &CircleLogic::getBoard() const
{
    return this->board;
}

int CircleLogic::getCurrentPlayer() const
{
    return this->currentPlayer;
}

int CircleLogic::getScore(int player) const
{
    return this->scores.at(player);
}

int CircleLogic::getNumPlayers() const
{
    return this->numPlayers;
}

int CircleLogic::getRemainingSteps(int player) const
{
    return this->remainingSteps.at(player);
}

std::vector<int> CircleLogic::getWinners() const
{
    std::vector<int> winners;
    int bestPlayer = 0;
    for (int player = 0; player < this->numPlayers; ++player) {
        if (this->scores.at(player) > this->scores.at(bestPlayer)) {
            winners.clear();
            winners.push_back(player);
            bestPlayer = player;
        } else if (this->scores.at(player) == this->scores.at(bestPlayer)) {
            winners.push_back(player);
        }
    }
    return winners;
}

bool CircleLogic::runNextRound()
{
    this->currentPlayer = (this->currentPlayer + 1) % this->numPlayers;
    if (this->remainingSteps[this->currentPlayer] <= 0) {
        // game over
        endGame();
        return false;
    }
    return true;
}

void CircleLogic::initCircleCounts()
{
    this->numCircles = 0;
    for (const std::vector<circle_ptr> &level: this->board.getLevels()) {
        this->numCircles += level.size();
    }
    this->numCircles--; // don't want to count outer circle
    int stepPerPlayer = this->numCircles / this->numPlayers;
    for (int player = 0; player < this->numPlayers; ++player) {
        this->remainingSteps[player] = stepPerPlayer;
    }
}

void CircleLogic::endGame()
{
    qInfo("Game over!");
    this->currentPlayer = GAME_OVER;
}

void CircleLogic::updateNeighbors(circle_ptr circle)
{
    const std::vector<circle_ptr> &neighbors = this->board.getGameNeighbors().at(circle);

    // flip neighboring circles (shift players)
    for (const circle_ptr &nb: neighbors) {
        int player = nb->getPlayer();
        if (player >= 0) {
            int newPlayer = (player + 1) % this->numPlayers;
            nb->setPlayer(newPlayer);
            this->scores[player]--;
            this->scores[newPlayer]++;
        }
    }

}
