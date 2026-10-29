#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "systemWindow.h"

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);

    // --------- LOAD OUR QSS -------------
    QString filepath = "styles/system.qss";
    QFile file(filepath);

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        QString text = stream.readAll();
        app.setStyleSheet(text);
    }

    //--------- CREATE OUR SYSTEM WINDOW -------------------
    systemWindow window;
    window.show();

    return app.exec();
}