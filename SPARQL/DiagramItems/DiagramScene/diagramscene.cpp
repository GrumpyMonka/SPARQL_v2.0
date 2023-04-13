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
    DiagramArrow* arrow;
    // arrow->setColor(myLineColor);
    if ( nullptr == startItem->parentItem()
        || nullptr == endItem->parentItem()
        || startItem->parentItem() != endItem->parentItem() )
    {
        arrow = new DiagramArrow( startItem, endItem );
        addItem( arrow );
    }
    else
    {
        arrow = new DiagramArrow( startItem, endItem, startItem->parentItem() );
    }
    startItem->addArrow( arrow );
    endItem->addArrow( arrow );
    arrow->setZValue( qMin( startItem->zValue(), endItem->zValue() ) - 1 );
    arrow->updatePosition();
    return arrow;
}

void DiagramScene::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if ( mouseEvent->button() == Qt::LeftButton )
    {
        DiagramItem* item;
        auto items_for_add = items( mouseEvent->scenePos() );
        bool flag_add = false;
        switch ( scene_mode )
        {
        case InsertItem:
            for ( auto& parent_item : items_for_add )
            {
                if ( DiagramItem::CheckItemOnDiagramItem( parent_item->type() )
                    && static_cast<DiagramItem*>( parent_item )->getSupportAddItem() )
                {
                    item = DiagramItem::FactoryDiagramItem( my_context_menu, item_for_insert, parent_item );
                    flag_add = true;
                    item->setPos( mouseEvent->scenePos() - parent_item->pos() );
                    break;
                }
            }
            if ( !flag_add )
            {
                item = DiagramItem::FactoryDiagramItem( my_context_menu, item_for_insert );
                addItem( item );
                if ( DiagramItem::CompositeItemType == item->type() )
                {
                    auto arrows = item->getArrows();
                    for ( auto& arrow : arrows )
                    {
                        addItem( arrow );
                        arrow->updatePosition();
                    }
                };
                item->setPos( mouseEvent->scenePos() );
            }
            setMode( MoveItem );
            // emit itemSelected( item );
            break;
        case InsertArrow:
            line_for_arrow = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(),
                mouseEvent->scenePos() ) );
            line_for_arrow->setPen( QPen( Qt::black, 2 ) );
            line_for_arrow->setZValue( 1000 );
            addItem( line_for_arrow );
            break;
        case MoveItem:
            if ( resize_item != nullptr )
            {
                setMode( Resize );
            }
            break;
        default:;
        }
    }
    else if ( mouseEvent->button() == Qt::RightButton )
    {
        line_for_arrow = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(),
            mouseEvent->scenePos() ) );
        line_for_arrow->setPen( QPen( Qt::black, 2 ) );
        line_for_arrow->setZValue( 1000 );
        addItem( line_for_arrow );
        setMode( InsertArrow );
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
    else if ( MoveItem == scene_mode )
    {
        QApplication::restoreOverrideCursor();
        resize_item = nullptr;
        auto selected_items = items();
        for ( auto item : selected_items )
        {
            if ( DiagramItem::CheckItemOnDiagramItem( item->type() ) )
            {

                auto diagram_item = static_cast<DiagramItem*>( item );
                if ( diagram_item->getAllowResize() )
                {
                    auto right_diff = mouseEvent->scenePos().x() - ( diagram_item->pos().x() + diagram_item->getEndPos().x() * diagram_item->scale() );
                    auto bottom_diff = mouseEvent->scenePos().y() - ( diagram_item->pos().y() + diagram_item->getEndPos().y() * diagram_item->scale() );
                    auto offset = 5;
                    if ( qAbs( right_diff ) <= offset )
                    {
                        if ( qAbs( bottom_diff ) <= offset )
                        {
                            QApplication::setOverrideCursor( Qt::SizeFDiagCursor );
                            resize_item = diagram_item;
                        }
                    }
                }
            }
        }
        auto blocks = items();
        for ( auto block : blocks )
        {
            if ( DiagramItem::CheckItemOnDiagramItem( block->type() ) )
            {
                static_cast<DiagramItem*>( block )->setPen( QPen( Qt::black, 1 ) );
                static_cast<DiagramItem*>( block )->setColorArrows( Qt::black );
            }
        }

        blocks = items( mouseEvent->scenePos() );
        if ( !blocks.empty() )
        {
            for ( auto block : blocks )
            {
                if ( DiagramItem::CheckItemOnDiagramItem( block->type() ) )
                {
                    auto dep_items = static_cast<DiagramItem*>( block )->getDependecies();
                    for ( auto pair : dep_items )
                    {
                        pair.first->setPen( QPen( Qt::red, 5 ) );
                        pair.first->setColorArrowToItem( Qt::red, pair.second );
                    }
                }
            }
        }

        QGraphicsScene::mouseMoveEvent( mouseEvent );
    }
    else if ( scene_mode == Resize )
    {
        double diff_x = ( mouseEvent->scenePos().x() - ( resize_item->getStartPos().x() * resize_item->scale() + resize_item->pos().x() ) ) / ( resize_item->boundingRect().width() );
        resize_item->setScale( diff_x );
    }
}

QGraphicsItem* DiagramScene::getParentItem( QGraphicsItem* item )
{
    auto temp = item;
    while ( true )
    {
        auto parent = temp->parentItem();
        if ( nullptr != parent
            && DiagramItem::CheckItemOnDiagramItem( parent->type() )
            && !static_cast<DiagramItem*>( parent )->getAllowLineToChild()
            && !static_cast<DiagramItem*>( parent )->getSupportAddItem() )
        {
            temp = parent;
        }
        else
        {
            break;
        }
    }
    return temp;
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
            auto start = startItems.first();
            auto end = endItems.first();

            start = getParentItem( start );
            end = getParentItem( end );

            if ( DiagramItem::CheckItemOnDiagramItem( start->type() )
                && DiagramItem::CheckItemOnDiagramItem( end->type() ) )
            {
                if ( start == end && nullptr != start )
                {
                    static_cast<DiagramItem*>( start )->contextMenuShow( mouseEvent );
                }
                else
                {
                    createArrow( static_cast<DiagramItem*>( start ),
                        static_cast<DiagramItem*>( end ) );
                }
            }
        }
    }
    line_for_arrow = 0;
    setMode( MoveItem );
    resize_item = nullptr;
    QGraphicsScene::mouseReleaseEvent( mouseEvent );
}

void DiagramScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    QGraphicsScene::mouseDoubleClickEvent( mouseEvent );
}

void DiagramScene::removeSelectedItems()
{
    auto selected_items_list = selectedItems();
    for ( auto item : selected_items_list )
    {
        removeItemSmart( item );
    }
}

void DiagramScene::removeItemSmart( QGraphicsItem* item )
{
    removeItem( item );
    delete item;
}
