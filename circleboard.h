#ifndef CIRCLEBOARD_H
#define CIRCLEBOARD_H
#include "circle.h"
#include <unordered_map>
#include <memory>

typedef std::shared_ptr<Circle> circle_ptr;
/**
 * @brief The CircleBoard class
 * Generates and manages circles on the board
 */
class CircleBoard
{
public:
    CircleBoard();
    CircleBoard(double outer_radius, QPointF center);
    void generateBoard(double outer_radius, QPointF center);
    /**
     * @brief getBaseCircle
     * @return base circle of the board
     */
    const circle_ptr &getBaseCircle() const;
    /**
     * @brief getCircles
     * @return all levels of the board
     */
    const std::vector<std::vector<circle_ptr>> &getLevels() const;
    const std::unordered_map<circle_ptr, std::vector<circle_ptr> > &getGameNeighbors() const;

private:
    QPointF rotate(double x, double y, double angle);
    std::vector<std::vector<circle_ptr>> levels;
    circle_ptr baseCircle;
    // list of neighbors for each circle
    std::unordered_map<circle_ptr, std::vector<circle_ptr>> neighbors;
    // neighbors relevant for the game (i.e. not including baseCircle)
    std::unordered_map<circle_ptr, std::vector<circle_ptr>> gameNeighbors;
    void initGameNeighbors();
};

#endif // CIRCLEBOARD_H
