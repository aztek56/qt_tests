#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas/glwidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setMinimumSize(1024,1024);
    glWidget = new GLWidget;

    ui->mainLayout->addWidget(glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    qDebug() << "Pressed menu window";
}
