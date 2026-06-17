#ifndef RESIZE_AWARE_LIST_H
#define RESIZE_AWARE_LIST_H
#include <QListView>

class ResizeawareListView : public QListView {
    Q_OBJECT
public:
    explicit ResizeawareListView(QWidget *parent = nullptr);
signals:
    void resized(QSize oldSize, QSize newSize);

    // QAbstractItemView interface
public:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // RESIZE_AWARE_LIST_H
