#include "terrainwidget.h"
#include "ui_terrainwidget.h"

TerrainWidget::TerrainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerrainWidget)
{
    ui->setupUi(this);
}

TerrainWidget::~TerrainWidget()
{
    delete ui;
}
