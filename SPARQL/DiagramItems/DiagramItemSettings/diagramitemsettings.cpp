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

QString DiagramItemSettings::getNameType()
{
    switch ( typeSettings() )
    {
    case DiagramItemSettingsType:
        return "Settings";
    case BasedItemSettingsType:
        return "Basic";
    case CompositeItemSettingsType:
        return "Composite";
    case SparqlItemSettinsType:
        return "SPARQL";
    case AtomItemSettingsType:
        return "Atom";
    case IOItemSettingsType:
        return "IO";
    case ProjectWindowSettingsType:
        return "Project";
    default:
        return "ERROR";
    }
}

QPixmap DiagramItemSettings::pixmapFrom( const QJsonValue& value )
{
    auto const encoded = value.toString().toLatin1();
    QPixmap p;
    p.loadFromData( QByteArray::fromBase64( encoded ), "PNG" );
    return p;
}

void DiagramItemSettings::setSettingFromString( const QString& str )
{
    QJsonDocument json = QJsonDocument::fromJson( str.toUtf8() );
    if ( json.isObject() )
    {
        setSettingFromJson( json.object() );
    }
}

QJsonArray DiagramItemSettings::jsonArrayFromPolygon( const QPolygonF& polygon )
{
    QJsonArray array_polygon;
    for ( const auto& point : polygon )
    {
        array_polygon.push_back( jsonFromPointF( point ) );
    }
    return array_polygon;
}

QPolygonF DiagramItemSettings::polygonFromJsonArray( const QJsonArray& array )
{
    QPolygonF polygon;
    for ( const QJsonValue& next_item : array )
    {
        polygon << pointFromJsonObject( next_item );
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

QJsonObject DiagramItemSettings::jsonFromPointF( const QPointF& point )
{
    QJsonObject object;
    object.insert( "X", point.x() );
    object.insert( "Y", point.y() );
    return object;
}

QPointF DiagramItemSettings::pointFromJsonObject( const QJsonValue& value )
{
    return QPointF( value["X"].toDouble(), value["Y"].toDouble() );
}

#include <basedblocksettings.h>
#include <compositeblocksettings.h>
#include <ioblocksettings.h>
#include <sparqlblocksettings.h>

DiagramItemSettings* DiagramItemSettings::getDiagramSettingsFromJson( const QJsonObject& object )
{
    auto type_block = object["Header"]["Type"].toString();
    if ( "Based" == type_block )
    {
        BasedBlockSettings* settings = new BasedBlockSettings();
        settings->setSettingFromJson( object );
        return settings;
    }
    else if ( "Sparql" == type_block )
    {
        SparqlBlockSettings* settings = new SparqlBlockSettings();
        settings->setSettingFromJson( object );
        return settings;
    }
    else if ( "Composite" == type_block )
    {
        CompositeBlockSettings* settings = new CompositeBlockSettings();
        settings->setSettingFromJson( object );
        return settings;
    }
    else if ( "IO" == type_block )
    {
        IOBlockSettings* settings = new IOBlockSettings();
        settings->setSettingFromJson( object );
        return settings;
    }
    return nullptr;
}
