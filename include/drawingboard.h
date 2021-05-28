#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <QWidget>
#include <QPixmap>

class DrawingBoard : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingBoard(QWidget *parent = nullptr);
    static const int BOARD_WIDTH = 480;
    static const int BOARD_HEIGHT = 670;
    void drawLine(const QPoint& start, const QPoint& end, const QColor& color, const int& penwidth);
    void cleanBoard();
    void updateBoard();

signals:

private:
    void paintEvent(QPaintEvent *event);
    QPixmap board;

};

#endif // DRAWINGBOARD_H
