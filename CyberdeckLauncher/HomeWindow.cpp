#include "HomeWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

HomeWindow:: HomeWindow(QWidget* parent): QWidget(parent){
    pageStack = new QStackedWidget(this);

    page1 = new QWidget();
    gridApps1 = new QGridLayout();

    page2 = new QWidget();
    gridApps2 = new QGridLayout();

    page1->setLayout(gridApps1);
    page2->setLayout(gridApps2);

    pageStack->addWidget(page1);
    pageStack->addWidget(page2);

    mainLayout = new QVBoxLayout(this);
    notesProcess = new QProcess(this);

    // ================ PAGE 1 =============================
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* appName = new QLabel("AQUARIUM LS");
    QLabel* time = new QLabel("10:34 AM");
    QLabel* battery = new QLabel("87%");
    header->addWidget(appName);
    header->addWidget(time);
    header->addWidget(battery);

    QPushButton* btnOpenNotes = new QPushButton("Notes");
    QPushButton* btnOpenMusic = new QPushButton("Music");
    QPushButton* btnOpenBrowser = new QPushButton("Browser");
    QPushButton* btnOpenSettings = new QPushButton("Settings");
    gridApps1->addWidget(btnOpenNotes, 0,0); //0, 0 reams row 0 column 0
    gridApps1->addWidget(btnOpenMusic, 0, 1);
    gridApps1->addWidget(btnOpenBrowser, 1,0);
    gridApps1->addWidget(btnOpenSettings,1, 1);

    // ======================= PAGE 2 =============================
    

    //============= ADDING TO MAIN LAYOUT ========================
    mainLayout->addLayout(header);
    mainLayout->addWidget(pageStack);

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