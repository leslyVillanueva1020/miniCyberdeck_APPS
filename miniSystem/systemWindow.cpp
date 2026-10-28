#include "systemWindow.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStackedWidget>
#include <QVBoxLayout>

systemWindow::systemWindow(QWidget* parent): QWidget(parent)
{
    setWindowTitle("miniSystem");
    resize(480, 320);

    // The main window contains the page stack.
    auto* windowLayout = new QVBoxLayout(this);

    pageStack = new QStackedWidget(this);
    windowLayout->addWidget(pageStack);

    // The stack contains two separate pages.
    overviewPage = new QWidget();
    settingsPage = new QWidget();

    pageStack->addWidget(overviewPage);
    pageStack->addWidget(settingsPage);

    // -------- Overview page --------
    auto* overviewLayout = new QVBoxLayout(overviewPage);

    auto* titleRow = new QHBoxLayout();
    titleRow->addWidget(new QLabel("Aquarium LS miniSystem"));
    titleRow->addStretch();
    titleRow->addWidget(new QLabel("🐟🫧"));
    overviewLayout->addLayout(titleRow);

    auto* statusRow = new QHBoxLayout();

    auto* clockColumn = new QVBoxLayout();
    clockColumn->addWidget(new QLabel("11:37"));
    clockColumn->addWidget(new QLabel("Tue, Oct 28"));
    statusRow->addLayout(clockColumn);

    auto* statusColumn = new QVBoxLayout();
    statusColumn->addWidget(new QLabel("Battery 82% — not charging"));
    statusColumn->addWidget(new QLabel("Wi-Fi connected: Home Wi-Fi"));
    statusRow->addLayout(statusColumn);

    overviewLayout->addLayout(statusRow);

    auto* deviceRow = new QHBoxLayout();
    deviceRow->addWidget(new QLabel("Raspberry Pi"));

    auto* deviceInfo = new QVBoxLayout();
    deviceInfo->addWidget(new QLabel("Raspberry Pi Zero 2 W"));
    deviceInfo->addWidget(new QLabel("IP 192.168.1.42"));
    deviceInfo->addWidget(new QLabel("Storage 64%"));
    deviceRow->addLayout(deviceInfo);

    overviewLayout->addLayout(deviceRow);
    overviewLayout->addStretch();

    auto* overviewButtons = new QHBoxLayout();
    auto* btnHome = new QPushButton("Home");
    auto* btnShutdown = new QPushButton("Shutdown");
    auto* btnSettings = new QPushButton("Settings");

    overviewButtons->addWidget(btnHome);
    overviewButtons->addWidget(btnShutdown);
    overviewButtons->addWidget(btnSettings);
    overviewLayout->addLayout(overviewButtons);

    // -------- Settings page --------
    auto* settingsLayout = new QVBoxLayout(settingsPage);

    auto* settingsHeader = new QHBoxLayout();
    settingsHeader->addWidget(new QLabel("miniSystem Settings"));
    settingsHeader->addStretch();

    auto* btnBack = new QPushButton("Back");
    settingsHeader->addWidget(btnBack);
    settingsLayout->addLayout(settingsHeader);

    settingsLayout->addWidget(new QLabel("Screen brightness"));

    auto* brightnessRow = new QHBoxLayout();
    auto* brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(70);

    auto* brightnessValue = new QLabel("70%");
    brightnessRow->addWidget(brightnessSlider);
    brightnessRow->addWidget(brightnessValue);
    settingsLayout->addLayout(brightnessRow);

    auto* bluetoothToggle = new QCheckBox("Bluetooth on");
    bluetoothToggle->setChecked(true);
    settingsLayout->addWidget(bluetoothToggle);
    settingsLayout->addStretch();

    // Update the number next to the slider as it moves.
    connect(brightnessSlider, &QSlider::valueChanged,
            this, [brightnessValue](int value) {
        brightnessValue->setText(QString("%1%").arg(value));
    });

    // Switch between the two pages.
    connect(btnSettings, &QPushButton::clicked,
            this, [this]() {
        pageStack->setCurrentWidget(settingsPage);
    });

    connect(btnBack, &QPushButton::clicked,
            this, [this]() {
        pageStack->setCurrentWidget(overviewPage);
    });
}