#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>

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

private slots:
    void on_patchButton_clicked();

    void on_restoreButton_clicked();

private:
    Ui::MainWindow *ui;

    QString getMBPModelVersion(void);
    int searchKernelExtensionFile(QFile* kernelFile);
    bool isCompatibleVersion(QString modelVersion);
    void backupKernelExtension(void);
    void patchKernelExtensionFile(QFile* kernelFile);
    int loadKernelExtension(QFile* kernelFile);
    int restoreOldKernelExtension(QFile* kernelFile);
};

#endif // MAINWINDOW_H
