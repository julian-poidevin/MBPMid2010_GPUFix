#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TEST
//#define REAL

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
    if(searchKernelExtensionFile(&kernelFile))
    {
        //Display Warning Message
        int answer = QMessageBox::question(this, "Warning", "This will patch the kernel configuration file.\nAre you sure you want to procede ?", QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
        {
            patchKernelExtensionFile(&kernelFile);
        }
        else
        {
            return;
        }
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

    MainWindow::setWindowTitle (APP_NAME);

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


//Parse system directory searching for AppleGraphicsPowerManagement.kext file
bool MainWindow::searchKernelExtensionFile(QFile* kernelExtensionFile)
{
    bool isFileFound;

#ifdef TEST
    QDir kextPath("/Users/Julian/Documents/Dev/Projects/MBPMid2010_GPUFix/");
#endif
#ifdef REAL
    QDir kextPath("/System/Library/Extensions/AppleGraphicsPowerManagement.kext/");
#endif

    QStringList listOfFiles;

    //Print Current app directory
    qDebug() << "Current Dir :" <<kextPath.absolutePath();

    //Recursively search for "Info.plist" file in appPath
    QDirIterator it(kextPath.absolutePath(),
                    QStringList() << "Info.plist",
                    QDir::NoSymLinks | QDir::Files,
                    QDirIterator::Subdirectories);

    //Check if the file was found
    if(it.hasNext())
    {
        while(it.hasNext())
        {
            it.next();
            listOfFiles.push_back(it.filePath());
        }
    }

    //Print files found
    qDebug() << "Files found :"<< listOfFiles;

    if(listOfFiles.length() <= 1 && listOfFiles.length() > 0)
    {
        //qDebug() << "Moins de 1";
        kernelExtensionFile->setFileName(listOfFiles.at(0));
        isFileFound = true;
    }
    else
    {
        //qDebug () << "No file was found...";
        isFileFound = false;
    }

    //Start search manually and only allow loading of the perfect named file (or kext)
    if(!isFileFound)
    {
        QMessageBox::information(this,"File not found","Any corresponding file was found, please search for the file");

        //TODO : FileDialog won't let user brown into .kext files Contents
        QString dir = QFileDialog::getOpenFileName(this, tr("Open Info.plist file"),
                                                   "/System/Library/Extensions/AppleGraphicsPowerManagement.kext/",
                                                   "Property List Files (Info.plist)");
        if(!(dir.isNull()))
        {
            //kernelExtensionFile->setFileName(dir);
            isFileFound = true;
        }
        else
        {
            isFileFound = false;
        }
    }

    return isFileFound;
}

bool MainWindow::isCompatibleVersion(QString modelVersion)
{
    //Compare version with compatible versions of MBPs
    bool isCompatibleVersion;

    //TODO : Search in a list
    if(modelVersion == "MacBookPro6,2")
    {
        isCompatibleVersion = true;
    }
    else
    {
        isCompatibleVersion = false;
    }

    return isCompatibleVersion;
}

void MainWindow::backupOldKernelExtension()
{
    //Save File to current location adding .bak extension
    qDebug() << "File Name" << kernelFile.fileName();

    //Save original file in kernelExtension file folder
    QFile::copy(kernelFile.fileName(), kernelFile.fileName() + ".bak");
}

void MainWindow::patchKernelExtensionFile(QFile *kernelFile)
{
    //Modify Kernel Extension File to add fix explained here :
    //https://forums.macrumors.com/threads/gpu-kernel-panic-in-mid-2010-whats-the-best-fix.1890097/
    //Use QSettings ? : http://doc.qt.io/qt-5/qsettings.html
    //https://openclassrooms.com/courses/enregistrer-vos-options-avec-qsettings
    //http://stackoverflow.com/questions/20240511/qsettings-mac-and-plist-files
    //https://forum.qt.io/topic/37247/qsettings-with-systemscope-not-saving-plist-file-in-os-x-mavericks/6

    backupOldKernelExtension();

    //Copy file in tmp dir for patch
    QFile::copy(kernelFile->fileName(), "/tmp/PatchedInfo.plist");

    //Creating QSettings object. "NativeFormat" is for accessing XML-based .plist files.
    QSettings settings("/tmp/PatchedInfo.plist", QSettings::NativeFormat);

    //Find a way to parse file to : IOKitPersonalities -> AGPM -> Machines -> MacBookPro6,2 -> Vendor10deDevice0a29

    //Check if kernelFile is Writable
    if(!settings.isWritable())
    {
        //TODO : handle not writable file
        return;
    }

    if(settings.contains("LogControl"))
    {
        qDebug() << "Contains Log Control";
    }
    else
    {
        qDebug() << "Does not contains Log Control";
    }
}

int MainWindow::loadKernelExtension(QFile *kernelFile)
{
    //Use Kext Utility or command lines utils to load the file in Kernel
    //kextload

    //See here : http://osxdaily.com/2015/06/24/load-unload-kernel-extensions-mac-os-x/
    int Status = 0;

    return Status;
}

int MainWindow::restoreOldKernelExtension(QFile *kernelFile)
{
    //Restore.bak extension
    int Status = 0;

    //QFile::copy(kernelFile->fileName()  + ".bak", kernelFile->fileName());

    return Status;
}
