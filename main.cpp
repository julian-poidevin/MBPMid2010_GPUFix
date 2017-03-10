#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow ui;

    if(ui.init())
    {
        ui.show();
        return app.exec();
    }
    else
    {
        return 0;
    }
}
