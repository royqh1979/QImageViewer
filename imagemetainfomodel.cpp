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
        if (mMetaInfo->valid()) {
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
            QStandardItem *itemCameraInfos=new QStandardItem(tr("Camera Information"));
            if (!mMetaInfo->serialNumber().isEmpty())
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Serial Number")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->serialNumber()))
                                        });
            if (!mMetaInfo->model().isEmpty())
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Model")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->model()))
                                        });
            if (!mMetaInfo->manufacturer().isEmpty())
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Manufactor")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->manufacturer()))
                                        });
            if (!mMetaInfo->software().isEmpty())
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Software")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->software()))
                                        });
            if (!mMetaInfo->lensModel().isEmpty())
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Lens Model")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->lensModel()))
                                        });
            if (!mMetaInfo->lensSerialNumber().isEmpty())
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Lens Serial Number")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->lensSerialNumber()))
                                        });
            if (!mMetaInfo->lensMake().isEmpty())
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Lens Manufactor")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->lensMake()))
                                        });

            if (mMetaInfo->minFocalLength()!=0 && mMetaInfo->maxFocalLength()!=0)
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Min/Max Focal Length")),
                                            new QStandardItem(QString("%1 / %2")
                                               .arg(mMetaInfo->minFocalLength())
                                               .arg(mMetaInfo->maxFocalLength()))
                                        });
            if (mMetaInfo->minFStop()!=0 && mMetaInfo->maxFStop()!=0)
                itemCameraInfos->appendRow({
                                            new QStandardItem(tr("Min/Max FStop")),
                                            new QStandardItem(QString("%1 / %2")
                                               .arg(mMetaInfo->minFStop())
                                               .arg(mMetaInfo->maxFStop()))
                                        });
            if (itemCameraInfos->rowCount()>0)
                appendRow(itemCameraInfos);
            else
                delete itemCameraInfos;
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
            if (!mMetaInfo->imageTitle().isEmpty())
                itemImageInfos->appendRow({
                                            new QStandardItem(tr("Image Title")),
                                            new QStandardItem(QString::fromUtf8(mMetaInfo->imageTitle()))
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
            if (mMetaInfo->apertureValue() != 0)
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Aperture")),
                                            new QStandardItem(tr("%1").arg(mMetaInfo->apertureValue()))
                                        });
            if (mMetaInfo->fNumber()!=0)
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("FNumber")),
                                            new QStandardItem(tr("f/%1").arg(mMetaInfo->fNumber()))
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

            if (mMetaInfo->exposureTime()!=0)
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Exposure Time")),
                                            new QStandardItem(tr("1/%1 Seconds").arg(int(1/mMetaInfo->exposureTime())))
                                        });
            if (mMetaInfo->brightnessValue()!=0)
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Brightness Value")),
                                            new QStandardItem(tr("%1").arg(mMetaInfo->exposureBiasValue()))
                                        });
            if (mMetaInfo->exposureBiasValue()!=0)
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Exposure Bias")),
                                            new QStandardItem(tr("%1").arg(mMetaInfo->exposureBiasValue()))
                                        });
            if (mMetaInfo->meteringMode()!=ImageMetaInfo::MeteringMode::Unknown) {
                QString meteringMode;
                switch(mMetaInfo->meteringMode()) {
                case ImageMetaInfo::MeteringMode::Average:
                    meteringMode = tr("Average");
                    break;
                case ImageMetaInfo::MeteringMode::CenterWeightedAverage:
                    meteringMode = tr("Center Weighted Average");
                    break;
                case ImageMetaInfo::MeteringMode::MultiSpot:
                    meteringMode = tr("Multi-Spot");
                    break;
                case ImageMetaInfo::MeteringMode::Partial:
                    meteringMode = tr("Partial");
                    break;
                case ImageMetaInfo::MeteringMode::Pattern:
                    meteringMode = tr("Pattern");
                    break;
                case ImageMetaInfo::MeteringMode::Spot:
                    meteringMode = tr("Spot");
                    break;
                default:
                    break;
                }
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Metering Mode")),
                                            new QStandardItem(tr("%1").arg(meteringMode))
                                        });
            }
            if (mMetaInfo->exposureProgram()!=ImageMetaInfo::ExposureProgram::Unknown) {
                QString exposureProgram;
                switch(mMetaInfo->exposureProgram()) {
                case ImageMetaInfo::ExposureProgram::Manual:
                    exposureProgram = tr("Manual");
                    break;
                case ImageMetaInfo::ExposureProgram::NormalProgram:
                    exposureProgram = tr("Normal Program");
                    break;
                case ImageMetaInfo::ExposureProgram::AperturePriority:
                    exposureProgram = tr("Aperture Priority");
                    break;
                case ImageMetaInfo::ExposureProgram::ShutterPriority:
                    exposureProgram = tr("Shutter Priority");
                    break;
                case ImageMetaInfo::ExposureProgram::CreativeProgram:
                    exposureProgram = tr("Creative Program");
                    break;
                case ImageMetaInfo::ExposureProgram::ActionProgram:
                    exposureProgram = tr("Action Program");
                    break;
                case ImageMetaInfo::ExposureProgram::PortraitMode:
                    exposureProgram = tr("Portrait Mode");
                    break;
                case ImageMetaInfo::ExposureProgram::LandscapeMode:
                    exposureProgram = tr("Landscape Mode");
                    break;
                default:
                    break;
                }
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Exposure Program")),
                                            new QStandardItem(tr("%1").arg(exposureProgram))
                                        });
            }
            if (mMetaInfo->exposureMode()!=0xFFFF) {
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Exposure Mode")),
                                            new QStandardItem(tr("%1").arg(mMetaInfo->exposureMode()))
                                        });
            }
            if (mMetaInfo->flash())
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("Flash")),
                                            new QStandardItem((mMetaInfo->flash()&1)?tr("On"):tr("Off"))
                                        });
            if (mMetaInfo->whiteBalance()!=0xFFFF) {
                itemPhotoInfos->appendRow({
                                            new QStandardItem(tr("White Balance")),
                                            new QStandardItem(tr("%1").arg(mMetaInfo->whiteBalance()))
                                        });
            }
    //        itemPhotoInfos->appendRow({
    //                                    new QStandardItem(tr("White Balance")),
    //                                    new QStandardItem(tr("%1").arg(mMetaInfo->iSOSpeedRatings()))
    //                                });
            if (itemPhotoInfos->rowCount()>0)
                appendRow(itemPhotoInfos);
            else
                delete itemPhotoInfos;
        }
        if (rowCount()==0) {
            setColumnCount(1);
            appendRow({
                          new QStandardItem(tr("No Information"))
                      });
        }
        endResetModel();
    }
}

ImageMetaInfo *ImageMetaInfoModel::metaInfo() const
{
    return mMetaInfo;
}
