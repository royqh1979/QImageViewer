#ifndef FILENAVIGATOR_H
#define FILENAVIGATOR_H

#include <QFileInfo>
#include <QFileInfoList>
#include <QObject>

class FileNavigator : public QObject
{
    Q_OBJECT
public:
    explicit FileNavigator(QObject *parent = nullptr);
    void open(const QString& path);
    const QString &path();
    void setCurrentFileIdx(int fileIdx);
    int currentFileIdx();
    void toNext();
    void toPrevious();
    void toFirst();
    void toLast();
    QFileInfo currentFileInfo();
    const QFileInfoList &fileInfos();
    int imageCount();
signals:
    void currentFileChanged(int oldFileId, int currentFileId);
    void pathChanged(const QString& oldPath, const QString& newPath);
private:
    QString mPath;
    QFileInfoList mFileInfos;
    int mCurrentFileIdx;
};

#endif // FILENAVIGATOR_H
