#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <updater.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow ui;

    app.setApplicationVersion (VERSION);
    app.setApplicationName (APP_NAME);

    //create the updater with the application as parent -> will live long enough start the tool on exit
    QtAutoUpdater::Updater *updater = new QtAutoUpdater::Updater("/Applications/MBPMid2010_GPU_Fix/MaintenanceTool", &app);//.exe is automatically added

    QObject::connect(updater, &QtAutoUpdater::Updater::checkUpdatesDone, [updater](bool hasUpdate, bool hasError) {
        qDebug() << "Has updates:" << hasUpdate << "\nHas errors:" << hasError;
        if(hasUpdate) {
            //As soon as the application quits, the maintenancetool will be started in update mode
            updater->runUpdaterOnExit();
            qDebug() << "Update info:" << updater->updateInfo();
        }
        //Quit the application
        qApp->quit();
    });

    //start the update check
    updater->checkForUpdates();
    return app.exec();

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
