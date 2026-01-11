#include "ImageTool.h"

ImageTool::ImageTool(QObject *parent)
    : QObject(parent)
    , m_scale(1.0)
{
}

void ImageTool::setImage(const QImage &image) {
    m_image = image;
}

void ImageTool::setScale(qreal scale) {
    m_scale = qMax(0.1, scale);
}
