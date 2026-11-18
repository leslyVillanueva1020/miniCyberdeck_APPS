#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QFontDatabase>
#include <QDebug>

#include "systemWindow.h"

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);

    // ======================== Load Pixelify Sans ========================

    int regularFontId =
        QFontDatabase::addApplicationFont(
            ":/fonts/PixelifySans-Regular.ttf"
        );

    int mediumFontId =
        QFontDatabase::addApplicationFont(
            ":/fonts/PixelifySans-Medium.ttf"
        );

    int semiBoldFontId =
        QFontDatabase::addApplicationFont(
            ":/fonts/PixelifySans-SemiBold.ttf"
        );

    qDebug() << "Regular font loaded:" << regularFontId;
    qDebug() << "Medium font loaded:" << mediumFontId;
    qDebug() << "SemiBold font loaded:" << semiBoldFontId;

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