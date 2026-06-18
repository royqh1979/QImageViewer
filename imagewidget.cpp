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

#include "imagewidget.h"
#include <QPainter>
#include <QScrollBar>
#include <QDebug>
#include <QWheelEvent>
#include <QTransform>
#include <QImageReader>
#include <QTimer>

ImageWidget::ImageWidget(QWidget *parent) :
  QAbstractScrollArea{parent},
  mRatio{1.0},
  mFitType{AutoFitType::Page},
  mWorkingFitType{AutoFitType::Page},
  mBackground{Qt::gray},
  mScrollAngleX{0},
  mScrollAngleY{0},
  mMovingImage{false},
  mSwapLeftRightWhenTurnPage{false}
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mFrameTimer = new QTimer(this);
    mFrameTimer->setSingleShot(true);
    connect(mFrameTimer, &QTimer::timeout,
            this, &ImageWidget::playNextFrame);
    mImageFrameCount = -1;
    mCurrentFrameNumber = -1;
}

float ImageWidget::ratio() const
{
    return mRatio;
}

void ImageWidget::setRatio(float newRatio)
{
    newRatio = std::max((float)0.01, newRatio);
    if (newRatio!=mRatio) {
        mRatio = newRatio;
        mWorkingFitType = AutoFitType::None;
        loadImage();
    }
}

ImageWidget::AutoFitType ImageWidget::fitType() const
{
    return mFitType;
}

void ImageWidget::setFitType(AutoFitType newFitType)
{
    if (mFitType!=newFitType) {
        mFitType = newFitType;
        mWorkingFitType = mFitType;
        if (mFitType == AutoFitType::None)
            mRatio = 1;
        loadImage();
        emit fitTypeChanged();
    }
}

void ImageWidget::setImage(const QString &newPath)
{
    mImage = QPixmap();
    mImageFrameCount = -1;
    mCurrentFrameNumber = -1;
    mImageReader = nullptr;
    mTransform = QTransform();
    viewport()->update();
    if (newPath.isEmpty())
        return;
    mImagePath = newPath;
    std::unique_ptr<QImageReader> reader = std::make_unique<QImageReader>(newPath);
    if (!reader->canRead() || reader->imageCount() == -1) {
        return;
    }
    mWorkingFitType = mFitType;
    if (reader->imageCount() <= 1) {
        mImageFrameCount = 1;
        loadImage();
    } else {
        mImageFrameCount = reader->imageCount();
        mImageReader = std::move(reader);
        play();
    }
    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);
}

QSize ImageWidget::imageSize() const
{
    return mImage.size();
}

QPixmap ImageWidget::currentFrame() const
{
    return mImage;
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    loadImage();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(viewport());
    painter.fillRect(0,0,viewport()->width(),viewport()->height(),mBackground);
    if (mImage.isNull())
        return ;
    int img_x = horizontalScrollBar()->value();
    int img_y = verticalScrollBar()->value();
    int x = std::max(0, (viewport()->width() - mImage.width())/2);
    int y = std::max(0, (viewport()->height() - mImage.height())/2);
    painter.drawPixmap(x,y,mImage,img_x,img_y,viewport()->width(), viewport()->height());
}

void ImageWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down
            && verticalScrollBar()->value()==verticalScrollBar()->maximum()) {
        emit requestNextImage();
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Up
            && verticalScrollBar()->value()==verticalScrollBar()->minimum()) {
        emit requestPrevImage(true);
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Left) {
        if (!mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->minimum()) {
            emit requestPrevImage(false);
            event->accept();
            return;
        }
        if (mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->maximum()) {
            emit requestNextImage();
            event->accept();
            return;
        }
    }
    if (event->key() == Qt::Key_Right) {
        if (!mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->maximum()) {
            emit requestNextImage();
            event->accept();
            return;
        }
        if (mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->minimum()) {
            emit requestPrevImage(false);
            event->accept();
            return;
        }

    }
    QAbstractScrollArea::keyPressEvent(event);
}

