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

void ResizeawareListView::keyPressEvent(QKeyEvent *event)
{
    if (hasFocus() &&
            (event->key() == Qt::Key_Enter
             || event->key() == Qt::Key_Return)
            && event->modifiers() == Qt::NoModifier)  {
        emit enterPressed();
        event->accept();
        return;
    }
    QListView::keyPressEvent(event);
}
