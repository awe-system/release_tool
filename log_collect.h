#ifndef LOG_COLLECT_H
#define LOG_COLLECT_H

#include <QDialog>

namespace Ui {
class log_collect;
}

class log_collect : public QDialog
{
    Q_OBJECT

public:
    explicit log_collect(QWidget *parent = 0);
    ~log_collect();

private:
    Ui::log_collect *ui;
};

#endif // LOG_COLLECT_H
