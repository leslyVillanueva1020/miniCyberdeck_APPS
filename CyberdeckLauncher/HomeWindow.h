#include <QWidget>
#include <QVBoxLayout>
#include <QProcess>
#include <QStackedWidget>
#include <QGridLayout>

class HomeWindow: public QWidget{
    public:
        HomeWindow(QWidget* parent = nullptr);

    private:
        QStackedWidget* pageStack;
        QGridLayout* gridApps1;
        QGridLayout* gridApps2;
        QVBoxLayout* mainLayout;

        QWidget* page1;
        QWidget* page2;

        QProcess* notesProcess;

        void openNotesApp();
};