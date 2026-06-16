#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imagewidget.h"
#include "dirmodel.h"
#include "imagemetainfo.h"
#include "imagemetainfomodel.h"
#include "thumbnailview.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>
#include <QMimeData>
#include <QDragEnterEvent>

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


    mDirModel = new DirModel(this);
    connect(mDirModel, &DirModel::currentFileChanged,
            this, &MainWindow::onCurrentFileChanged);

    mThumbnailDelegate = new ThumbnailDelegate(this);
    ui->dirView->setItemDelegate(mThumbnailDelegate);
    ui->dirView->setModel(mDirModel);
    connect(ui->dirView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onDirViewCurrentChanged);

    mImageMetaInfoModel = new ImageMetaInfoModel(this);
    ui->imageMetaInfoView->setModel(mImageMetaInfoModel);
    ui->imageMetaInfoView->setHeaderHidden(true);
    ui->dockMetaInfo->setVisible(false);

    setAcceptDrops(true);
    updateStatusBar();
    applySettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFolder(const QString &path)
{
    mDirModel->open(path);
}

void MainWindow::onCurrentFileChanged(int oldFileId, int currentFileId)
{
    if (mDirModel->imageCount()<=0)
        return;
    QModelIndex index = mDirModel->index(currentFileId,0);

    ui->dirView->selectionModel()->select(index,QItemSelectionModel::SelectionFlag::Select);
    ui->dirView->setCurrentIndex(index);
    mImageWidget->setImage(mDirModel->imagePath(mDirModel->currentFileIdx()));
    ImageMetaInfo *info=new ImageMetaInfo(mDirModel->imagePath(mDirModel->currentFileIdx()));
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
    mDirModel->toPrevious();
    if (scrollToBottom) {
        mImageWidget->scrollToBottom();
    }
}

void MainWindow::onRequestNextImage()
{
    mDirModel->toNext();
}

void MainWindow::onDirViewCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid())
        return;
    mDirModel->setCurrentFileIdx(current.row());
}


void MainWindow::on_actionNext_triggered()
{
    mDirModel->toNext();
}


void MainWindow::on_actionPrevious_triggered()
{
    mDirModel->toPrevious();
}


void MainWindow::on_actionFirst_triggered()
{
    mDirModel->toFirst();
}


void MainWindow::on_actionLast_triggered()
{
    mDirModel->toLast();
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
    if (mDirModel->imageCount()<=0 || mDirModel->currentFileIdx()==-1) {
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
        mPageInfo->setText(QString("%1/%2").arg(mDirModel->currentFileIdx()+1)
                               .arg(mDirModel->imageCount()));
        ui->statusbar->showMessage(mDirModel->imageFileName(mDirModel->currentFileIdx()));
    }
}

void MainWindow::applySettings()
{
    mDirModel->setThumbnailSize(350);
    mThumbnailDelegate->setThumbnailSize(350);
    ui->dirView->doItemsLayout();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    qDebug()<<mimeData->urls().count();
    if (mimeData->urls().count()==1) {
        QList<QUrl> urlList = mimeData->urls();
        QString fileName = urlList.first().toLocalFile();
        qDebug()<<fileName;
        if (QFileInfo{fileName}.isDir())
            event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->urls().count()==1) {
        QList<QUrl> urlList = mimeData->urls();
        QString fileName = urlList.first().toLocalFile();
        openFolder(fileName);
        event->acceptProposedAction();
    }
}

