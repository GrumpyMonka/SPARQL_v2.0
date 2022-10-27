#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

#include <QGraphicsSceneMouseEvent>

#include <diagramitem.h>
#include <diagramitemsettings.h>

class DiagramScene : public QGraphicsScene
{
public:
    enum SceneMode
    {
        InsertItem,
        InsertArrow,
        MoveItem
    };

    explicit DiagramScene( QObject* parent = nullptr );

    void setContextMenu( QMenu* context_menu );
    QMenu* getContextMenu();

public slots:
    void setMode( SceneMode );
    void setDiagramItemForInserted( DiagramItemSettings* settings );
    DiagramArrow* createArrow( DiagramItem*, DiagramItem* );

signals:
    void itemSelected( DiagramItem* );

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;

private:
    SceneMode scene_mode;
    DiagramItemSettings* item_for_insert;
    QMenu* my_context_menu;
    QGraphicsLineItem* line_for_arrow;
};

#endif // DIAGRAMSCENE_H
