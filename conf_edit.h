#ifndef CONF_EDIT_H
#define CONF_EDIT_H

#include <QDialog>
#include <QTextEdit>

#include <lt_data/json_obj.h>
#include <lt_function/utils.h>
namespace Ui {
class conf_edit;
}

class conf_edit : public QDialog
{
    Q_OBJECT

public:
    virtual void timerEvent(QTimerEvent *event);
    void update_color_key_color(QTextEdit *item, const QString &key, const QBrush &brush);

    explicit conf_edit(QWidget *parent = 0);
    ~conf_edit();
private slots:
    void on_service_currentTextChanged(const QString &arg1);

    void on_xml_currentTextChanged(const QString &arg1);

    void on_dir_currentTextChanged(const QString &arg1);

    void on_file_currentTextChanged(const QString &arg1);

    void on_save_clicked();

private:
    void update_conf_color();
    void alert(const QString &str);
    void update_single_xml();
    void update_service_name();
    QString deply_xml_path();
    void update_node();
    void update_dir();
    void update_files();
    void update_file();
private:
    Ui::conf_edit *ui;
    int id;
};

#endif // CONF_EDIT_H
