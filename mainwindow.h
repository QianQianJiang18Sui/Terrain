#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initUI();
private:
    Ui::MainWindow *ui;

    QDockWidget*     m_widget_property = nullptr;
    QDockWidget*     m_widget_tools = nullptr;
};
#endif // MAINWINDOW_H
