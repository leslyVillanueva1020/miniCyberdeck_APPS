#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class terminalWindow: public QWidget{
    public:
        terminalWindow(QWidget* parent = nullptr);
    private:
        QVBoxLayout* mainLayout;
        QHBoxLayout* bottomRow;
};