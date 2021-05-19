#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "turtle.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->outputBox->setText("Error line 10: XXCXXX \n Warning line 20: Xxwqewqe");
    QString code =
    R"(repeat(10) {
        zolw1.go(10);
    }
    zolw1.brush.color.set("#000000"); // XXXXXX COMMENT
    zolw2.hidden = true;
    if (x < 20) {								// XXXXXX COMMENT
        zolw.go(2 + 2);
    } else {
        zolw.go(x - 20);
    }
    Turtle zolwik;
     Point point(1, 0);
    )";
    ui->inputBox->setText(code);

  //  this->ui->turtleboard->show();
    turtle = new Turtle(this->ui->drawingBoard);
    Turtle* turtle2 = new Turtle(this->ui->drawingBoard);
    this->ui->turtleBoard->addTurtle(turtle);
    this->ui->turtleBoard->addTurtle(turtle2);
    turtle->go(200);
    delete turtle2;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    for(int i = 0; i < 60; i++)
    {
        turtle->go(20);
        turtle->right(360/60);
    }
}
