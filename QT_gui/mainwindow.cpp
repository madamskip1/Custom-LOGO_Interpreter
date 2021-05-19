#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "turtle.h"
#include "Parser/Parser.h"
#include "Parser/Lexer.h"
#include "Parser/Logger.h"
#include "Parser/SourceReader.h"

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
    turtle->toggleVisible();
    turtle->go(200);
    turtle2->right(90);
    turtle2->go(20);
    turtle2->move(50, 50);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString input = this->ui->inputBox->toPlainText();

    SourceReader* reader = new SourceReader();
    Lexer* lexer = new Lexer(reader);
    Logger* logger = new Logger();
    Parser parser(lexer, logger);

    reader->setSourceString(input.toStdString());
    parser.parse();

    QString output = QString::fromStdString(logger->toString());

    this->ui->outputBox->setText(output);

    // tutaj interpretować

}
