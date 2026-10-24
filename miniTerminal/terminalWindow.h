#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>

class terminalWindow: public QWidget{
    public:
        terminalWindow(QWidget* parent = nullptr);
    private:
        QVBoxLayout* mainLayout;
        QHBoxLayout* bottomRow;
        QPlainTextEdit* displayCommands;
        QLineEdit* commandInput;
        QPushButton* btnClear;
        QPushButton* btnHistory;
        QPushButton* btnRun;
};