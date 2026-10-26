#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QProcess>

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

        QProcess* terminalProcess;

        void runCommand();
        void readOutput();
        void readError();

        void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

};