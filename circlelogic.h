#ifndef CIRCLELOGIC_H
#define CIRCLELOGIC_H

#include "circleboard.h"
#include "circlebot.h"
#include <unordered_set>

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
    static const int DEFAULT_BOT_PLAYER = 1; // bot is second player by default

    CircleLogic(int numPlayers=DEFAULT_NUM_PLAYERS,
                int boardDepth=DEFAULT_BOARD_DEPTH,
                int boardSymmetry=DEFAULT_BOARD_SYMMETRY,
                int botPlayer=DEFAULT_BOT_PLAYER,
                int botDifficulty=CircleBot::DEFAULT_BOT_DIFFICULTY);
    /**
     * @brief selectCircle
     * @param circle
     * @return false if game is over
     */
    bool selectCircle(circle_ptr circle);
    void setUpBoard(double radius, QPointF center);
    void initBot();
    const CircleBoard &getBoard() const;
    int getCurrentPlayer() const;
    int getScore(int player) const;
    int getNumPlayers() const;
    int getRemainingSteps(int player) const;
    bool botStep();
    /**
     * @brief getWinners
     * @return list of players with highest score
     */
    std::vector<int> getWinners() const;
    int getFilledNeighborCount(const circle_ptr& circle) const;
    const std::unordered_set<circle_ptr> &getUnfilledCircles() const;

    static const int NO_PLAYER;
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
    /**
     * @brief botOn is bot turned on?
     */
    bool botOn;
    /**
     * @brief botPlayer stores which player is the bot
     */
    int botPlayer;
    int botDifficulty;
    /**
     * @brief bot playing the opponent
     */
    CircleBot bot;

    std::unordered_map<int, int> scores;
    std::unordered_map<int, int> remainingSteps;

    std::unordered_set<circle_ptr> unFilledCircles;
    std::unordered_map<circle_ptr, int> filledNeighborCounts;

    bool runNextRound();
    void updateNeighbors(circle_ptr circle);
    void initCircleCounts();
    void endGame();

};

#endif // CIRCLELOGIC_H
