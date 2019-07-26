#include "main_from.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    main_form w;
    w.show();

    return a.exec();
}
