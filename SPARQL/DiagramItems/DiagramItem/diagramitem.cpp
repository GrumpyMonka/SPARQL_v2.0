#include "diagramitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QtMath>

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
    int index = arrows.indexOf( arrow );

    if ( index != -1 )
        arrows.removeAt( index );
}

void DiagramItem::removeArrows()
{
    foreach ( DiagramArrow* arrow, arrows )
    {
        // arrow->startItem()->removeArrow( arrow );
        // arrow->endItem()->removeArrow( arrow );
        scene()->removeItem( arrow );
        delete arrow;
    }
}

void DiagramItem::addArrow( DiagramArrow* arrow )
{
    arrows.append( arrow );
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap( 200, 200 );
    pixmap.fill( Qt::transparent );
    QPainter painter( &pixmap );
    painter.setPen( QPen( Qt::black, 8 ) );
    painter.translate( 100, 100 );
    painter.drawPolyline( my_polygon );

    return pixmap;
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
    if ( DiagramItemBased::Type == code )
    //    || DiagramItemComposite::Type == code
    //    || DiagramItemSparql::Type == code
    //    || DiagramAtom::Type == code)
    {
        return true;
    }
    return false;
}

DiagramItem* DiagramItem::FactoryDiagramItem( QMenu* context_menu,
    DiagramItemSettings* settings, QGraphicsItem* parent )
{
    switch ( settings->type() )
    {
    case BasedBlockSettings::Type:
        return new DiagramItemBased( context_menu,
            static_cast<BasedBlockSettings*>( settings ), parent );
        break;
    case AtomBlockSettings::Type:
        return new DiagramItemAtom( context_menu,
            static_cast<AtomBlockSettings*>( settings ), parent );
    default:
        break;
    }
    return nullptr;
}
