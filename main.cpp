#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow ui;

    app.setApplicationVersion (VERSION);
    app.setApplicationName (APP_NAME);

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
