#pragma once

#include <QWidget>
#include <QLabel>
#include <QTimer>

class QStackedWidget;

class systemWindow : public QWidget
{
public:
    explicit systemWindow(QWidget* parent = nullptr);

private:
    QStackedWidget* pageStack;
    QWidget* overviewPage;
    QWidget* settingsPage;

    QLabel* timeLabel;
    QLabel* dateLabel;
    QTimer* clockTimer;

    void updateClock();
};