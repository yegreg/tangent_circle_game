#include "circlebot.h"
#include "circlelogic.h"
#include <random>
#include <time.h>

#include <QDebug>

CircleBot::CircleBot()
{

}

CircleBot::CircleBot(const CircleLogic *parentGame)
{
    this->parentGame = parentGame;
    qDebug() << "Unfilled from bot: " << this->parentGame->getUnfilledCircles().size();
    srand(time(NULL));
    qDebug("Bot set up.");
}

circle_ptr CircleBot::getNextStep() const
{
    return greedyNextStep();
}

circle_ptr CircleBot::greedyNextStep() const
{
    qDebug("Bot starts thinking");
    std::vector<circle_ptr> bestMoves;
    // most number of circles that can be permanently occupied
    int highestOccupation = 0;

    qDebug() << "Number of unfilled circles: " << parentGame->getUnfilledCircles().size();
    for (const circle_ptr &circle: parentGame->getUnfilledCircles()) {
        qDebug("Computing occupation");
        int occupation = computeOccupation(circle);
        qDebug("Occupation computed");
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

int CircleBot::getUnfilledNeighborCount(const circle_ptr &circle) const
{
    int allNeighbors = this->parentGame->getBoard().getGameNeighbors().at(circle).size();
    int filledNeighbors = this->parentGame->getFilledNeighborCount(circle);
    return allNeighbors - filledNeighbors;
}

int CircleBot::computeOccupation(const circle_ptr &circle) const
{
    int occupation = 0;
    int numPlayers = this->parentGame->getNumPlayers();
    const std::unordered_map<circle_ptr, std::vector<circle_ptr> > &neighbors =
            this->parentGame->getBoard().getGameNeighbors();
    const std::vector<circle_ptr> currentNbrs = neighbors.at(circle);
    qInfo("Looking at neighbors");
    for (const circle_ptr &currentNeighbor: currentNbrs) {
        if (getUnfilledNeighborCount(currentNeighbor) == 1) {
            if (currentNeighbor->getPlayer() == this->parentGame->getCurrentPlayer()) {
                occupation--;
            } else if (currentNeighbor->getPlayer()
                       == (this->parentGame->getCurrentPlayer() + numPlayers - 1) % numPlayers) {
                occupation++;
            }
        }
    }

    return occupation;
}




