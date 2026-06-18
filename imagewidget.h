/*
 * Copyright (C) 2025 Roy Qu (royqh1979@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QAbstractScrollArea>
#include <QImageReader>

class QTimer;
class ImageWidget : public QAbstractScrollArea
{
    Q_OBJECT
public:
    enum class AutoFitType {
        None,
        Width,
        Height,
        Page
    };
    explicit ImageWidget(QWidget *parent = nullptr);

    float ratio() const;
    void setRatio(float newRatio);

    AutoFitType fitType() const;
    void setFitType(AutoFitType newFitType);
    void setImage(const QString &newPath);
    QSize imageSize() const;
    QString imagePath() const;

    QPixmap currentFrame() const;

    const QColor &background() const;
    void setBackground(const QColor &newBackground);

    bool swapLeftRightWhenTurnPage() const;
    void setSwapLeftRightWhenTurnPage(bool newSwapLeftRightWhenTurnPage);

    void rotate(int degree);
    void horizontalFlip();
    void verticalFlip();

    void scrollToTop();
    void scrollToBottom();
    void scrollToLeft();
    void scrollToRight();

    void play();
    void pause();
    void stop();
    void nextFrame();
    void prevFrame();
    bool isAnimation() const;
signals:
    void imageUpdated();
    void fitTypeChanged();
    void requestPrevImage(bool scrollToBottom);
    void requestNextImage();
    void swapLeftRightWhenTurnPageChanged();

private:
    void playNextFrame();
    void jumpToFrame(std::unique_ptr<QImageReader> &reader, int frameNumber);
    void loadImage();
    void postProcessImage();
    void scaleImage();
    void updateImage();
    void scrollImageByMouseMove(QMouseEvent *event);
private:
    float mRatio;
    AutoFitType mFitType;
    AutoFitType mWorkingFitType;
    QString mImagePath;
    QPixmap mImage;
    QTransform mTransform;
    std::unique_ptr<QImageReader> mImageReader;
    int mImageFrameCount;
    int mCurrentImageDelay;
    int mCurrentFrameNumber;
    QColor mBackground;
    int mScrollAngleX;
    int mScrollAngleY;
    QPoint mOldMousePos;
    QTimer *mFrameTimer;
    bool mMovingImage;
    bool mSwapLeftRightWhenTurnPage;

    // QWidget interface
    Q_PROPERTY(bool swapLeftRightWhenTurnPage READ swapLeftRightWhenTurnPage WRITE setSwapLeftRightWhenTurnPage NOTIFY swapLeftRightWhenTurnPageChanged)

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // IMAGEWIDGET_H
