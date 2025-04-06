#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imagewidget.h"
#include "filenavigator.h"
#include "imagemetainfo.h"
#include "imagemetainfomodel.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPageInfo = new QLabel(this);
    mPageInfo->setText(" ");
    ui->statusbar->addPermanentWidget(mPageInfo);
    mZoomFactor = new QSpinBox(this);
    mZoomFactor->setSuffix("%");
    mZoomFactor->setRange(1,1000);
//    connect(mZoomFactor, qOverload<int>(&QSpinBox::valueChanged),
//            this, &MainWindow::onZoomFactorChanged);
    ui->statusbar->addPermanentWidget(mZoomFactor);
    mImageSizeInfo = new QLabel(this);
    mImageSizeInfo->setText(" ");
    ui->statusbar->addPermanentWidget(mImageSizeInfo);

    mImageWidget = new ImageWidget(ui->centralwidget);
    QHBoxLayout *layout = new QHBoxLayout(ui->centralwidget);
    layout->setMargin(0);
    layout->addWidget(mImageWidget);
    connect(mImageWidget, &ImageWidget::requestPrevImage,
            this, &MainWindow::onRequestPrevImage);
    connect(mImageWidget, &ImageWidget::requestNextImage,
            this, &MainWindow::onRequestNextImage);


    mFileNavigator = new FileNavigator(this);
    connect(mFileNavigator, &FileNavigator::currentFileChanged,
            this, &MainWindow::onCurrentFileChanged);


    mImageMetaInfoModel = new ImageMetaInfoModel(this);
    ui->imageMetaInfoView->setModel(mImageMetaInfoModel);
    ui->imageMetaInfoView->setHeaderHidden(true);
    updateStatusBar();
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
    ImageMetaInfo *info=new ImageMetaInfo(mFileNavigator->currentFileInfo().absoluteFilePath());
    mImageMetaInfoModel->setMetaInfo(info);
    ui->imageMetaInfoView->setFirstColumnSpanned(0,QModelIndex(),true);
    ui->imageMetaInfoView->setFirstColumnSpanned(1,QModelIndex(),true);
    ui->imageMetaInfoView->setFirstColumnSpanned(2,QModelIndex(),true);
    ui->imageMetaInfoView->expandAll();
    ui->imageMetaInfoView->resizeColumnToContents(0);
    updateStatusBar();
}

void MainWindow::onRequestPrevImage(bool scrollToBottom)
{
    mFileNavigator->toPrevious();
    if (scrollToBottom) {
        mImageWidget->scrollToBottom();
    }
}

void MainWindow::onRequestNextImage()
{
    mFileNavigator->toNext();
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
    QString path = QFileDialog::getExistingDirectory(this, "Choose folder");
    if (!path.isEmpty())
        openFolder(path);
}


void MainWindow::on_actionClose_triggered()
{
    openFolder("");
}

void MainWindow::updateStatusBar()
{
    if (mFileNavigator->imageCount()<=0 || mFileNavigator->currentFileIdx()==-1) {
        mZoomFactor->blockSignals(true);
        mZoomFactor->setValue(100);
        mZoomFactor->blockSignals(false);
        mZoomFactor->setEnabled(false);
        mImageSizeInfo->setText(" ");
        mPageInfo->setText(" ");
        ui->statusbar->clearMessage();
    } else {
        mZoomFactor->blockSignals(true);
        mZoomFactor->setValue(mImageWidget->ratio()*100);
        mZoomFactor->blockSignals(false);
        mZoomFactor->setEnabled(true);
        mImageSizeInfo->setText(QString(" %1x%2 ").arg(mImageWidget->imageSize().width()).arg(mImageWidget->imageSize().height()));
        mPageInfo->setText(QString("%1/%2").arg(mFileNavigator->currentFileIdx()+1)
                               .arg(mFileNavigator->imageCount()));
        ui->statusbar->showMessage(mFileNavigator->currentFileInfo().fileName());
    }
}

