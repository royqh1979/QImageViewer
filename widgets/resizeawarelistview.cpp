#include "resizeawarelistview.h"

#include <QResizeEvent>
#include <QTimer>

ResizeawareListView::ResizeawareListView(QWidget *parent):
    QListView(parent)
{
    mResizeTimer = new QTimer(this);
    mResizeTimer->setSingleShot(true);
    mResizeTimer->setInterval(1000);

    connect(mResizeTimer, &QTimer::timeout,
            this, &ResizeawareListView::onResizeTimerTimeout);
}

void ResizeawareListView::onResizeTimerTimeout() {
    emit resized();
}

void ResizeawareListView::resizeEvent(QResizeEvent *event)
{
    //QListView::resizeEvent(event);
    mResizeTimer->start();
}
