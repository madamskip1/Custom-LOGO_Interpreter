QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/include

QMAKE_CXXFLAGS_WARN_OFF = -Wunused-parameter
QMAKE_CXXFLAGS_WARN_OFF = -Wno-unused-parameter
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter
QMAKE_CXXFLAGS_WARN_OFF -= -Wno-unused-parameter

SOURCES += \
    AST/ArithmeticExpression.cpp \
    AST/Assignable.cpp \
    AST/AssignmentStatement.cpp \
    AST/Boolean.cpp \
    AST/CallFuncStatement.cpp \
    AST/ClassAssignment.cpp \
    AST/Color.cpp \
    AST/Condition.cpp \
    AST/DefFuncStatement.cpp \
    AST/DeleteStatement.cpp \
    AST/Expression.cpp \
    AST/IfStatement.cpp \
    AST/InstructionsBlock.cpp \
    AST/Node.cpp \
    AST/Number.cpp \
    AST/Parameter.cpp \
    AST/ProgramRootNode.cpp \
    AST/RepeatConditionStatement.cpp \
    AST/RepeatStatement.cpp \
    AST/RepeatTimeStatement.cpp \
    AST/ReturnStatement.cpp \
    AST/VarDeclare.cpp \
    AST/VariableExpression.cpp \
    main.cpp \
    mainwindow.cpp \
    src/BlockScope.cpp \
    src/Brush.cpp \
    src/Context.cpp \
    src/Interpreter.cpp \
    src/Lexer.cpp \
    src/Log.cpp \
    src/Logger.cpp \
    src/Node.cpp \
    src/Parser.cpp \
    src/Point.cpp \
    src/SourceReader.cpp \
    src/Token.cpp \
    src/TurtleStdFunc.cpp \
    src/Variable.cpp \
    src/drawingboard.cpp \
    src/turtle.cpp \
    src/turtleboard.cpp \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AST/ArithmeticExpression.h \
    AST/Assignable.h \
    AST/AssignmentStatement.h \
    AST/Boolean.h \
    AST/CallFuncStatement.h \
    AST/ClassAssignment.h \
    AST/Color.h \
    AST/Condition.h \
    AST/DefFuncStatement.h \
    AST/DeleteStatement.h \
    AST/Expression.h \
    AST/IfStatement.h \
    AST/InstructionsBlock.h \
    AST/Node.h \
    AST/Number.h \
    AST/Parameter.h \
    AST/ProgramRootNode.h \
    AST/RepeatConditionStatement.h \
    AST/RepeatStatement.h \
    AST/RepeatTimeStatement.h \
    AST/ReturnStatement.h \
    AST/VarDeclare.h \
    AST/VariableExpression.h \
    deletestatement.h \
    include/BlockScope.h \
    include/Brush.h \
    include/Context.h \
    include/Interpreter.h \
    include/Lexer.h \
    include/Log.h \
    include/Logger.h \
    include/Node.h \
    include/Parser.h \
    include/Point.h \
    include/SourceReader.h \
    include/Token.h \
    include/TokenType.h \
    include/TurtleStdFunc.h \
    include/Variable.h \
    include/drawingboard.h \
    include/turtle.h \
    include/turtleboard.h \
    mainwindow.h \
    tests/catch.hpp
