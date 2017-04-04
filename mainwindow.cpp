#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TEST
//#define REAL

//#define WINDOWS
#define MAC

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
        //TODO : Uncomment
        //int answer = QMessageBox::question(this, "Warning", "This will patch the kernel configuration file.\nAre you sure you want to procede ?", QMessageBox::Yes | QMessageBox::No);

        //if (answer == QMessageBox::Yes)
        if(1)
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
#ifdef MAC
    QDir kextPath("/Users/Julian/Documents/Dev/Projects/MBPMid2010_GPUFix/");
#endif
#ifdef WINDOWS
    QDir kextPath("C:/Users/jpoidevin/Desktop/Documents Pro/03 - Dev Temp/MBPMid2010_GPUFix/MBPMid2010_GPUFix/");
#endif
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

#ifdef MAC

    //TODO : Search in a list
    if(modelVersion == "MacBookPro6,2")
    {
        isCompatibleVersion = true;
    }
    else
    {
        isCompatibleVersion = false;
    }

#endif
#ifdef WINDOWS
    isCompatibleVersion = true;
#endif

    return isCompatibleVersion;
}

void MainWindow::backupOldKernelExtension()
{
    //Save File to current location adding .bak extension
    //qDebug() << "File Name" << kernelFile.fileName();

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

#ifdef MAC
#define PATCHED_FILE_PATH "/tmp/PatchedInfo.plist"
#endif
#ifdef WINDOWS
#define PATCHED_FILE_PATH "C:/temp/PatchedInfo.plist"
#endif


    //Remove file if already exists
    if (QFile::exists(PATCHED_FILE_PATH))
    {
        QFile::remove(PATCHED_FILE_PATH);
    }

    //Copy file in tmp dir for patch
    QFile::copy(kernelFile->fileName(), PATCHED_FILE_PATH);

    QFile tmpFile(PATCHED_FILE_PATH);

    if(!tmpFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Could not open tmp File";
        return;
    }

    /* Definition of struct and enum to automaticaly parse file*/
    typedef enum
    {
        FindChild,
        FindSibling,
        NextSibling,
        FirstChild,
        FillArray
    }EActions;

    typedef struct{
        QString nodeName;
        QVector<int> ArrayValues;
        EActions ActionToPerform;
    }nodeTree;

    QVector<nodeTree> confTree={
        {"MacBookPro6,2"        , {}            , FindChild    },
        {"dict"                 , {}            , NextSibling  },
        {"Vendor10deDevice0a29" , {}            , FindChild    },
        {"BoostPState"          , {}            , FindSibling  },
        {"array"                , {}            , NextSibling  },
        {"integer"              , {}            , FirstChild   },
        {""                     , {2,2,2,2}     , FillArray    }
    };


    //The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;

    // Set data into the QDomDocument before processing
    xmlBOM.setContent(&tmpFile);

    // Extract the root markup
    QDomElement root = xmlBOM.firstChildElement("plist");

    //Parse tree
    QDomElement MacBookPro62 = findElementChild(root,"MacBookPro6,2").nextSiblingElement("dict");
    //qDebug() << MacBookPro62.tagName() << MacBookPro62.text() << MacBookPro62.nodeName();

    QDomElement GPUDeviceID = findElementChild(MacBookPro62,"Vendor10deDevice0a29");
    qDebug() << "GPUDeviceID" << GPUDeviceID.tagName() << GPUDeviceID.text();

    QDomElement BoostPState = findElementSibling(GPUDeviceID,"BoostPState");
    qDebug() << "BoostPState" << BoostPState.tagName() << BoostPState.text();

    QDomElement BoostPStateArray = BoostPState.nextSiblingElement("array");
    qDebug() << "BoostPStateArray" << BoostPStateArray.tagName() << BoostPStateArray.text() << BoostPStateArray.nodeName();

    QDomElement BoostPStateArrayInteger = BoostPStateArray.firstChildElement("integer");
    qDebug() << "BoostPStateArrayInteger" << BoostPStateArrayInteger.tagName() << BoostPStateArrayInteger.text() << BoostPStateArrayInteger.nodeName();

    //Finally managing to change node value
    //TODO : Create function with loop
    BoostPStateArrayInteger.firstChild().setNodeValue("test1");
    BoostPStateArrayInteger.nextSibling().firstChild().setNodeValue("test2");
    BoostPStateArrayInteger.nextSibling().nextSibling().firstChild().setNodeValue("test3");
    BoostPStateArrayInteger.nextSibling().nextSibling().nextSibling().firstChild().setNodeValue("test4");

    QDomElement Heuristic = findElementSibling(GPUDeviceID,"Heuristic");
    qDebug() << "Heuristic" << Heuristic.tagName() << Heuristic.text();






    // Write changes to same file
    tmpFile.resize(0);
    QTextStream stream;
    stream.setDevice(&tmpFile);
    xmlBOM.save(stream, 4);

    tmpFile.close();
}

int MainWindow::loadKernelExtension(QFile *kernelFile)
{
    //Use Kext Utility or command lines utils to load the file in Kernel
    //kextload

    //Disable srcutils: https://derflounder.wordpress.com/2015/10/05/configuring-system-integrity-protection-without-booting-to-recovery-hd/

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

void MainWindow::searchForItem(QXmlStreamReader* xmlReader, QString tagType, QString tagName)
{
    //https://openclassrooms.com/forum/sujet/qtxml-atteindre-un-node-donnee-pour-en-prendre-son-texte-22633

    while (QXmlStreamReader::EndDocument != xmlReader->readNext())
    {
        if (xmlReader->name() == tagType)
        {
            if(xmlReader->readElementText() == tagName)
            {
                qDebug() << "Text found : " + tagName;
                break;
                //return;
            }
        }
    }
}

QDomElement MainWindow::findElementChild(QDomElement parent, const QString &textToFind)
{
    for(QDomElement elem = parent.firstChildElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        if(elem.text()==textToFind) return elem;

        QDomElement e = findElementChild(elem, textToFind);

        if(!e.isNull()) return e;
    }

    return QDomElement();
}

QDomElement MainWindow::findElementSibling(QDomElement parent, const QString &textToFind)
{
    for(QDomElement elem = parent.nextSiblingElement(); !elem.isNull(); elem = elem.nextSiblingElement())
    {
        if(elem.text()==textToFind) return elem;

        QDomElement e = findElementChild(elem, textToFind);

        if(!e.isNull()) return e;
    }

    return QDomElement();
}
