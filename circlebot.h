#ifndef CIRCLEBOT_H
#define CIRCLEBOT_H

#include "circleboard.h"

class CircleLogic;

class CircleBot
{
public:
    CircleBot();
    CircleBot(const CircleLogic* parentGame);

    circle_ptr getNextStep() const;
private:
    const CircleLogic* parentGame;

    circle_ptr greedyNextStep() const;
    int computeOccupation(const circle_ptr &circle) const;
    int getUnfilledNeighborCount(const circle_ptr &circle) const;

};

#endif // CIRCLEBOT_H
