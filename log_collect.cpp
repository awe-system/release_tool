#include "log_collect.h"
#include "ui_log_collect.h"

log_collect::log_collect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::log_collect)
{
    ui->setupUi(this);
}

log_collect::~log_collect()
{
    delete ui;
}
