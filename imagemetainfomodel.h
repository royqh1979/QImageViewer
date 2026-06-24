#ifndef IMAGEMETAINFOMODEL_H
#define IMAGEMETAINFOMODEL_H

#include <QStandardItemModel>

class ImageMetaInfoModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ImageMetaInfoModel(QObject *parent = nullptr);
    ~ImageMetaInfoModel();
    void setImagePath(const QString& imagePath);
private:
    QString mImagePath;
};

#endif // IMAGEMETAINFOMODEL_H
