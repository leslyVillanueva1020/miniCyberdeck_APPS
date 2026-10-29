#include "systemWindow.h"

#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDateTime>

systemWindow::systemWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("miniSystem");
    resize(480, 320);

    // The main window displays one page from the stack at a time.
    auto* windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(0, 0, 0, 0);

    pageStack = new QStackedWidget(this);
    windowLayout->addWidget(pageStack);

    // Create the two pages.
    overviewPage = new QWidget();
    overviewPage->setObjectName("overviewPage");

    settingsPage = new QWidget();
    settingsPage->setObjectName("settingsPage");

    pageStack->addWidget(overviewPage);
    pageStack->addWidget(settingsPage);

    // ======================== OVERVIEW PAGE ========================

    auto* overviewLayout = new QVBoxLayout(overviewPage);
    overviewLayout->setContentsMargins(12, 10, 12, 10);
    overviewLayout->setSpacing(8);

    // App header
    auto* appHeader = new QFrame();
    appHeader->setObjectName("appHeader");

    auto* titleRow = new QHBoxLayout(appHeader);
    titleRow->setContentsMargins(10, 4, 10, 4);

    auto* title = new QLabel("AQUARIUM LS  miniSystem");
    title->setObjectName("pageTitle");

    auto* fish = new QLabel("🐟");
    fish->setObjectName("fishIcon");

    titleRow->addWidget(title);
    titleRow->addStretch();
    titleRow->addWidget(fish);

    overviewLayout->addWidget(appHeader);

    // Clock, battery, and Wi-Fi cards
    auto* statusRow = new QHBoxLayout();
    statusRow->setSpacing(8);

    auto* clockCard = new QFrame();
    clockCard->setObjectName("clockCard");

    auto* clockLayout = new QVBoxLayout(clockCard);
    clockLayout->setContentsMargins(10, 8, 10, 8);

    // auto* time = new QLabel("11:37");
    // time->setObjectName("timeLabel");
    timeLabel = new QLabel();
    timeLabel->setObjectName("timeLabel");

    // auto* date = new QLabel("Tue, Oct 28");
    // date->setObjectName("dateLabel");
    dateLabel = new QLabel();
    dateLabel->setObjectName("dateLabel");

    clockLayout->addStretch();
    clockLayout->addWidget(timeLabel);
    clockLayout->addWidget(dateLabel);
    clockLayout->addStretch();

    clockTimer = new QTimer(this);

    statusRow->addWidget(clockCard, 3);

    auto* statusCards = new QVBoxLayout();
    statusCards->setSpacing(8);

    auto* batteryCard = new QFrame();
    batteryCard->setObjectName("batteryCard");

    auto* batteryLayout = new QVBoxLayout(batteryCard);
    batteryLayout->setContentsMargins(10, 6, 10, 6);

    auto* battery = new QLabel("Battery 82%");
    battery->setObjectName("cardTitle");

    auto* charging = new QLabel("Not charging");
    charging->setObjectName("smallText");

    batteryLayout->addWidget(battery);
    batteryLayout->addWidget(charging);

    auto* wifiCard = new QFrame();
    wifiCard->setObjectName("wifiCard");

    auto* wifiLayout = new QVBoxLayout(wifiCard);
    wifiLayout->setContentsMargins(10, 6, 10, 6);

    auto* wifi = new QLabel("Wi-Fi connected");
    wifi->setObjectName("cardTitle");

    auto* network = new QLabel("Home Wi-Fi");
    network->setObjectName("smallText");

    wifiLayout->addWidget(wifi);
    wifiLayout->addWidget(network);

    statusCards->addWidget(batteryCard);
    statusCards->addWidget(wifiCard);
    statusRow->addLayout(statusCards, 2);

    overviewLayout->addLayout(statusRow, 3);

    // Device information card
    auto* deviceCard = new QFrame();
    deviceCard->setObjectName("deviceCard");

    auto* deviceRow = new QHBoxLayout(deviceCard);
    deviceRow->setContentsMargins(10, 6, 10, 6);

    auto* deviceBadge = new QLabel("Pi");
    deviceBadge->setObjectName("deviceBadge");
    deviceBadge->setAlignment(Qt::AlignCenter);

    auto* deviceInfo = new QVBoxLayout();

    auto* deviceName = new QLabel("Raspberry Pi Zero 2 W");
    deviceName->setObjectName("cardTitle");

    auto* ipAddress = new QLabel("IP 192.168.1.42");
    ipAddress->setObjectName("smallText");

    auto* storage = new QLabel("Storage 64%");
    storage->setObjectName("smallText");

    deviceInfo->addWidget(deviceName);
    deviceInfo->addWidget(ipAddress);
    deviceInfo->addWidget(storage);

    deviceRow->addWidget(deviceBadge);
    deviceRow->addLayout(deviceInfo);
    deviceRow->addStretch();

    overviewLayout->addWidget(deviceCard, 2);

    // Overview buttons
    auto* overviewButtons = new QHBoxLayout();
    overviewButtons->setSpacing(8);

    auto* btnHome = new QPushButton("Home");
    btnHome->setObjectName("btnHome");

    auto* btnShutdown = new QPushButton("Shut down");
    btnShutdown->setObjectName("btnShutdown");

    auto* btnSettings = new QPushButton("Settings");
    btnSettings->setObjectName("btnSettings");

    overviewButtons->addWidget(btnHome);
    overviewButtons->addWidget(btnShutdown);
    overviewButtons->addWidget(btnSettings);

    overviewLayout->addLayout(overviewButtons);

    // ======================== SETTINGS PAGE ========================

    auto* settingsLayout = new QVBoxLayout(settingsPage);
    settingsLayout->setContentsMargins(12, 10, 12, 10);
    settingsLayout->setSpacing(10);

    // Settings app header
    auto* settingsAppHeader = new QFrame();
    settingsAppHeader->setObjectName("appHeader");

    auto* settingsHeader = new QHBoxLayout(settingsAppHeader);
    settingsHeader->setContentsMargins(10, 4, 10, 4);

    auto* settingsTitle = new QLabel("miniSystem");
    settingsTitle->setObjectName("settingsTitle");

    auto* settingsFish = new QLabel("🐟");
    settingsFish->setObjectName("fishIcon");

    settingsHeader->addWidget(settingsTitle);
    settingsHeader->addStretch();
    settingsHeader->addWidget(settingsFish);

    settingsLayout->addWidget(settingsAppHeader);

    // Brightness card
    auto* brightnessCard = new QFrame();
    brightnessCard->setObjectName("settingsCard");

    auto* brightnessLayout = new QVBoxLayout(brightnessCard);
    brightnessLayout->setContentsMargins(12, 8, 12, 8);

    auto* brightnessHeader = new QHBoxLayout();

    auto* brightnessLabel = new QLabel("Screen brightness");
    brightnessLabel->setObjectName("cardTitle");

    auto* brightnessValue = new QLabel("70%");
    brightnessValue->setObjectName("cardTitle");

    brightnessHeader->addWidget(brightnessLabel);
    brightnessHeader->addStretch();
    brightnessHeader->addWidget(brightnessValue);

    auto* brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(70);

    brightnessLayout->addLayout(brightnessHeader);
    brightnessLayout->addWidget(brightnessSlider);

    settingsLayout->addWidget(brightnessCard);

    // Bluetooth card
    auto* bluetoothCard = new QFrame();
    bluetoothCard->setObjectName("settingsCard");

    auto* bluetoothLayout = new QHBoxLayout(bluetoothCard);
    bluetoothLayout->setContentsMargins(12, 8, 12, 8);

    auto* bluetoothText = new QLabel("Bluetooth");
    bluetoothText->setObjectName("cardTitle");

    auto* bluetoothToggle = new QCheckBox("On");
    bluetoothToggle->setObjectName("bluetoothToggle");
    bluetoothToggle->setChecked(true);

    bluetoothLayout->addWidget(bluetoothText);
    bluetoothLayout->addStretch();
    bluetoothLayout->addWidget(bluetoothToggle);

    settingsLayout->addWidget(bluetoothCard);
    settingsLayout->addStretch();

    connect(
        clockTimer, 
        &QTimer::timeout,
        this,
        [this](){
            updateClock();
        }
    );

    updateClock();
    clockTimer->start(1000); //refreshes ever 1000 mmilliseconds

    // Return to the Overview page.
    auto* btnBack = new QPushButton("Home");
    btnBack->setObjectName("btnHome");
    settingsLayout->addWidget(btnBack);

    // Page navigation
    connect(btnSettings, &QPushButton::clicked,
            this, [this]() {
        pageStack->setCurrentWidget(settingsPage);
    });

    connect(btnBack, &QPushButton::clicked,
            this, [this]() {
        pageStack->setCurrentWidget(overviewPage);
    });

    // Update the percentage as the brightness slider moves.
    connect(brightnessSlider, &QSlider::valueChanged,
            this, [brightnessValue](int value) {
        brightnessValue->setText(QString("%1%").arg(value));
    });
}

void systemWindow::updateClock(){
    const QDateTime now = QDateTime::currentDateTime();

    timeLabel->setText(now.toString("h:mm AP"));
    dateLabel->setText(now.toString("ddd, MMM d"));
}