void ImageWidget::loadImage()
{
    if (mImageReader) {
        if (!mImageReader->canRead()) {
            mImageReader = std::make_unique<QImageReader>(mImagePath);
        } else if (mCurrentFrameNumber < 0)
            mImageReader->jumpToImage(mImageFrameCount-1);
        mCurrentImageDelay = mImageReader->nextImageDelay();
        mImage = QPixmap::fromImage(mImageReader->read());
        mCurrentFrameNumber = mImageReader->currentImageNumber();
    } else {
        mImage = QPixmap::fromImage(QImage{mImagePath});
    }
    if (!mImage.isNull()) {
        QSize oldSize = mImage.size();
        switch(mWorkingFitType) {
        case AutoFitType::Page:
            mImage = mImage.scaled(viewport()->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
            mRatio = mImage.size().width()/oldSize.width();
            break;
        case AutoFitType::None:
            mImage = mImage.scaled(
                            oldSize.width() * mRatio,
                            oldSize.height() * mRatio,
                            Qt::KeepAspectRatio, Qt::SmoothTransformation);
            break;
        case AutoFitType::Height:
            mImage = mImage.scaledToHeight(viewport()->height(), Qt::SmoothTransformation);
            mRatio = (float)mImage.height() / oldSize.height();
            break;
        case AutoFitType::Width:
            mImage = mImage.scaledToWidth(viewport()->width(), Qt::SmoothTransformation);
            mRatio = (float)mImage.width() / oldSize.width();
            break;
        }
    } else {
        mImageFrameCount = 0;
        mCurrentFrameNumber = -1;
    }
    mImage = mImage.transformed(mTransform);
    updateImage();
}

void ImageWidget::updateImage()
{
    if (!mImage.isNull()) {
        verticalScrollBar()->setRange(0, mImage.height()-viewport()->height());
        horizontalScrollBar()->setRange(0, mImage.width()-viewport()->width());
        verticalScrollBar()->setSingleStep(mImage.height()/10);
        horizontalScrollBar()->setSingleStep(mImage.width()/10);
    } else {
        verticalScrollBar()->setRange(0,0);
        horizontalScrollBar()->setRange(0,0);
    }
    emit imageUpdated();
    viewport()->update();
}

void ImageWidget::scrollImageByMouseMove(QMouseEvent( *event))
{
    if (!mMovingImage)
        return;
    QPoint delta = event->pos() - mOldMousePos;
    mOldMousePos = event->pos();
    horizontalScrollBar()->setValue(
                horizontalScrollBar()->value() - delta.x());
    verticalScrollBar()->setValue(
                verticalScrollBar()->value() - delta.y());
}

bool ImageWidget::swapLeftRightWhenTurnPage() const
{
    return mSwapLeftRightWhenTurnPage;
}

void ImageWidget::setSwapLeftRightWhenTurnPage(bool newSwapLeftRightWhenTurnPage)
{
    if (mSwapLeftRightWhenTurnPage == newSwapLeftRightWhenTurnPage)
        return;
    mSwapLeftRightWhenTurnPage = newSwapLeftRightWhenTurnPage;
    emit swapLeftRightWhenTurnPageChanged();
}

void ImageWidget::rotate(int degree)
{
    if (mImage.isNull())
        return;
    mTransform = mTransform * QTransform().rotate(degree);
    mImage = mImage.transformed(QTransform().rotate(90));
    updateImage();
}

void ImageWidget::horizontalFlip()
{
    if (mImage.isNull())
        return;
    mTransform = mTransform * QTransform().scale(-1,1);
    mImage = mImage.transformed(QTransform().scale(-1,1));
    updateImage();
}

void ImageWidget::verticalFlip()
{
    if (mImage.isNull())
        return;
    mTransform = mTransform * QTransform().scale(1,-1);
    mImage = mImage.transformed(QTransform().scale(1,-1));
    updateImage();
}

void ImageWidget::scrollToTop()
{
    verticalScrollBar()->setValue(verticalScrollBar()->minimum());
}

void ImageWidget::scrollToBottom()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void ImageWidget::scrollToLeft()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->minimum());
}

void ImageWidget::scrollToRight()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->maximum());
}

void ImageWidget::play()
{
    if (mImageFrameCount<=1)
        return;
    if (!mImageReader) {
        mImageReader = std::make_unique<QImageReader>(mImagePath);
        mImageReader->jumpToImage(mCurrentFrameNumber);
    }
    playNextFrame();
}

