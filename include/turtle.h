#ifndef TURTLE_H
#define TURTLE_H

#include <QObject>
#include <QPoint>
#include "drawingboard.h"
#include "turtleboard.h"
#include "Variable.h"
#include "Context.h"

class TurtleBoard;

class Turtle
        : public QObject, public Variable
{
    Q_OBJECT
public:
    Turtle(DrawingBoard* drawingBoardPtr);
    ~Turtle();
    void setTurtleBoard(TurtleBoard* turtleBoardPtr);

    void go(const int& distance);
    void left(const int& angle);
    void right(const int& angle);
    void toStart();
    void move(int x, int y);

    void toggleVisible();
    void hide();
    void show();

    void draw();

    bool hidden = false;
    int direction;

    //void callFunction(std::vector<std::string> identifiers, Context* context);

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
