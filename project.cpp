#include "project.h"
#include "ui_project.h"

#include <QFileDialog>
#include <QMessageBox>

#include <lt_data/json_obj.h>
#include <lt_function/utils.h>

extern QString project_dir;


void project::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}


void project::message(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("Message",str_q,
                                       QMessageBox::Information,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}


project::project(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::project)
{
    ui->setupUi(this);
    ui->project_dir->setText(project_dir);
}

project::~project()
{
    delete ui;
}

void project::on_view_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), project_dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->project_dir->setText(dir);
    project_dir = dir;
}

void project::on_quit_clicked()
{
    close();
}

void project::on_save_clicked()
{
    auto basedir = QDir::currentPath();
    auto cmd = "echo " + project_dir + " > " + basedir +"/../release_tool/project_dir";
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("save project dir failed :" + QString(output.c_str()));
    }
}

void project::on_pull_clicked()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/git_pull.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("project pull failed :" + QString(output.c_str()));
    }
    else
    {
        message("pull succeed : " + QString(output.c_str()));
    }
}
