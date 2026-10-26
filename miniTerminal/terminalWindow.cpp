#include "terminalWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QIcon>
#include <QDebug>
#include <QProcess>

terminalWindow:: terminalWindow(QWidget* parent): QWidget(parent){
    setObjectName("terminalWindow");
    mainLayout = new QVBoxLayout(this);

    terminalProcess = new QProcess(this);

    displayCommands = new QPlainTextEdit();
    displayCommands->setReadOnly(true);
    displayCommands->setObjectName("displayCommands");

    commandInput = new QLineEdit();
    commandInput->setPlaceholderText("user@cyberdeck:~$");
    commandInput->setObjectName("commandInput");

    btnClear = new QPushButton("");
    QIcon clearIcon("icons/trashh.svg");
    btnClear->setIcon(clearIcon);
    btnClear->setObjectName("btnClear");

    btnHistory = new QPushButton("");
    QIcon historyIcon("icons/rotate-ccw-clock.svg");
    btnHistory->setIcon(historyIcon);
    btnHistory->setObjectName("btnHistory");

    btnRun = new QPushButton("");
    QIcon playIcon("icons/play.svg");
    btnRun->setIcon(playIcon);
    btnRun->setObjectName("btnRun");

    bottomRow = new QHBoxLayout();
    bottomRow->addWidget(commandInput, 1);
    bottomRow->addWidget(btnClear);
    bottomRow->addWidget(btnHistory);
    bottomRow->addWidget(btnRun);

    historyDrawer = new QFrame();
    historyDrawer->setObjectName("historyDrawer");
    historyDrawer->setVisible(false);

    historyList = new QListWidget();
    historyList->setObjectName("historyList");

    QVBoxLayout* drawerLayout = new QVBoxLayout(historyDrawer);
    drawerLayout->addWidget(historyList);

    QHBoxLayout* mainRow = new QHBoxLayout();
    QVBoxLayout* terminalColumn = new QVBoxLayout();

    terminalColumn->addWidget(displayCommands);
    terminalColumn->addLayout(bottomRow);

    mainRow->addLayout(terminalColumn, 1);
    mainRow->addWidget(historyDrawer);

    mainLayout->addLayout(mainRow);


    connect(
        btnClear, //send signal
        &QPushButton::clicked, 
        displayCommands, //recieves signal
        &QPlainTextEdit ::clear //func that runs
    );

    connect(
        btnRun,
        &QPushButton::clicked,
        this,
        &terminalWindow::runCommand
    );

    // if enter pressed after typing run command as well
    connect(
        commandInput,
        &QLineEdit::returnPressed,
        this,
        &terminalWindow::runCommand
    );

    connect(
        terminalProcess,
        &QProcess::readyReadStandardOutput,
        this,
        &terminalWindow::readOutput
    );

    connect(
        terminalProcess,
        &QProcess::readyReadStandardError,
        this,
        &terminalWindow::readError
    );

    connect(
        terminalProcess,
        &QProcess::finished,
        this,
        &terminalWindow::processFinished
    );

    // not sure if commandInput should also be cleared
    connect(
        btnClear,
        &QPushButton::clicked,
        commandInput,
        &QLineEdit::clear
    );

    connect(
        btnHistory,
        &QPushButton::clicked,
        this,
        &terminalWindow::toggleHistory
    );
    
}

void terminalWindow:: runCommand(){
    QString command = commandInput->text().trimmed();

    if(command.isEmpty()){
        return;
    }

    HistoryEntry newEntry;

    newEntry.command = command;
    newEntry.timeStamp = QDateTime::currentDateTime();

    history.append(newEntry);

    displayCommands->appendPlainText(
        "user@cyberdeck:~$ " + command
    );

   #ifdef Q_OS_WIN
        terminalProcess->start("cmd.exe", {"/c", command});
    #else
        terminalProcess->start("/bin/sh", {"-c", command});
    #endif

    commandInput-> clear();
}

void terminalWindow::readOutput(){
    QByteArray output = terminalProcess->readAllStandardOutput();
    //qbyterarray- raw data recieved from the process
    //readAllStandardOutput- retrieves the data

    displayCommands->appendPlainText( //appendPlainText- adds to terminal display
        QString::fromLocal8Bit(output)
        //QString fromLocal8Bit-converts it into readable text
    );
}

void terminalWindow::readError(){
    QByteArray error = terminalProcess->readAllStandardError();

    displayCommands->appendPlainText(
        "[error] " + QString::fromLocal8Bit(error)
    );
}

void terminalWindow::processFinished( int exitCode, QProcess::ExitStatus exitStatus){
    if(exitStatus == QProcess::NormalExit){
        qDebug() << "Process finished with exit code: " << exitCode;
    }
    else{
        qDebug() << "Process crashed.";
    }
}

void terminalWindow:: toggleHistory(){
    if(historyDrawer->isVisible()){
        historyDrawer->setVisible(false);
    }
    else {
        historyDrawer->setVisible(true);
    }

}