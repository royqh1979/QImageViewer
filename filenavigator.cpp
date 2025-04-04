#include "filenavigator.h"

#include <QDir>
#include <QImageReader>

FileNavigator::FileNavigator(QObject *parent) : QObject(parent), mCurrentFileIdx{-1}
{

}

void FileNavigator::open(const QString &path)
{
    if (path == mPath)
        return;
    QString oldPath = mPath;
    mPath = path;
    QDir dir{mPath};
    QFileInfoList fileInfos =  dir.entryInfoList(QDir::Filter::Files);
    mFileInfos.clear();
    foreach(const QFileInfo & info, fileInfos) {
        if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLocal8Bit())) {
            mFileInfos.append(info);
        }
    }
    emit pathChanged(oldPath, mPath);
    if (mFileInfos.isEmpty())
        mCurrentFileIdx = -1;
    else
        toFirst();
}

const QString &FileNavigator::path()
{
    return mPath;
}

void FileNavigator::setCurrentFileIdx(int fileIdx)
{
    if (fileIdx < 0)
        fileIdx = 0;
    if (fileIdx >= mFileInfos.count())
        fileIdx = mFileInfos.count()-1;
    if (fileIdx == mCurrentFileIdx)
        return;
    int oldIdx = mCurrentFileIdx;
    emit currentFileChanged(oldIdx, mCurrentFileIdx);
}

int FileNavigator::currentFileIdx()
{
    return mCurrentFileIdx;
}

void FileNavigator::toNext()
{
    setCurrentFileIdx(currentFileIdx()+1);
}

void FileNavigator::toPrevious()
{
    setCurrentFileIdx(currentFileIdx()-1);
}

void FileNavigator::toFirst()
{
    setCurrentFileIdx(0);
}

void FileNavigator::toLast()
{
    setCurrentFileIdx(mFileInfos.count()-1);
}

QFileInfo FileNavigator::currentFileInfo()
{
    if (mCurrentFileIdx<0 || mCurrentFileIdx>=mFileInfos.count())
        return QFileInfo();
    return mFileInfos[mCurrentFileIdx];
}

const QFileInfoList &FileNavigator::fileInfos()
{
    return mFileInfos;
}

QImage FileNavigator::currentImage()
{
    QFileInfo info = currentFileInfo();
    if (!info.exists()) {
        return QImage();
    }
    return QImage{currentFileInfo().absoluteFilePath()};
}
