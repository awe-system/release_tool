#ifndef SWITCH_OP_H
#define SWITCH_OP_H

#include <QDialog>

namespace Ui {
class switch_op;
}

class switch_op : public QDialog
{
    Q_OBJECT

public:
    explicit switch_op(QWidget *parent = 0);
    ~switch_op();
private:
    void message(const QString &str);
    void alert(const QString &str);
private slots:
    void on_maintenace_clicked();

    void on_running_clicked();

private:
    Ui::switch_op *ui;
};

#endif // SWITCH_OP_H
