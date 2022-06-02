#include "pi_net.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pi_net w;
    w.show();

    return a.exec();
}
