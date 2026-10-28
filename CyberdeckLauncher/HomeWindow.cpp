#include "HomeWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QtGlobal>

HomeWindow:: HomeWindow(QWidget* parent): QWidget(parent){
    resize(480,320);
    pageStack = new QStackedWidget(this);

    page1 = new QWidget();
    gridApps1 = new QGridLayout();

    page2 = new QWidget();
    gridApps2 = new QGridLayout();

    page1->setLayout(gridApps1);
    page2->setLayout(gridApps2);

    pageStack->addWidget(page1);
    pageStack->addWidget(page2);

    page1->installEventFilter(this);
    page2-> installEventFilter(this);
    pageStack->installEventFilter(this);

    mainLayout = new QVBoxLayout(this);
    notesProcess = new QProcess(this);
    terminalProcess = new QProcess(this);

    trackingSwipe = false;

    // ================ HEADER =============================
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* appName = new QLabel("AQUARIUM LS");
    QLabel* time = new QLabel("10:34 AM");
    QLabel* battery = new QLabel("87%");
    header->addWidget(appName);
    header->addWidget(time);
    header->addWidget(battery);


    // ================ PAGE 1 =============================
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


    for(QWidget* widget: page1->findChildren<QWidget*>()){
        widget->installEventFilter(this);
    }

    for(QWidget* widget: page2->findChildren<QWidget*>()){
        widget->installEventFilter(this);
    }
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

    connect(
        notesProcess, 
        &QProcess::started, 
        this, 
        [this](){
        qDebug() << "MiniNotes Started successfully!";
        hide();
    });

    connect(
    btnOpenTerminal,
    &QPushButton::clicked,
    this,
    &HomeWindow::openTerminalApp
    );

    connect(
        terminalProcess,
        &QProcess::started,
        this,
        [this]() {
            qDebug() << "MiniTerminal started successfully!";
            hide();
        }
    );

    connect(
        terminalProcess,
        &QProcess::errorOccurred,
        this,
        [this](QProcess::ProcessError) {
            qDebug() << "MiniTerminal failed to start:"
                    << terminalProcess->errorString();
        }
    );

    connect(
        terminalProcess,
        &QProcess::finished,
        this,
        [this](int exitCode, QProcess::ExitStatus) {
            qDebug() << "MiniTerminal closed.";
            qDebug() << "Exit code:" << exitCode;

            show();
        }
    );

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

void HomeWindow::openTerminalApp()
{
    if (terminalProcess->state() == QProcess::Running) {
        qDebug() << "MiniTerminal is already running.";
        return;
    }

    qDebug() << "Open Terminal App button clicked!";

    QString terminalProgram =
        "C:/Users/alexi/miniCyberdeck_APPS/miniTerminal/build/miniTerminal.exe";

    QString terminalWorkingDirectory =
        "C:/Users/alexi/miniCyberdeck_APPS/miniTerminal";

    terminalProcess->setWorkingDirectory(terminalWorkingDirectory);
    terminalProcess->start(terminalProgram);
}

bool HomeWindow::eventFilter(QObject* watched, QEvent* event){
    Q_UNUSED(watched);

    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton){
            swipeStart = mouseEvent->globalPosition().toPoint();
            trackingSwipe = true;
        }

        return false;
    } 

    if(event->type() == QEvent::MouseButtonRelease){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (!trackingSwipe || mouseEvent->button() != Qt::LeftButton) {
            return false;
        }

        QPoint swipeEnd = mouseEvent->globalPosition().toPoint();

        int horizontalDistance = swipeEnd.x() - swipeStart.x();

        int verticalDistance = swipeEnd.y() - swipeStart.y();

        trackingSwipe = false;

        const int minimumSwipeDistance = 70;

        if (qAbs(horizontalDistance) < minimumSwipeDistance) {
            return false;
        }

        if (qAbs(horizontalDistance) <= qAbs(verticalDistance)) {
            return false;
        }

        if (horizontalDistance < 0) {
            int nextPage =
                pageStack->currentIndex() + 1;

            if (nextPage < pageStack->count()) {
                pageStack->setCurrentIndex(nextPage);
            }
        }
        else {
            int previousPage =
                pageStack->currentIndex() - 1;

            if (previousPage >= 0) {
                pageStack->setCurrentIndex(previousPage);
            }
        }

        return true;
    }

    return QWidget::eventFilter(watched, event);
}
