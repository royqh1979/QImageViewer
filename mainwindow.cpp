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
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mInFullScreen = false;
    mPageInfo = new QLabel(this);
    mPageInfo->setText(" ");
    ui->statusbar->addPermanentWidget(mPageInfo);
    mZoomFactor = new QSpinBox(this);
    mZoomFactor->setSuffix("%");
    mZoomFactor->setRange(1,1000);
    connect(mZoomFactor, qOverload<int>(&QSpinBox::valueChanged),
            this, &MainWindow::onZoomFactorChanged);
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

    setWindowIcon(QPixmap(":/icons/imageviewer.png"));
    setAcceptDrops(true);
    updateStatusBar();
    applySettings();

    addActions(menuBar()->actions());
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
    if (mDirModel->imageCount()>0) {
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
    }
    updateActions();
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

void MainWindow::onZoomFactorChanged(int newVal)
{
    mImageWidget->setRatio(newVal/100.0);
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
    int thumbnailSize = 200;
    mDirModel->setThumbnailSize(thumbnailSize);
    mThumbnailDelegate->setThumbnailSize(thumbnailSize);
    ui->dirView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->dirView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->dirView->setMinimumWidth(mDirModel->thumbnailSize()+ui->dirView->verticalScrollBar()->sizeHint().width()+20);
    ui->dirView->setMaximumWidth(mDirModel->thumbnailSize()+ui->dirView->verticalScrollBar()->sizeHint().width()+20);
    ui->dockDir->setWidget(ui->dirView);
    ui->dirView->doItemsLayout();
}

void MainWindow::updateActions()
{
    ui->menuAnimation->menuAction()->setVisible(mImageWidget->isAnimation());
    ui->menuAnimation->menuAction()->setEnabled(mImageWidget->isAnimation());
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->urls().count()==1) {
        QList<QUrl> urlList = mimeData->urls();
        QString fileName = urlList.first().toLocalFile();
        if (QFileInfo::exists(fileName))
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && event->modifiers() == Qt::NoModifier
            && mInFullScreen) {
        on_actionFull_Screen_triggered();
    }
}


void MainWindow::on_actionRotate_Left_triggered()
{
    mImageWidget->rotate(-90);
}


void MainWindow::on_actionRotate_Right_triggered()
{
    mImageWidget->rotate(90);
}


void MainWindow::on_actionFlip_Horizontal_triggered()
{
    mImageWidget->horizontalFlip();
}


void MainWindow::on_actionFlip_Vertical_triggered()
{
    mImageWidget->verticalFlip();
}


void MainWindow::on_actionStop_Animation_triggered()
{
    mImageWidget->pause();
}


void MainWindow::on_actionPlay_Animation_triggered()
{
    mImageWidget->play();
}


void MainWindow::on_actionNext_Frame_triggered()
{
    mImageWidget->nextFrame();
}


void MainWindow::on_actionPrev_Frame_triggered()
{
    mImageWidget->prevFrame();
}


void MainWindow::on_actionFull_Screen_triggered()
{
    if (mInFullScreen) {
        ui->dockDir->blockSignals(true);
        if (ui->dockDir->isVisible()!=ui->actionShow_Contents->isChecked())
            ui->dockDir->setVisible(ui->actionShow_Contents->isChecked());
        ui->dockDir->blockSignals(false);
        //ui->toolBar->setVisible(true);
        ui->menubar->setVisible(true);
        ui->statusbar->setVisible(true);
        if (mMaximizedBeforeFullScreen)
            showMaximized();
        else
            showNormal();
        mInFullScreen = false;
    } else {
        mMaximizedBeforeFullScreen = isMaximized();
        ui->dockDir->blockSignals(true);
        ui->dockDir->setVisible(false);
        ui->dockDir->blockSignals(false);
        //ui->toolBar->setVisible(false);
        ui->menubar->setVisible(false);
        ui->statusbar->setVisible(false);
        showFullScreen();
        mInFullScreen = true;
    }
}


void MainWindow::on_actionShow_Contents_toggled(bool arg1)
{
    Q_UNUSED(arg1);
    if (!mInFullScreen)
        ui->dockDir->setVisible(ui->actionShow_Contents->isChecked());
}


void MainWindow::on_dockDir_visibilityChanged(bool visible)
{
    ui->actionShow_Contents->blockSignals(true);
    ui->actionShow_Contents->setChecked(visible);
    ui->actionShow_Contents->blockSignals(false);
}

