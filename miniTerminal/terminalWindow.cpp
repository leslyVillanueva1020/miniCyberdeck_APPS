#include "terminalWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

terminalWindow:: terminalWindow(QWidget* parent): QWidget(parent){
    mainLayout = new QVBoxLayout(this);

    QLabel* placeholderLabel = new QLabel("Adding placeholder things to ensure when i run the cmake file something appears.");
    QPushButton* placeholderBtn = new QPushButton("Testing");

    mainLayout->addWidget(placeholderLabel);
    mainLayout->addWidget(placeholderBtn);
    
}