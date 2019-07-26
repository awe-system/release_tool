#include "release_op.h"
#include "ui_release_op.h"

#include <QDir>
#include <QMessageBox>
#include <QThread>

#include <lt_data/json_obj.h>
#include <lt_function/utils.h>


extern QString project_dir;
static int cnt = 0;
void release_op::timerEvent(QTimerEvent *event)
{
    QString cur_md5 = md5_log();
    if(last_md5 == cur_md5)
    {
        same_cnt++;
        if(same_cnt == 120)
        {
            killTimer(id);
            ui->release->setEnabled(true);
        }
    }
    else
    {
        same_cnt = 0;
        update_logs();
        last_md5 = cur_md5;
    }
}

release_op::release_op(const QString &commit, const QString &main_version,QWidget *parent) :
    QDialog(parent),pool(1),same_cnt(0),
    ui(new Ui::release_op)
{
    ui->setupUi(this);
    ui->commit->setText(commit);
    ui->mainversion->setText(main_version);
    basedir = QDir::currentPath();
}

release_op::~release_op()
{
    delete ui;
}


void release_op::alert(const QString &str)
{
    QString str_q = str;
    QMessageBox *msg = new QMessageBox("warning",str_q,
                                       QMessageBox::Critical,QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape,0);
    msg->show();
}


void release_op::on_release_clicked()
{
    auto basedir = QDir::currentPath();
    auto cmd = basedir +"/../release_tool/release.sh " + project_dir;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
    }
    else
    {
        ui->tag->setText(QString(output.c_str()));
        start_update_data();
    }
}

void release_op::update_logs()
{
    auto cmd = basedir +"/../release_tool/updatelog.sh " + project_dir + " " + ui->tag->text() ;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        alert(QString(output.c_str()));
    }
    else
    {
        ui->logs->setText(QString(output.c_str()));
        QTextCursor cursor = ui->logs->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->logs->setTextCursor(cursor);
    }
}

QString release_op::md5_log()
{
    auto cmd = basedir +"/../release_tool/md5log.sh " + project_dir + " " + ui->tag->text() ;
    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    if(err)
    {
        return QString("");
    }
    else
    {
        return QString(output.c_str());
    }
}


void release_op::start_update_data()
{
    ui->release->setEnabled(false);
    id = startTimer(1000);
}
