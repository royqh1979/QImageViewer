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
    void on_actionNext_triggered();

    void on_actionPrevious_triggered();

    void on_actionFirst_triggered();

    void on_actionLast_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();
private:
    void updateStatusBar();
    void applySettings();
private:
    Ui::MainWindow *ui;
    ImageWidget *mImageWidget;
    DirModel *mDirModel;
    QSpinBox *mZoomFactor;
    QLabel *mImageSizeInfo;
    QLabel *mPageInfo;
    ImageMetaInfoModel *mImageMetaInfoModel;
    ThumbnailDelegate *mThumbnailDelegate;

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};
#endif // MAINWINDOW_H
