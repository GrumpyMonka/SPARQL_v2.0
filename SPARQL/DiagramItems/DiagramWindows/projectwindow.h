#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <sgraphicsview.h>

class ProjectWindow : public SGraphicsView
{
    Q_OBJECT
public:
    SGViewTypes typeSGView() override { return ProjectWindowType; }
    int modeDiagramBlocks() override { return BlocksLibrary::ModeBlocks::Based
        | BlocksLibrary::ModeBlocks::Composite
        | BlocksLibrary::ModeBlocks::SPARQL; }

    explicit ProjectWindow( QMenu* context_menu, QWidget* parent = nullptr );

    QVector<DiagramItem*> getDiagramItems();
    QVector<DiagramArrow*> getDiagramArrows();
    void saveProject() override;
    void openProject() override;

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;
signals:

private:
    QMenu* context_menu;
};

#endif // PROJECTWINDOW_H
