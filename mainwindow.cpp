#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exit()
{
    close();
    qApp->quit();
}

void MainWindow::on_patchButton_clicked()
{
    //Display Warning Message
    int answer = QMessageBox::question(this, "Warning", "This will patch the kernel configuration file.\nAre you sure you want to procede ?", QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
    {

    }
    else
    {
        return;
    }
}

void MainWindow::on_restoreButton_clicked()
{
    //Display Warning Message
    int answer = QMessageBox::question(this, "Warning", "This will restore the old configuration.\nAre you sure you want to procede ?", QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
    {

    }
    else
    {
        return;
    }
}

bool MainWindow::init()
{
    bool isInitOk = true;

    //Search for compatibility
    if(isCompatibleVersion(getMBPModelVersion()))
    {
        isInitOk = true;
    }
    else
    {
        QMessageBox::information(this,"Mac not compatible","Sorry, your Mac is not compatible.\nThe application will close");
        isInitOk = false;
    }

    return isInitOk;
}

QString MainWindow::getMBPModelVersion()
{
    QString MBPModelVersion;
    QProcess process;

    //Execute commande line
    process.start("sysctl -n hw.model");

    //Wait forever until finished
    process.waitForFinished(-1);

    //Get command line output
    MBPModelVersion = process.readAllStandardOutput();

    //Remove carriage return ("\n") from string
    MBPModelVersion = MBPModelVersion.simplified();

    return MBPModelVersion;
}

int MainWindow::searchKernelExtensionFile(QFile *kernelFile)
{
    //Parse system directory searching for AppleGraphicsPowerManagement.kext file
    //See here : https://developer.apple.com/library/content/documentation/Darwin/Conceptual/KEXTConcept/Articles/command_line_tools.html
    //"Locate Kext"
    int FileFound;

    return FileFound;
}

bool MainWindow::isCompatibleVersion(QString modelVersion)
{
    //Compare version with compatible versions of MBPs
    bool isCompatibleVersion;
    QString MBPModelVersion;

    MBPModelVersion = getMBPModelVersion();

    //TODO : Search in a list
    if(MBPModelVersion == "MacBookPro6,2")
    {
        std::cout<<"Succes"<<std::endl;
        isCompatibleVersion = true;
    }
    else
    {
        std::cout<<"Fail"<<std::endl;
        isCompatibleVersion = false;
    }

    return isCompatibleVersion;
}

void MainWindow::backupKernelExtension()
{
    //Save File to current location adding .bak extension
}

void MainWindow::patchKernelExtensionFile(QFile *kernelFile)
{
    //Modify Kernel Extension File to add fix explained here :
    //https://forums.macrumors.com/threads/gpu-kernel-panic-in-mid-2010-whats-the-best-fix.1890097/

}

int MainWindow::loadKernelExtension(QFile *kernelFile)
{
    //Use Kext Utility or command lines utils to load the file in Kernel
    //kextload

    //See here : http://osxdaily.com/2015/06/24/load-unload-kernel-extensions-mac-os-x/
    int Status;

    return Status;
}

int MainWindow::restoreOldKernelExtension(QFile *kernelFile)
{
    //Restore.bak extension
    int Status;

    return Status;
}


