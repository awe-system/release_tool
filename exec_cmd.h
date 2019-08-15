#ifndef EXEC_CMD_H
#define EXEC_CMD_H

#include <QDialog>
#include <lt_data/json_obj.h>
#include <lt_function/utils.h>

namespace Ui {
class exec_cmd;
}

class exec_cmd : public QDialog
{
    Q_OBJECT

public:
    virtual void timerEvent( QTimerEvent *event);
    explicit exec_cmd(const QString& title, const QString cmd,json_obj after_cmds = json_obj(),QWidget *parent = 0);
    ~exec_cmd();

private:
    QString cmd_by_stage();
    void update_log_by_stage();
    void update_logs();
private:
    Ui::exec_cmd *ui;
    int id;
    QString cmd;
    json_obj after_cmds;
    int stage = 0;
    QString res_msg = "";
};

#endif // EXEC_CMD_H
