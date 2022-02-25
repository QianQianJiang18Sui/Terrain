#include "Material.h"
#include <QOpenGLTexture>
#include <QPainter>
#include <QFont>
#include <QTextOption>
#include <QPen>

QOpenGLTexture* genTexture(int width, int height, const QString &text, int textPixelSize, const QColor &textColor)
{
    QOpenGLTexture *texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    //img是图片，模式为QImage::Format_ARGB32_Premultiplied是因为按官方的说法，这个模式快。
    QImage img(width, height, QImage::Format_ARGB32_Premultiplied);
    img.fill(QColor(0, 0, 0, 0));	//填充为透明
    QPainter painter;
    QFont font;
    painter.begin(&img);
    font.setPixelSize(textPixelSize);
    painter.setFont(font);
    QPen pen;
    pen.setColor(textColor);
    painter.setPen(pen);
    QTextOption option(Qt::AlignLeft | Qt::AlignTop);//这里使文字绘制时自动换行
    option.setWrapMode(QTextOption::WordWrap);
    QRectF rect(0, 0, width, height);
    painter.drawText(rect, text, option);//绘制到图片
    painter.end();

    //创建为纹理。
    texture->setData(img);
    //    texture->setMipLevelRange(0, mipLevelMax);//off mipmap 没必要，开了影响效果
    texture->setMinificationFilter(QOpenGLTexture::Linear);//线性滤波方式，文字看上去柔和
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}
