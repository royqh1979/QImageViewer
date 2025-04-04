#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imagewidget.h"
#include "filenavigator.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mImageWidget = new ImageWidget(ui->centralwidget);
    QHBoxLayout *layout = new QHBoxLayout(ui->centralwidget);
    layout->setMargin(0);
    layout->addWidget(mImageWidget);

    mFileNavigator = new FileNavigator(this);
    connect(mFileNavigator, &FileNavigator::currentFileChanged,
            this, &MainWindow::onCurrentFileChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFolder(const QString &path)
{
    mFileNavigator->open(path);
}

void MainWindow::onCurrentFileChanged(int oldFileId, int currentFileId)
{
    mImageWidget->setImage(QPixmap(mFileNavigator->currentFileInfo().absoluteFilePath()));
}


void MainWindow::on_actionNext_triggered()
{
    mFileNavigator->toNext();
}


void MainWindow::on_actionPrevious_triggered()
{
    mFileNavigator->toPrevious();
}


void MainWindow::on_actionFirst_triggered()
{
    mFileNavigator->toFirst();
}


void MainWindow::on_actionLast_triggered()
{
    mFileNavigator->toLast();
}


void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog;

}

