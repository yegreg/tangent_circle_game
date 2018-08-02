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
    static const int DEFAULT_NUM_PLAYERS = 2;
    static const int DEFAULT_BOARD_DEPTH = 4;
    static const int DEFAULT_BOARD_SYMMETRY = 4;

    CircleLogic(int numPlayers=DEFAULT_NUM_PLAYERS,
                int boardDepth=DEFAULT_BOARD_DEPTH,
                int boardSymmetry=DEFAULT_BOARD_SYMMETRY);
    /**
     * @brief selectCircle
     * @param circle
     * @return false if game is over
     */
    bool selectCircle(circle_ptr circle);
    void setUpBoard(double radius, QPointF center);
    const CircleBoard &getBoard() const;
    int getCurrentPlayer() const;
    int getScore(int player) const;
    int getNumPlayers() const;
    int getRemainingSteps(int player) const;
    /**
     * @brief getWinners
     * @return list of players with highest score
     */
    std::vector<int> getWinners() const;

    static const int PLAYER_ONE;
    static const int PLAYER_TWO;
    static const int PLAYER_THREE;
    static const int GAME_OVER;

private:
    CircleBoard board;
    int numCircles;
    int currentPlayer;
    int numPlayers;
    int boardDepth;
    int boardSymmetry;

    std::unordered_map<int, int> scores;
    std::unordered_map<int, int> remainingSteps;

    bool runNextRound();
    void updateNeighbors(circle_ptr circle);
    void initCircleCounts();
    void endGame();

};

#endif // CIRCLELOGIC_H
