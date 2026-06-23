#include "imagemetainfo.h"
#include "TinyEXIF.h"
#include <QStandardItemModel>
#include <fstream>
ImageMetaInfo::ImageMetaInfo()
{
    mValid = false;
}

ImageMetaInfo::ImageMetaInfo(const QString &filename)
{
    mFilename = filename;
    mValid = parseInfo(filename);
}

bool ImageMetaInfo::valid() const
{
    return mValid;
}

uint32_t ImageMetaInfo::imageWidth() const
{
    return mImageWidth;
}

uint32_t ImageMetaInfo::imageHeight() const
{
    return mImageHeight;
}

uint32_t ImageMetaInfo::relatedImageWidth() const
{
    return mRelatedImageWidth;
}

uint32_t ImageMetaInfo::relatedImageHeight() const
{
    return mRelatedImageHeight;
}

const QByteArray &ImageMetaInfo::imageDescription() const
{
    return mImageDescription;
}

const QByteArray &ImageMetaInfo::manufacturer() const
{
    return mManufacturer;
}

const QByteArray &ImageMetaInfo::model() const
{
    return mModel;
}

const QByteArray &ImageMetaInfo::serialNumber() const
{
    return mSerialNumber;
}

double ImageMetaInfo::getXResolution() const
{
    return mXResolution;
}

double ImageMetaInfo::getYResolution() const
{
    return mYResolution;
}

ImageMetaInfo::ResolutionUnit ImageMetaInfo::resolutionUnit() const
{
    return mResolutionUnit;
}

uint16_t ImageMetaInfo::bitsPerSample() const
{
    return mBitsPerSample;
}

const QByteArray &ImageMetaInfo::software() const
{
    return mSoftware;
}

const QByteArray &ImageMetaInfo::dateTime() const
{
    return mDateTime;
}

const QByteArray &ImageMetaInfo::dateTimeOriginal() const
{
    return mDateTimeOriginal;
}

const QByteArray &ImageMetaInfo::dateTimeDigitized() const
{
    return mDateTimeDigitized;
}

const QByteArray &ImageMetaInfo::subSecTimeOriginal() const
{
    return mSubSecTimeOriginal;
}

const QByteArray &ImageMetaInfo::copyright() const
{
    return mCopyright;
}

double ImageMetaInfo::exposureTime() const
{
    return mExposureTime;
}

double ImageMetaInfo::fNumber() const
{
    return mFNumber;
}

ImageMetaInfo::ExposureProgram ImageMetaInfo::exposureProgram() const
{
    return mExposureProgram;
}

uint16_t ImageMetaInfo::iSOSpeedRatings() const
{
    return mISOSpeedRatings;
}

double ImageMetaInfo::shutterSpeedValue() const
{
    return mShutterSpeedValue;
}

double ImageMetaInfo::apertureValue() const
{
    return mApertureValue;
}

double ImageMetaInfo::brightnessValue() const
{
    return mBrightnessValue;
}

double ImageMetaInfo::exposureBiasValue() const
{
    return mExposureBiasValue;
}

double ImageMetaInfo::subjectDistance() const
{
    return mSubjectDistance;
}

double ImageMetaInfo::focalLength() const
{
    return mFocalLength;
}

double ImageMetaInfo::focalLengthIn35mm() const
{
    return mFocalLengthIn35mm;
}

QString ImageMetaInfo::latitude() const
{
    return mLatitude;
}

QString ImageMetaInfo::longitude() const
{
    return mLongitude;
}

double ImageMetaInfo::altitude() const
{
    return mAltitude;
}

