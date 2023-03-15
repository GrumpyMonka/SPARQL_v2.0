#ifndef COMPOSITEBLOCKWINDOW_H
#define COMPOSITEBLOCKWINDOW_H

#include <sgraphicsview.h>

#include <compositeblocksettings.h>

class CompositeBlockWindow : public SGraphicsView
{
public:
    SGViewTypes typeSGView() override { return CompositeBlockWindowType; }
    int modeDiagramBlocks() override { return BlocksLibrary::ModeBlocks::Based
        | BlocksLibrary::ModeBlocks::Composite
        | BlocksLibrary::ModeBlocks::SPARQL
        | BlocksLibrary::ModeBlocks::IO; }

    explicit CompositeBlockWindow( QMenu* context_menu, QWidget* parent = nullptr );

    CompositeBlockSettings* getSettings();
    void setSettings( CompositeBlockSettings* settings = new CompositeBlockSettings() );
    QVector<DiagramItem*> getDiagramItems();
    QVector<DiagramArrow*> getDiagramArrows();

public slots:
    void slotOnCreateButtonClicked() override;
    void slotOnSaveButtonClicked() override;
    void slotOnOpenButtonClicked() override;

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;
    QLineEdit* line_name_block;
};

#endif // COMPOSITEBLOCKWINDOW_H
