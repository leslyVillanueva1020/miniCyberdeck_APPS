#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFontDatabase>

#include "HomeWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ============== LOAD FONTS ===============
    QString fontPath = ":/fonts/PixelifySans-VariableFont_wght.ttf";

    int fontId = QFontDatabase::addApplicationFont(fontPath);

    if (fontId == -1) {
        qDebug() << "Failed to load Pixelify Sans";
    }
    else {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId);
    }

    //=============== LOAD QSS ==================
    QString filepath = "styles/launcher.qss";
    QFile file(filepath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString text = stream.readAll();
        app.setStyleSheet(text);
        file.close();
    }
    else {
        qDebug() << "Could not open stylesheet:" << filepath;
    }

    HomeWindow window;
    window.show();

    return app.exec();
}