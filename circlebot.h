#ifndef CIRCLEBOT_H
#define CIRCLEBOT_H

#include "circleboard.h"

class CircleLogic;

class CircleBot
{
public:
    CircleBot();
    CircleBot(const CircleLogic* parentGame,
              int botLevel=SMART_GREEDY_BOT,
              double randomStepProbability=DEFAULT_RANDOM_STEP_PROB);

    circle_ptr getNextStep() const;

    static const int RANDOM_BOT = 0;
    static const int SILLY_GREEDY_BOT = 1;
    static const int PART_RANDOM_BOT = 2;
    static const int SMART_GREEDY_BOT = 3;

    static const int DEFAULT_BOT_DIFFICULTY = SMART_GREEDY_BOT;

    static constexpr double DEFAULT_RANDOM_STEP_PROB = 0.5;

private:
    const CircleLogic* parentGame;
    int botLevel;
    double randomStepProbability;

    circle_ptr greedyNextStep() const;
    circle_ptr randomNextStep() const;
    circle_ptr sillyGreedyNextStep() const;
    circle_ptr partRandomNextStep() const;
    circle_ptr generalGreedyStep(int (*occupationFunction)(const circle_ptr &circle)) const;

    int computeOccupation(const circle_ptr &circle) const;
    int computeNaiveOccupation(const circle_ptr &circle) const;
    int getUnfilledNeighborCount(const circle_ptr &circle) const;

};

#endif // CIRCLEBOT_H
