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
#include <QTransform>
#include <QPainter>

// ============================================================
// Pixel-art helper
// ============================================================
static void setPixelArt(
    QLabel* label,
    const QString& resourcePath,
    const QSize& size,
    bool flipHorizontal = false,
    int verticalOffset = 0)
{
    QPixmap pixmap(resourcePath);

    if (pixmap.isNull()) {
        qDebug() << "Could not load artwork:" << resourcePath;
        return;
    }

    if (flipHorizontal) {
        pixmap = pixmap.transformed(
            QTransform().scale(-1, 1)
        );
    }

    QPixmap scaledPixmap =
        pixmap.scaled(
            size,
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        );

    QPixmap canvas(size);
    canvas.fill(Qt::transparent);

    QPainter painter(&canvas);

    const int x =
        (size.width() - scaledPixmap.width()) / 2;

    const int y =
        (size.height() - scaledPixmap.height()) / 2
        + verticalOffset;

    painter.drawPixmap(x, y, scaledPixmap);
    painter.end();

    label->setPixmap(canvas);
    label->setFixedSize(size);

    label->setAlignment(
        Qt::AlignHCenter | Qt::AlignBottom
    );
}

// ============================================================
// Constructor
// ============================================================

systemWindow::systemWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("miniSystem");
    resize(480, 320);

    qDebug() << "Battery asset exists:"
             << QFile::exists(":/assets/battery_50_percent.png");

    qDebug() << "Two bubbles asset exists:"
             << QFile::exists(":/assets/two_bubbles.png");

    // ========================================================
    // Storage information
    // ========================================================

    const QStorageInfo root = QStorageInfo::root();

    qint64 totalBytes = 0;
    qint64 freeBytes = 0;

    // ========================================================
    // Main window and page stack
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

    // --------------------------------------------------------
    // Shared Aquarium LS header
    // --------------------------------------------------------

    auto* overviewHeader = new QFrame();
    overviewHeader->setObjectName("appHeader");

    auto* overviewHeaderLayout =
        new QHBoxLayout(overviewHeader);

    overviewHeaderLayout->setContentsMargins(10, 4, 10, 4);
    overviewHeaderLayout->setSpacing(4);

    auto* overviewBrand =
        new QLabel("AQUARIUM LS  miniSystem");

    overviewBrand->setObjectName("pageTitle");

    auto* overviewFish = new QLabel();
    overviewFish->setObjectName("fishIcon");

    setPixelArt(
        overviewFish,
        ":/assets/fish_header.png",
        QSize(28, 20)
    );

    auto* overviewHeaderBubbles = new QLabel();

    setPixelArt(
        overviewHeaderBubbles,
        ":/assets/bubbles.png",
        QSize(28, 20)
    );

    overviewHeaderLayout->addWidget(overviewBrand);
    overviewHeaderLayout->addStretch();
    overviewHeaderLayout->addWidget(overviewFish);
    overviewHeaderLayout->addWidget(overviewHeaderBubbles);

    overviewLayout->addWidget(overviewHeader);

    // IMPORTANT:
    // There is intentionally no Overview title row here.
    // The main page goes directly to the clock, battery,
    // and Wi-Fi section.

    // ========================================================
    // Clock, battery, and Wi-Fi section
    // ========================================================

    auto* statusRow = new QHBoxLayout();
    statusRow->setSpacing(8);

    // --------------------------------------------------------
    // Clock card
    // --------------------------------------------------------

    auto* clockCard = new QFrame();
    clockCard->setObjectName("clockCard");

    timeLabel = new QLabel();
    timeLabel->setObjectName("timeLabel");

    dateLabel = new QLabel();
    dateLabel->setObjectName("dateLabel");

    auto* clockLayout = new QHBoxLayout(clockCard);
    clockLayout->setContentsMargins(10, 8, 10, 8);

    auto* clockTextLayout = new QVBoxLayout();
    clockTextLayout->setSpacing(2);

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

    // --------------------------------------------------------
    // Battery and Wi-Fi cards
    // --------------------------------------------------------

    auto* statusCards = new QVBoxLayout();
    statusCards->setSpacing(8);

    // --------------------------------------------------------
    // Battery card
    // --------------------------------------------------------

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
    batteryTextLayout->setSpacing(2);
    batteryTextLayout->addWidget(battery);
    batteryTextLayout->addWidget(charging);

    auto* batteryRow = new QHBoxLayout();
    batteryRow->setSpacing(8);
    batteryRow->addWidget(batteryIcon);
    batteryRow->addLayout(batteryTextLayout);

    batteryLayout->addLayout(batteryRow);

    // --------------------------------------------------------
    // Wi-Fi card
    // --------------------------------------------------------

    auto* wifiCard = new QFrame();
    wifiCard->setObjectName("wifiCard");

    auto* wifiLayout = new QVBoxLayout(wifiCard);
    wifiLayout->setContentsMargins(10, 6, 10, 6);

    auto* wifiIcon = new QLabel();

    setPixelArt(
        wifiIcon,
        ":/assets/wifi_disconnected_1.png",
        QSize(32, 24)
    );

    auto* wifi = new QLabel("Wi-Fi: disconnected");
    wifi->setObjectName("cardTitle");

    auto* network = new QLabel("Unknown Wi-Fi");
    network->setObjectName("smallText");

    auto* wifiTextLayout = new QVBoxLayout();
    wifiTextLayout->setSpacing(2);
    wifiTextLayout->addWidget(wifi);
    wifiTextLayout->addWidget(network);

    auto* wifiRow = new QHBoxLayout();
    wifiRow->setSpacing(8);
    wifiRow->addWidget(wifiIcon);
    wifiRow->addLayout(wifiTextLayout);

    wifiLayout->addLayout(wifiRow);

    statusCards->addWidget(batteryCard);
    statusCards->addWidget(wifiCard);

    statusRow->addLayout(statusCards, 2);
    overviewLayout->addLayout(statusRow, 3);

    // ========================================================
    // Raspberry Pi device card
    // ========================================================

    auto* deviceCard = new QFrame();
    deviceCard->setObjectName("deviceCard");

    auto* deviceRow = new QHBoxLayout(deviceCard);
    deviceRow->setContentsMargins(10, 6, 10, 0);
    deviceRow->setSpacing(8);

    auto* deviceBadge = new QLabel();
    deviceBadge->setObjectName("deviceBadge");

    setPixelArt(
        deviceBadge,
        ":/assets/raspberry_pi.png",
        QSize(32, 32)
    );

    auto* deviceInfo = new QVBoxLayout();
    deviceInfo->setSpacing(2);

    auto* deviceName =
        new QLabel("Raspberry Pi Zero 2 W");

    deviceName->setObjectName("cardTitle");

    auto* ipAddress = new QLabel("IP ???");
    ipAddress->setObjectName("smallText");

    auto* storage = new QLabel("Storage Used: 30%");
    storage->setObjectName("smallText");

    deviceInfo->addWidget(deviceName);
    deviceInfo->addWidget(ipAddress);
    deviceInfo->addWidget(storage);

    // --------------------------------------------------------
    // Raspberry Pi card decorations
    // --------------------------------------------------------

    auto* deviceBubbles = new QLabel();

    setPixelArt(
        deviceBubbles,
        ":/assets/two_bubbles.png",
        QSize(26, 28),
        false,
        -5
    );

    auto* deviceCoral1 = new QLabel();

    setPixelArt(
        deviceCoral1,
        ":/assets/coral1.png",
        QSize(24, 52),
        false,
        8
    );

    auto* deviceCoral2 = new QLabel();

    setPixelArt(
        deviceCoral2,
        ":/assets/coral2.png",
        QSize(22, 52)
    );

    auto* decorationLayout = new QHBoxLayout();
    decorationLayout->setContentsMargins(0, 0, 0, 0);
    decorationLayout->setSpacing(4);
    decorationLayout->setAlignment(
        Qt::AlignRight | Qt::AlignBottom
    );

    decorationLayout->addWidget(
        deviceBubbles,
        0,
        Qt::AlignBottom
    );

    decorationLayout->addWidget(
        deviceCoral1,
        0,
        Qt::AlignBottom
    );

    decorationLayout->addWidget(
        deviceCoral2,
        0,
        Qt::AlignBottom
    );

    deviceRow->addWidget(deviceBadge);
    deviceRow->addLayout(deviceInfo);
    deviceRow->addStretch();
    deviceRow->addLayout(decorationLayout);

    overviewLayout->addWidget(deviceCard, 2);

    // ========================================================
    // Overview buttons
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
    settingsLayout->setSpacing(8);

    // --------------------------------------------------------
    // Settings shared Aquarium LS header
    // --------------------------------------------------------

    auto* settingsHeaderFrame = new QFrame();
    settingsHeaderFrame->setObjectName("appHeader");

    auto* settingsHeaderLayout =
        new QHBoxLayout(settingsHeaderFrame);

    settingsHeaderLayout->setContentsMargins(10, 4, 10, 4);
    settingsHeaderLayout->setSpacing(4);

    auto* settingsBrand =
        new QLabel("AQUARIUM LS  miniSystem");

    settingsBrand->setObjectName("pageTitle");

    auto* settingsHeaderFish = new QLabel();
    settingsHeaderFish->setObjectName("fishIcon");

    setPixelArt(
        settingsHeaderFish,
        ":/assets/fish_header.png",
        QSize(28, 20)
    );

    auto* settingsHeaderBubbles = new QLabel();

    setPixelArt(
        settingsHeaderBubbles,
        ":/assets/bubbles.png",
        QSize(28, 20)
    );

    settingsHeaderLayout->addWidget(settingsBrand);
    settingsHeaderLayout->addStretch();
    settingsHeaderLayout->addWidget(settingsHeaderFish);
    settingsHeaderLayout->addWidget(settingsHeaderBubbles);

    settingsLayout->addWidget(settingsHeaderFrame);

    // --------------------------------------------------------
    // Settings title and artwork row
    // --------------------------------------------------------

    auto* settingsTitleRow = new QHBoxLayout();
    settingsTitleRow->setContentsMargins(4, 0, 4, 0);
    settingsTitleRow->setSpacing(3);

    auto* settingsPageTitle = new QLabel("settings");
    settingsPageTitle->setObjectName("sectionTitle");

    auto* settingsPageBubbles = new QLabel();

    setPixelArt(
        settingsPageBubbles,
        ":/assets/two_bubbles.png",
        QSize(28, 30),
        true
    );

    auto* settingsPageFish = new QLabel();

    setPixelArt(
        settingsPageFish,
        ":/assets/title_fish.png",
        QSize(28, 20)
    );

    auto* settingsPageCoral1 = new QLabel();

    setPixelArt(
        settingsPageCoral1,
        ":/assets/coral1.png",
        QSize(22, 44)
    );

    auto* settingsPageCoral2 = new QLabel();

    setPixelArt(
        settingsPageCoral2,
        ":/assets/coral2.png",
        QSize(20, 44)
    );

    settingsTitleRow->addWidget(settingsPageTitle);
    settingsTitleRow->addStretch();

    settingsTitleRow->addWidget(
        settingsPageBubbles,
        0,
        Qt::AlignBottom
    );

    settingsTitleRow->addWidget(
        settingsPageFish,
        0,
        Qt::AlignBottom
    );

    settingsTitleRow->addWidget(
        settingsPageCoral1,
        0,
        Qt::AlignBottom
    );

    settingsTitleRow->addWidget(
        settingsPageCoral2,
        0,
        Qt::AlignBottom
    );

    settingsLayout->addLayout(settingsTitleRow);

    // ========================================================
    // Brightness card
    // ========================================================

    auto* brightnessCard = new QFrame();
    brightnessCard->setObjectName("settingsCard");

    auto* brightnessLayout =
        new QVBoxLayout(brightnessCard);

    brightnessLayout->setContentsMargins(12, 8, 12, 8);

    auto* brightnessHeader = new QHBoxLayout();

    auto* brightnessSun = new QLabel();

    setPixelArt(
        brightnessSun,
        ":/assets/sun.png",
        QSize(28, 28)
    );

    auto* brightnessLabel =
        new QLabel("Screen brightness");

    brightnessLabel->setObjectName("cardTitle");

    auto* brightnessValue = new QLabel("70%");
    brightnessValue->setObjectName("cardTitle");

    brightnessHeader->addWidget(brightnessSun);
    brightnessHeader->addWidget(brightnessLabel);
    brightnessHeader->addStretch();
    brightnessHeader->addWidget(brightnessValue);

    auto* brightnessSlider =
        new QSlider(Qt::Horizontal);

    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(70);

    brightnessLayout->addLayout(brightnessHeader);
    brightnessLayout->addWidget(brightnessSlider);

    settingsLayout->addWidget(brightnessCard);

    // ========================================================
    // Bluetooth card
    // ========================================================

    auto* bluetoothCard = new QFrame();
    bluetoothCard->setObjectName("settingsCard");

    auto* bluetoothLayout =
        new QHBoxLayout(bluetoothCard);

    bluetoothLayout->setContentsMargins(12, 8, 12, 8);

    auto* bluetoothIcon = new QLabel();

    setPixelArt(
        bluetoothIcon,
        ":/assets/bluetooth_icon.png",
        QSize(24, 28)
    );

    auto* bluetoothText =
        new QLabel("Bluetooth");

    bluetoothText->setObjectName("cardTitle");

    auto* bluetoothToggle =
        new QCheckBox("On");

    bluetoothToggle->setObjectName("bluetoothToggle");

    bluetoothLayout->addWidget(bluetoothIcon);
    bluetoothLayout->addWidget(bluetoothText);
    bluetoothLayout->addStretch();
    bluetoothLayout->addWidget(bluetoothToggle);

    settingsLayout->addWidget(bluetoothCard);
    settingsLayout->addStretch();

    // ========================================================
    // Back button
    // ========================================================

    auto* btnBack = new QPushButton("Back");
    btnBack->setObjectName("btnHome");
    btnBack->setIcon(
        QIcon(":/assets/back_arrow.png")
    );
    btnBack->setIconSize(QSize(20, 18));

    settingsLayout->addWidget(btnBack);

    // ========================================================
    // Storage information
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
    // Bluetooth status
    // ========================================================

    auto* bluetoothStatusProcess = new QProcess(this);

    connect(
        bluetoothStatusProcess,
        &QProcess::finished,
        this,
        [bluetoothStatusProcess, bluetoothToggle]()
        {
            const QString output =
                QString::fromUtf8(
                    bluetoothStatusProcess
                        ->readAllStandardOutput()
                );

            bluetoothToggle->setChecked(
                output.contains("Powered: yes")
            );
        }
    );

    bluetoothStatusProcess->start(
        "bluetoothctl",
        {"show"}
    );

    connect(
        bluetoothToggle,
        &QCheckBox::clicked,
        this,
        [this, bluetoothStatusProcess](bool checked)
        {
            auto* powerProcess = new QProcess(this);

            connect(
                powerProcess,
                &QProcess::finished,
                this,
                [powerProcess, bluetoothStatusProcess]()
                {
                    bluetoothStatusProcess->start(
                        "bluetoothctl",
                        {"show"}
                    );

                    powerProcess->deleteLater();
                }
            );

            powerProcess->start(
                "bluetoothctl",
                {
                    "power",
                    checked ? "on" : "off"
                }
            );
        }
    );

    // ========================================================
    // Network information
    // ========================================================

    bool foundNetwork = false;

    for (const QNetworkInterface& interface
         : QNetworkInterface::allInterfaces()) {

        for (const QNetworkAddressEntry& entry
             : interface.addressEntries()) {

            if (
                interface.name() == "wlan0"
                && entry.ip().protocol()
                   == QAbstractSocket::IPv4Protocol
            ) {
                ipAddress->setText(
                    QString("IP %1")
                        .arg(entry.ip().toString())
                );

                foundNetwork = true;
            }
        }
    }

    if (foundNetwork) {
        wifi->setText("Wi-Fi: connected");

        setPixelArt(
            wifiIcon,
            ":/assets/wifi_connected.png",
            QSize(32, 24)
        );
    }
    else {
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
            const QString output =
                QString::fromUtf8(
                    networkProcess->readAllStandardOutput()
                );

            for (const QString& line
                 : output.split('\n')) {

                if (line.startsWith("*:")) {
                    network->setText(
                        line.sliced(2).trimmed()
                        + " Wi-Fi"
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
    // Battery information
    // ========================================================

    auto* socket = new QLocalSocket(this);

    connect(
        socket,
        &QLocalSocket::connected,
        this,
        [socket]()
        {
            socket->write("get battery\n");
            socket->write("get battery_power_plugged\n");
            socket->write("get battery_allow_charging\n");
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

                if (line.startsWith("battery:")) {
                    bool ok = false;

                    const double value =
                        line.section(':', 1)
                            .trimmed()
                            .toDouble(&ok);

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
                    powerPlugged =
                        line.section(':', 1)
                            .trimmed()
                            == "true";

                    gotPowerPlugged = true;
                }
                else if (
                    line.startsWith(
                        "battery_allow_charging:"
                    )
                ) {
                    chargingAllowed =
                        line.section(':', 1)
                            .trimmed()
                            == "true";

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
    // Clock and date
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
    // Shutdown logic
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
                            << process
                                   ->readAllStandardError();
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
                                << process
                                       ->errorString();
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
    // Page navigation
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
            pageStack->setCurrentWidget(settingsPage);
        }
    );

    connect(
        btnBack,
        &QPushButton::clicked,
        this,
        [this]()
        {
            pageStack->setCurrentWidget(overviewPage);
        }
    );

    // ========================================================
    // Brightness slider
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
// Clock update
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