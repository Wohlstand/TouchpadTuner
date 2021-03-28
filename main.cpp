#include "touchpad_tuner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TouchpadTuner w;
    w.show();
    return a.exec();
}
