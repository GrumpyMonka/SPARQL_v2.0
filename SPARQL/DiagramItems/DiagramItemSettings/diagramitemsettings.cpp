#include "diagramitemsettings.h"

#include <QBuffer>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>

DiagramItemSettings::DiagramItemSettings()
{
}

QJsonValue DiagramItemSettings::jsonValFromPixmap( const QPixmap& p )
{
    QBuffer buffer;
    buffer.open( QIODevice::WriteOnly );
    p.save( &buffer, "PNG" );
    auto const encoded = buffer.data().toBase64();
    return { QLatin1String( encoded ) };
}

QPixmap DiagramItemSettings::pixmapFrom( const QJsonValue& val )
{
    auto const encoded = val.toString().toLatin1();
    QPixmap p;
    p.loadFromData( QByteArray::fromBase64( encoded ), "PNG" );
    return p;
}

void DiagramItemSettings::setSettingFromString( const QString& str )
{
    QJsonDocument json = QJsonDocument::fromJson( str.toUtf8() );
    if ( json.isObject() )
    {
        setSettingFromJson( json["data"] );
    }
}

QJsonArray DiagramItemSettings::jsonArrayFromPolygon( const QPolygonF& polygon )
{
    QJsonArray array_polygon;
    for ( const auto& point : polygon )
    {
        QJsonObject temp_object;
        temp_object.insert( "x", point.x() );
        temp_object.insert( "y", point.y() );
        array_polygon.push_back( temp_object );
    }
    return array_polygon;
}

QPolygonF DiagramItemSettings::polygonFromJsonArray( const QJsonArray& array )
{
    QPolygonF polygon;
    for ( const QJsonValue& next_item : array )
    {
        polygon << QPointF( next_item["x"].toDouble(), next_item["y"].toDouble() );
    }
    return polygon;
}

QPixmap DiagramItemSettings::image() const
{
    QPixmap pixmap( 200, 200 );
    pixmap.fill( Qt::transparent );
    QPainter painter( &pixmap );
    painter.setPen( QPen( Qt::black, 8 ) );
    painter.translate( 100, 100 );
    painter.drawPolyline(
        QPolygonF() << QPointF( -50, -50 )
                    << QPointF( 50, -50 )
                    << QPointF( 50, 50 )
                    << QPointF( -50, 50 )
                    << QPointF( -50, -50 ) );

    return pixmap;
}
