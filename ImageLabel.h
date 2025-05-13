#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPixmap>

class ImageLabel : public QLabel {
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = nullptr);

signals:
    void OnDoubleClicked();

public:
    void    setPixmap(const QPixmap &pixmap);
    QPixmap pixmap() { return pixmap_; }

private:
    void resizeEvent(QResizeEvent *ev) override;
    void mouseDoubleClickEvent(QMouseEvent *ev) override;

private:
    QPixmap pixmap_;
};

#endif // IMAGELABEL_H
