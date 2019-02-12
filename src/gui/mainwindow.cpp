#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gl/glwidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
