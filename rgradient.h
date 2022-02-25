/*!
 *  @brief	   �Զ��彥��ɫ����
 *  @details   ��CustomPlot����ֲ�������ڼ��㽥��ɫ����
 *  @author    wey
 *  @version   1.0
 *  @date      2021-11-16 15:03:04
 *  @warning
 *  @copyright NanJing RenGu.
 */
#pragma once

#include <QColor>
#include <QVector>
#include <QMap>
#include <QObject>


class RGradient
{

public:
    enum GradientPreset {
        gpGrayscale
        , gpHot
        , gpCold
        , gpNight
        , gpCandy
        , gpGeography
        , gpIon
        , gpThermal
        , gpPolar
        , gpSpectrum
        , gpJet
        , gpHues
    };
    Q_ENUMS(GradientPreset)

    RGradient();
    RGradient(GradientPreset preset);

    ~RGradient();

    //[1]
    void setColorStopAt(double position, const QColor &color);
    QMap<double, QColor> getColorStops() { return mColorStops; }

    void loadPreset(GradientPreset preset);

    //[2]
    void createColorBuffer();

    //[3]
    QColor getColor(double percent);

private:
    enum ColorInterpolation {
        ciRGB  ///< Color channels red, green and blue are linearly interpolated
        , ciHSV ///< Color channels hue, saturation and value are linearly interpolated (The hue is interpolated over the shortest angle distance)
    };

private:
    void clearColorStops();

    void setColorInterpolation(ColorInterpolation interpolation);

private:
    QVector<QRgb> mColorBuffer;		/*!< ϸ����ɫ�� */
    QMap<double, QColor> mColorStops;		/*!< ����ɫ���� */
    int mLevelCount;				/*!< ��ɫ�����ֵĲ㼶 */

    bool mColorBufferInvalidated;
    ColorInterpolation mColorInterpolation;
};

