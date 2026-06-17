#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ImageWidget;
class DirModel;
class QSpinBox;
class QLabel;
class ImageMetaInfoModel;
class ThumbnailDelegate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openFolder(const QString& path);
private slots:
    void onCurrentFileChanged(int oldFileId, int currentFileId);
    void onRequestPrevImage(bool scrollToBottom);
    void onRequestNextImage();
    void onDirViewCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void onZoomFactorChanged(int newVal);
    void onDirViewSizeChanged();
    void on_actionNext_triggered();

    void on_actionPrevious_triggered();

    void on_actionFirst_triggered();

    void on_actionLast_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();
    void on_actionRotate_Left_triggered();

    void on_actionRotate_Right_triggered();

    void on_actionFlip_Horizontal_triggered();

    void on_actionFlip_Vertical_triggered();

    void on_actionStop_Animation_triggered();

    void on_actionPlay_Animation_triggered();

    void on_actionNext_Frame_triggered();

    void on_actionPrev_Frame_triggered();

    void on_actionFull_Screen_triggered();

    void on_actionShow_Contents_toggled(bool arg1);

    void on_dockDir_visibilityChanged(bool visible);

private:
    void updateStatusBar();
    void applySettings();
    void updateActions();
private:
    Ui::MainWindow *ui;
    ImageWidget *mImageWidget;
    DirModel *mDirModel;
    QSpinBox *mZoomFactor;
    QLabel *mImageSizeInfo;
    QLabel *mPageInfo;
    ImageMetaInfoModel *mImageMetaInfoModel;
    ThumbnailDelegate *mThumbnailDelegate;
    bool mInFullScreen;
    bool mMaximizedBeforeFullScreen;

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

};
#endif // MAINWINDOW_H
