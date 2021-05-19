QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Parser/Assignable.cpp \
    Parser/AssignmentStatement.cpp \
    Parser/Boolean.cpp \
    Parser/CallFuncStatement.cpp \
    Parser/ClassAssignment.cpp \
    Parser/Color.cpp \
    Parser/Condition.cpp \
    Parser/DefFuncStatement.cpp \
    Parser/Expression.cpp \
    Parser/IfStatement.cpp \
    Parser/InstructionsBlock.cpp \
    Parser/Lexer.cpp \
    Parser/Log.cpp \
    Parser/Logger.cpp \
    Parser/Node.cpp \
    Parser/Number.cpp \
    Parser/Parameter.cpp \
    Parser/Parser.cpp \
    Parser/ProgramRootNode.cpp \
    Parser/RepeatStatement.cpp \
    Parser/RepeatTimeStatement.cpp \
    Parser/ReturnStatement.cpp \
    Parser/SourceReader.cpp \
    Parser/Token.cpp \
    Parser/VarDeclare.cpp \
    Parser/Variable.cpp \
    drawingboard.cpp \
    main.cpp \
    mainwindow.cpp \
    turtle.cpp \
    turtleboard.cpp

HEADERS += \
    Parser/Assignable.h \
    Parser/AssignmentStatement.h \
    Parser/Boolean.h \
    Parser/CallFuncStatement.h \
    Parser/ClassAssignment.h \
    Parser/Color.h \
    Parser/Condition.h \
    Parser/DefFuncStatement.h \
    Parser/Expression.h \
    Parser/IfStatement.h \
    Parser/InstructionsBlock.h \
    Parser/Lexer.h \
    Parser/Log.h \
    Parser/Logger.h \
    Parser/Node.h \
    Parser/Number.h \
    Parser/Parameter.h \
    Parser/Parser.h \
    Parser/ProgramRootNode.h \
    Parser/RepeatStatement.h \
    Parser/RepeatTimeStatement.h \
    Parser/ReturnStatement.h \
    Parser/SourceReader.h \
    Parser/Token.h \
    Parser/TokenType.h \
    Parser/VarDeclare.h \
    Parser/Variable.h \
    drawingboard.h \
    mainwindow.h \
    turtle.h \
    turtleboard.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
