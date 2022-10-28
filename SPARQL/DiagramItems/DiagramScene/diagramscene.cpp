#include "diagramscene.h"

DiagramScene::DiagramScene( QObject* parent )
    : QGraphicsScene( parent )
{
    scene_mode = MoveItem;
    line_for_arrow = 0;
}

void DiagramScene::setContextMenu( QMenu* context_menu )
{
    my_context_menu = context_menu;
}

QMenu* DiagramScene::getContextMenu()
{
    return my_context_menu;
}

void DiagramScene::setMode( SceneMode mode )
{
    scene_mode = mode;
}

void DiagramScene::setDiagramItemForInserted( DiagramItemSettings* settings )
{
    item_for_insert = settings;
}

DiagramArrow* DiagramScene::createArrow( DiagramItem* startItem, DiagramItem* endItem )
{
    DiagramArrow* arrow = new DiagramArrow( startItem, endItem );
    // arrow->setColor(myLineColor);
    startItem->addArrow( arrow );
    endItem->addArrow( arrow );
    arrow->setZValue( -1000.0 );
    addItem( arrow );
    arrow->updatePosition();
    return arrow;
}

void DiagramScene::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if ( mouseEvent->button() != Qt::LeftButton )
        return;

    DiagramItem* item;
    switch ( scene_mode )
    {
    case InsertItem:
        item = DiagramItem::FactoryDiagramItem( my_context_menu, item_for_insert );
        addItem( item );
        item->setPos( mouseEvent->scenePos() );
        setMode( MoveItem );
        // emit itemSelected( item );
        break;
    case InsertArrow:
        line_for_arrow = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(),
            mouseEvent->scenePos() ) );
        line_for_arrow->setPen( QPen( Qt::black, 2 ) );
        addItem( line_for_arrow );
        break;
    case MoveItem:
        break;
    default:;
    }

    /*
        foreach ( QGraphicsItem* temp, this->selectedItems() )
        {
            if ( DiagramItem::CheckItemOnDiagramItem( temp->type() ) )
            {
                // emit itemSelected( static_cast<DiagramItem*>( temp ) );
                break;
            }
        }
    */
    QGraphicsScene::mousePressEvent( mouseEvent );
}

void DiagramScene::mouseMoveEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if ( InsertArrow == scene_mode && 0 != line_for_arrow )
    {
        QLineF new_line( line_for_arrow->line().p1(), mouseEvent->scenePos() );
        line_for_arrow->setLine( new_line );
    }
    else if ( MoveItem == scene_mode && !this->selectedItems().empty() )
    {
        QGraphicsScene::mouseMoveEvent( mouseEvent );
    }
    /*
    foreach ( QGraphicsItem* temp, this->selectedItems() )
    {
        if ( DiagramItem::CheckItemOnDiagramItem( temp->type() ) )
        {
            emit itemSelected( static_cast<DiagramItem*>( temp ) );
            break;
        }
    }*/
}

void DiagramScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if ( 0 != line_for_arrow && InsertArrow == scene_mode )
    {
        QList<QGraphicsItem*> startItems = items( line_for_arrow->line().p1() );
        if ( startItems.count() && startItems.first() == line_for_arrow )
            startItems.removeFirst();
        QList<QGraphicsItem*> endItems = items( line_for_arrow->line().p2() );
        if ( endItems.count() && endItems.first() == line_for_arrow )
            endItems.removeFirst();

        removeItem( line_for_arrow );
        delete line_for_arrow;

        if ( !startItems.empty() && !endItems.empty() )
        {
            QGraphicsItem* temp = endItems.first();
            while ( endItems.first()->parentItem() != nullptr )
            {
                if ( DiagramItem::CheckItemOnDiagramItem( endItems.first()->parentItem()->type() ) )
                {
                    temp = endItems.first()->parentItem();
                }
                endItems.first() = endItems.first()->parentItem();
            }
            endItems.first() = temp;

            temp = startItems.first();
            while ( startItems.first()->parentItem() != nullptr )
            {
                if ( DiagramItem::CheckItemOnDiagramItem( startItems.first()->parentItem()->type() ) )
                {
                    temp = startItems.first()->parentItem();
                }
                startItems.first() = startItems.first()->parentItem();
            }
            startItems.first() = temp;

            if ( DiagramItem::CheckItemOnDiagramItem( startItems.first()->type() )
                && DiagramItem::CheckItemOnDiagramItem( endItems.first()->type() )
                && startItems.first() != endItems.first() )
            {
                createArrow( static_cast<DiagramItem*>( startItems.first() ),
                    static_cast<DiagramItem*>( endItems.first() ) );
            }
        }
    }
    line_for_arrow = 0;
    QGraphicsScene::mouseReleaseEvent( mouseEvent );
}

void DiagramScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
}
