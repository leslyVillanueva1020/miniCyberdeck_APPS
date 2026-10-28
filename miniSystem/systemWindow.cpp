#include "systemWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

systemWindow::systemWindow(QWidget* parent): QWidget(parent){
    mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout");

    // ================== TITLE ROW SECTION =====================
    titleRow = new QHBoxLayout();
    titleRow->setObjectName("titleRow");

    title = new QLabel("Aquarium LS miniSystem");
    QLabel* fishes = new QLabel("🐟🫧");

    titleRow->addWidget(title);
    titleRow->addWidget(fishes);

    mainLayout->addLayout(titleRow);

    // =================== MIDDLE VBOX SECTION =====================
    midSection = new QVBoxLayout();
    midSection->setObjectName("midSection");

    //----------- topMidSection --------------
    topMidSection = new QHBoxLayout();

    // left side---------------
    leftSide = new QVBoxLayout();
    leftSide->setObjectName("leftSide");

    QLabel* time = new QLabel("11:37");
    QLabel* date = new QLabel("Tue, Oct 28");

    leftSide->addWidget(time);
    leftSide->addWidget(date);

    topMidSection->addLayout(leftSide);

    // right side---------------
    rightSide = new QVBoxLayout();
    rightSide->setObjectName("rightSide");

    QLabel* battery = new QLabel("Battery 82% not charging");
    QLabel* wifi = new QLabel("Connected Home Wifi");

    rightSide->addWidget(battery);
    rightSide->addWidget(wifi);

    topMidSection->addLayout(rightSide);

    midSection->addLayout(topMidSection);

    //------------- Bottom Section -----------
    bottomMidSection = new QHBoxLayout();
    bottomMidSection->setObjectName("bottomMidSection");

    QLabel* rasIcon = new QLabel("Ras Pi Icon");

    infoSection = new QVBoxLayout();

    QLabel* raspberryInfo = new QLabel("Raspberry Pi Zero 2W");
    QLabel* rasIP = new QLabel("IP 192.168.1.42");
    QLabel* rasStorage = new QLabel("Storage 64%");

    infoSection->addWidget(raspberryInfo);
    infoSection->addWidget(rasIP);
    infoSection->addWidget(rasStorage);

    bottomMidSection->addWidget(rasIcon);

    bottomMidSection->addLayout(infoSection);

    midSection->addLayout(bottomMidSection);

    mainLayout->addLayout(midSection);

    // ================== BOTTOM ROW SECTION =======================
    bottomRow = new QHBoxLayout();

    btnHome =new QPushButton("Home");
    btnHome->setObjectName("btnHome");

    btnShutdown = new QPushButton("Shutdown");
    btnShutdown->setObjectName("btnShutdown");

    btnSettings = new QPushButton("Settings");
    btnSettings->setObjectName("btnSettings");

    bottomRow->addWidget(btnHome);
    bottomRow->addWidget(btnShutdown);
    bottomRow->addWidget(btnSettings);
      
    mainLayout->addLayout(bottomRow);

}