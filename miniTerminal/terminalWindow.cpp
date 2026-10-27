#include "terminalWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QIcon>
#include <QDebug>
#include <QProcess>
#include <QTextCursor>
#include <QTextCharFormat>

terminalWindow:: terminalWindow(QWidget* parent): QWidget(parent){
    setObjectName("terminalWindow");
    mainLayout = new QVBoxLayout(this);

    terminalProcess = new QProcess(this);

    displayCommands = new QPlainTextEdit();
    displayCommands->setReadOnly(true);
    displayCommands->setObjectName("displayCommands");

    commandInput = new QLineEdit();
    commandInput->setPlaceholderText("Type a command ...");
    commandInput->setObjectName("commandInput");

    btnClear = new QPushButton("");
    QIcon clearIcon("icons/trashh.svg");
    btnClear->setIcon(clearIcon);
    btnClear->setObjectName("btnClear");

    btnHistory = new QPushButton("");
    QIcon historyIcon("icons/rotate-ccw-clock.svg");
    btnHistory->setIcon(historyIcon);
    btnHistory->setObjectName("btnHistory");

    btnRun = new QPushButton("");
    QIcon playIcon("icons/play.svg");
    btnRun->setIcon(playIcon);
    btnRun->setObjectName("btnRun");

    bottomRow = new QHBoxLayout();
    bottomRow->addWidget(commandInput, 1);
    bottomRow->addWidget(btnClear);
    bottomRow->addWidget(btnHistory);
    bottomRow->addWidget(btnRun);

    historyDrawer = new QFrame();
    historyDrawer->setObjectName("historyDrawer");
    historyDrawer->setVisible(false);

    historyList = new QListWidget();
    historyList->setObjectName("historyList");

    QVBoxLayout* drawerLayout = new QVBoxLayout(historyDrawer);
    drawerLayout->addWidget(historyList);

    QHBoxLayout* mainRow = new QHBoxLayout();
    QVBoxLayout* terminalColumn = new QVBoxLayout();

    terminalColumn->addWidget(displayCommands);
    terminalColumn->addLayout(bottomRow);

    mainRow->addLayout(terminalColumn, 1);
    mainRow->addWidget(historyDrawer);

    mainLayout->addLayout(mainRow);


    connect(
        btnClear, //send signal
        &QPushButton::clicked, 
        displayCommands, //recieves signal
        &QPlainTextEdit ::clear //func that runs
    );

    connect(
        btnRun,
        &QPushButton::clicked,
        this,
        &terminalWindow::runCommand
    );

    // if enter pressed after typing run command as well
    connect(
        commandInput,
        &QLineEdit::returnPressed,
        this,
        &terminalWindow::runCommand
    );

    connect(
        terminalProcess,
        &QProcess::readyReadStandardOutput,
        this,
        &terminalWindow::readOutput
    );

    connect(
        terminalProcess,
        &QProcess::readyReadStandardError,
        this,
        &terminalWindow::readError
    );

    connect(
        terminalProcess,
        &QProcess::finished,
        this,
        &terminalWindow::processFinished
    );

    // not sure if commandInput should also be cleared
    connect(
        btnClear,
        &QPushButton::clicked,
        commandInput,
        &QLineEdit::clear
    );

    connect(
        btnHistory,
        &QPushButton::clicked,
        this,
        &terminalWindow::toggleHistory
    );

    connect(
        historyList,
        &QListWidget::itemClicked,
        this,
        &terminalWindow::historyItemSelected
    );
    
    // MUST BE AT END
    startShell();
}

void terminalWindow:: runCommand(){
    QString command = commandInput->text().trimmed();

    if(command.isEmpty()){
        return;
    }

    QByteArray commandBytes = command.toLocal8Bit();
    commandBytes.append('\n');

    terminalProcess->write(commandBytes);

    HistoryEntry newEntry;

    newEntry.command = command;
    newEntry.timeStamp = QDateTime::currentDateTime();

    history.append(newEntry);

    commandInput-> clear();
}

void terminalWindow::readOutput(){
    QString output =
        QString::fromLocal8Bit(
            terminalProcess->readAllStandardOutput()
        );

    const QString promptStart = "user@cyberdeck:";
    const QColor promptColor("#FF3DBD");
    const QColor outputColor("#13E7F5");

    int searchPosition = 0;

    while(searchPosition < output.length()){
        int promptPosition =
            output.indexOf(promptStart, searchPosition);

        // No more prompts were found.
        // Everything remaining is normal cyan output.
        if(promptPosition == -1){
            appendColoredText(
                output.mid(searchPosition),
                outputColor
            );

            break;
        }

        // Add any normal output before the prompt in cyan.
        if(promptPosition > searchPosition){
            appendColoredText(
                output.mid(
                    searchPosition,
                    promptPosition - searchPosition
                ),
                outputColor
            );
        }

        int promptEnd =
            output.indexOf('>', promptPosition);

        // If the prompt is incomplete, keep it cyan for now.
        if(promptEnd == -1){
            appendColoredText(
                output.mid(promptPosition),
                outputColor
            );

            break;
        }

        // Include the > character in the pink prompt.
        QString prompt =
            output.mid(
                promptPosition,
                promptEnd - promptPosition + 1
            );

        appendColoredText(prompt, promptColor);

        // Continue processing text after the prompt.
        searchPosition = promptEnd + 1;
    }
}

void terminalWindow::readError(){
    QByteArray error = terminalProcess->readAllStandardError();

    appendColoredText(
        "[error] " + QString::fromLocal8Bit(error),
        QColor("#13E7F5")
    );
}

void terminalWindow::processFinished( int exitCode, QProcess::ExitStatus exitStatus){
    if(exitStatus == QProcess::NormalExit){
        qDebug() << "Process finished with exit code: " << exitCode;
    }
    else{
        qDebug() << "Process crashed.";
    }
}

void terminalWindow:: toggleHistory(){
    if(historyDrawer->isVisible()){
        historyDrawer->setVisible(false);
    }
    else {
        updateHistoryList();
        historyDrawer->setVisible(true);
    }

}

void terminalWindow:: updateHistoryList(){
    historyList->clear();

    for(HistoryEntry currEntry: history){
        QString formattedTime = currEntry.timeStamp.toString("hh:mm:ss AP");
        QString combinedInfo = formattedTime + " " + currEntry.command;
        
        QListWidgetItem* item = new QListWidgetItem(combinedInfo);
        item->setData(Qt::UserRole, currEntry.command);
        historyList->addItem(item);
    }
}

void terminalWindow:: historyItemSelected(QListWidgetItem* item){
    QVariant commandData = item->data(Qt::UserRole);
    QString command = commandData.toString();

    commandInput->setText(command);
}

void terminalWindow::startShell(){
    #ifdef Q_OS_WIN
        terminalProcess->start(
            "cmd.exe",
            {"/K", "prompt user@cyberdeck:$P$G"}
        );
    #else
        terminalProcess->start("/bin/bash", {"-i"});
    #endif
}

void terminalWindow::appendColoredText(
    const QString& text,
    const QColor& color
){
    QTextCursor cursor = displayCommands->textCursor();

    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format;
    format.setForeground(color);

    cursor.insertText(text, format);

    displayCommands->setTextCursor(cursor);
}