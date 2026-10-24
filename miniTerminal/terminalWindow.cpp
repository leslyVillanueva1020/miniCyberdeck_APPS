#include "terminalWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QIcon>

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
    
}