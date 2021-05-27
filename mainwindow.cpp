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
        try
        {
            if (!mainContext)
            {
                mainContext = std::make_unique<Context>();
                mainContext->setDrawingBoard(this->ui->drawingBoard);
                mainContext->setTurtleBoard(this->ui->turtleBoard);
            }
            Interpreter interpreter(std::move(program), mainContext.get());
            interpreter.run();
            this->ui->drawingBoard->updateBoard();
            this->ui->turtleBoard->updateBoard();
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
