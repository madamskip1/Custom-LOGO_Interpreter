#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "turtle.h"
#include "Parser.h"
#include "Lexer.h"
#include "Logger.h"
#include "SourceReader.h"
#include "Interpreter.h"

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
  //  turtle = new Turtle(this->ui->drawingBoard);
  //  Turtle* turtle2 = new Turtle(this->ui->drawingBoard);
  //  this->ui->turtleBoard->addTurtle(turtle);
  //  this->ui->turtleBoard->addTurtle(turtle2);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString input = this->ui->inputBox->toPlainText();

    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    reader.setSourceString(input.toStdString());

    std::unique_ptr<AST::ProgramRootNode> program = parser.parse();

    if (!logger.hasAnyError())
    {
        Interpreter interpreter(std::move(program), this->ui->drawingBoard, this->ui->turtleBoard);
        try
        {
            interpreter.run();
        }
        catch(std::exception e)
        {
            qDebug() << e.what();
        }
    }

    QString output = QString::fromStdString(logger.toString());

    this->ui->outputBox->setText(output);

    // tutaj interpretować

}
