#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QProcess>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QFrame>
#include <QListWidget>

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

        QFrame* historyDrawer;
        QListWidget* historyList;

        struct HistoryEntry{
            QString command;
            QDateTime timeStamp;
        };

        QList<HistoryEntry> history;

        void runCommand();
        void readOutput();
        void readError();
        void toggleHistory();

        void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

};