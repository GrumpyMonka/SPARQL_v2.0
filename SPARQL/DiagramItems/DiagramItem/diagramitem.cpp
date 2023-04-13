#include "diagramitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QtMath>

#include <diagramarrow.h>

DiagramItem::DiagramItem( QMenu* contextMenu, QGraphicsItem* parent )
    : QGraphicsPolygonItem( parent )
{
    my_context_menu = contextMenu;

    my_polygon << QPointF( -50, -50 ) << QPointF( 50, -50 )
               << QPointF( 50, 50 ) << QPointF( -50, 50 )
               << QPointF( -50, -50 );

    setItemPolygon( my_polygon );
    setFlag( QGraphicsItem::ItemIsMovable, true );
    setFlag( QGraphicsItem::ItemIsSelectable, true );
    setFlag( QGraphicsItem::ItemSendsGeometryChanges, true );
    setSupportAddItem( false );
    setAllowLineToChild( false );
}

DiagramItem::~DiagramItem()
{
    removeArrows();
}

void DiagramItem::setOutputText( const QString& output_text )
{
    auto arrows = getStartArrows();
    for ( auto arrow : arrows )
    {
        arrow->setText( output_text );
        // arrow->setVisibleText( false );
    }
}

DiagramItemSettings* DiagramItem::getSettings()
{
    return {};
}

void DiagramItem::setSupportAddItem( bool flag )
{
    support_add_item = flag;
}

bool DiagramItem::getSupportAddItem()
{
    return support_add_item;
}

void DiagramItem::setAllowLineToChild( bool flag )
{
    allow_line_to_child = flag;
}

bool DiagramItem::getAllowLineToChild()
{
    return allow_line_to_child;
}

void DiagramItem::setContextMenu( QMenu* contextMenu )
{
    my_context_menu = contextMenu;
}

QMenu* DiagramItem::getContextMenu()
{
    return my_context_menu;
}

void DiagramItem::setItemPolygon( QPolygonF& polygon )
{
    my_polygon = polygon;
    setPolygon( my_polygon );
}

QPointF DiagramItem::getStartPos()
{
    if ( my_polygon.size() == 0 )
        return QPointF();
    QPointF min = my_polygon[0];
    foreach ( QPointF temp, my_polygon )
    {
        if ( min.x() > temp.x() )
        {
            min.setX( temp.x() );
        }
        if ( min.y() > temp.y() )
        {
            min.setY( temp.y() );
        }
    }
    return min;
}

QPointF DiagramItem::getEndPos()
{
    if ( my_polygon.size() == 0 )
        return QPointF();
    QPointF max = my_polygon[0];
    foreach ( QPointF temp, my_polygon )
    {
        if ( max.x() < temp.x() )
        {
            max.setX( temp.x() );
        }
        if ( max.y() < temp.y() )
        {
            max.setY( temp.y() );
        }
    }
    return max;
}

void DiagramItem::removeArrow( DiagramArrow* arrow )
{
    auto index = arrows.indexOf( arrow );
    if ( -1 != index )
    {
        arrows.removeAt( index );
    }
}

void DiagramItem::removeArrows()
{
    foreach ( DiagramArrow* arrow, arrows )
    {
        delete arrow;
    }
}

void DiagramItem::addArrow( DiagramArrow* arrow )
{
    arrows.append( arrow );
}

void DiagramItem::setAllowResize( bool flag )
{
    allow_resize = flag;
}

bool DiagramItem::getAllowResize()
{
    return allow_resize;
}

void DiagramItem::mousePressEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    if ( mouseEvent->button() == Qt::LeftButton )
    {
        if ( mouseEvent->pos().x() >= getEndPos().x() - 5 )
        {
            QApplication::setOverrideCursor( Qt::SizeFDiagCursor );
        }
        else
        {
            QApplication::restoreOverrideCursor();
        }
    }
}

void DiagramItem::contextMenuShow( QGraphicsSceneMouseEvent* mouseEvent )
{
    scene()->clearSelection();
    setSelected( true );
    if ( nullptr != my_context_menu )
        my_context_menu->exec( mouseEvent->screenPos() );
}

