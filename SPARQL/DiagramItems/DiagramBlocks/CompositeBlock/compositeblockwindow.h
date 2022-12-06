#ifndef COMPOSITEBLOCKWINDOW_H
#define COMPOSITEBLOCKWINDOW_H

#include <sgraphicsview.h>

class CompositeBlockWindow : public SGraphicsView
{
public:
    SGViewTypes typeSGView() override { return CompositeBlockWindowType; }
    int modeDiagramBlocks() override { return BlocksLibrary::ModeBlocks::Based
        | BlocksLibrary::ModeBlocks::Composite
        | BlocksLibrary::ModeBlocks::SPARQL
        | BlocksLibrary::ModeBlocks::IO; }

    explicit CompositeBlockWindow( QWidget* parent = nullptr );

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;
};

#endif // COMPOSITEBLOCKWINDOW_H
