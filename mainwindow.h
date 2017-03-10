#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QProcess>
#include <iostream>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QStack>
#include <QFileDialog>

using namespace std; // Indique quel espace de noms on va utiliser

//GoogleC++ Style Guide : https://google.github.io/styleguide/cppguide.html

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool init();

private slots:
    void on_patchButton_clicked();

    void on_restoreButton_clicked();

    void exit();
private:
    Ui::MainWindow *ui;

    QFile kernelFile;

    QString getMBPModelVersion(void);
    bool isCompatibleVersion(QString modelVersion);
    bool MainWindow::searchKernelExtensionFile(QFile* kernelExtensionFile);
    void backupKernelExtension(void);
    void patchKernelExtensionFile(QFile* kernelFile);
    int loadKernelExtension(QFile* kernelFile);
    int restoreOldKernelExtension(QFile* kernelFile);
};

#endif // MAINWINDOW_H
