#include "diagramarrow.h"

#include <QPainter>
#include <QPen>
#include <qmath.h>

DiagramArrow::DiagramArrow( DiagramItem* startItem, DiagramItem* endItem, QGraphicsItem* parent )
    : QGraphicsLineItem( parent )
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag( QGraphicsItem::ItemIsSelectable, true );
    myColor = Qt::black;
    setPen( QPen( myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );

    proxy_widget = new QGraphicsProxyWidget( this );
    line_edit = new QLineEdit();
    line_edit->setVisible( false );
    proxy_widget->setWidget( line_edit );
}

DiagramArrow::~DiagramArrow()
{
    startItem()->removeArrow( this );
    endItem()->removeArrow( this );
}

QRectF DiagramArrow::boundingRect() const
{
    qreal extra = ( pen().width() + 20 ) / 2.0;

    return QRectF( line().p1(), QSizeF( line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y() ) )
        .normalized()
        .adjusted( -extra, -extra, extra, extra );
}

QPainterPath DiagramArrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon( arrowHead );
    return path;
}

void DiagramArrow::updatePosition()
{
    QLineF line;
    if ( nullptr == parentItem() )
    {
        line = { myStartItem->scenePos(), myEndItem->scenePos() };
    }
    else
    {
        line = { myStartItem->pos(), myEndItem->pos() };
    }
    setLine( line );
}

void DiagramArrow::paint( QPainter* painter, const QStyleOptionGraphicsItem*,
    QWidget* )
{
    if ( myStartItem->collidesWithItem( myEndItem ) )
        return;

    QPointF start_item_pos;
    QPointF end_item_pos;
    if ( nullptr == parentItem() )
    {
        start_item_pos = myStartItem->scenePos();
        end_item_pos = myEndItem->scenePos();
    }
    else
    {
        start_item_pos = myStartItem->pos();
        end_item_pos = myEndItem->pos();
    }

    QPen myPen = pen();
    myPen.setColor( myColor );
    qreal arrowSize = 20;
    painter->setPen( myPen );
    painter->setBrush( myColor );

    QLineF centerLine( start_item_pos, end_item_pos );
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + end_item_pos;
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for ( int i = 1; i < endPolygon.count(); ++i )
    {
        p2 = endPolygon.at( i ) + end_item_pos;
        polyLine = QLineF( p1, p2 );
        QLineF::IntersectType intersectType = polyLine.intersect( centerLine, &intersectPoint );
        if ( intersectType == QLineF::BoundedIntersection )
            break;
        p1 = p2;
    }

    setLine( QLineF( intersectPoint, start_item_pos ) );

    double angle = std::atan2( -line().dy(), line().dx() );

    QPointF arrowP1 = line().p1() + QPointF( sin( angle + M_PI / 3 ) * arrowSize, cos( angle + M_PI / 3 ) * arrowSize );
    QPointF arrowP2 = line().p1() + QPointF( sin( angle + M_PI - M_PI / 3 ) * arrowSize, cos( angle + M_PI - M_PI / 3 ) * arrowSize );

    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;

    painter->drawLine( line() );
    painter->drawPolygon( arrowHead );
    if ( isSelected() )
    {
        painter->setPen( QPen( myColor, 1, Qt::DashLine ) );
        QLineF myLine = line();
        myLine.translate( 0, 4.0 );
        painter->drawLine( myLine );
        myLine.translate( 0, -8.0 );
        painter->drawLine( myLine );
    }

    auto rect = boundingRect();
    if ( line_edit->text().isEmpty() )
    {
        line_edit->setVisible( false );
    }
    line_edit->setGeometry( rect.x() + rect.width() / 2 - 40, rect.y() + rect.height() / 2 - 12, 80, 25 );
}

void DiagramArrow::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* mouseEvent )
{
    line_edit->setVisible( true );
    line_edit->setText( "->text<-" );
}

void DiagramArrow::setText( const QString& str )
{
    if ( !str.isEmpty() )
        line_edit->setVisible( true );
    line_edit->setText( str );
}

void DiagramArrow::setVisibleText( bool flag )
{
    line_edit->setVisible( flag );
}

QString DiagramArrow::getText()
{
    return line_edit->text();
}
