#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewidget.h"
#include "dirmodel.h"
#include "imagemetainfo.h"
#include "imagemetainfomodel.h"
#include "thumbnailview.h"
#include "settings.h"
#include "settingsdialog/settingsdialog.h"

#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QScrollBar>
#include <QToolTip>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qApp->setStyle(QStyleFactory::create("fusion"));
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
    connect(mImageWidget, &QWidget::customContextMenuRequested,
            this , &MainWindow::onImageWidgetContextMenuRequested);
    mImageWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    mDirModel = new DirModel(this);
    connect(mDirModel, &DirModel::currentFileChanged,
            this, &MainWindow::onCurrentFileChanged);

    mThumbnailDelegate = new ThumbnailDelegate(this);
    ui->dirView->setItemDelegate(mThumbnailDelegate);
    ui->dirView->setModel(mDirModel);
    connect(ui->dirView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onDirViewCurrentChanged);

    connect(ui->dirView, &ResizeawareListView::resized,
            this, &MainWindow::onDirViewSizeChanged);

    mImageMetaInfoModel = new ImageMetaInfoModel(this);
    ui->imageMetaInfoView->setModel(mImageMetaInfoModel);
    ui->imageMetaInfoView->setHeaderHidden(true);
    ui->dockMetaInfo->setVisible(false);

    QActionGroup *fitActionGroup = new QActionGroup(this);
    fitActionGroup->addAction(ui->actionFit_Width);
    fitActionGroup->addAction(ui->actionFit_Height);
    fitActionGroup->addAction(ui->actionFit_Page);
    fitActionGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
    //updateImageFitType();

    connect(ui->actionFit_Width, &QAction::triggered,
            this, &MainWindow::updateImageFitType);
    connect(ui->actionFit_Height, &QAction::triggered,
            this, &MainWindow::updateImageFitType);
    connect(ui->actionFit_Page, &QAction::triggered,
            this, &MainWindow::updateImageFitType);

    resize(pSettings->ui().mainWindowWidth(), pSettings->ui().mainWindowHeight());
    move(pSettings->ui().mainWindowLeft(), pSettings->ui().mainWindowTop());
    ui->dockDir->setVisible(pSettings->ui().showContentsPanel());

    ui->menuAnimation->menuAction()->setVisible(false);

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

void MainWindow::open(const QString &path)
{
    ui->statusbar->showMessage(tr("Openning \"%1\"").arg(path));
    mDirModel->open(path);
    ui->statusbar->clearMessage();
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
    } else {
        mImageWidget->setImage("");
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

void MainWindow::onDirViewSizeChanged()
{
    int width = ui->dirView->width()-ui->dirView->verticalScrollBar()->sizeHint().width()-20;
    mThumbnailDelegate->setThumbnailSize(width);
    mDirModel->setThumbnailSize(width);
    ui->dirView->doItemsLayout();
}

void MainWindow::updateImageFitType()
{
    if (ui->actionFit_Height->isChecked())
        mImageWidget->setFitType(ImageWidget::AutoFitType::Height);
    else if (ui->actionFit_Width->isChecked())
        mImageWidget->setFitType(ImageWidget::AutoFitType::Width);
    else if (ui->actionFit_Page->isChecked())
        mImageWidget->setFitType(ImageWidget::AutoFitType::Page);
    else
        mImageWidget->setFitType(ImageWidget::AutoFitType::None);
}

void MainWindow::onImageWidgetContextMenuRequested(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    menu->addAction(ui->actionFull_Screen);
    menu->addSeparator();
    menu->addAction(ui->actionFit_Page);
    menu->addAction(ui->actionFit_Width);
    menu->addAction(ui->actionFit_Height);
    menu->popup(mImageWidget->mapToGlobal(pos));
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
    QString path = QFileDialog::getOpenFileName(this, tr("Choose image file"));
    if (!path.isEmpty())
        open(path);
}


void MainWindow::on_actionClose_triggered()
{
    mDirModel->close();
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
    QFont font{pSettings->ui().fontName(), pSettings->ui().fontSize()};
    qApp->setFont(font);
    setFont(font);

    ui->actionFit_Width->setChecked(pSettings->view().fitMode() == "Width");
    ui->actionFit_Height->setChecked(pSettings->view().fitMode() == "Height");
    ui->actionFit_Page->setChecked(pSettings->view().fitMode() == "Page");

    updateImageFitType();

    int thumbnailSize = pSettings->view().thumbnailSize();
    mDirModel->setThumbnailSize(thumbnailSize);
    mThumbnailDelegate->setThumbnailSize(thumbnailSize);
    ui->dirView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->dirView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    int oldMin = ui->dirView->minimumWidth();
    int oldMax = ui->dirView->maximumWidth();
    ui->dirView->blockSignals(true);
    ui->dirView->setMinimumWidth(mDirModel->thumbnailSize()+ui->dirView->verticalScrollBar()->sizeHint().width()+20);
    ui->dirView->setMaximumWidth(mDirModel->thumbnailSize()+ui->dirView->verticalScrollBar()->sizeHint().width()+20);
    ui->dockDir->setWidget(ui->dirView);
    ui->dirView->setMinimumWidth(oldMin);
    ui->dirView->setMaximumWidth(oldMax);
    ui->dirView->blockSignals(false);
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
        open(fileName);
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    pSettings->ui().setShowContentsPanel(ui->actionShow_Contents->isChecked());
    pSettings->ui().setMainWindowWidth(width());
    pSettings->ui().setMainWindowHeight(height());
    pSettings->ui().setMainWindowLeft(pos().x());
    pSettings->ui().setMainWindowTop(pos().y());
    if (ui->actionFit_Width->isChecked())
        pSettings->view().setFitMode("Width");
    else if (ui->actionFit_Height->isChecked())
        pSettings->view().setFitMode("Height");
    else if (ui->actionFit_Page->isChecked())
        pSettings->view().setFitMode("Page");
    else
        pSettings->view().setFitMode("None");
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
        ui->toolBar->setVisible(true);
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
        ui->toolBar->setVisible(false);
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


void MainWindow::on_actionOption_triggered()
{
    PSettingsDialog optionDlg = SettingsDialog::optionDialog(this);
    connect(optionDlg.get(), &SettingsDialog::settingsChanged,
            this, &MainWindow::applySettings);
    optionDlg->exec();
}


void MainWindow::on_actionImage_Meta_Info_toggled(bool arg1)
{
    if (!mInFullScreen)
        ui->dockMetaInfo->setVisible(ui->actionImage_Meta_Info->isChecked());
}


void MainWindow::on_dockMetaInfo_visibilityChanged(bool visible)
{
    ui->actionImage_Meta_Info->blockSignals(true);
    ui->actionImage_Meta_Info->setChecked(visible);
    ui->actionImage_Meta_Info->blockSignals(false);
}

