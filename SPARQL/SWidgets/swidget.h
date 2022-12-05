#ifndef SWIDGET_H
#define SWIDGET_H

#include <QWidget>

#include <blockslibrary.h>

class SWidget : public QWidget
{
    Q_OBJECT
public:
    enum SWidgetTypes
    {
        SWidgetType,
        SGraphicsViewType,
        BasedBlockWindowType
    };
    virtual SWidgetTypes typeSWidget() { return SWidgetType; }
    virtual int modeDiagramBlocks() { return BlocksLibrary::ModeBlocks::None; }
    virtual void saveProject() {}
    virtual void openProject() {}

    explicit SWidget( QWidget* parent = nullptr );
};

#endif // SWIDGET_H
