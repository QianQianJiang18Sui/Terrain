#ifndef TERRAINWIDGET_H
#define TERRAINWIDGET_H

#include <QWidget>

namespace Ui {
class TerrainWidget;
}

class TerrainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TerrainWidget(QWidget *parent = nullptr);
    ~TerrainWidget();

private:
    Ui::TerrainWidget *ui;
};

#endif // TERRAINWIDGET_H
