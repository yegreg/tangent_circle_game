#ifndef CIRCLELOGIC_H
#define CIRCLELOGIC_H

#include "circleboard.h"


/**
 * @brief The CircleLogic class
 * Handles game logic
 */
class CircleLogic
{
public:
    CircleLogic(int numPlayers = 2);
    void selectCircle(circle_ptr circle);
    void setUpBoard(double radius, QPointF center);
    const CircleBoard &getBoard() const;
    int getCurrentPlayer() const;

    static const int PLAYER_ONE = 0;
    static const int PLAYER_TWO = 1;
    static const int PLAYER_THREE = 2;

    /* minimum numbers of neighboring circlis belonging
     * to a given player required to convert a circle */
    static const int CONVERSION_MINIMUM = 3;

private:
    CircleBoard board;
    int currentPlayer;
    int numPlayers;
    void runNextRound();
    void updateNeighbors(circle_ptr circle);
    // number of neighbors of a circle belonging to a given player
    std::unordered_map<circle_ptr, std::unordered_map<int, int>> neighborCounts;
    void initNeighborCounts();

};

#endif // CIRCLELOGIC_H
