#include "ImageLabel.h"

ImageLabel::ImageLabel(QWidget *parent) : QLabel{parent} {}

void ImageLabel::setPixmap(const QPixmap &pixmap) {
    pixmap_ = pixmap;

    // this->setAlignment(Qt::AlignCenter);
    auto size = this->size();
    this->QLabel::setPixmap(
            this->pixmap_.scaled(size, Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
}

void ImageLabel::resizeEvent(QResizeEvent *ev) {
    auto size = ev->size();
    this->QLabel::setPixmap(
            this->pixmap_.scaled(size, Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));

    qDebug() << size << this->pixmap().size();
}

void ImageLabel::mouseDoubleClickEvent(QMouseEvent *ev) {
    Q_UNUSED(ev);
    qDebug() << "on double click ";
    emit this->OnDoubleClicked();
}
