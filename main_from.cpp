#include "deploy_op.h"
#include "log_collect.h"
#include "main_from.h"
#include "project.h"
#include "release_op.h"
#include "ui_main_from.h"

#include <QDir>
#include <QMessageBox>
#include <lt_data/json_obj.h>
#include <lt_function/utils.h>


QString project_dir="";
static json_obj all_branches;
static json_obj all_commits;

void main_form::set_project()
{
    project * op = new project();
    op->exec();
    load_branch();
    setWindowTitle("Project:["+project_dir+"]");
}

void main_form::release()
{
    release_op * op = new release_op(ui->commits->currentText(),ui->mainversion->text());
    op->exec();
    checkout_branch();
}

void main_form::log()
{
    log_collect * op = new log_collect();
    op->exec();
    checkout_branch();
}

void main_form::deploy()
{
    deploy_op * op = new deploy_op(ui->commits->currentText());
    op->exec();
    checkout_branch();
}

void main_form::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}

void main_form::try_load_project_dir()
{
    auto basedir = QDir::currentPath();
    auto cmd = "cat " + basedir +"/../release_tool/project_dir";
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("load project dir failed :" + QString(output.c_str()));
    }
    else
    {
        project_dir = QString(output.c_str()).trimmed();
    }
    if(project_dir == "")
    {
        project_dir = "/";
    }
}

void main_form::checkout_branch()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/checkout.sh " + project_dir + " " + ui->branch_box->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("checkout_branch"+cmd+" :"+ QString(output.c_str()));
    }
    show_version();
}

void main_form::checkout_commit()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/checkout.sh " + project_dir + " " + ui->commits->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("checkout_commit "+ cmd +" : "+ QString(output.c_str()));
    }
    show_version();
}

void main_form::update_branch_list()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/all_branches.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("update_branch_list "+ cmd +" : "+ QString(output.c_str()));
    }
    all_branches.loads(output);
    ui->branch_box->clear();
    for(auto branch : all_branches.array_val)
    {
        ui->branch_box->addItem(QString(branch.s_val.c_str()));
    }
}

void main_form::load_branch()
{
    update_branch_list();
    update_commits();
    show_log();
    checkout_branch();
}

void main_form::update_commits()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/git_commits.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert("update_commits "+ cmd +" : "+ QString(output.c_str()));
    }
    all_commits.loads(output);
    ui->commits->clear();
    for(auto com : all_commits.array_val)
    {
        ui->commits->addItem(QString(com.s_val.c_str()));
    }
}

void main_form::show_log()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_log.sh " + project_dir + " " + ui->commits->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    ui->logs->setText(QString(output.c_str()));
}

void main_form::show_version()
{
    show_mainversion();
    show_tag();
}

void main_form::show_mainversion()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_mainversion.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    ui->mainversion->setText(QString(output.c_str()));
}

void main_form::show_tag()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/get_tag.sh " + project_dir + " " + ui->commits->currentText();
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    ui->tag->setText(QString(output.c_str()));
}

main_form::main_form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main_from)
{
    ui->setupUi(this);
    connect(ui->actionproject,SIGNAL(triggered()), this, SLOT(set_project()));
    connect(ui->actiondeploy,SIGNAL(triggered()), this, SLOT(deploy()));
    connect(ui->actionrelease,SIGNAL(triggered()), this, SLOT(release()));
    connect(ui->actionlog,SIGNAL(triggered()), this, SLOT(log()));
    try_load_project_dir();
    ui->logs->setReadOnly(1);
    load_branch();
    setWindowTitle("Project:["+project_dir+"]");
}

main_form::~main_form()
{
    delete ui;
}

void main_form::on_commits_currentTextChanged(const QString &arg1)
{
    checkout_commit();
    show_log();
    show_tag();
}

void main_form::on_branch_box_currentTextChanged(const QString &arg1)
{
    checkout_branch();
    update_commits();
    show_log();
    checkout_branch();
}

