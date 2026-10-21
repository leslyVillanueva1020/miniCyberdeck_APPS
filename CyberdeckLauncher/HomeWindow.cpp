#include "HomeWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QtGlobal>

HomeWindow:: HomeWindow(QWidget* parent): QWidget(parent){
    pageStack = new QStackedWidget(this);

    page1 = new QWidget();
    gridApps1 = new QGridLayout();

    page2 = new QWidget();
    gridApps2 = new QGridLayout();

    page1->setLayout(gridApps1);
    page2->setLayout(gridApps2);

    pageStack->addWidget(page1);
    pageStack->addWidget(page2);

    mainLayout = new QVBoxLayout(this);
    notesProcess = new QProcess(this);

    trackingSwipe = false;

    // ================ PAGE 1 =============================
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* appName = new QLabel("AQUARIUM LS");
    QLabel* time = new QLabel("10:34 AM");
    QLabel* battery = new QLabel("87%");
    header->addWidget(appName);
    header->addWidget(time);
    header->addWidget(battery);

    QPushButton* btnOpenNotes = new QPushButton("Notes");
    QPushButton* btnOpenMusic = new QPushButton("Music");
    QPushButton* btnOpenBrowser = new QPushButton("Browser");
    QPushButton* btnOpenSettings = new QPushButton("Settings");
    gridApps1->addWidget(btnOpenNotes, 0,0); //0, 0 reams row 0 column 0
    gridApps1->addWidget(btnOpenMusic, 0, 1);
    gridApps1->addWidget(btnOpenBrowser, 1,0);
    gridApps1->addWidget(btnOpenSettings,1, 1);

    // ======================= PAGE 2 =============================
    QPushButton* btnOpenTerminal = new QPushButton("Terminal");
    gridApps2->addWidget(btnOpenTerminal, 0, 0);


    // ============ NAVIGATION TEMP =====================
    QHBoxLayout* navigationLayout = new QHBoxLayout();
    QPushButton* btnPrev = new QPushButton("<");
    QPushButton* btnNext = new QPushButton(">");
    QLabel* pageIndicator = new QLabel("● ○");

    navigationLayout->addWidget(btnPrev);
    navigationLayout->addWidget(pageIndicator);
    navigationLayout->addWidget(btnNext);

    //============= ADDING TO MAIN LAYOUT ========================
    mainLayout->addLayout(header);
    mainLayout->addWidget(pageStack);
    mainLayout->addLayout(navigationLayout);

    connect(btnNext, &QPushButton::clicked, this, [this](){
        int nextPage = pageStack->currentIndex() + 1;
        if(nextPage < pageStack->count()){
            pageStack->setCurrentIndex(nextPage);
        }
    });

    connect(btnPrev, &QPushButton::clicked, this, [this](){
        int previousPage = pageStack->currentIndex() - 1;
        if (previousPage >= 0) {
            pageStack->setCurrentIndex(previousPage);
        }
    });

    connect(pageStack, &QStackedWidget::currentChanged, this,
        [pageIndicator](int pageIndex){
            if(pageIndex == 0){
                pageIndicator->setText("● ○");
            }
            else if(pageIndex == 1){
                pageIndicator->setText("○ ●");
            }
        }
    );

    connect(
        btnOpenNotes,
        &QPushButton:: clicked,
        this,
        &HomeWindow::openNotesApp
    );

    connect(notesProcess, &QProcess::started, this, [this](){
        qDebug() << "MiniNotes Started successfully!";
        hide();
    });

    connect(notesProcess, &QProcess::errorOccurred, this,
    [this](QProcess::ProcessError error){
        qDebug() << "MiniNotes failed to start:"
                 << notesProcess->errorString();
    });

    connect(notesProcess, &QProcess::finished, this,
    [this](int exitCode, QProcess::ExitStatus exitStatus){
        qDebug() << "MiniNotes closed.";
        qDebug() << "Exit code:" << exitCode;

        show();
    });
}

void HomeWindow::openNotesApp(){
    if(notesProcess->state() == QProcess::Running){
        qDebug() << "MiniNotes is already running.";
        return;
    }
    qDebug() << "Open Notes App button clicked!";

    QString notesProgram =
        "C:/Users/alexi/miniCyberdeck_APPS/MiniNotes/build/MiniNotes.exe";

    QString notesWorkingDirectory =
        "C:/Users/alexi/miniCyberdeck_APPS/MiniNotes";

    notesProcess->setWorkingDirectory(notesWorkingDirectory);
    notesProcess->start(notesProgram);   
}

void HomeWindow::mousePressEvent(QMouseEvent* event){
    swipeStart = event->position().toPoint();
    trackingSwipe = true;
    event->accept();

    qDebug() << "Mouse press detected!";
}

void HomeWindow::mouseReleaseEvent(QMouseEvent* event){
    if(!trackingSwipe){
        return;
    }

    QPoint swipEnd = event->position().toPoint();
    int horizontalDistance = swipEnd.x() - swipeStart.x(); //negative # mean, left positive # means right
    int verticalDistance = swipEnd.y() - swipeStart.y();
    qDebug() << "Swipe distance: " << horizontalDistance;

    const int minimumSwipeDistance = 80;

    if(qAbs(horizontalDistance) < minimumSwipeDistance){
        trackingSwipe = false;
        event->accept();
        return;
    }

    if(qAbs(horizontalDistance) <= qAbs(verticalDistance)){
        trackingSwipe = false;
        event->accept();
        return;
    }

    if (horizontalDistance < 0) {
        int nextPage = pageStack->currentIndex() + 1;

        if (nextPage < pageStack->count()) {
            pageStack->setCurrentIndex(nextPage);
        }
    }
    else {
        int previousPage = pageStack->currentIndex() - 1;

        if (previousPage >= 0) {
            pageStack->setCurrentIndex(previousPage);
        }
    }

    trackingSwipe = false;

    event->accept();
}