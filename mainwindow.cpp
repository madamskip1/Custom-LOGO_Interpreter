#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
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

    runScript(input.toStdString(), false);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Otwórz plik");
        if(fileName == "") return;

    runScript(fileName.toStdString(), true);
}

void MainWindow::runScript(std::string scriptString, bool isFile)
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);

    if(isFile)
    {
        reader.setSourceFile(scriptString);
    }
    else
    {
        reader.setSourceString(scriptString);
    }

    std::unique_ptr<AST::ProgramRootNode> program = parser.parse();

    if (!Logger::hasAnyError())
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
        catch(const std::exception& e)
        {
            qDebug() << e.what();
        }
    }

    QString output = QString::fromStdString(Logger::toString());
    Logger::clearLogs();
    this->ui->outputBox->setText(output);
}

void MainWindow::on_pushButton_3_clicked()
{
    mainContext.reset();
    this->ui->drawingBoard->cleanBoard();
    this->ui->drawingBoard->updateBoard();
    this->ui->turtleBoard->updateBoard();
}
