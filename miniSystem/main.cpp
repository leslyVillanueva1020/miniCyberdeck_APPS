#include <QApplication>

#include "systemWindow.h"

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);

    systemWindow window;
    window.show();

    return app.exec();
}