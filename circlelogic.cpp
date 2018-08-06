#include "circlelogic.h"
#include <QDebug>

const int CircleLogic::NO_PLAYER = -1; // unoccupied
const int CircleLogic::PLAYER_ONE = 0;
const int CircleLogic::PLAYER_TWO = 1;
const int CircleLogic::PLAYER_THREE = 2;
const int CircleLogic::GAME_OVER = -2;

CircleLogic::CircleLogic(int numPlayers, int boardDepth, int boardSymmetry, int botPlayer, int botDifficulty)
{
    if (numPlayers == 1) {
        qInfo("Setting up bot.");
        this->numPlayers = 2;
        this->botOn = true;
        this->botPlayer = botPlayer;
        this->botDifficulty = botDifficulty;
        qDebug() << "Bot is player " << this->botPlayer;
        qDebug() << "Bot difficulty: " << this->botDifficulty;
    } else {
        this->numPlayers = numPlayers;
        this->botOn = false;
    }
    this->boardDepth = boardDepth;
    this->boardSymmetry = boardSymmetry;
    this->currentPlayer = 0;

    for (int player = 0; player < this->numPlayers; ++player) {
        this->scores[player] = 0;
    }
}

bool CircleLogic::selectCircle(circle_ptr circle)
{
    if (this->remainingSteps[currentPlayer] <= 0) {
        return false;
    }
    if (circle->getPlayer() == NO_PLAYER) {
        circle->setPlayer(this->currentPlayer);
        this->scores[currentPlayer]++;
        this->unFilledCircles.erase(circle);
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
    if (this->botOn) {
        initBot();
    }
}

void CircleLogic::initBot()
{
    this->bot = CircleBot(this, this->botDifficulty);
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

int CircleLogic::getFilledNeighborCount(const circle_ptr &circle) const
{
    return this->filledNeighborCounts.at(circle);
}

const std::unordered_set<circle_ptr> &CircleLogic::getUnfilledCircles() const
{
    return this->unFilledCircles;
}

bool CircleLogic::runNextRound()
{
    this->currentPlayer = (this->currentPlayer + 1) % this->numPlayers;
    if (this->remainingSteps[this->currentPlayer] <= 0) {
        // game over
        endGame();
        return false;
    }
    if (this->botOn && this->currentPlayer == this->botPlayer) {
        return botStep();
    }
    return true;
}

bool CircleLogic::botStep() {
    return this->selectCircle(this->bot.getNextStep());
}

void CircleLogic::initCircleCounts()
{
    this->unFilledCircles.clear();
    this->numCircles = 0;
    for (const std::vector<circle_ptr> &level: this->board.getLevels()) {
        this->numCircles += level.size();
        for (const circle_ptr &circle: level) {
            this->unFilledCircles.insert(circle);
            this->filledNeighborCounts[circle] = 0;
        }
    }
    this->numCircles--; // don't want to count outer circle
    this->unFilledCircles.erase(this->board.getBaseCircle());
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
        if (player > NO_PLAYER) {
            int newPlayer = (player + 1) % this->numPlayers;
            nb->setPlayer(newPlayer);
            this->scores[player]--;
            this->scores[newPlayer]++;
        }
        this->filledNeighborCounts[nb]++;
    }

}
