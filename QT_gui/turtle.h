#ifndef TURTLE_H
#define TURTLE_H

#include <QObject>
#include <QPoint>
#include "drawingboard.h"
#include "turtleboard.h"

class TurtleBoard;

class Turtle
        : public QObject
{
    Q_OBJECT
public:
    Turtle(DrawingBoard* drawingBoardPtr);
    void setTurtleBoard(TurtleBoard* turtleBoardPtr);

    void go(const int& distance);
    void left(const int& angle);
    void right(const int& angle);
    void toStart();
    void move(int x, int y);

    void draw();

    bool hidden = true;
    int direction;

signals:
    void turtleMoved();

private:
    DrawingBoard* board;
    TurtleBoard* turtleBoard;
    QPoint position;

    QPoint movePoint(QPoint pos, int direct, int distance);
    float getRealDirection();
    float getRealDirection(int direct);
};

#endif // TURTLE_H
