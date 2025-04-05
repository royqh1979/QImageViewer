#include "imagemetainfo.h"
#include "TinyEXIF.h"
#include <fstream>
ImageMetaInfo::ImageMetaInfo()
{
    mValid = false;
}

ImageMetaInfo::ImageMetaInfo(const QString &filename)
{
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

bool ImageMetaInfo::flashUsed() const
{
    return mFlashUsed;
}

double ImageMetaInfo::focalLengthIn35mm() const
{
    return mFocalLengthIn35mm;
}

double ImageMetaInfo::latitude() const
{
    return mLatitude;
}

double ImageMetaInfo::longitude() const
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
    TinyEXIF::EXIFInfo imageEXIF(stream);
    if (!imageEXIF.Fields) {
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
    case 2:
        mResolutionUnit = ResolutionUnit::Inch;
        break;
    case 3:
        mResolutionUnit = ResolutionUnit::Centimeter;
        break;
    default:
        mResolutionUnit = ResolutionUnit::None;
    }

    mBitsPerSample = imageEXIF.BitsPerSample;             // Number of bits per component
    mSoftware = QByteArray::fromStdString(imageEXIF.Software);               // Software used
    mDateTime = QByteArray::fromStdString(imageEXIF.DateTime);               // File change date and time
    mDateTimeOriginal = QByteArray::fromStdString(imageEXIF.DateTimeOriginal);       // Original file date and time (may not exist)
    mDateTimeDigitized = QByteArray::fromStdString(imageEXIF.DateTimeDigitized);      // Digitization date and time (may not exist)
    mSubSecTimeOriginal = QByteArray::fromStdString(imageEXIF.SubSecTimeOriginal);     // Sub-second time that original picture was taken
    mCopyright = QByteArray::fromStdString(imageEXIF.Copyright);              // File copyright information
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
        mExposureProgram = ExposureProgram::NotDefined;
    }

    mISOSpeedRatings = imageEXIF.ISOSpeedRatings;           // ISO speed
    mShutterSpeedValue = imageEXIF.ShutterSpeedValue;           // Shutter speed (reciprocal of exposure time)
    mApertureValue = imageEXIF.ApertureValue;               // The lens aperture
    mBrightnessValue = imageEXIF.BrightnessValue;             // The value of brightness
    mExposureBiasValue = imageEXIF.ExposureBiasValue;           // Exposure bias value in EV
    mSubjectDistance = imageEXIF.SubjectDistance;             // Distance to focus point in meters
    mFocalLength = imageEXIF.FocalLength;                 // Focal length of lens in millimeters
    mFlashUsed = imageEXIF.Flash & 1;
    mFocalLengthIn35mm = imageEXIF.LensInfo.FocalLengthIn35mm;       // Focal length in 35mm film
    mLatitude = imageEXIF.GeoLocation.Latitude;                // Image latitude expressed as decimal
    mLongitude = imageEXIF.GeoLocation.Longitude;               // Image longitude expressed as decimal
    mAltitude = imageEXIF.GeoLocation.Altitude;                // Altitude in meters, relative to sea level

    return true;
}
