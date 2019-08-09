#include "switch_op.h"
#include "ui_switch_op.h"

#include <qmessagebox.h>

#include <lt_data/json_obj.h>
#include <lt_function/utils.h>

switch_op::switch_op(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::switch_op)
{
   ///mnt/share/3.project/nginx_conf/nginx_switch_pro.sh
    ui->setupUi(this);
    ui->link->setOpenExternalLinks(true);
    ui->link->setText("<a href=\"https://ccs-product.capitalonline.net/#/account_dash/\">test switch result");

}

void switch_op::message(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("Message",str_q,
                                       QMessageBox::Information,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}


void switch_op::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}

switch_op::~switch_op()
{
    delete ui;
}

void switch_op::on_maintenace_clicked()
{
    std::string output;
    QString cmd = "mnt/share/3.project/nginx_conf/nginx_switch_pre.sh";
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("switch to maintenace failed :" + QString(output.c_str()));
    }
    else
    {
        message("switch to maintenace ok");
    }

}

void switch_op::on_running_clicked()
{
    std::string output;
    QString cmd = "mnt/share/3.project/nginx_conf/nginx_switch_pro.sh";
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("switch to running failed :" + QString(output.c_str()));
    }
    else
    {
        message("switch to running ok");
    }
}
