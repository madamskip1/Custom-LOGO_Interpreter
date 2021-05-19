#ifndef TURTLEBOARD_H
#define TURTLEBOARD_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include "turtle.h"

class Turtle;

class TurtleBoard : public QWidget
{
    Q_OBJECT
public:
    TurtleBoard(QWidget * parent = nullptr);
    void drawTurtle(QPoint top, QPoint bottomLeft, QPoint bottomRight, QColor color);

    void addTurtle(Turtle* turtle);

public slots:
    void redrawTurtles();

private:
    void paintEvent(QPaintEvent * event);

    QPixmap board;
    QVector<Turtle*> turtles;

};

#endif // TURTLEBOARD_H
