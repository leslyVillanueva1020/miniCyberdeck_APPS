#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class systemWindow: public QWidget
{
    private:
        QVBoxLayout* mainLayout;

        QHBoxLayout* titleRow;
        QLabel* title;

        QVBoxLayout* midSection;

        QHBoxLayout* topMidSection;
        QVBoxLayout* rightSide;
        QVBoxLayout* leftSide;

        QHBoxLayout* bottomMidSection;
        QVBoxLayout* infoSection;

        QHBoxLayout* bottomRow;
        QPushButton* btnHome;
        QPushButton* btnShutdown;
        QPushButton* btnSettings;

    public:
        systemWindow(QWidget* parent = nullptr);

};