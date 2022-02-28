#include "Material.h"
#include <QDebug>

QOpenGLTexture* genTexture(const QString &text, int textPixelSize, QColor textColor, QFont font)
{
    auto size = calImageSize(text, textPixelSize, font);

    QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    //img是图片，模式为QImage::Format_ARGB32_Premultiplied是因为按官方的说法，这个模式快。
    QImage img(size.width(), size.height(), QImage::Format_ARGB32_Premultiplied);
    img.fill(QColor(0, 0, 0, 0));	//填充为透明
    QPainter painter;
    painter.begin(&img);
    font.setPixelSize(textPixelSize);
    painter.setFont(font);

    QPen pen;
    pen.setColor(textColor);
    painter.setPen(pen);
    QTextOption option(Qt::AlignLeft | Qt::AlignTop);//这里使文字绘制时自动换行
    option.setWrapMode(QTextOption::WordWrap);
    QRectF rect(0, 0, size.width(), size.height());
    painter.drawText(rect, text, option);//绘制到图片
    painter.end();

    //创建为纹理。
    img = img.mirrored(false, true);
    texture->setData(img);
    //    texture->setMipLevelRange(0, mipLevelMax);//off mipmap 没必要，开了影响效果
    texture->setMinificationFilter(QOpenGLTexture::Linear);//线性滤波方式，文字看上去柔和
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}

QSize calImageSize(const QString &text, int textPixelSize, QFont font)
{
    QImage img(10, 10, QImage::Format_ARGB32_Premultiplied);
    QPainter painter;
    painter.begin(&img);
    font.setPixelSize(textPixelSize);
    painter.setFont(font);

    QFontMetricsF fm = painter.fontMetrics();
    auto width = fm.width(text);
    auto height = fm.height();
    return QSize(width, height);
}
