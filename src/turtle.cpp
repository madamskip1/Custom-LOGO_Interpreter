#include <QtMath>
#include <QDebug>
#include "../AST/Expression.h"
#include "turtle.h"
#include "drawingboard.h"
#include "Context.h"
#include "Point.h"

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
    Point end = movePoint(position, direction, distance);
    if (brush.getEnabled())
    {
        board->drawLine(QPoint(position.x, position.y), QPoint(end.x, end.y), QColor(QString::fromStdString(brush.getColor())), brush.getSize());
    }
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
    position.x = (DrawingBoard::BOARD_WIDTH / 2);
    position.y = (DrawingBoard::BOARD_HEIGHT / 2);
    direction = 0;
    emit turtleMoved();
}

void Turtle::moveTo(int x, int y)
{
    position.x = x;
    position.y = y;
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

    Point top = movePoint(position, direction, 30);
    Point bottomLeft = movePoint(position, direction + 135, 15);
    Point bottomRight = movePoint(position, direction - 135, 15);

    turtleBoard->drawTurtle(QPoint(top.x, top.y), QPoint(bottomLeft.x, bottomLeft.y), QPoint(bottomRight.x, bottomRight.y),  QColor(QString::fromStdString(brush.getColor())));
}

void Turtle::callFunction(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers[0] == "go")
    {
        go(std::get<int>(context->args.at("arg0")->value));
        return;
    }
    if (identifiers[0] == "left" || identifiers[0] == "right")
    {
        int angle = std::get<int>(context->args.at("arg0")->value);
        if (identifiers[0] == "right")
        {
            right(angle);
        }
        else
        {
            left(angle);
        }
        return;
    }
    else if (identifiers[0] == "moveTo")
    {
        qDebug() << "Dupa1";
        int arg_x = std::get<int>(context->args.at("arg0")->value);
        qDebug() << "Dupa2";
        qDebug() << arg_x;
        qDebug() << "Dupa3";
        int arg_y = std::get<int>(context->args.at("arg1")->value);
        qDebug() << "Dupa4";
        qDebug() << arg_y;
        moveTo(arg_x, arg_y);
    }

}

void Turtle::getSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers[0] == "pos")
    {
        position.getSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);
        return;
    }
    else if (identifiers[0] == "brush")
    {
        brush.getSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);
        return;
    }
    else if (identifiers[0] == "direction")
    {
        context->returnVariant = direction;
        return;
    }
    else if (identifiers[0] == "hidden")
    {
        context->returnVariant = hidden;
        return;
    }
}

void Turtle::setSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers[0] == "hidden")
    {
        hidden = std::get<bool>(context->setVariant);
        context->setVariant = std::monostate{};
        emit turtleMoved();
        return;
    }
    else if (identifiers[0] == "direction")
    {
        direction = std::get<int>(context->setVariant);
        context->setVariant = std::monostate{};
        emit turtleMoved();
        return;
    }
    else if (identifiers[0] == "brush")
    {
        brush.setSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);
        emit turtleMoved();
        return;
    }
    else if (identifiers[0] == "pos")
    {
        position.setSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);
        emit turtleMoved();
        return;
    }
}



Point Turtle::movePoint(Point pos, int direct, int distance)
{
    if (distance == 0)
        return pos;

    if (direct == 0)
    {
        return Point(position.x, position.y - distance);
    }
    else if (direct == 90)
    {
        return Point(position.x + distance, position.y);
    }
    else if (direct == 180)
    {
        return Point(position.x, position.y + distance);
    }
    else if (direct == 270)
    {
        return Point(position.x - distance, position.y);
    }

    float realDirection = getRealDirection(direct);

    float radians = qDegreesToRadians(realDirection);
    float cos = qCos(radians);
    float sin = qSin(radians);

    return Point(position.x + distance * sin, position.y + distance * cos);
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
