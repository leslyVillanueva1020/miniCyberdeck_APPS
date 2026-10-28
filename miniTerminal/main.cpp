#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "terminalWindow.h"
int main(int argc, char *argv[]){
    QApplication app(argc, argv);\

    // -------- LOAD OUR QSS --------
    QString filepath = "styles/terminal.qss";
    QFile file(filepath);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        QString text = stream.readAll();
        app.setStyleSheet(text);
    }

    terminalWindow window;
    window.show();

    return app.exec();
}