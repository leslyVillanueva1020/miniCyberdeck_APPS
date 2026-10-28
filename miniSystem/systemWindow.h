#pragma once

#include <QWidget>

class QStackedWidget;

class systemWindow : public QWidget
{
public:
    explicit systemWindow(QWidget* parent = nullptr);

private:
    QStackedWidget* pageStack;
    QWidget* overviewPage;
    QWidget* settingsPage;
};