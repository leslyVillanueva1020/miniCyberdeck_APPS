#include <QApplication>

#include "terminalWindow.h"
int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    terminalWindow window;
    window.show();

    return app.exec();
}