void ImageWidget::pause()
{
    mImageReader = nullptr;
    mFrameTimer->stop();
}

void ImageWidget::nextFrame()
{
    mCurrentFrameNumber++;
    loadImage();
}

void ImageWidget::prevFrame()
{
    mCurrentFrameNumber--;
    loadImage();
}

void ImageWidget::playNextFrame()
{
    if (!mImageReader || mImageFrameCount<=1)
        return;
    mFrameTimer->setInterval(mCurrentImageDelay);
    mFrameTimer->start();
    mCurrentFrameNumber++;
    loadImage();
}

bool ImageWidget::isAnimation() const
{
    return mImageFrameCount>1;
}

void ImageWidget::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() == Qt::KeyboardModifier::ControlModifier) {
        if (e->angleDelta().y()*mScrollAngleY<0)
            mScrollAngleY = 0;
        mScrollAngleY += e->angleDelta().y();
        if (mScrollAngleY>=120) {
            setRatio(mRatio - 0.05);
            while (mScrollAngleY >= 120)
                mScrollAngleY -= 120;
        } else if (mScrollAngleY <= -120) {
            setRatio(mRatio + 0.05);
            while (mScrollAngleY <= -120)
                mScrollAngleY += 120;
        }
        e->accept();
        return;
    }

    if (std::abs(e->angleDelta().y())>=std::abs(e->angleDelta().x())) {
        if (e->angleDelta().y()*mScrollAngleY<0)
            mScrollAngleY = 0;
        mScrollAngleY += e->angleDelta().y();
        if (mScrollAngleY>=120) {
            if (verticalScrollBar()->value()==verticalScrollBar()->minimum())
                emit requestPrevImage(true);
            else
                verticalScrollBar()->setValue(verticalScrollBar()->value()-verticalScrollBar()->singleStep());
            mScrollAngleY %= 120;
        } else if (mScrollAngleY <= -120) {
            if (verticalScrollBar()->value()==verticalScrollBar()->maximum())
                emit requestNextImage();
            else
                verticalScrollBar()->setValue(verticalScrollBar()->value()+verticalScrollBar()->singleStep());
            mScrollAngleY = - ((-mScrollAngleY) % 120);
        }
        e->accept();
    } else {
        if (e->angleDelta().x()*mScrollAngleX<0)
            mScrollAngleX = 0;
        mScrollAngleX += e->angleDelta().y();
        if (mScrollAngleX>=120) {
            if (!mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->minimum())
                emit requestPrevImage(false);
            else if (mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->maximum())
                emit requestNextImage();
            else
                horizontalScrollBar()->setValue(horizontalScrollBar()->value()-horizontalScrollBar()->singleStep());
            mScrollAngleX %= 120;
        } else if (mScrollAngleX <= -120) {
            if (!mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->maximum())
                emit requestNextImage();
            else if (mSwapLeftRightWhenTurnPage && horizontalScrollBar()->value()==horizontalScrollBar()->minimum())
                emit requestPrevImage(false);
            else
                horizontalScrollBar()->setValue(horizontalScrollBar()->value()+horizontalScrollBar()->singleStep());
            mScrollAngleX = - ((-mScrollAngleX) % 120);
        }
        e->accept();
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mMovingImage) {
        scrollImageByMouseMove(event);
        event->accept();
    }
}

void ImageWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (mFitType==AutoFitType::None)
            mWorkingFitType = AutoFitType::Page;
        else
            mWorkingFitType = mFitType;
        loadImage();
        event->accept();
    }
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mOldMousePos=event->pos();
        mMovingImage = true;
        setCursor(Qt::CursorShape::SizeAllCursor);
        event->accept();
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        scrollImageByMouseMove(event);
        mMovingImage = false;
        setCursor(Qt::CursorShape::ArrowCursor);
        event->accept();
    }
}

const QColor &ImageWidget::background() const
{
    return mBackground;
}

void ImageWidget::setBackground(const QColor &newBackground)
{
    if (mBackground!=newBackground) {
        mBackground = newBackground;
        viewport()->update();
    }
}
