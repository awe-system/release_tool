#ifndef LOG_COLLECT_H
#define LOG_COLLECT_H

#include <QDialog>
#include <QTextEdit>

#include <lt_data/json_obj.h>
#include <lt_function/utils.h>
namespace Ui {
class log_collect;
}

class log_collect : public QDialog
{
    Q_OBJECT

public:
    virtual void timerEvent(QTimerEvent *event);
    explicit log_collect(QWidget *parent = 0);
    ~log_collect();

private slots:
    void on_fixname_clicked(bool checked);
    void on_single_clicked();

    void on_whole_clicked();

    void on_start_clicked();

private:
    void update_log_color();
    void update_color_key_color(QTextEdit *item, const QString &key, const QBrush &brush);
    QString md5_log();
    void update_logs();
    QString deply_xml_path();
    void start_update_data();
    void update_single_xml();
    void update_whole_xml();
    void update_view_after_radio_select();
    void add_mainversions();
    void update_service_name();
    void alert(const QString &str);
private:
    Ui::log_collect *ui;
    int same_cnt;
    QString last_md5;
    QString basedir;
    int id;
    json_obj version_map;
};

#endif // LOG_COLLECT_H
