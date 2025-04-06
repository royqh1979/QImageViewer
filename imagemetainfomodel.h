#ifndef IMAGEMETAINFOMODEL_H
#define IMAGEMETAINFOMODEL_H

#include <QStandardItemModel>

class ImageMetaInfo;
class ImageMetaInfoModel : public QStandardItemModel
{
    Q_OBJECT
public:
    ImageMetaInfoModel(QObject *parent = nullptr);
    ~ImageMetaInfoModel();
    void setMetaInfo(ImageMetaInfo *metaInfo);
    ImageMetaInfo *metaInfo() const;

private:
    ImageMetaInfo *mMetaInfo;
};

#endif // IMAGEMETAINFOMODEL_H
