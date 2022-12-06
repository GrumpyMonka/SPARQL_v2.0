#include "ioblocksettings.h"

#include <QJsonObject>

IOBlockSettings::IOBlockSettings()
{
}

void IOBlockSettings::setSettingFromJson( const QJsonObject& object )
{
}

QJsonObject IOBlockSettings::getJsonFromSetting()
{
    return {};
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
                << QPointF( 60, 0 )
                << QPointF( 80, 30 )
                << QPointF( -80, 30 )
                << QPointF( -100, 0 )
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
