#include "HomeWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

HomeWindow:: HomeWindow(QWidget* parent): QWidget(parent){
    mainLayout = new QVBoxLayout(this);
    notesProcess = new QProcess(this);

    QVBoxLayout* placholder = new QVBoxLayout();

    QLabel* titleLabel = new QLabel("SOMETHING HEREEEE");
    QPushButton* btnOpenNotes = new QPushButton("Notes");

    placholder->addWidget(titleLabel);
    placholder->addWidget(btnOpenNotes);
    mainLayout->addLayout(placholder);

    connect(
        btnOpenNotes,
        &QPushButton:: clicked,
        this,
        &HomeWindow::openNotesApp
    );

    connect(notesProcess, &QProcess::started, this, [this](){
        qDebug() << "MiniNotes Started successfully!";
        hide();
    });

    connect(notesProcess, &QProcess::errorOccurred, this,
    [this](QProcess::ProcessError error){
        qDebug() << "MiniNotes failed to start:"
                 << notesProcess->errorString();
    });

    connect(notesProcess, &QProcess::finished, this,
    [this](int exitCode, QProcess::ExitStatus exitStatus){
        qDebug() << "MiniNotes closed.";
        qDebug() << "Exit code:" << exitCode;

        show();
    });
}

void HomeWindow::openNotesApp(){
    if(notesProcess->state() == QProcess::Running){
        qDebug() << "MiniNotes is already running.";
        return;
    }
    qDebug() << "Open Notes App button clicked!";

    QString notesProgram =
        "C:/Users/alexi/miniCyberdeck_APPS/MiniNotes/build/MiniNotes.exe";

    QString notesWorkingDirectory =
        "C:/Users/alexi/miniCyberdeck_APPS/MiniNotes";

    notesProcess->setWorkingDirectory(notesWorkingDirectory);
    notesProcess->start(notesProgram);   
}