#include "terminalWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>

terminalWindow:: terminalWindow(QWidget* parent): QWidget(parent){
    mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("terminalWindow");

    QPlainTextEdit* displayCommands = new QPlainTextEdit();
    displayCommands->setReadOnly(true);
    displayCommands->setObjectName("displayCommands");

    QLineEdit* commandInput = new QLineEdit();
    commandInput->setPlaceholderText("user@cyberdeck:~$");
    commandInput->setObjectName("commandInput");

    QPushButton* btnClear = new QPushButton("Clear");
    btnClear->setObjectName("btnClear");

    QPushButton* btnHistory = new QPushButton("History");
    btnHistory->setObjectName("btnHistory");

    QPushButton* btnRun = new QPushButton("Run");
    btnRun->setObjectName("btnRun");

    bottomRow = new QHBoxLayout();
    bottomRow->addWidget(commandInput);
    bottomRow->addWidget(btnClear);
    bottomRow->addWidget(btnHistory);
    bottomRow->addWidget(btnRun);

    mainLayout->addWidget(displayCommands);
    mainLayout->addLayout(bottomRow);
    
}