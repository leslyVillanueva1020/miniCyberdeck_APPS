#pragma once

#include <QTextEdit>
#include <QPaintEvent>

class NotebookTextEdit : public QTextEdit
{
public:
    explicit NotebookTextEdit(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int notebookLineSpacing = 20;
};