bool ImageMetaInfo::parseInfo(const QString &filename)
{
    std::ifstream stream(filename.toLocal8Bit(), std::ios::binary);
    if (!stream)
        return false;
    TinyEXIF::EXIFInfo imageEXIF;
    if (imageEXIF.parseFrom(stream)!= TinyEXIF::PARSE_SUCCESS) {
        return false;
    }
    mImageWidth = imageEXIF.ImageWidth;                // Image width reported in EXIF data
    mImageHeight = imageEXIF.ImageHeight;               // Image height reported in EXIF data
    mRelatedImageWidth = imageEXIF.RelatedImageWidth;         // Original image width reported in EXIF data
    mRelatedImageHeight = imageEXIF.RelatedImageHeight;        // Original image height reported in EXIF data
    mImageDescription = QByteArray::fromStdString(imageEXIF.ImageDescription);       // Image description
    mManufacturer = QByteArray::fromStdString(imageEXIF.Make);                   // Camera manufacturer's name
    mModel = QByteArray::fromStdString(imageEXIF.Model);                  // Camera model
    mSerialNumber = QByteArray::fromStdString(imageEXIF.SerialNumber);           // Serial number of the body of the camera

    mXResolution = imageEXIF.XResolution;                 // Number of pixels per ResolutionUnit in the ImageWidth direction
    mYResolution = imageEXIF.YResolution;                 // Number of pixels per ResolutionUnit in the ImageLength direction
    // Unit of measurement for XResolution and YResolution
    // 1: no absolute unit of measurement. Used for images that may have a non-square aspect ratio, but no meaningful absolute dimensions
    // 2: inch
    // 3: centimeter
    switch(imageEXIF.ResolutionUnit) {
    case 1:
        mResolutionUnit = ResolutionUnit::No;
        break;
    case 2:
        mResolutionUnit = ResolutionUnit::Inch;
        break;
    case 3:
        mResolutionUnit = ResolutionUnit::Centimeter;
        break;
    default:
        mResolutionUnit = ResolutionUnit::NotDefined;
    }

    mBitsPerSample = imageEXIF.BitsPerSample;             // Number of bits per component
    mSoftware = QByteArray::fromStdString(imageEXIF.Software);               // Software used
    mDateTime = QByteArray::fromStdString(imageEXIF.DateTime);               // File change date and time
    mDateTimeOriginal = QByteArray::fromStdString(imageEXIF.DateTimeOriginal);       // Original file date and time (may not exist)
    mDateTimeDigitized = QByteArray::fromStdString(imageEXIF.DateTimeDigitized);      // Digitization date and time (may not exist)
    mSubSecTimeOriginal = QByteArray::fromStdString(imageEXIF.SubSecTimeOriginal);     // Sub-second time that original picture was taken
    mCopyright = QByteArray::fromStdString(imageEXIF.Copyright);              // File copyright information
    mArtist = QByteArray::fromStdString(imageEXIF.Artist);              // The name of the camera owner, photographer or image creator.

    mImageTitle = QByteArray::fromStdString(imageEXIF.ImageTitle);
    mPhotographer = QByteArray::fromStdString(imageEXIF.Photographer);
    mImageEditor = QByteArray::fromStdString(imageEXIF.ImageEditor);
    mCameraFirmware = QByteArray::fromStdString(imageEXIF.CameraFirmware);
    mRAWDevelopingSoftware = QByteArray::fromStdString(imageEXIF.RAWDevelopingSoftware);
    mImageEditingSoftware = QByteArray::fromStdString(imageEXIF.ImageEditingSoftware);
    mMetadataEditingSoftware = QByteArray::fromStdString(imageEXIF.MetadataEditingSoftware);
    mLensSerialNumber = QByteArray::fromStdString(imageEXIF.LensInfo.SerialNumber);
    mLensModel = QByteArray::fromStdString(imageEXIF.LensInfo.Model);
    mLensMake = QByteArray::fromStdString(imageEXIF.LensInfo.Make);

    mRating = imageEXIF.Rating;
    mRatingPercent = imageEXIF.RatingPercent;
    mExposureTime = imageEXIF.ExposureTime;                // Exposure time in seconds
    mFNumber = imageEXIF.FNumber;                     // F/stop
    // Exposure program
    // 0: not defined
    // 1: manual
    // 2: normal program
    // 3: aperture priority
    // 4: shutter priority
    // 5: creative program
    // 6: action program
    // 7: portrait mode
    // 8: landscape mode
    switch(imageEXIF.ExposureProgram) {
    case 1:
        mExposureProgram = ExposureProgram::Manual;
        break;
    case 2:
        mExposureProgram = ExposureProgram::NormalProgram;
        break;
    case 3:
        mExposureProgram = ExposureProgram::AperturePriority;
        break;
    case 4:
        mExposureProgram = ExposureProgram::ShutterPriority;
        break;
    case 5:
        mExposureProgram = ExposureProgram::CreativeProgram;
        break;
    case 6:
        mExposureProgram = ExposureProgram::ActionProgram;
        break;
    case 7:
        mExposureProgram = ExposureProgram::PortraitMode;
        break;
    case 8:
        mExposureProgram = ExposureProgram::LandscapeMode;
        break;
    default:
        mExposureProgram = ExposureProgram::Unknown;
    }
    // Metering mode
    // 0: unknown
    // 1: average
    // 2: center weighted average
    // 3: spot
    // 4: multi-spot
    // 5: pattern
    // 6: partial
    switch(imageEXIF.MeteringMode) {
    case 1:
        mMeteringMode = MeteringMode::Average;
        break;
    case 2:
        mMeteringMode = MeteringMode::CenterWeightedAverage;
        break;
    case 3:
        mMeteringMode = MeteringMode::Spot;
        break;
    case 4:
        mMeteringMode = MeteringMode::MultiSpot;
        break;
    case 5:
        mMeteringMode = MeteringMode::Pattern;
        break;
    case 6:
        mMeteringMode = MeteringMode::Partial;
        break;
    default:
        mMeteringMode = MeteringMode::Unknown;
    }

    mISOSpeedRatings = imageEXIF.ISOSpeedRatings;           // ISO speed
    mShutterSpeedValue = imageEXIF.ShutterSpeedValue;           // Shutter speed (reciprocal of exposure time)
    mApertureValue = imageEXIF.ApertureValue;               // The lens aperture
    mBrightnessValue = imageEXIF.BrightnessValue;             // The value of brightness
    mExposureBiasValue = imageEXIF.ExposureBiasValue;           // Exposure bias value in EV
    mSubjectDistance = imageEXIF.SubjectDistance;             // Distance to focus point in meters
    mFocalLength = imageEXIF.FocalLength;                 // Focal length of lens in millimeters
    mFlash = imageEXIF.Flash;
    mFocalLengthIn35mm = imageEXIF.LensInfo.FocalLengthIn35mm;       // Focal length in 35mm film
    mMinFocalLength = imageEXIF.LensInfo.FocalLengthMin;
    mMaxFocalLength = imageEXIF.LensInfo.FocalLengthMax;
    mMinFStop = imageEXIF.LensInfo.FStopMin;
    mMaxFStop = imageEXIF.LensInfo.FStopMax;
    mExposureMode = imageEXIF.ExposureMode;
    mWhiteBalance = imageEXIF.WhiteBalance;
    if (imageEXIF.GeoLocation.LatComponents.direction)
        mLatitude = QString("%1°%2'%3\" %4").arg(int(imageEXIF.GeoLocation.LatComponents.degrees))
                .arg(int(imageEXIF.GeoLocation.LatComponents.minutes))
                .arg(int(imageEXIF.GeoLocation.LatComponents.seconds))
                .arg(QChar(imageEXIF.GeoLocation.LatComponents.direction));   // Image latitude expressed as decimal
    if (imageEXIF.GeoLocation.LonComponents.direction)
        mLongitude = QString("%1°%2'%3\" %4").arg(int(imageEXIF.GeoLocation.LonComponents.degrees))
                .arg(int(imageEXIF.GeoLocation.LonComponents.minutes))
                .arg(int(imageEXIF.GeoLocation.LonComponents.seconds))
                .arg(QChar(imageEXIF.GeoLocation.LonComponents.direction));   // Image longitude expressed as decimal
    mAltitude = imageEXIF.GeoLocation.Altitude;                // Altitude in meters, relative to sea level

    return true;
}

