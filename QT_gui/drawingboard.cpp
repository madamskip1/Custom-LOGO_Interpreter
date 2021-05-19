#include "drawingboard.h"

#include <QPainter>
#include <QStyleOption>
#include <QPaintEvent>
#include <QImage>

DrawingBoard::DrawingBoard(QWidget *parent) : QWidget(parent)
{
    board = QPixmap(DrawingBoard::BOARD_WIDTH, DrawingBoard::BOARD_HEIGHT);

}

void DrawingBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOption StyleOpt;

    StyleOpt.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &StyleOpt, &painter, this);

    QRect dirtyRect = event->rect();
    painter.drawPixmap(dirtyRect, board, dirtyRect);
}


void DrawingBoard::drawLine(const QPoint &start, const QPoint &end, const QColor color, const int &penwidth)
{
    QPainter painter(&board);
    painter.setPen(QPen(color, penwidth));
    painter.drawLine(start, end);
    int rad = penwidth / 2 + 2;
    update(QRect(start, end).normalized().adjusted(-rad, -rad, rad, rad));
}

