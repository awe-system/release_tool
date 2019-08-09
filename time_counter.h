#ifndef TIME_COUNTER_H
#define TIME_COUNTER_H

#include <QDialog>

namespace Ui {
class time_counter;
}

class time_counter : public QDialog
{
    Q_OBJECT

public:
    virtual void timerEvent( QTimerEvent *event);
    explicit time_counter(QWidget *parent = 0);
    ~time_counter();

public:
    bool is_cancel;
private slots:
    void on_start_clicked();

    void on_time_counter_accepted();

    void on_dial_valueChanged(int value);

private:
    Ui::time_counter *ui;
    int id;
};

#endif // TIME_COUNTER_H
