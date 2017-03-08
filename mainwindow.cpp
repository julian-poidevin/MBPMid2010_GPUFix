#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(ui->patchButton);
    layout->addWidget(ui->restoreButton);

    MainWindow::setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_patchButton_clicked()
{
    //Display Warning Message
    int answer = QMessageBox::question(this, "Warning", "Are you sure you want to procede ?", QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::Yes)
    {

    }

}

void MainWindow::on_restoreButton_clicked()
{

}

QString MainWindow::getMBPModelVersion()
{
    /* Execute Following Command Line and return result */
    //sysctl -n hw.model
    //or
    //ioreg -l | awk '/product-name/ { split($0, line, "\""); printf("%s\n", line[4]); }'
}

int MainWindow::searchKernelExtensionFile(QFile *kernelFile)
{
    //Parse system directory searching for AppleGraphicsPowerManagement.kext file
    //See here : https://developer.apple.com/library/content/documentation/Darwin/Conceptual/KEXTConcept/Articles/command_line_tools.html
    //"Locate Kext"
}

bool MainWindow::isCompatibleVersion(QString modelVersion)
{
    //Compare version with compatible versions of MBPs

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

}

int MainWindow::restoreOldKernelExtension(QFile *kernelFile)
{
    //Restore.bak extension
}


