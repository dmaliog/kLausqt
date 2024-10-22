#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QMouseEvent>

class CustomTextEdit : public QTextEdit {
    Q_OBJECT

public:
    explicit CustomTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {}

signals:
    void wordHovered(const QString &word);

protected:
    void mouseMoveEvent(QMouseEvent *event) override {
        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString word = cursor.selectedText().trimmed();

        emit wordHovered(word);
        QTextEdit::mouseMoveEvent(event);
    }
};

#endif // CUSTOMTEXTEDIT_H
