#ifndef SWIDGET_H
#define SWIDGET_H

#include <QWidget>

class SWidget : public QWidget
{
    Q_OBJECT
public:
    enum
    {
        SWidgetType = 0,
        SGraphicsViewType = 2,
    };
    virtual int type() { return SWidgetType; }

    explicit SWidget( QWidget* parent = nullptr );
};

#endif // SWIDGET_H
