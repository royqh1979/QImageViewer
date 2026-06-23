#include "imagemetainfomodel.h"
#include "imagemetainfo.h"
#include <cfloat>

ImageMetaInfoModel::ImageMetaInfoModel(QObject *parent):QStandardItemModel(parent),
    mMetaInfo{nullptr}
{
    setColumnCount(2);
}

ImageMetaInfoModel::~ImageMetaInfoModel()
{
    if (!mMetaInfo)
        delete mMetaInfo;
}

void ImageMetaInfoModel::setMetaInfo(ImageMetaInfo *info)
{
    if (info!=mMetaInfo) {
        beginResetModel();
        clear();
        if (!mMetaInfo)
            delete mMetaInfo;
        mMetaInfo = info;
        if (!mMetaInfo->valid()) {
            setColumnCount(1);
            appendRow({
                          new QStandardItem(tr("No Information"))
                      });
            return;
        }
        setColumnCount(2);
        QStandardItem *itemGPSInfos=new QStandardItem(tr("GPS Information"));
        if (!mMetaInfo->latitude().isEmpty())
            itemGPSInfos->appendRow({
                                        new QStandardItem(tr("Latitude")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->latitude()))
                                    });
        if (!mMetaInfo->longitude().isEmpty())
            itemGPSInfos->appendRow({
                                        new QStandardItem(tr("Longitude")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->longitude()))
                                    });
        if (mMetaInfo->altitude()!=DBL_MAX)
            itemGPSInfos->appendRow({
                                        new QStandardItem(tr("Altitude")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->altitude()))
                                    });
        if (itemGPSInfos->rowCount()>0)
            appendRow(itemGPSInfos);
        else
            delete itemGPSInfos;

//http://en.wikipedia.org/wiki/Exif#/media/File:DigiKam_EXIF_information_screenshot.png
        QStandardItem *itemImageInfos=new QStandardItem(tr("Image Information"));
        if (!mMetaInfo->artist().isEmpty())
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Artist")),
                                        new QStandardItem(QString::fromUtf8(mMetaInfo->artist()))
                                    });
        if (!mMetaInfo->copyright().isEmpty())
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Copyright")),
                                        new QStandardItem(QString::fromUtf8(mMetaInfo->copyright()))
                                    });
        if (!mMetaInfo->dateTime().isEmpty())
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Date Time")),
                                        new QStandardItem(QString::fromLocal8Bit(mMetaInfo->dateTime()))
                                    });
        if (!mMetaInfo->manufacturer().isEmpty())
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Manufactuer")),
                                        new QStandardItem(QString::fromUtf8(mMetaInfo->manufacturer()))
                                    });
        if (!mMetaInfo->software().isEmpty())
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Software")),
                                        new QStandardItem(QString::fromUtf8(mMetaInfo->software()))
                                    });
        if (!mMetaInfo->model().isEmpty())
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Model")),
                                        new QStandardItem(QString::fromUtf8(mMetaInfo->model()))
                                    });
        if (!mMetaInfo->imageDescription().isEmpty())
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Image Description")),
                                        new QStandardItem(QString::fromUtf8(mMetaInfo->imageDescription()))
                                    });
        if (mMetaInfo->rating()!=0)
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Rating")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->rating()))
                                    });
        if (mMetaInfo->ratingPercent()!=0)
            itemImageInfos->appendRow({
                                        new QStandardItem(tr("Rating")),
                                          new QStandardItem(tr("%1%").arg(mMetaInfo->ratingPercent()))
                                    });

        if (itemImageInfos->rowCount()>0)
            appendRow(itemImageInfos);
        else
            delete itemImageInfos;
        QStandardItem *itemPhotoInfos=new QStandardItem(tr("Photo Information"));
        if (mMetaInfo->apertureValue() != 0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("Aperture")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->apertureValue()))
                                    });
        if (mMetaInfo->exposureBiasValue()!=0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("Exposure Bias")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->exposureBiasValue()))
                                    });
        if (mMetaInfo->exposureTime()!=0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("Exposure Time")),
                                        new QStandardItem(tr("1/%1 Seconds").arg(int(1/mMetaInfo->exposureTime())))
                                    });
        if (mMetaInfo->flash())
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("Flash")),
                                        new QStandardItem((mMetaInfo->flash()&1)?tr("On"):tr("Off"))
                                    });
        if (mMetaInfo->fNumber()!=0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("FNumber")),
                                        new QStandardItem(tr("f/%1").arg(mMetaInfo->fNumber()))
                                    });
        if (mMetaInfo->focalLength()!=0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("Focal Length")),
                                        new QStandardItem(tr("%1 mm").arg(mMetaInfo->focalLength()))
                                    });
        if (mMetaInfo->focalLengthIn35mm()!=0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("Focal length(35m film)")),
                                        new QStandardItem(tr("%1 mm").arg(mMetaInfo->focalLengthIn35mm()))
                                    });
        if (mMetaInfo->iSOSpeedRatings()!=0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("ISO Speed Rating")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->iSOSpeedRatings()))
                                    });
        if (mMetaInfo->shutterSpeedValue()!=0)
            itemPhotoInfos->appendRow({
                                        new QStandardItem(tr("Shutter Speed Value")),
                                        new QStandardItem(tr("%1").arg(mMetaInfo->shutterSpeedValue()))
                                    });
//        itemPhotoInfos->appendRow({
//                                    new QStandardItem(tr("White Balance")),
//                                    new QStandardItem(tr("%1").arg(mMetaInfo->iSOSpeedRatings()))
//                                });
        if (itemPhotoInfos->rowCount()>0)
            appendRow(itemPhotoInfos);
        else
            delete itemPhotoInfos;

        endResetModel();
    }
}

ImageMetaInfo *ImageMetaInfoModel::metaInfo() const
{
    return mMetaInfo;
}
