#include "diagramitemsettings.h"

#include <QBuffer>
#include <QDebug>
#include <QJsonDocument>

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
