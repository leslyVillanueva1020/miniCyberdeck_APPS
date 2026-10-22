#include <QWidget>
#include <QVBoxLayout>

class terminalWindow: public QWidget{
    public:
        terminalWindow(QWidget* parent = nullptr);
    private:
        QVBoxLayout* mainLayout;
};