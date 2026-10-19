#include "NotebookTextEdit.h"

#include <QPainter>
#include <QPen>
#include <QColor>
#include <QScrollBar>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QAbstractTextDocumentLayout>
#include <QPalette>

NotebookTextEdit::NotebookTextEdit(QWidget* parent)
    : QTextEdit(parent)
{
    // Set the notebook's paper color
    // setStyleSheet(
    //     "background-color: #FFF8E9;"
    //     "color: #513457;"
    //     "border: 1px solid #D8BFCB;"
    //     "border-radius: 10px;"
    //     "padding: 12px;"
    // );

    // Give the text a consistent line height
    QTextCursor cursor(document());

    QTextBlockFormat format;
    format.setLineHeight(28, QTextBlockFormat::FixedHeight);

    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);

    // Keep the formatting for newly typed paragraphs
    setTextCursor(cursor);

    // Repaint when scrolling
    connect(
        verticalScrollBar(),
        &QScrollBar::valueChanged,
        this,
        [this](int) {
            viewport()->update();
        }
    );
}

void NotebookTextEdit::paintEvent(QPaintEvent* event)
{
    QPainter painter(viewport());

    painter.setPen(QPen(QColor("#E8CED7"), 1));

    int scrollOffset = verticalScrollBar()->value();

    int height = viewport()->height();
    int width = viewport()->width();

    int firstLine = -scrollOffset % notebookLineSpacing;

    if (firstLine < 0) {
        firstLine += notebookLineSpacing;
    }

    for (int y = firstLine; y < height; y += notebookLineSpacing)
    {
        painter.drawLine(0, y, width, y);
    }

    painter.end();

    QTextEdit::paintEvent(event);
}