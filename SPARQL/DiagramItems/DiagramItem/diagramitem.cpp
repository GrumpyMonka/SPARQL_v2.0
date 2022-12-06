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

void DiagramItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    scene()->clearSelection();
    setSelected( true );
    my_context_menu->exec( event->screenPos() );
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

QList<DiagramArrow*> DiagramItem::getArrows()
{
    return arrows;
}

#include <diagramitematom.h>
#include <diagramitembased.h>
#include <diagramitemcomposite.h>
#include <diagramitemsparql.h>

bool DiagramItem::CheckItemOnDiagramItem( const qint64 code )
{
    if ( DiagramItem::BasedItemType == code
        //    || DiagramItemComposite  == code
        //    || DiagramItemSparql  == code
        || DiagramItem::AtomItemType == code )
    {
        return true;
    }
    return false;
}

DiagramItem* DiagramItem::FactoryDiagramItem( QMenu* context_menu,
    DiagramItemSettings* settings, QGraphicsItem* parent )
{
    switch ( settings->typeSettings() )
    {
    case DiagramItemSettings::BasedItemSettingsType:
        return new DiagramItemBased( context_menu, parent,
            new BasedBlockSettings( *static_cast<BasedBlockSettings*>( settings ) ) );
        break;
    case DiagramItemSettings::AtomItemSettingsType:
        return new DiagramItemAtom( context_menu, parent,
            new AtomBlockSettings( *static_cast<AtomBlockSettings*>( settings ) ) );
    default:
        break;
    }
    return nullptr;
}

QList<DiagramArrow*> DiagramItem::getStartArrows()
{
    QList<DiagramArrow*> result;
    auto arrows = getArrows();
    for ( auto arrow : arrows )
    {
        if ( this == arrow->startItem() )
            result.push_back( arrow );
    }
    return result;
}

QList<DiagramArrow*> DiagramItem::getEndArrows()
{
    QList<DiagramArrow*> result;
    auto arrows = getArrows();
    for ( auto arrow : arrows )
    {
        if ( this == arrow->endItem() )
            result.push_back( arrow );
    }
    return result;
}
