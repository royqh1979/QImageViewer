#ifndef IMAGEMETAINFO_H
#define IMAGEMETAINFO_H
#include <QString>

class ImageMetaInfo
{
public:
    ImageMetaInfo();
    ImageMetaInfo(const QString& filename);
    enum class ResolutionUnit{
        NotDefined,
        No,
        Inch,
        Centimeter
    };
    enum class ExposureProgram{
        Unknown,
        Manual,
        NormalProgram,
        AperturePriority,
        ShutterPriority,
        CreativeProgram,
        ActionProgram,
        PortraitMode,
        LandscapeMode
    };

    enum class MeteringMode{
        Unknown,
        Average,
        CenterWeightedAverage,
        Spot,
        MultiSpot,
        Pattern,
        Partial,
    };

    bool valid() const;
    uint32_t imageWidth() const;
    uint32_t imageHeight() const;
    uint32_t relatedImageWidth() const;
    uint32_t relatedImageHeight() const;
    const QByteArray &imageDescription() const;
    const QByteArray &manufacturer() const;
    const QByteArray &model() const;
    const QByteArray &serialNumber() const;
    double getXResolution() const;
    double getYResolution() const;
    ResolutionUnit resolutionUnit() const;
    uint16_t bitsPerSample() const;
    const QByteArray &software() const;
    const QByteArray &dateTime() const;
    const QByteArray &dateTimeOriginal() const;
    const QByteArray &dateTimeDigitized() const;
    const QByteArray &subSecTimeOriginal() const;
    const QByteArray &copyright() const;
    double exposureTime() const;
    double fNumber() const;
    ExposureProgram exposureProgram() const;
    uint16_t iSOSpeedRatings() const;
    double shutterSpeedValue() const;
    double apertureValue() const;
    double brightnessValue() const;
    double exposureBiasValue() const;
    double subjectDistance() const;
    double focalLength() const;
    double focalLengthIn35mm() const;
    QString latitude() const;
    QString longitude() const;
    double altitude() const;
    const QString &filename() const;

    const QByteArray &artist() const;

    double ratingPercent() const;

    double rating() const;

    uint16_t flash() const;

    double minFocalLength() const;

    double maxFocalLength() const;

    const QByteArray &imageTitle() const;
    const QByteArray &photographer() const;
    const QByteArray &imageEditor() const;
    const QByteArray &cameraFirmware() const;
    const QByteArray &rAWDevelopingSoftware() const;
    const QByteArray &imageEditingSoftware() const;
    const QByteArray &metadataEditingSoftware() const;
    const QByteArray &lensSerialNumber() const;

    const QByteArray &lensMake() const;
    const QByteArray &lensModel() const;

    double minFStop() const;
    double maxFStop() const;

    MeteringMode meteringMode() const;

    uint16_t exposureMode() const;
    uint16_t whiteBalance() const;

private:
    bool parseInfo(const QString& filename);
private:
    QString mFilename;
    bool mValid;
    uint32_t mImageWidth;                // Image width reported in EXIF data
    uint32_t mImageHeight;               // Image height reported in EXIF data
    uint32_t mRelatedImageWidth;         // Original image width reported in EXIF data
    uint32_t mRelatedImageHeight;        // Original image height reported in EXIF data
    QByteArray mImageDescription;       // Image description
    QByteArray mManufacturer;                   // Camera manufacturer's name
    QByteArray mModel;                  // Camera model
    QByteArray mSerialNumber;           // Serial number of the body of the camera

    double mXResolution;                 // Number of pixels per ResolutionUnit in the ImageWidth direction
    double mYResolution;                 // Number of pixels per ResolutionUnit in the ImageLength direction
    ResolutionUnit mResolutionUnit;            // Unit of measurement for XResolution and YResolution
                                        // 1: no absolute unit of measurement. Used for images that may have a non-square aspect ratio, but no meaningful absolute dimensions
                                        // 2: inch
                                        // 3: centimeter
    uint16_t mBitsPerSample;             // Number of bits per component
    QByteArray mSoftware;               // Software used
    QByteArray mDateTime;               // File change date and time
    QByteArray mDateTimeOriginal;       // Original file date and time (may not exist)
    QByteArray mDateTimeDigitized;      // Digitization date and time (may not exist)
    QByteArray mSubSecTimeOriginal;     // Sub-second time that original picture was taken
    QByteArray mCopyright;              // File copyright information
    QByteArray mArtist;                 // The name of the camera owner, photographer or image creator.
    QByteArray mImageTitle;
    QByteArray mPhotographer;
    QByteArray mImageEditor;
    QByteArray mCameraFirmware;
    QByteArray mRAWDevelopingSoftware;
    QByteArray mImageEditingSoftware;
    QByteArray mMetadataEditingSoftware;
    QByteArray mLensSerialNumber;
    QByteArray mLensMake;
    QByteArray mLensModel;


    double mRating;
    double mRatingPercent;
    double mExposureTime;                // Exposure time in seconds
    double mFNumber;                     // F/stop
    uint16_t mExposureMode;
    uint16_t mWhiteBalance;
    ExposureProgram mExposureProgram;           // Exposure program
                                        // 0: not defined
                                        // 1: manual
                                        // 2: normal program
                                        // 3: aperture priority
                                        // 4: shutter priority
                                        // 5: creative program
                                        // 6: action program
                                        // 7: portrait mode
                                        // 8: landscape mode

    MeteringMode mMeteringMode;         // Metering mode
                                        // 0: unknown
                                        // 1: average
                                        // 2: center weighted average
                                        // 3: spot
                                        // 4: multi-spot
                                        // 5: pattern
                                        // 6: partial
    uint16_t mISOSpeedRatings;           // ISO speed
    double mShutterSpeedValue;           // Shutter speed (reciprocal of exposure time)
    double mApertureValue;               // The lens aperture
    double mBrightnessValue;             // The value of brightness
    double mExposureBiasValue;           // Exposure bias value in EV
    double mSubjectDistance;             // Distance to focus point in meters
    double mFocalLength;                 // Focal length of lens in millimeters
    double mMinFocalLength;
    double mMaxFocalLength;
    double mMinFStop;
    double mMaxFStop;
    uint16_t mFlash;
    double mFocalLengthIn35mm;       // Focal length in 35mm film
    QString mLatitude;                // Image latitude
    QString mLongitude;               // Image longitude
    double mAltitude;                // Altitude in meters, relative to sea level
};

#endif // IMAGEMETAINFO_H
