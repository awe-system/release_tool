#include "exec_cmd.h"
#include "ui_exec_cmd.h"


QString exec_cmd::cmd_by_stage()
{
    json_obj cmd_obj = after_cmds.array_val[stage -1];
    if(cmd_obj.type() != json_obj_type_string)
    {
        return "";
    }
    return QString(cmd_obj.s_val.c_str());
}

void exec_cmd::update_log_by_stage()
{
    if(after_cmds.type() != json_obj_type_array || stage > after_cmds.array_val.size())
    {

        res_msg +="\nall cmd OK!!\n";
        update_logs();
        return;
    }
    QString cmd = cmd_by_stage();
    if(cmd == "")
    {
        res_msg +="\n-----------------------------------------------ERROR cmd!!!!!!!!!!\n";
        update_logs();
        return;
    }

    std::string output;
    auto err = buf_exec_cpp(cmd.toStdString(),output);
    res_msg += QString(output.c_str());
    if(err)
    {
        res_msg +="\n"+cmd+"\n------------------------------------------------------------------ERROR!\n";
    }
    else
    {
        res_msg += "\n"+cmd+"\n--------------------------------------------------------------------OK!\n";
    }
    ++stage;
    id = startTimer(10);
    update_logs();
}

void exec_cmd::update_logs()
{
    ui->logs->setText(res_msg);
    QTextCursor cursor = ui->logs->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->logs->setTextCursor(cursor);
}

void exec_cmd::timerEvent(QTimerEvent *event)
{
    killTimer(id);
    if(stage ==0 )
    {
        std::string output;
        auto err = buf_exec_cpp(cmd.toStdString(),output);
        res_msg = QString(output.c_str());
        if(err)
        {
            res_msg +="\n"+cmd+"\n---------------------------------------------------ERROR!\n";
        }
        else
        {
            res_msg += "\n"+cmd+"\n---------------------------------------------------OK!\n";
        }
        if(!after_cmds.is_empty())
        {
            ++stage;
            id = startTimer(10);
        }
        else
        {
            res_msg +="\nall cmd OK!!\n";
        }
        update_logs();
    }
    else
    {
        update_log_by_stage();
    }
}

exec_cmd::exec_cmd(const QString& title, const QString _cmd, json_obj _after_cmds, QWidget *parent):
    cmd(_cmd),after_cmds(_after_cmds),
    QDialog(parent),
    ui(new Ui::exec_cmd)
{
    ui->setupUi(this);
    setWindowTitle(title);
    id = startTimer(1000);
    ui->logs->setText("processing ... ...");
}

exec_cmd::~exec_cmd()
{
    delete ui;
}
