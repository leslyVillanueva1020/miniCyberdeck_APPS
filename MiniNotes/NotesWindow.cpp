#include "NotesWindow.h"
#include "NotebookTextEdit.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QMessageBox>
#include <QDialogButtonBox>

NotesWindow:: NotesWindow(QWidget* parent)
    : QWidget(parent){                     
    // this means that id nobody gives me a parent use nullptr when declaration

    pageStack = new QStackedWidget();

    listPage = new QWidget();
    listPage->setObjectName("listPage");

    editorPage = new QWidget();
    editorPage->setObjectName("editorPage");

    mainLayout = new QVBoxLayout(this); //this = NotesWindow
    listPageLayout = new QVBoxLayout();
    editorPageLayout = new QVBoxLayout();
    editorTopLayout = new QHBoxLayout();

    pageStack->addWidget(listPage);
    pageStack->addWidget(editorPage);

    mainLayout->addWidget(pageStack);

    btnNew = new QPushButton("New Note", listPage);
    btnNew->setObjectName("btnNew");

    btnDelete = new QPushButton("", listPage);
    QIcon trashIcon("assets/icons/trash.svg");
    btnDelete->setIcon(trashIcon);

    // btnDelete->setIcon(QIcon("assets/icons/trash.svg"));
    btnDelete->setFixedWidth(52);
    btnDelete->setToolTip("Delete notes");
    btnDelete->setAccessibleName("Delete notes");

    btnDelete->setIconSize(QSize(24, 24));
    btnDelete->setObjectName("btnDelete");

    titleLabel = new QLabel("♡ my notes ✿", listPage);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedWidth(150); //TODO FIX LATER BASED ON RASBPERRY PI

    noteList = new QListWidget(listPage);
    noteList->setObjectName("notesList");

    btnBack = new QPushButton("", editorPage);
    QIcon backArrowIcon("assets/icons/arrow-left.svg");
    btnBack->setIcon(backArrowIcon);
    btnBack->setObjectName("btnBack");

    btnBack->setFixedWidth(40);
    btnBack->setIconSize(QSize(24,24));
    btnBack->setToolTip("Back to notes");
    btnBack->setAccessibleName("Back to notes");

    noteTitleEdit = new QLineEdit("Note", editorPage);
    noteTitleEdit->setObjectName("noteTitleEdit");

    textEdit = new NotebookTextEdit(editorPage);
    textEdit->setObjectName("textEdit");

// ---------------- SETTING UP 2 SCREEN FOR ListPage ---------------------
    
    listPageLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    listPageLayout->addWidget(noteList);

    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(btnNew, 1);
    buttonLayout->addWidget(btnDelete);

    listPageLayout->addLayout(buttonLayout);

    listPage->setLayout(listPageLayout);

// --------------- SETTING UP 2 SCREEN FOR EditorPage ----------------------
    editorTopLayout->addWidget(btnBack);
    editorTopLayout->addWidget(noteTitleEdit);

    editorPageLayout->addLayout(editorTopLayout);
    editorPageLayout->addWidget(textEdit);

    editorPage->setLayout(editorPageLayout);

// --------------- SETTING UP TEMP HOME BTN !!!!!FINISH LATER!!!!!!----------------------
// this will later be connected to physical button
    QPushButton* btnHomeScreen = new QPushButton("Home");
    mainLayout->addWidget(btnHomeScreen);
    connect(
        btnHomeScreen,
        &QPushButton::clicked,
        this,
        &NotesWindow::goHome
    );

    connect(
        btnNew, //sender
        &QPushButton::clicked, //&sender type::signal
        this, //reciever
        &NotesWindow::newNote //&reciever typerr :: function
    );

    connect(
        btnDelete,
        &QPushButton::clicked,
        this,
        &NotesWindow::showDeleteDialog
    );

    connect(
        btnBack,
        &QPushButton::clicked,
        this,
        &NotesWindow::goBack
    );

    connect(
        noteList,
        &QListWidget::itemClicked,
        this,
        &NotesWindow::loadNote
    );

    connect(
        noteTitleEdit,
        &QLineEdit::editingFinished,
        this,
        &NotesWindow::renameNote
    );

    loadNoteList();

}

void NotesWindow:: saveNote(){
    if(currentNoteName.isEmpty()){
        return;
    }

    // qDebug() << "Save button clicked!";
    QString text = textEdit->toPlainText(); //the actual note being typed
    // qDebug() << text; 

    QDir dir;
    dir.mkpath("notes");

    QString filePath = "notes/" + currentNoteName + ".txt";
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        //file is QFile obj but &file is address of that QFile obj
        QTextStream stream(&file);
        stream << text;
    }
}

void NotesWindow:: loadNote(QListWidgetItem* item){
    currentNoteName = item->text();

    QString filePath = "notes/" + currentNoteName + ".txt";
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        QString text = stream.readAll();
        textEdit->setPlainText(text);

        //same thing as line above just in a shorter way
        // textEdit->setPlainText(stream.readAll());

        
        noteTitleEdit->setText(currentNoteName);
        pageStack->setCurrentWidget(editorPage);
    }
}


