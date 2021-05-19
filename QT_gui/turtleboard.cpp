#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include "turtleboard.h"
#include "drawingboard.h"

TurtleBoard::TurtleBoard(QWidget *parent) : QWidget(parent)
{
    board = QPixmap(DrawingBoard::BOARD_WIDTH, DrawingBoard::BOARD_HEIGHT);
    board.fill(Qt::transparent);
}

void TurtleBoard::drawTurtle(QPoint top, QPoint bottomLeft, QPoint bottomRight, QColor color)
{
    QPainter painter(&board);
    painter.setPen(color);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    QPoint *points = new QPoint[3];
    points[0] = top;
    points[1] = bottomLeft;
    points[2] = bottomRight;
    painter.drawPolygon(points, 3);

    update();
}

void TurtleBoard::setAllToStart()
{
    for (const auto turtle : turtles)
        turtle->toStart();
}

void TurtleBoard::addTurtle(Turtle *turtle)
{
    turtles.push_back(turtle);
    turtle->setTurtleBoard(this);
    QObject::connect(turtle, &Turtle::turtleMoved, this, &TurtleBoard::redrawTurtles);
    redrawTurtles();
}

void TurtleBoard::removeTurtle(Turtle *turtle)
{
    auto iterator = std::find(turtles.begin(), turtles.end(), turtle);
    if (iterator != turtles.end())
    {
        turtles.erase(iterator);
    }
    redrawTurtles();
}

void TurtleBoard::redrawTurtles()
{
    board.fill(Qt::transparent);
    for (const auto turtle : turtles)
        turtle->draw();
}

void TurtleBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOption StyleOpt;

    StyleOpt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &StyleOpt, &painter, this);

    QRect dirtyRect = event->rect();
    painter.drawPixmap(dirtyRect, board, dirtyRect);
}
