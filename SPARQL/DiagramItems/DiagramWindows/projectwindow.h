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

    explicit ProjectWindow( QWidget* parent = nullptr );

    QVector<DiagramItem*> getDiagramItems();
    QVector<DiagramArrow*> getDiagramArrows();
    void saveProject() override;
    void openProject() override;

private:
    QWidget* addCustomWidget() override;
    QWidget* addCustomBotWidget() override;
signals:
    void ERROR( QString );
};

#endif // PROJECTWINDOW_H
