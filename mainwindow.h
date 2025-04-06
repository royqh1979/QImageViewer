#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ImageWidget;
class FileNavigator;
class QSpinBox;
class QLabel;
class ImageMetaInfoModel;
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
    void on_actionNext_triggered();

    void on_actionPrevious_triggered();

    void on_actionFirst_triggered();

    void on_actionLast_triggered();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();
private:
    void updateStatusBar();
private:
    Ui::MainWindow *ui;
    ImageWidget *mImageWidget;
    FileNavigator *mFileNavigator;
    QSpinBox *mZoomFactor;
    QLabel *mImageSizeInfo;
    QLabel *mPageInfo;
    ImageMetaInfoModel *mImageMetaInfoModel;
};
#endif // MAINWINDOW_H
