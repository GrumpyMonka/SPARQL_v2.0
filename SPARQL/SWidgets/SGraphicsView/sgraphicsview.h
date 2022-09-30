#ifndef SGRAPHICSVIEW_H
#define SGRAPHICSVIEW_H

#include <diagramscene.h>

#include <QGraphicsView>
#include <QGroupBox>
#include <QGridLayout>
#include <QSpinBox>
#include <QSlider>

class SGraphicsView : public QWidget
{
    Q_OBJECT
public:
    explicit SGraphicsView( QWidget *parent = nullptr );
    void SetDiagramScene( DiagramScene* );

    DiagramScene* GetScene();

private:
    void CreateDiagramView();
    void CreatePositionManager();

    DiagramScene* CreateDiagramScene();

signals:

private:
    DiagramScene* diagramScene;
    QGraphicsView* graphicsView;
    QGroupBox* groupBox;
    QGridLayout* gridLayout;
    QSpinBox* xSpinBox;
    QSpinBox* ySpinBox;
    QSlider* xSlider;
    QSlider* ySlider;
};

#endif // SGRAPHICSVIEW_H
