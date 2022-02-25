#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "propertywidget.h"
#include "terrainwidget.h"
#include "terrain_widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    //centeral widget
    auto viewer = new WidgetTerrain();
    this->setCentralWidget(viewer);
    //right
    if (!m_widget_property)
    {
        m_widget_property = new QDockWidget(tr("Property"), this);
        m_widget_property->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
        m_widget_property->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        m_widget_property->setFixedSize(250, 244);
        auto property = new PropertyWidget();
        m_widget_property->setWidget(property);
        this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_widget_property);
    }

    //bottom
    if (!m_widget_tools)
    {
        m_widget_tools = new QDockWidget(tr("Property"), this);
        m_widget_tools->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
        m_widget_tools->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);
        m_widget_tools->setMinimumSize(800, 200);
        auto tools = new TerrainWidget();
        m_widget_tools->setWidget(tools);
        this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_widget_tools);
    }
    setDockNestingEnabled(true);

}

