#include "circlebot.h"
#include "circlelogic.h"
#include <random>
#include <time.h>

#include <QDebug>

CircleBot::CircleBot()
{

}

CircleBot::CircleBot(const CircleLogic *parentGame, int botLevel, double randomStepProbability)
{
    this->parentGame = parentGame;
    this->botLevel = botLevel;
    this->randomStepProbability = randomStepProbability;
    srand(time(NULL));
}

circle_ptr CircleBot::getNextStep() const
{
    switch (this->botLevel) {
    case RANDOM_BOT: return randomNextStep();
    case SILLY_GREEDY_BOT: return sillyGreedyNextStep();
    case PART_RANDOM_BOT: return partRandomNextStep();
    case SMART_GREEDY_BOT: return greedyNextStep();
    default: {
        qDebug("Unknown bot level.");
        return greedyNextStep();
    }
    }
}

circle_ptr CircleBot::greedyNextStep() const
{
    std::vector<circle_ptr> bestMoves;
    // most number of circles that can be permanently occupied
    int highestOccupation = INT_MIN;

    for (const circle_ptr &circle: parentGame->getUnfilledCircles()) {
        int occupation = computeOccupation(circle);
        if (occupation > highestOccupation) {
            highestOccupation = occupation;
            bestMoves.clear();
            bestMoves.push_back(circle);
        } else if (occupation == highestOccupation) {
            bestMoves.push_back(circle);
        }
    }

    int randomIndex = rand() % bestMoves.size();
    return bestMoves.at(randomIndex);

}

circle_ptr CircleBot::randomNextStep() const
{
    const std::unordered_set<circle_ptr> &unoccupied = parentGame->getUnfilledCircles();
    int randomIndex = rand() % unoccupied.size();
    auto it = std::next(std::begin(unoccupied), randomIndex);
    return *it;
}

circle_ptr CircleBot::sillyGreedyNextStep() const
{
    std::vector<circle_ptr> bestMoves;
    // most number of circles that can be permanently occupied
    int highestOccupation = INT_MIN;

    for (const circle_ptr &circle: parentGame->getUnfilledCircles()) {
        int occupation = computeNaiveOccupation(circle);
        if (occupation > highestOccupation) {
            highestOccupation = occupation;
            bestMoves.clear();
            bestMoves.push_back(circle);
        } else if (occupation == highestOccupation) {
            bestMoves.push_back(circle);
        }
    }

    int randomIndex = rand() % bestMoves.size();
    return bestMoves.at(randomIndex);
}

circle_ptr CircleBot::partRandomNextStep() const
{
    double r = ((double) rand() / (RAND_MAX));
    if (r < this->randomStepProbability) {
        return randomNextStep();
    } else {
        return greedyNextStep();
    }
}

int CircleBot::getUnfilledNeighborCount(const circle_ptr &circle) const
{
    int allNeighbors = this->parentGame->getBoard().getGameNeighbors().at(circle).size();
    int filledNeighbors = this->parentGame->getFilledNeighborCount(circle);
    return allNeighbors - filledNeighbors;
}

/**
 * @brief CircleBot::computeOccupation
 * computes number of circles permanently occupied by this step
 * (a circle is permanently occupied, if it belongs to us, and all of
 * its neighbors are filled, so it cannot be switched back)
 * @param circle
 * @return
 */
int CircleBot::computeOccupation(const circle_ptr &circle) const
{
    int occupation = 0;
    int numPlayers = this->parentGame->getNumPlayers();
    const std::unordered_map<circle_ptr, std::vector<circle_ptr> > &neighbors =
            this->parentGame->getBoard().getGameNeighbors();
    const std::vector<circle_ptr> currentNbrs = neighbors.at(circle);
    // keeps track if all neighbors are filled
    bool allNeighborsFilled = true;
    // check how many neighbors are occupied - given to opponent
    for (const circle_ptr &currentNeighbor: currentNbrs) {
        if (getUnfilledNeighborCount(currentNeighbor) == 1) {
            if (currentNeighbor->getPlayer() == this->parentGame->getCurrentPlayer()) {
                occupation--;
            } else if (currentNeighbor->getPlayer()
                       == (this->parentGame->getCurrentPlayer() + numPlayers - 1) % numPlayers) {
                occupation++;
            }
        }
        if (currentNeighbor->getPlayer() == CircleLogic::NO_PLAYER) {
            allNeighborsFilled = false;
        }
    }

    if (allNeighborsFilled) {
        // selected circle will also be permanently occupied
        occupation++;
    }

    return occupation;
}

/**
 * @brief CircleBot::computeNaiveOccupation
 * Computes the number of circles that get switched to our color minus
 * the ones that are switched from our color to opponent
 * @param circle
 * @return
 */
int CircleBot::computeNaiveOccupation(const circle_ptr &circle) const
{
    int occupation = 0;
    int numPlayers = this->parentGame->getNumPlayers();
    const std::unordered_map<circle_ptr, std::vector<circle_ptr> > &neighbors =
            this->parentGame->getBoard().getGameNeighbors();
    const std::vector<circle_ptr> currentNbrs = neighbors.at(circle);
    for (const circle_ptr &currentNeighbor: currentNbrs) {
        if (currentNeighbor->getPlayer() == this->parentGame->getCurrentPlayer()) {
            occupation--;
        } else if (currentNeighbor->getPlayer()
                   == (this->parentGame->getCurrentPlayer() + numPlayers - 1) % numPlayers) {
            occupation++;
        }
    }
    return occupation;
}




