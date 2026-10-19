#include <QWidget>
#include <QVBoxLayout>
#include <QProcess>

class HomeWindow: public QWidget{
    public:
        HomeWindow(QWidget* parent = nullptr);

    private:
        QVBoxLayout* mainLayout;

        QProcess* notesProcess;

        void openNotesApp();
};