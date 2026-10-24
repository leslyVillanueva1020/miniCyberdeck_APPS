#include "terminalWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QIcon>
#include <QDebug>

terminalWindow:: terminalWindow(QWidget* parent): QWidget(parent){
    setObjectName("terminalWindow");
    mainLayout = new QVBoxLayout(this);

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

    mainLayout->addWidget(displayCommands);
    mainLayout->addLayout(bottomRow, 0);

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

    //not sure if commandInput should also be cleared
    // connect(
    //     btnClear,
    //     &QPushButton::clicked,
    //     commandInput,
    //     &QLineEdit::clear
    // );
    
}

void terminalWindow:: runCommand(){
    QString command = commandInput->text().trimmed();

    if(command.isEmpty()){
        return;
    }

    displayCommands->appendPlainText(
        "user@cyberdeck:~$ " + command
    );

    commandInput-> clear();
}