#ifndef DEPLOY_OP_H
#define DEPLOY_OP_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class deploy_op;
}

class deploy_op : public QDialog
{
    Q_OBJECT

public:
    virtual void timerEvent(QTimerEvent *event);
    explicit deploy_op(const QString &commit, QWidget *parent = 0);
    ~deploy_op();
    void update_color_key_color(QTextEdit *item, const QString &key, const QBrush &brush);
private slots:
    void on_tag_currentTextChanged(const QString &arg1);

    void on_single_clicked();

    void on_whole_clicked();

    void on_service_currentTextChanged(const QString &arg1);

    void on_xml_currentTextChanged(const QString &arg1);

    void on_deploy_clicked();

private:
    QString md5_log();
    void update_logs();
    void start_update_data();
    void alert(const QString &str);
    QString deply_xml_path();
    void add_tags();
    void update_tag_by_commit();
    void update_service_name();
    void update_commit_by_tag();
    void checkout_tag();
    void update_view_after_radio_select();
    void update_whole_xml();
    void update_single_xml();
    void update_xml_content();
    void update_xml_color();
    void update_log_color();
private:
    Ui::deploy_op *ui;
    int same_cnt;
    QString last_md5;
    QString basedir;
    int id;
};

#endif // DEPLOY_OP_H
