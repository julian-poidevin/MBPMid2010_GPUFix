#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow ui;

    ui.show();

    return app.exec();
}
