#ifndef COMPOSITEBLOCKWINDOW_H
#define COMPOSITEBLOCKWINDOW_H

#include <sgraphicsview.h>

class CompositeBlockWindow : public SGraphicsView
{
public:
    enum
    {
        DiagramMode = 7
    };

    explicit CompositeBlockWindow( QWidget* parent = nullptr );

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;
};

#endif // COMPOSITEBLOCKWINDOW_H
