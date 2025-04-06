#include "imagemetainfomodel.h"
#include "imagemetainfo.h"

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
        setColumnCount(2);
        QStandardItem *itemGPSInfos=new QStandardItem(tr("GPS Information"));
        appendRow(itemGPSInfos);
        itemGPSInfos->appendRow({
                                    new QStandardItem(tr("Latitude")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->latitude()))
                                });
        itemGPSInfos->appendRow({
                                    new QStandardItem(tr("Longitude")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->longitude()))
                                });

        itemGPSInfos->appendRow({
                                    new QStandardItem(tr("Altitude")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->altitude()))
                                });

//http://en.wikipedia.org/wiki/Exif#/media/File:DigiKam_EXIF_information_screenshot.png
        QStandardItem *itemImageInfos=new QStandardItem(tr("Image Information"));
        appendRow(itemImageInfos);
        itemImageInfos->appendRow({
                                    new QStandardItem(tr("Date Time")),
                                    new QStandardItem(QString::fromLocal8Bit(mMetaInfo->dateTime()))
                                });
        itemImageInfos->appendRow({
                                    new QStandardItem(tr("Manufactuer")),
                                    new QStandardItem(QString::fromUtf8(mMetaInfo->manufacturer()))
                                });
        itemImageInfos->appendRow({
                                    new QStandardItem(tr("Model")),
                                    new QStandardItem(QString::fromUtf8(mMetaInfo->model()))
                                });
        QStandardItem *itemPhotoInfos=new QStandardItem(tr("Photo Information"));
        appendRow(itemPhotoInfos);
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("Aperture")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->apertureValue()))
                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("Exposure Bias")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->exposureBiasValue()))
                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("Exposure Time")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->exposureTime()))
                                });
//        itemPhotoInfos->appendRow({
//                                    new QStandardItem(tr("Exposure Mode")),
//                                    new QStandardItem(tr("%1").arg(mMetaInfo->iSOSpeedRatings()))
//                                });
        //        itemPhotoInfos->appendRow({
        //                                    new QStandardItem(tr("Exposure Program")),
        //                                    new QStandardItem(tr("%1").arg(mMetaInfo->iSOSpeedRatings()))
        //                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("Flash")),
                                    new QStandardItem(mMetaInfo->flashUsed()?tr("On"):tr("Off"))
                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("FNumber")),
                                    new QStandardItem(mMetaInfo->fNumber())
                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("Focal Length")),
                                    new QStandardItem(tr("%1 mm").arg(mMetaInfo->focalLength()))
                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("Focal length(35m film)")),
                                    new QStandardItem(tr("%1 mm").arg(mMetaInfo->focalLengthIn35mm()))
                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("ISO Speed Rating")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->iSOSpeedRatings()))
                                });
        itemPhotoInfos->appendRow({
                                    new QStandardItem(tr("Shutter Speed")),
                                    new QStandardItem(tr("%1").arg(mMetaInfo->shutterSpeedValue()))
                                });
//        itemPhotoInfos->appendRow({
//                                    new QStandardItem(tr("White Balance")),
//                                    new QStandardItem(tr("%1").arg(mMetaInfo->iSOSpeedRatings()))
//                                });


        endResetModel();
    }
}

ImageMetaInfo *ImageMetaInfoModel::metaInfo() const
{
    return mMetaInfo;
}
