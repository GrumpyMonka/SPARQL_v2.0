#ifndef SGRAPHICSVIEW_H
#define SGRAPHICSVIEW_H

#include <diagramscene.h>

#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>

class SGraphicsView : public QWidget
{
    Q_OBJECT
public:
    explicit SGraphicsView( QWidget* parent = nullptr );
    void setDiagramScene( DiagramScene* );
    void createSidePanel();

    DiagramScene* getScene();

public slots:
    void setItemForScene( DiagramItemSettings* );
    void setSceneMode( int mode );

protected:
    virtual QWidget* addCustomWidget() = 0;
    virtual QWidget* addCustomBotWidget() = 0;

private:
    void createDiagramView();

    DiagramScene* createDiagramScene();

private slots:
    void updatePosData();
    void updatePosItems();

signals:

private:
    DiagramScene* diagram_scene;
    QGraphicsView* graphics_view;
    QGroupBox* group_box;
    QGridLayout* grid_layout;
    QVBoxLayout* group_box_layout;
    QSpinBox* x_spin_box;
    QSpinBox* y_spin_box;
    QSlider* x_slider;
    QSlider* y_slider;
    int heigth_scene = 5000;
    int width_scene = 5000;
};

#endif // SGRAPHICSVIEW_H
