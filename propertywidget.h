#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>

namespace Ui {
class PropertyWidget;
}

class PropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyWidget(QWidget *parent = nullptr);
    ~PropertyWidget();

private:
    Ui::PropertyWidget *ui;
};

#endif // PROPERTYWIDGET_H
