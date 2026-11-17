#include "systemWindow.h"

#include <QCheckBox>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLocalSocket>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPixmap>
#include <QProcess>
#include <QPushButton>
#include <QSlider>
#include <QStackedWidget>
#include <QStorageInfo>
#include <QTimer>
#include <QVBoxLayout>

// ============================================================
// Pixel-Art Helper
// ============================================================

static void setPixelArt(
    QLabel* label,
    const QString& resourcePath,
    const QSize& size)
{
    QPixmap pixmap(resourcePath);

    if (pixmap.isNull()) {
        qDebug() << "Could not load artwork:" << resourcePath;
        return;
    }

    label->setPixmap(
        pixmap.scaled(
            size,
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        )
    );

    label->setFixedSize(size);
    label->setAlignment(Qt::AlignCenter);
}

// ============================================================
// Constructor
// ============================================================

systemWindow::systemWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("miniSystem");
    resize(480, 320);

    // ========================================================
    // Resource Test
    // ========================================================

    qDebug() << "Battery asset exists:"
             << QFile::exists(
                    ":/assets/battery_50_percent.png"
                );

    // ========================================================
    // Device and Storage Information
    // ========================================================

    const QStorageInfo root = QStorageInfo::root();

    qint64 totalBytes = 0;
    qint64 freeBytes = 0;

    // ========================================================
    // Main Window and Page Stack
    // ========================================================

    auto* windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(0, 0, 0, 0);

    pageStack = new QStackedWidget(this);
    windowLayout->addWidget(pageStack);

    overviewPage = new QWidget();
    overviewPage->setObjectName("overviewPage");

    settingsPage = new QWidget();
    settingsPage->setObjectName("settingsPage");

    pageStack->addWidget(overviewPage);
    pageStack->addWidget(settingsPage);

    // ========================================================
    // OVERVIEW PAGE
    // ========================================================

    auto* overviewLayout = new QVBoxLayout(overviewPage);
    overviewLayout->setContentsMargins(12, 10, 12, 10);
    overviewLayout->setSpacing(8);

    // ========================================================
    // Overview Header
    // ========================================================

    auto* appHeader = new QFrame();
    appHeader->setObjectName("appHeader");

    auto* titleRow = new QHBoxLayout(appHeader);
    titleRow->setContentsMargins(10, 4, 10, 4);

    auto* title = new QLabel("AQUARIUM LS  miniSystem");
    title->setObjectName("pageTitle");

    auto* fish = new QLabel();
    fish->setObjectName("fishIcon");

    setPixelArt(
        fish,
        ":/assets/fish_header.png",
        QSize(28, 20)
    );

    auto* headerBubbles = new QLabel();

    setPixelArt(
        headerBubbles,
        ":/assets/bubbles.png",
        QSize(28, 20)
    );

    titleRow->addWidget(title);
    titleRow->addStretch();
    titleRow->addWidget(fish);
    titleRow->addWidget(headerBubbles);

    overviewLayout->addWidget(appHeader);

    // ========================================================
    // Clock, Battery, and Wi-Fi Section
    // ========================================================

    auto* statusRow = new QHBoxLayout();
    statusRow->setSpacing(8);

    // ========================================================
    // Clock Card
    // ========================================================

    auto* clockCard = new QFrame();
    clockCard->setObjectName("clockCard");

    timeLabel = new QLabel();
    timeLabel->setObjectName("timeLabel");

    dateLabel = new QLabel();
    dateLabel->setObjectName("dateLabel");

    auto* clockLayout = new QHBoxLayout(clockCard);
    clockLayout->setContentsMargins(10, 8, 10, 8);

    auto* clockTextLayout = new QVBoxLayout();

    clockTextLayout->addStretch();
    clockTextLayout->addWidget(timeLabel);
    clockTextLayout->addWidget(dateLabel);
    clockTextLayout->addStretch();

    auto* sunsetImage = new QLabel();

    setPixelArt(
        sunsetImage,
        ":/assets/sunset.png",
        QSize(64, 48)
    );

    clockLayout->addLayout(clockTextLayout);
    clockLayout->addStretch();
    clockLayout->addWidget(
        sunsetImage,
        0,
        Qt::AlignRight | Qt::AlignBottom
    );

    clockTimer = new QTimer(this);

    statusRow->addWidget(clockCard, 3);

    // ========================================================
    // Right-Side Status Cards
    // ========================================================

    auto* statusCards = new QVBoxLayout();
    statusCards->setSpacing(8);

    // ========================================================
    // Battery Card
    // ========================================================

    auto* batteryCard = new QFrame();
    batteryCard->setObjectName("batteryCard");

    auto* batteryLayout = new QVBoxLayout(batteryCard);
    batteryLayout->setContentsMargins(10, 6, 10, 6);

    auto* batteryIcon = new QLabel();

    setPixelArt(
        batteryIcon,
        ":/assets/battery_50_percent.png",
        QSize(36, 28)
    );

    auto* battery = new QLabel("Battery ???%");
    battery->setObjectName("cardTitle");

    auto* charging = new QLabel("Charging Status: Unknown");
    charging->setObjectName("smallText");

    auto* batteryTextLayout = new QVBoxLayout();
    batteryTextLayout->addWidget(battery);
    batteryTextLayout->addWidget(charging);

    auto* batteryRow = new QHBoxLayout();
    batteryRow->addWidget(batteryIcon);
    batteryRow->addLayout(batteryTextLayout);

    batteryLayout->addLayout(batteryRow);

    // ========================================================
    // Wi-Fi Card
    // ========================================================

    auto* wifiCard = new QFrame();
    wifiCard->setObjectName("wifiCard");

    auto* wifiLayout = new QVBoxLayout(wifiCard);
    wifiLayout->setContentsMargins(10, 6, 10, 6);

    auto* wifiIcon = new QLabel();

    setPixelArt(
        wifiIcon,
        ":/assets/wifi_connected.png",
        QSize(32, 24)
    );

    auto* wifi = new QLabel("Wi-Fi: unknown");
    wifi->setObjectName("cardTitle");

    auto* network = new QLabel("Unknown Wi-Fi");
    network->setObjectName("smallText");

    auto* wifiTextLayout = new QVBoxLayout();
    wifiTextLayout->addWidget(wifi);
    wifiTextLayout->addWidget(network);

    auto* wifiRow = new QHBoxLayout();
    wifiRow->addWidget(wifiIcon);
    wifiRow->addLayout(wifiTextLayout);

    wifiLayout->addLayout(wifiRow);

    statusCards->addWidget(batteryCard);
    statusCards->addWidget(wifiCard);

    statusRow->addLayout(statusCards, 2);
    overviewLayout->addLayout(statusRow, 3);

    // ========================================================
    // Device Information Card
    // ========================================================

    auto* deviceCard = new QFrame();
    deviceCard->setObjectName("deviceCard");

    auto* deviceRow = new QHBoxLayout(deviceCard);
    deviceRow->setContentsMargins(10, 6, 10, 6);
    deviceRow->setSpacing(8);

    // Raspberry Pi artwork
    auto* deviceBadge = new QLabel();
    deviceBadge->setObjectName("deviceBadge");

    setPixelArt(
        deviceBadge,
        ":/assets/raspberry_pi.png",
        QSize(32, 32)
    );

    // Device text
    auto* deviceInfo = new QVBoxLayout();
    deviceInfo->setSpacing(2);

    auto* deviceName = new QLabel("Raspberry Pi Zero 2 W");
    deviceName->setObjectName("cardTitle");

    auto* ipAddress = new QLabel("IP ???");
    ipAddress->setObjectName("smallText");

    auto* storage = new QLabel("Now Loading...");
    storage->setObjectName("smallText");

    deviceInfo->addWidget(deviceName);
    deviceInfo->addWidget(ipAddress);
    deviceInfo->addWidget(storage);

    // Coral artwork
    auto* coral1 = new QLabel();

    setPixelArt(
        coral1,
        ":/assets/coral1.png",
        QSize(24, 36)
    );

    auto* coral2 = new QLabel();

    setPixelArt(
        coral2,
        ":/assets/coral2.png",
        QSize(20, 32)
    );

    // Bubble artwork
    auto* bubble7 = new QLabel();

    setPixelArt(
        bubble7,
        ":/assets/bubble_7x7.png",
        QSize(7, 7)
    );

    auto* bubble10 = new QLabel();

    setPixelArt(
        bubble10,
        ":/assets/bubble_10x10.png",
        QSize(10, 10)
    );

    // Bubbles are horizontal.
    auto* bubbleRow = new QHBoxLayout();
    bubbleRow->setContentsMargins(0, 0, 0, 0);
    bubbleRow->setSpacing(2);
    bubbleRow->setAlignment(Qt::AlignVCenter);

    bubbleRow->addWidget(bubble7);
    bubbleRow->addWidget(bubble10);

