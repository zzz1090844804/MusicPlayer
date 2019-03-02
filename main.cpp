#include "widget.h"
#include "mysql.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    if (!CreateConnection()) return 1;

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
