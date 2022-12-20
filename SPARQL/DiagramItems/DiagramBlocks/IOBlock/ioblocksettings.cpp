#include "ioblocksettings.h"

#include <QJsonArray>
#include <QJsonObject>

IOBlockSettings::IOBlockSettings()
{
}

void IOBlockSettings::setSettingFromJson( const QJsonObject& object )
{
    if ( object["Header"]["Type"].toString() == "IO" )
    {
        QJsonValue header = object["Header"];
        QJsonValue body = object["Body"];

        block_name = header["Name"].toString();
        type_block = ConvertTypeBlock.key( header["Type_Block"].toString() );

        text = body["Text"].toString();
        pos = pointFromJsonObject( body["Pos"] );
        support_add_item = body["Support_Add_Item"].toBool();
        polygon = polygonFromJsonArray( body["Polygon"].toArray() );
    }
}

QJsonObject IOBlockSettings::getJsonFromSetting()
{
    QJsonObject object;

    QJsonObject header;
    QJsonObject body;

    header.insert( "Type", "IO" );
    header.insert( "Name", block_name );
    header.insert( "Type_Block", ConvertTypeBlock[type_block] );

    body.insert( "Text", text );
    body.insert( "Pos", jsonFromPointF( pos ) );
    body.insert( "Support_Add_Item", support_add_item );
    body.insert( "Polygon", jsonArrayFromPolygon( polygon ) );

    object.insert( "Header", header );
    object.insert( "Body", body );

    return object;
}

QPixmap IOBlockSettings::image() const
{
    return {};
}

void IOBlockSettings::setType( TypeIO type )
{
    polygon.clear();
    if ( Input == type )
    {
        polygon << QPointF( -80, -30 )
                << QPointF( 80, -30 )
                << QPointF( 100, 0 )
                << QPointF( 80, 30 )
                << QPointF( -80, 30 )
                << QPointF( -60, 0 )
                << QPointF( -80, -30 );
    }
    else
    {
        polygon << QPointF( -80, -30 )
                << QPointF( 80, -30 )
                << QPointF( 100, 0 )
                << QPointF( 80, 30 )
                << QPointF( -80, 30 )
                << QPointF( -60, 0 )
                << QPointF( -80, -30 );
    }
    type_block = type;
}

QVector<DiagramItemSettings*> IOBlockSettings::GetBasedIOBlocks()
{
    QVector<DiagramItemSettings*> block_list;
    IOBlockSettings* settings = new IOBlockSettings();
    settings->setType( IOBlockSettings::Input );
    settings->block_name = "(IO)Input";
    block_list.push_back( settings );

    settings = new IOBlockSettings();
    settings->setType( IOBlockSettings::Output );
    settings->block_name = "(IO)Output";
    block_list.push_back( settings );
    return block_list;
}
