#include "atomblocksettings.h"

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
    transparent = false;
    pixmap = image();
}

void AtomBlockSettings::setSettingFromJson( const QJsonValue& value )
{
}

QJsonObject AtomBlockSettings::getJsonFromSetting()
{
    return {};
}

QPolygonF AtomBlockSettings::GetDefaultAreaPolygon()
{
    return QPolygonF() << QPointF( -300, 200 )
                       << QPointF( 300, 200 )
                       << QPointF( 300, -200 )
                       << QPointF( -300, -200 )
                       << QPointF( -300, 200 );
}

QVector<DiagramItemSettings*> AtomBlockSettings::GetBasedAtomBlocks()
{
    QVector<DiagramItemSettings*> list;
    auto setting = new AtomBlockSettings();
    setting->type_block = DEFAULT_VAR;
    setting->color_text = "red";
    setting->polygon.clear();
    setting->polygon << QPointF( -50, -50 )
                     << QPointF( 50, -50 )
                     << QPointF( 50, 50 )
                     << QPointF( -50, 50 )
                     << QPointF( -50, -50 );
    setting->pixmap = setting->image();
    list.push_back( setting );

    setting = new AtomBlockSettings();
    setting->type_block = DEFAULT_VALUE;
    setting->color_text = "blue";
    setting->pixmap = setting->image();
    list.push_back( setting );

    setting = new AtomBlockSettings();
    setting->type_block = DEFAULT_AREA;
    setting->flag_text = false;
    setting->polygon.clear();
    setting->transparent = true;
    setting->polygon = GetDefaultAreaPolygon();
    setting->pixmap = setting->image();
    setting->transparent = true;
    list.push_back( setting );

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