QVariant DiagramItem::itemChange( GraphicsItemChange change, const QVariant& value )
{
    if ( change == QGraphicsItem::ItemPositionChange )
    {
        foreach ( DiagramArrow* arrow, arrows )
        {
            arrow->updatePosition();
        }
    }

    return value;
}

QVector<DiagramArrow*> DiagramItem::getArrows()
{
    return arrows;
}

QVector<QPair<DiagramItem*, DiagramItem*>> DiagramItem::getDependecies()
{
    /*if ( dependencies.empty() )
    {
        QVector<QPair<DiagramItem*, DiagramItem*>> data;
        auto arrows = getEndArrows();
        for ( auto arrow : arrows )
        {
            data.push_back( { arrow->startItem(), this } );
        }
        return data;
    }*/
    return dependencies;
}

void DiagramItem::setColorArrowToItem( const QColor& color, DiagramItem* item )
{
    auto arrows = getStartArrows();
    for ( auto arrow : arrows )
    {
        if ( arrow->endItem() == item )
        {
            arrow->setColor( color );
            arrow->updatePosition();
        }
    }
}

void DiagramItem::setColorArrows( const QColor& color )
{
    for ( auto arrow : arrows )
    {
        arrow->setColor( color );
        arrow->updatePosition();
    }
}

void DiagramItem::addDependecies( DiagramItem* start, DiagramItem* end )
{
    if ( nullptr != start && nullptr != end )
    {
        dependencies.push_back( { start, end } );
    }
}

void DiagramItem::clearDependecies()
{
    dependencies.clear();
}

void DiagramItem::setDependecies( QVector<QPair<DiagramItem*, DiagramItem*>> data )
{
    if ( data.empty() )
    {
        auto arrows = getEndArrows();
        for ( auto arrow : arrows )
        {
            data.push_back( { arrow->startItem(), this } );
        }
    }
    dependencies = data;
}

bool DiagramItem::CheckItemOnDiagramItem( const qint64 code )
{
    if ( DiagramItem::BasedItemType == code
        || DiagramItem::CompositeItemType == code
        || DiagramItem::SparqlItemType == code
        || DiagramItem::AtomItemType == code
        || DiagramItem::IOItemType == code )
    {
        return true;
    }
    return false;
}

#include <diagramitematom.h>
#include <diagramitembased.h>
#include <diagramitemcomposite.h>
#include <diagramitemio.h>
#include <diagramitemsparql.h>

DiagramItem* DiagramItem::FactoryDiagramItem( QMenu* context_menu,
    DiagramItemSettings* settings, QGraphicsItem* parent )
{
    switch ( settings->typeSettings() )
    {
    case DiagramItemSettings::BasedItemSettingsType:
        return new DiagramItemBased( context_menu, parent,
            new BasedBlockSettings( *static_cast<BasedBlockSettings*>( settings ) ) );
        break;
    case DiagramItemSettings::CompositeItemSettingsType:
        return new DiagramItemComposite( context_menu, parent,
            new CompositeBlockSettings( *static_cast<CompositeBlockSettings*>( settings ) ) );
        break;
    case DiagramItemSettings::SparqlItemSettinsType:
        return new DiagramItemSparql( context_menu, parent,
            new SparqlBlockSettings( *static_cast<SparqlBlockSettings*>( settings ) ) );
        break;
    case DiagramItemSettings::AtomItemSettingsType:
        return new DiagramItemAtom( context_menu, parent,
            new AtomBlockSettings( *static_cast<AtomBlockSettings*>( settings ) ) );
        break;
    case DiagramItemSettings::IOItemSettingsType:
        return new DiagramItemIO( context_menu, parent,
            new IOBlockSettings( *static_cast<IOBlockSettings*>( settings ) ) );
        break;

    default:
        break;
    }
    return nullptr;
}

QVector<DiagramArrow*> DiagramItem::getStartArrows()
{
    QVector<DiagramArrow*> result;
    auto arrows = getArrows();
    for ( auto arrow : arrows )
    {
        if ( this == arrow->startItem() )
            result.push_back( arrow );
    }
    return result;
}

QVector<DiagramArrow*> DiagramItem::getEndArrows()
{
    QVector<DiagramArrow*> result;
    auto arrows = getArrows();
    for ( auto arrow : arrows )
    {
        if ( this == arrow->endItem() )
            result.push_back( arrow );
    }
    return result;
}