uint16_t ImageMetaInfo::exposureMode() const
{
    return mExposureMode;
}

uint16_t ImageMetaInfo::whiteBalance() const
{
    return mWhiteBalance;
}

ImageMetaInfo::MeteringMode ImageMetaInfo::meteringMode() const
{
    return mMeteringMode;
}

double ImageMetaInfo::minFStop() const
{
    return mMinFStop;
}

double ImageMetaInfo::maxFStop() const
{
    return mMaxFStop;
}

const QByteArray &ImageMetaInfo::lensMake() const
{
    return mLensMake;
}

const QByteArray &ImageMetaInfo::lensModel() const
{
    return mLensModel;
}

const QByteArray &ImageMetaInfo::imageTitle() const
{
    return mImageTitle;
}

const QByteArray &ImageMetaInfo::photographer() const
{
    return mPhotographer;
}

const QByteArray &ImageMetaInfo::imageEditor() const
{
    return mImageEditor;
}

const QByteArray &ImageMetaInfo::cameraFirmware() const
{
    return mCameraFirmware;
}

const QByteArray &ImageMetaInfo::rAWDevelopingSoftware() const
{
    return mRAWDevelopingSoftware;
}

const QByteArray &ImageMetaInfo::imageEditingSoftware() const
{
    return mImageEditingSoftware;
}

const QByteArray &ImageMetaInfo::metadataEditingSoftware() const
{
    return mMetadataEditingSoftware;
}

const QByteArray &ImageMetaInfo::lensSerialNumber() const
{
    return mLensSerialNumber;
}

double ImageMetaInfo::maxFocalLength() const
{
    return mMaxFocalLength;
}

double ImageMetaInfo::minFocalLength() const
{
    return mMinFocalLength;
}

uint16_t ImageMetaInfo::flash() const
{
    return mFlash;
}

double ImageMetaInfo::rating() const
{
    return mRating;
}

double ImageMetaInfo::ratingPercent() const
{
    return mRatingPercent;
}

const QByteArray &ImageMetaInfo::artist() const
{
    return mArtist;
}

const QString &ImageMetaInfo::filename() const
{
    return mFilename;
}

