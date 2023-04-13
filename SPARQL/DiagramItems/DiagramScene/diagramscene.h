#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

#include <QGraphicsSceneMouseEvent>

#include <diagramarrow.h>
#include <diagramitem.h>
#include <diagramitemsettings.h>

class DiagramScene : public QGraphicsScene
{
public:
    enum SceneMode
    {
        None,
        InsertItem,
        InsertArrow,
        MoveItem,
        Resize
    };

    explicit DiagramScene( QObject* parent = nullptr );

    void setContextMenu( QMenu* context_menu );
    QMenu* getContextMenu();
    QGraphicsItem* getParentItem( QGraphicsItem* );

public slots:
    void setMode( SceneMode );
    void setDiagramItemForInserted( DiagramItemSettings* settings );
    DiagramArrow* createArrow( DiagramItem*, DiagramItem* );
    void removeSelectedItems();
    void removeItemSmart( QGraphicsItem* item );

signals:
    void itemSelected( DiagramItem* );

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent ) override;

private:
    SceneMode scene_mode;
    DiagramItem* resize_item;
    DiagramItemSettings* item_for_insert;
    QMenu* my_context_menu;
    QGraphicsLineItem* line_for_arrow;
};

#endif // DIAGRAMSCENE_H
