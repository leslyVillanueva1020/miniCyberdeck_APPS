#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>


class NotesWindow: public QWidget
{
    //member variables = what an obj HAS
    private:
        QString currentNoteName;
        QTextEdit* textEdit;

        QPushButton* btnNew;
        QPushButton* btnDelete;
        QPushButton* btnBack;

        QLineEdit* noteTitleEdit;

        QLabel* titleLabel;

        QVBoxLayout* mainLayout;
        QHBoxLayout* buttonLayout;

        QVBoxLayout* listPageLayout;
        QVBoxLayout* editorPageLayout;
        QHBoxLayout* editorTopLayout;

        QListWidget* noteList;

        QStackedWidget* pageStack;

        QWidget* listPage;
        QWidget* editorPage;

        void saveNote();
        void loadNote(QListWidgetItem* item);
        void newNote();
        void loadNoteList();
        void deleteNote();
        void goBack();
        void renameNote();
        void showDeleteDialog();
        void goHome();

    // memmeber functions what an obj DOES
    public:
        NotesWindow(QWidget* parent = nullptr);
};
