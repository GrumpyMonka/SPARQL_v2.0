#include "diagramitem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QtMath>

DiagramItem::DiagramItem( QMenu* contextMenu, QGraphicsItem* parent )
    : QGraphicsPolygonItem( parent )
{
    myContextMenu = contextMenu;

    myPolygon << QPointF( -50, -50 ) << QPointF( 50, -50 )
              << QPointF( 50, 50 ) << QPointF( -50, 50 )
              << QPointF( -50, -50 );

    setPolygon( myPolygon );
    setFlag( QGraphicsItem::ItemIsMovable, true );
    setFlag( QGraphicsItem::ItemIsSelectable, true );
    setFlag( QGraphicsItem::ItemSendsGeometryChanges, true );
}

QPointF DiagramItem::getStartPos()
{
    if ( myPolygon.size() == 0 )
        return QPointF();
    QPointF min = myPolygon[0];
    foreach ( QPointF temp, myPolygon )
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
    if ( myPolygon.size() == 0 )
        return QPointF();
    QPointF max = myPolygon[0];
    foreach ( QPointF temp, myPolygon )
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
    painter.drawPolyline( myPolygon );

    return pixmap;
}

void DiagramItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    scene()->clearSelection();
    setSelected( true );
    myContextMenu->exec( event->screenPos() );
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
