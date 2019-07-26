#ifndef PROJECT_H
#define PROJECT_H

#include <QDialog>

namespace Ui {
class project;
}

class project : public QDialog
{
    Q_OBJECT

public:
    explicit project(QWidget *parent = 0);
    ~project();

private slots:
    void on_view_clicked();

    void on_quit_clicked();

    void on_save_clicked();
    void on_pull_clicked();

private:
    void alert(const QString &str);
    void message(const QString &str);
private:
    Ui::project *ui;
};

#endif // PROJECT_H
