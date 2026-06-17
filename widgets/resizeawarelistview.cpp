#include "resizeawarelistview.h"

#include <QResizeEvent>


ResizeawareListView::ResizeawareListView(QWidget *parent):
    QListView(parent)
{

}

void ResizeawareListView::resizeEvent(QResizeEvent *event)
{
    emit resized(event->oldSize(), event->size());
}
