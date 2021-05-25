#include <QtMath>
#include <QDebug>
#include "turtle.h"
#include "drawingboard.h"

Turtle::Turtle(DrawingBoard* drawingBoardPtr)
{
    toStart();
    board = drawingBoardPtr;
}

Turtle::~Turtle()
{
    turtleBoard->removeTurtle(this);
}

void Turtle::setTurtleBoard(TurtleBoard *turtleBoardPtr)
{
    turtleBoard = turtleBoardPtr;
}

void Turtle::go(const int &distance)
{
    QPoint end = movePoint(position, direction, distance);
    board->drawLine(position, end, Qt::blue, 2);
    position = end;

    emit turtleMoved();
}

void Turtle::left(const int &angle)
{
    direction -= angle;
    emit turtleMoved();
}

void Turtle::right(const int &angle)
{
    direction += angle;
    direction = direction % 360;
    emit turtleMoved();
}

void Turtle::toStart()
{
    position.setX(DrawingBoard::BOARD_WIDTH / 2);
    position.setY(DrawingBoard::BOARD_HEIGHT / 2);
    direction = 0;
    emit turtleMoved();
}

void Turtle::move(int x, int y)
{
    position.setX(x);
    position.setY(y);
    emit turtleMoved();
}

void Turtle::toggleVisible()
{
    hidden = !hidden;
    emit turtleMoved();
}

void Turtle::hide()
{
    hidden = true;
    emit turtleMoved();
}

void Turtle::show()
{
    hidden = false;
    emit turtleMoved();
}

void Turtle::draw()
{
    if (hidden)
        return;

    QPoint top = movePoint(position, direction, 30);
    QPoint bottomLeft = movePoint(position, direction + 135, 15);
    QPoint bottomRight = movePoint(position, direction - 135, 15);

    turtleBoard->drawTurtle(top, bottomLeft, bottomRight, Qt::blue);
}
/*
void Turtle::callFunction(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers[0] == "go")
    {
        go(200);
    }
}*/

QPoint Turtle::movePoint(QPoint pos, int direct, int distance)
{
    if (distance == 0)
        return pos;

    if (direct == 0)
    {
        return QPoint(pos.x(), pos.y() - distance);
    }
    else if (direct == 90)
    {
        return QPoint(pos.x() + distance, pos.y());
    }
    else if (direct == 180)
    {
        return QPoint(pos.x(), pos.y() + distance);
    }
    else if (direct == 270)
    {
        return QPoint(pos.x() - distance, pos.y());
    }

    float realDirection = getRealDirection(direct);

    float radians = qDegreesToRadians(realDirection);
    float cos = qCos(radians);
    float sin = qSin(radians);

    return QPoint(pos.x() + distance * sin, pos.y() + distance * cos);
}

float Turtle::getRealDirection()
{
    return getRealDirection(direction);
}

float Turtle::getRealDirection(int direct)
{
    if (direct == 0 || direct == 180)
        return (direct + 180.0f);
    else
        return (180.0f - direct);
}
