#include "time_counter.h"
#include "ui_time_counter.h"

void time_counter::timerEvent(QTimerEvent *event)
{
    QDateTime begin_time = QDateTime::currentDateTime();
    QDateTime end_time = ui->deploy_time->dateTime();
    auto secs = begin_time.secsTo(end_time);
    double sec_d = (double)secs;
    if(sec_d > 0)
    {
        ui->leftsecond->display(sec_d);
    }
    else
    {
        killTimer(id);
        is_cancel = false;
        close();
    }
}

time_counter::time_counter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::time_counter)
{
    is_cancel = true;
    ui->setupUi(this);
    QTime current_time = QTime::currentTime();
    QDate current_date = QDate::currentDate();
    ui->dial->setValue(current_time.hour());
    ui->deploy_time->setDate(current_date);
    ui->deploy_time->setTime(current_time);
}

time_counter::~time_counter()
{
    delete ui;
}

void time_counter::on_start_clicked()
{
    ui->leftsecond->setDecMode();
    QDateTime begin_time = QDateTime::currentDateTime();
    QDateTime end_time = ui->deploy_time->dateTime();
    auto secs = begin_time.secsTo(end_time);
    double sec_d = (double)secs;
    if(sec_d > 0)
    {
        ui->leftsecond->display(sec_d);
        id = startTimer(1000);
        ui->start->setEnabled(false);
    }
}


void time_counter::on_time_counter_accepted()
{

}

void time_counter::on_dial_valueChanged(int value)
{
    QTime time = ui->deploy_time->time();
    time.setHMS(value,0,0);
    ui->deploy_time->setTime(time);
}
