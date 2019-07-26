#ifndef RELEASE_OP_H
#define RELEASE_OP_H

#include <QDialog>
#include <lt_function/thread_pool.hpp>

namespace Ui {
class release_op;
}

class release_op : public QDialog
{
    Q_OBJECT

public:
    virtual void timerEvent( QTimerEvent *event);
    release_op(const QString &commit, const QString &main_version,QWidget *parent = 0);
    ~release_op();

private:
    void alert(const QString &str);

private slots:
    void on_release_clicked();
    void update_logs();
    QString md5_log();
private:
    void start_update_data();
private:
    QString last_md5;
    data_channel::thread_pool pool;
    Ui::release_op *ui;
    QString basedir;
    int id;
    int same_cnt;
};

#endif // RELEASE_OP_H
