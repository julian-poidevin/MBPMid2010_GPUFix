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
