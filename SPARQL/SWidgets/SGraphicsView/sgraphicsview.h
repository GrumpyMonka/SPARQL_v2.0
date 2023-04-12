#ifndef SGRAPHICSVIEW_H
#define SGRAPHICSVIEW_H

#include <diagramscene.h>

#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>

#include <swidget.h>

class SGraphicsView : public SWidget
{
    Q_OBJECT
public:
    enum SGViewTypes
    {
        SGViewType,
        ProjectWindowType,
        CompositeBlockWindowType,
        SparqlBlockWindowType
    };
    virtual SGViewTypes typeSGView() { return SGViewType; }
    SWidgetTypes typeSWidget() override { return SGraphicsViewType; }
    virtual int modeDiagramBlocks() override { return BlocksLibrary::ModeBlocks::None; }

    explicit SGraphicsView( QMenu* context_menu, QWidget* parent = nullptr );
    void setDiagramScene( DiagramScene* );
    void createSidePanel();
    void removeSelectedGraphicsItems();
    QWidget* getWidgetOnGraphicsView();

    DiagramScene* getScene();

public slots:
    void setItemForScene( DiagramItemSettings* );
    void setSceneMode( int mode );
    virtual void slotOnCreateButtonClicked();
    virtual void slotOnSaveButtonClicked();
    virtual void slotOnOpenButtonClicked();
    virtual void slotCustom();
    QString openFile();
    void saveFile( const QString& );

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
    void blockCreated( DiagramItemSettings* );
    void ERROR( QString );

private:
    QWidget* widget_on_view;
    DiagramScene* diagram_scene;
    QMenu* context_menu;
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
