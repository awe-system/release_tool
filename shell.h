#ifndef SHELL_H
#define SHELL_H

#include <QDialog>
#include <lt_data/json_obj.h>
#include <lt_function/utils.h>
namespace Ui {
class shell;
}

class shell : public QDialog
{
    Q_OBJECT

public:
    explicit shell(QWidget *parent = 0);
    ~shell();
private slots:

    void on_addip_clicked();

    void on_add_clicked();

    void on_clear_clicked();

    void on_exe_clicked();

    void on_remove_clicked();

private:
    void exec_on_ip(const QString&ip);
    void gen_shell();
    QString deply_xml_path();
    void add_ip_tolist(const QString& ip);
    void alert(const QString &str);
    void update_whole_xml();
private:
    Ui::shell *ui;
};

#endif // SHELL_H
