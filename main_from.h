#ifndef MAIN_FROM_H
#define MAIN_FROM_H

#include <QMainWindow>

namespace Ui {
class main_from;
}

class main_form : public QMainWindow
{
    Q_OBJECT

private slots:
    void set_project();
    void release();
    void deploy();

    void on_commits_currentTextChanged(const QString &arg1);

    void on_branch_box_currentTextChanged(const QString &arg1);


private:
    void alert(const QString &str);
private:
    void try_load_project_dir();
    void checkout_branch();
    void checkout_commit();
    void update_branch_list();
    void load_branch();
    void update_commits();
    void show_log();
    void show_version();
    void show_mainversion();
    void show_tag();
public:
    explicit main_form(QWidget *parent = 0);
    ~main_form();

private:
    Ui::main_from *ui;
};

#endif // MAIN_FROM_H
