#ifndef KLAUSPRO_H
#define KLAUSPRO_H

#include <QDialog>

namespace Ui {
class kLausPro;
}

class kLausPro : public QDialog
{
    Q_OBJECT

public:
    explicit kLausPro(QWidget *parent = nullptr);
    ~kLausPro();

private slots:
    void on_pushButton_clicked();

private:
    Ui::kLausPro *ui;
};

#endif // KLAUSPRO_H