void NotesWindow::newNote()
{
    QInputDialog dialog(this);

    dialog.setWindowTitle("New Note");
    dialog.setLabelText("Enter note title:");
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setTextEchoMode(QLineEdit::Normal);
    dialog.setObjectName("newNoteDialog");

    // Get the buttons created by QInputDialog
    QDialogButtonBox* buttonBox =
        dialog.findChild<QDialogButtonBox*>();

    if (buttonBox) {
        // Center the buttons using a horizontal layout
        QHBoxLayout* centeredLayout = new QHBoxLayout();

        QWidget* buttonContainer = new QWidget(&dialog);
        buttonContainer->setObjectName("newNoteButtonContainer");

        QPushButton* okButton =
            buttonBox->button(QDialogButtonBox::Ok);

        QPushButton* cancelButton =
            buttonBox->button(QDialogButtonBox::Cancel);

        if (okButton && cancelButton) {
            // Move buttons into our centered layout
            centeredLayout->addStretch();
            centeredLayout->addWidget(okButton);
            centeredLayout->addWidget(cancelButton);
            centeredLayout->addStretch();

            buttonContainer->setLayout(centeredLayout);

            // Replace the original button box's layout position
            if (QVBoxLayout* layout =
                    qobject_cast<QVBoxLayout*>(dialog.layout())) {
                layout->addWidget(buttonContainer);
            }
        }
    }

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString noteName = dialog.textValue().trimmed();

    if (noteName.isEmpty()) {
        return;
    }

    QDir dir;
    dir.mkpath("notes");

    QString filePath = "notes/" + noteName + ".txt";
    QFile file(filePath);

    if (QFile::exists(filePath)) {
        qDebug() << "Note already exists:" << filePath;
        return;
    }

    if (file.open(QIODevice::WriteOnly)) {
        currentNoteName = noteName;

        QListWidgetItem* newItem = new QListWidgetItem(noteName);
        noteList->addItem(newItem);
        noteList->setCurrentItem(newItem);

        textEdit->clear();
        noteTitleEdit->setText(noteName);
        pageStack->setCurrentWidget(editorPage);
    } else {
        qDebug() << "File error:" << file.errorString();
    }
}


void NotesWindow:: loadNoteList(){
    QDir dir("notes");

    QStringList filters;
    filters << "*.txt";
    QStringList files = dir.entryList(filters);

    for(const QString& file: files){
        QFileInfo info(file);
        QString noteName = info.completeBaseName();
        noteList->addItem(noteName);
    }
}

void NotesWindow:: goBack(){
    // qDebug() << "BACK button clicked!";
    saveNote();
    pageStack->setCurrentWidget(listPage);
}


void NotesWindow:: renameNote(){
    QString old = currentNoteName;
    QString text = noteTitleEdit->text();
    text = text.trimmed();

    if(text.isEmpty()){
        noteTitleEdit->setText(old);
        return;
    }

    if(text == old){
        return;
    }

    QString oldPath = "notes/" + currentNoteName + ".txt";
    QString newPath = "notes/" + text + ".txt";

    if(QFile::exists(newPath)){
        noteTitleEdit->setText(old);
        return;
    }

    if (QFile::rename(oldPath, newPath))
    {
        currentNoteName = text;
        noteList->currentItem()->setText(text);
    }
}

void NotesWindow:: showDeleteDialog(){
    QDialog dialog(this);
    dialog.setWindowTitle("Delete Notes");
    dialog.setObjectName("deleteDialog");

    QListWidget* deleteList = new QListWidget(&dialog); //&dialog is parent
    deleteList->setObjectName("deleteList");

    QPushButton* btnCancelDialog = new QPushButton("Cancel", &dialog);
    QPushButton* btnDeleteSelected = new QPushButton("Delete", &dialog);

    btnCancelDialog->setObjectName("btnCancelDialog");
    btnDeleteSelected->setObjectName("btnDeleteSelected");
        
    QVBoxLayout* dialogLayout = new QVBoxLayout(&dialog);
    QHBoxLayout* dialogButtonLayout = new QHBoxLayout();

    QLabel* deleteTitleLabel = new QLabel("♡ Delete Notes ✿", &dialog);
    deleteTitleLabel->setObjectName("deleteTitleLabel");
    deleteTitleLabel->setAlignment(Qt::AlignCenter);
    deleteTitleLabel->setFixedWidth(150);    
    
    dialogLayout->addWidget(deleteTitleLabel, 0, Qt::AlignHCenter);

    dialogButtonLayout->addWidget(btnCancelDialog);
    dialogButtonLayout->addWidget(btnDeleteSelected);

    int count = noteList->count();

    for(int i=0; i<count; i++){
        QListWidgetItem* item = noteList->item(i);
        QString title = item->text();
        QListWidgetItem* deleteItem = new QListWidgetItem(title);
        deleteItem->setCheckState(Qt::Unchecked);
        deleteList->addItem(deleteItem);
    }

    dialogLayout->addWidget(deleteList);
    dialogLayout->addLayout(dialogButtonLayout);

    connect(
        btnCancelDialog,
        &QPushButton::clicked,
        &dialog,
        &QDialog::reject
    );

    connect(
        btnDeleteSelected,
        &QPushButton::clicked,
        this,
        [this, deleteList, &dialog](){
            int count = deleteList->count();
            for(int i=0; i<count;i++){
                QListWidgetItem* item2 = deleteList->item(i);
                QString title = item2->text();

                if(item2->checkState() == Qt::Checked){
                    // qDebug() << item2->text();
                    QString filePath = "notes/" + title + ".txt";
                    QFile::remove(filePath);

                    if (title == currentNoteName) {
                        currentNoteName.clear();
                        textEdit->clear();
                        noteTitleEdit->clear();
                    }
                }
            }

            noteList->clear();
            loadNoteList();
            dialog.accept();
        }
    );
        
    dialog.exec();
}

void NotesWindow:: goHome(){
    saveNote();
    close();
}