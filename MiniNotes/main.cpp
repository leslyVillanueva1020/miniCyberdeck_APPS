#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFontDatabase>

#include "NotesWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // -------- LOAD OUR CUSTOM FONTS --------

    int fredokaId = QFontDatabase::addApplicationFont(
        "assets/fonts/Fredoka-Regular.ttf"
    );

    int fredokaSemiBoldId = QFontDatabase::addApplicationFont(
        "assets/fonts/Fredoka-SemiBold.ttf"
    );

    int nunitoId = QFontDatabase::addApplicationFont(
        "assets/fonts/Nunito-Regular.ttf"
    );

    int nunitoSemiBoldId = QFontDatabase::addApplicationFont(
        "assets/fonts/Nunito-SemiBold.ttf"
    );

    // // Check whether our fonts loaded successfully
    // qDebug() << "Fredoka Regular:" << fredokaId;
    // qDebug() << "Fredoka SemiBold:" << fredokaSemiBoldId;
    // qDebug() << "Nunito Regular:" << nunitoId;
    // qDebug() << "Nunito SemiBold:" << nunitoSemiBoldId;

    // -------- LOAD OUR QSS --------

    QString filepath = "styles/notes.qss";
    QFile file(filepath);

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString text = stream.readAll();
        app.setStyleSheet(text);
    }

    // -------- CREATE OUR NOTES WINDOW --------

    NotesWindow window;
    window.show();

    return app.exec();
}