// ======================== Device Decorations ========================

    // Small bubble above the larger bubble.
    auto* bubbleColumn = new QVBoxLayout();
    bubbleColumn->setContentsMargins(0, 0, 0, 0);
    bubbleColumn->setSpacing(3);
    bubbleColumn->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    bubbleColumn->addWidget(
        bubble7,
        0,
        Qt::AlignHCenter
    );

    bubbleColumn->addWidget(
        bubble10,
        0,
        Qt::AlignHCenter
    );

    // Bubbles on the left, coral pieces side-by-side on the right.
    auto* decorationLayout = new QHBoxLayout();
    decorationLayout->setContentsMargins(0, 0, 0, 0);
    decorationLayout->setSpacing(4);
    decorationLayout->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    decorationLayout->addLayout(bubbleColumn);

    decorationLayout->addWidget(
        coral1,
        0,
        Qt::AlignBottom
    );

    decorationLayout->addWidget(
        coral2,
        0,
        Qt::AlignBottom
    );

    // Build device row
   deviceRow->addWidget(deviceBadge);
    deviceRow->addLayout(deviceInfo);
    deviceRow->addStretch();
    deviceRow->addLayout(decorationLayout);

    deviceRow->setAlignment(
        decorationLayout,
        Qt::AlignRight | Qt::AlignBottom
    );

    overviewLayout->addWidget(deviceCard, 2);

    // ========================================================
    // Overview Buttons
    // ========================================================

    auto* overviewButtons = new QHBoxLayout();
    overviewButtons->setSpacing(8);

    auto* btnHome = new QPushButton("Home");
    btnHome->setObjectName("btnHome");

    auto* btnShutdown = new QPushButton("Shut down");
    btnShutdown->setObjectName("btnShutdown");
    btnShutdown->setIcon(
        QIcon(":/assets/shutdown_icon.png")
    );
    btnShutdown->setIconSize(QSize(20, 20));

    auto* btnSettings = new QPushButton("Settings");
    btnSettings->setObjectName("btnSettings");
    btnSettings->setIcon(
        QIcon(":/assets/settings.png")
    );
    btnSettings->setIconSize(QSize(20, 20));

    overviewButtons->addWidget(btnHome);
    overviewButtons->addWidget(btnShutdown);
    overviewButtons->addWidget(btnSettings);

    overviewLayout->addLayout(overviewButtons);

    // ========================================================
    // SETTINGS PAGE
    // ========================================================

    auto* settingsLayout = new QVBoxLayout(settingsPage);
    settingsLayout->setContentsMargins(12, 10, 12, 10);
    settingsLayout->setSpacing(10);

    // ========================================================
    // Settings Header
    // ========================================================

    auto* settingsAppHeader = new QFrame();
    settingsAppHeader->setObjectName("appHeader");

    auto* settingsHeader = new QHBoxLayout(settingsAppHeader);
    settingsHeader->setContentsMargins(10, 4, 10, 4);

    auto* settingsTitle = new QLabel("miniSystem");
    settingsTitle->setObjectName("settingsTitle");

    auto* settingsFish = new QLabel();
    settingsFish->setObjectName("fishIcon");

    setPixelArt(
        settingsFish,
        ":/assets/title_fish.png",
        QSize(28, 20)
    );

    settingsHeader->addWidget(settingsTitle);
    settingsHeader->addStretch();
    settingsHeader->addWidget(settingsFish);

    settingsLayout->addWidget(settingsAppHeader);

    // ========================================================
    // Brightness Card
    // ========================================================

    auto* brightnessCard = new QFrame();
    brightnessCard->setObjectName("settingsCard");

    auto* brightnessLayout = new QVBoxLayout(brightnessCard);
    brightnessLayout->setContentsMargins(12, 8, 12, 8);

    auto* brightnessHeader = new QHBoxLayout();

    auto* brightnessSun = new QLabel();

    setPixelArt(
        brightnessSun,
        ":/assets/sun.png",
        QSize(28, 28)
    );

    auto* brightnessLabel = new QLabel("Screen brightness");
    brightnessLabel->setObjectName("cardTitle");

    auto* brightnessValue = new QLabel("70%");
    brightnessValue->setObjectName("cardTitle");

    brightnessHeader->addWidget(brightnessSun);
    brightnessHeader->addWidget(brightnessLabel);
    brightnessHeader->addStretch();
    brightnessHeader->addWidget(brightnessValue);

    auto* brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(70);

    brightnessLayout->addLayout(brightnessHeader);
    brightnessLayout->addWidget(brightnessSlider);

    settingsLayout->addWidget(brightnessCard);

    // ========================================================
    // Bluetooth Card
    // ========================================================

    auto* bluetoothCard = new QFrame();
    bluetoothCard->setObjectName("settingsCard");

    auto* bluetoothLayout = new QHBoxLayout(bluetoothCard);
    bluetoothLayout->setContentsMargins(12, 8, 12, 8);

    auto* bluetoothIcon = new QLabel();

    setPixelArt(
        bluetoothIcon,
        ":/assets/bluetooth_icon.png",
        QSize(24, 28)
    );

    auto* bluetoothText = new QLabel("Bluetooth");
    bluetoothText->setObjectName("cardTitle");

    auto* bluetoothToggle = new QCheckBox("On");
    bluetoothToggle->setObjectName("bluetoothToggle");

    bluetoothLayout->addWidget(bluetoothIcon);
    bluetoothLayout->addWidget(bluetoothText);
    bluetoothLayout->addStretch();
    bluetoothLayout->addWidget(bluetoothToggle);

    settingsLayout->addWidget(bluetoothCard);
    settingsLayout->addStretch();

    // ========================================================
    // Back Button
    // ========================================================

    auto* btnBack = new QPushButton("Back");
    btnBack->setObjectName("btnHome");

    // This requires back_arrow.png to be in the .qrc file.
    btnBack->setIcon(
        QIcon(":/assets/back_arrow.png")
    );
    btnBack->setIconSize(QSize(20, 18));

    settingsLayout->addWidget(btnBack);

    // ========================================================
    // Storage Information
    // ========================================================

    if (root.isValid() && root.isReady()) {
        totalBytes = root.bytesTotal();
        freeBytes = root.bytesFree();

        if (totalBytes > 0) {
            const double usedPercent =
                static_cast<double>(
                    totalBytes - freeBytes
                ) / totalBytes * 100.0;

            storage->setText(
                QString("Storage Used: %1%")
                    .arg(static_cast<int>(usedPercent))
            );
        }
    }

    // ========================================================
    // Bluetooth Information
    // ========================================================

    auto* statusProcess = new QProcess(this);

    connect(
        statusProcess,
        &QProcess::finished,
        this,
        [statusProcess, bluetoothToggle]()
        {
            const QString status =
                QString::fromUtf8(
                    statusProcess->readAllStandardOutput()
                );

            const bool isPowered =
                status.contains("Powered: yes");

            bluetoothToggle->setChecked(isPowered);
        }
    );

    statusProcess->start(
        "bluetoothctl",
        {"show"}
    );

    connect(
        bluetoothToggle,
        &QCheckBox::clicked,
        this,
        [this, statusProcess](bool checked)
        {
            auto* powerProcess = new QProcess(this);

            connect(
                powerProcess,
                &QProcess::finished,
                this,
                [statusProcess, powerProcess]()
                {
                    statusProcess->start(
                        "bluetoothctl",
                        {"show"}
                    );

                    powerProcess->deleteLater();
                }
            );

            const QString action =
                checked ? "on" : "off";

            powerProcess->start(
                "bluetoothctl",
                {"power", action}
            );
        }
    );

    // ========================================================
    // Network Information
    // ========================================================

    bool found = false;

    for (const QNetworkInterface& interface
         : QNetworkInterface::allInterfaces()) {

        for (const QNetworkAddressEntry& entry
             : interface.addressEntries()) {

            if (interface.name() == "wlan0"
                && entry.ip().protocol()
                   == QAbstractSocket::IPv4Protocol) {

                ipAddress->setText(
                    QString("IP %1")
                        .arg(entry.ip().toString())
                );

                found = true;
            }
        }
    }

    if (found) {
        wifi->setText("Wi-Fi: connected");

        setPixelArt(
            wifiIcon,
            ":/assets/wifi_connected.png",
            QSize(32, 24)
        );
    } else {
        wifi->setText("Wi-Fi: disconnected");

        setPixelArt(
            wifiIcon,
            ":/assets/wifi_disconnected_1.png",
            QSize(32, 24)
        );
    }

    auto* networkProcess = new QProcess(this);

    connect(
        networkProcess,
        &QProcess::finished,
        this,
        [networkProcess, network]()
        {
            const QString wifiOutput =
                QString::fromUtf8(
                    networkProcess->readAllStandardOutput()
                );

            const QStringList lines =
                wifiOutput.split('\n');

            for (const QString& line : lines) {
                if (line.startsWith("*:")) {
                    const QString wifiName =
                        line.sliced(2).trimmed();

                    network->setText(
                        wifiName + " Wi-Fi"
                    );

                    break;
                }
            }
        }
    );

    networkProcess->start(
        "nmcli",
        {
            "-g",
            "IN-USE,SSID",
            "device",
            "wifi",
            "list"
        }
    );

    // ========================================================
    // Battery Information
    // ========================================================

    auto* socket = new QLocalSocket(this);

    connect(
        socket,
        &QLocalSocket::connected,
        this,
        [socket]()
        {
            socket->write("get battery\n");
            socket->write(
                "get battery_power_plugged\n"
            );
            socket->write(
                "get battery_allow_charging\n"
            );
        }
    );

    connect(
        socket,
        &QLocalSocket::readyRead,
        this,
        [this, socket, battery, charging, batteryIcon]()
        {
            while (socket->canReadLine()) {
                const QString line =
                    QString::fromUtf8(
                        socket->readLine()
                    ).trimmed();

                qDebug() << line;

                if (line.startsWith("battery:")) {
                    const QString numberText =
                        line.section(':', 1).trimmed();

                    bool ok = false;

                    const double value =
                        numberText.toDouble(&ok);

                    if (ok) {
                        battery->setText(
                            QString("Battery %1%")
                                .arg(value, 0, 'f', 0)
                        );
                    }
                }
                else if (
                    line.startsWith(
                        "battery_power_plugged:"
                    )
                ) {
                    const QString valueText =
                        line.section(':', 1).trimmed();

                    powerPlugged =
                        (valueText == "true");

                    gotPowerPlugged = true;
                }
                else if (
                    line.startsWith(
                        "battery_allow_charging:"
                    )
                ) {
                    const QString valueText =
                        line.section(':', 1).trimmed();

                    chargingAllowed =
                        (valueText == "true");

                    gotChargingAllowed = true;
                }

                if (
                    gotPowerPlugged
                    && gotChargingAllowed
                ) {
                    if (
                        powerPlugged
                        && chargingAllowed
                    ) {
                        charging->setText(
                            "Charging status: Charging"
                        );

                        setPixelArt(
                            batteryIcon,
                            ":/assets/charging_1.png",
                            QSize(36, 28)
                        );
                    }
                    else {
                        charging->setText(
                            "Charging status: Not charging"
                        );
                    }
                }
            }
        }
    );

    socket->connectToServer(
        "/tmp/pisugar-server.sock"
    );

    // ========================================================
    // Clock and Date
    // ========================================================

    connect(
        clockTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            updateClock();
        }
    );

    updateClock();
    clockTimer->start(1000);

    // ========================================================
    // Shutdown Logic
    // ========================================================

    connect(
        btnShutdown,
        &QPushButton::clicked,
        this,
        [this]()
        {
            QMessageBox box(this);

            box.setWindowTitle("Shutdown");
            box.setText(
                "Do you want to shutdown raspberry pi?"
            );

            auto* shutdownNow =
                box.addButton(
                    "Shutdown",
                    QMessageBox::DestructiveRole
                );

            auto* cancelShutdown =
                box.addButton(
                    "Cancel",
                    QMessageBox::RejectRole
                );

            box.setDefaultButton(cancelShutdown);
            box.exec();

            if (box.clickedButton() == shutdownNow) {
                auto* process = new QProcess(this);

                connect(
                    process,
                    &QProcess::finished,
                    this,
                    [process](
                        int exitCode,
                        QProcess::ExitStatus status
                    )
                    {
                        Q_UNUSED(status);

                        qDebug()
                            << "Exit code:"
                            << exitCode;

                        qDebug()
                            << "Error output:"
                            << process->readAllStandardError();
                    }
                );

                connect(
                    process,
                    &QProcess::errorOccurred,
                    this,
                    [process](
                        QProcess::ProcessError error
                    )
                    {
                        if (
                            error
                            == QProcess::FailedToStart
                        ) {
                            qDebug()
                                << "Could not start program:"
                                << process->errorString();
                        }
                    }
                );

                process->start(
                    "systemctl",
                    {"poweroff"}
                );
            }
        }
    );

    // ========================================================
    // Page Navigation
    // ========================================================

    connect(
        btnHome,
        &QPushButton::clicked,
        this,
        &QWidget::close
    );

    connect(
        btnSettings,
        &QPushButton::clicked,
        this,
        [this]()
        {
            pageStack->setCurrentWidget(
                settingsPage
            );
        }
    );

    connect(
        btnBack,
        &QPushButton::clicked,
        this,
        [this]()
        {
            pageStack->setCurrentWidget(
                overviewPage
            );
        }
    );

    // ========================================================
    // Brightness Slider
    // ========================================================

    connect(
        brightnessSlider,
        &QSlider::valueChanged,
        this,
        [brightnessValue](int value)
        {
            brightnessValue->setText(
                QString("%1%").arg(value)
            );
        }
    );
}

// ============================================================
// Clock Update
// ============================================================

void systemWindow::updateClock()
{
    const QDateTime now =
        QDateTime::currentDateTime();

    timeLabel->setText(
        now.toString("h:mm AP")
    );

    dateLabel->setText(
        now.toString("ddd, MMM d")
    );
}