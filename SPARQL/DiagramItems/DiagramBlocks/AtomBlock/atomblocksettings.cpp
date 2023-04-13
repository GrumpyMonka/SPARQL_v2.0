#include "atomblocksettings.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>
#include <QtMath>

AtomBlockSettings::AtomBlockSettings()
{
    int n = 32;
    double Pi = 3.1415926535;
    double step = 2 * Pi / n;
    int razmer = 50;
    int radius = 20;

    for ( int i = 0; i < n / 4; i++ )
    {
        polygon << QPointF( radius * cos( i * step ) + razmer - radius,
            radius * sin( i * step ) + razmer - radius );
    }
    polygon << QPointF( -( razmer - radius ), razmer );

    for ( int i = n / 4; i < 2 * n / 4; i++ )
    {
        polygon << QPointF( radius * cos( i * step ) - ( razmer - radius ),
            radius * sin( i * step ) + razmer - radius );
    }
    polygon << QPointF( -razmer, -( razmer - radius ) );

    for ( int i = 2 * n / 4; i < 3 * n / 4; i++ )
    {
        polygon << QPointF( radius * cos( i * step ) - ( razmer - radius ),
            radius * sin( i * step ) - ( razmer - radius ) );
    }
    polygon << QPointF( razmer - radius, -razmer );

    for ( int i = 3 * n / 4; i < 4 * n / 4; i++ )
    {
        polygon << QPointF( radius * cos( i * step ) + razmer - radius,
            radius * sin( i * step ) - ( razmer - radius ) );
    }
    polygon << QPointF( razmer, razmer - radius );

    block_name = "AtomBlock";
    flag_text = true;
    text = "";
    color = Qt::black;
    color_text = "blue";
    transparent_background = false;
    pixmap = image();
    support_add_item = false;
    z_value = 2;
}

void AtomBlockSettings::setSettingFromJson( const QJsonObject& object )
{
    if ( object["Header"]["Type"].toString() == "Atom" )
    {
        QJsonValue header = object["Header"];
        QJsonValue body = object["Body"];

        block_name = header["Name"].toString();
        type_block = header["Type_Block"].toString();

        flag_text = body["Flag_Text"].toBool();
        text = body["Text"].toString();
        color_text = body["Color_Text"].toString();
        transparent_background = body["Transparent_Background"].toBool();
        polygon = polygonFromJsonArray( body["Polygon"].toArray() );
        pos = pointFromJsonObject( body["Pos"] );
        support_add_item = body["Support_Add_Item"].toBool();
    }
}

QJsonObject AtomBlockSettings::getJsonFromSetting()
{
    QJsonObject object;

    QJsonObject header;
    QJsonObject body;

    header.insert( "Type", QJsonValue( "Atom" ) );
    header.insert( "Name", QJsonValue( block_name ) );
    header.insert( "Type_Block", QJsonValue( type_block ) );

    body.insert( "Flag_Text", QJsonValue( flag_text ) );
    body.insert( "Text", QJsonValue( text ) );
    body.insert( "Color_Text", QJsonValue( color_text ) );
    body.insert( "Transparent_Background", QJsonValue( transparent_background ) );
    body.insert( "Polygon", jsonArrayFromPolygon( polygon ) );
    body.insert( "Pos", jsonFromPointF( pos ) );
    body.insert( "Support_Add_Item", support_add_item );

    object.insert( "Header", header );
    object.insert( "Body", body );

    return object;
}

QPolygonF AtomBlockSettings::GetDefaultAreaPolygon()
{
    return QPolygonF() << QPointF( -400, 250 )
                       << QPointF( 400, 250 )
                       << QPointF( 400, -250 )
                       << QPointF( -400, -250 )

                       << QPointF( -400, 285 )
                       << QPointF( 400, 285 )
                       << QPointF( 400, 250 );
}

AtomBlockSettings* AtomBlockSettings::GetAreaAtomBlock()
{
    auto settings = new AtomBlockSettings();
    settings->type_block = DEFAULT_AREA;
    settings->block_name = DEFAULT_AREA;
    settings->flag_text = false;
    settings->polygon.clear();
    settings->transparent_background = true;
    settings->polygon = GetDefaultAreaPolygon();
    settings->pixmap = settings->image();
    settings->transparent_background = false;
    settings->support_add_item = true;
    settings->support_resize = true;
    settings->z_value = 0;
    return settings;
}

AtomBlockSettings* AtomBlockSettings::GetVarAtomBlock()
{
    auto settings = new AtomBlockSettings();
    settings->type_block = DEFAULT_VAR;
    settings->block_name = DEFAULT_VAR;
    settings->color_text = "red";
    settings->polygon.clear();
    settings->polygon << QPointF( -50, -50 )
                      << QPointF( 50, -50 )
                      << QPointF( 50, 50 )
                      << QPointF( -50, 50 )
                      << QPointF( -50, -50 );
    settings->pixmap = settings->image();
    return settings;
}

AtomBlockSettings* AtomBlockSettings::GetValueAtomBlock()
{
    auto settings = new AtomBlockSettings();
    settings->type_block = DEFAULT_VALUE;
    settings->block_name = DEFAULT_VALUE;
    settings->color_text = "blue";
    settings->pixmap = settings->image();
    return settings;
}

QVector<DiagramItemSettings*> AtomBlockSettings::GetBasedAtomBlocks()
{
    QVector<DiagramItemSettings*> list;
    list.push_back( GetVarAtomBlock() );
    list.push_back( GetValueAtomBlock() );
    list.push_back( GetAreaAtomBlock() );
    return list;
}

QPixmap AtomBlockSettings::image() const
{
    // if ( setting_->type_image == "self" )
    //{
    QPixmap pixmap( 150, 150 );
    pixmap.fill( Qt::transparent );
    QPainter painter( &pixmap );
    painter.setPen( QPen( Qt::black, 8 ) );
    painter.translate( 75, 75 );
    painter.drawPolyline( polygon );

    painter.setPen( QPen( Qt::black, 4 ) );
    // int index = 150 / setting_->block_name.size() * 0.75;
    // painter.setFont( QFont( "Consolas", index ) );
    // painter.drawText( -75, -75, 150, 150, Qt::AlignCenter, setting_->block_name );
    return pixmap;
}
