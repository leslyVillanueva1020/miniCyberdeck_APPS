#include <QWidget>
#include <QVBoxLayout>
#include <QProcess>
#include <QStackedWidget>
#include <QGridLayout>
#include <QPoint>
#include <QMouseEvent>

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
 
        QPoint swipeStart; //where the mouse first touched
        bool trackingSwipe; //whether a swipe is currently in progress

        void openNotesApp();
    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
};