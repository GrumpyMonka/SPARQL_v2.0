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
    void SetDiagramScene( DiagramScene* );
    void CreateSidePanel();

    DiagramScene* GetScene();

protected:
    virtual QWidget* AddCustomWidget() = 0;
    virtual QWidget* AddCustomBotWidget() = 0;

private:
    void CreateDiagramView();

    DiagramScene* CreateDiagramScene();

private slots:
    void updatePosData();
    void updatePosItems();

signals:

private:
    DiagramScene* diagramScene;
    QGraphicsView* graphicsView;
    QGroupBox* groupBox;
    QGridLayout* gridLayout;
    QVBoxLayout* groupBoxLayout;
    QSpinBox* xSpinBox;
    QSpinBox* ySpinBox;
    QSlider* xSlider;
    QSlider* ySlider;
    int heigthScene = 5000;
    int widthScene = 5000;
};

#endif // SGRAPHICSVIEW_H
