#include "projectwindowsettings.h"

#include <QJsonArray>
#include <QJsonObject>

ProjectWindowSettings::ProjectWindowSettings()
{
    block_name = "Project";
}

QJsonArray ProjectWindowSettings::getJsonArrayFromLineSaver( const QVector<LineSaver>& lines )
{
    QJsonArray lines_array;
    for ( const auto& line : lines )
    {
        QJsonObject line_obj;
        line_obj.insert( "Text", line.text );
        line_obj.insert( "Start", line.start_block );
        line_obj.insert( "End", line.end_block );
        lines_array.push_back( line_obj );
    }
    return lines_array;
}

void ProjectWindowSettings::setSettingFromJson( const QJsonValue& value )
{
    QJsonObject header;
    QJsonObject body;

    header.insert( "Type", "Project" );
    header.insert( "Name", block_name );

    body.insert( "Lines", getJsonArrayFromLineSaver( lines_list ) );

    QJsonArray areas_array;
    for ( const auto& area : areas )
    {
        QJsonObject area_obj;
        area_obj.insert( "Settings", area.settings->getJsonFromSetting() );
        area_obj.insert( "Lines", getJsonArrayFromLineSaver( area.lines ) );
        QJsonArray blocks_array;
        for ( const auto& block : area.blocks )
        {
            blocks_array.push_back( block->getJsonFromSetting() );
        }
        area_obj.insert( "Blocks", blocks_array );
        areas_array.push_back( area_obj );
    }
    body.insert( "Areas", areas_array );

    json_object.insert( "Header", header );
    json_object.insert( "Body", body );
}

QJsonObject ProjectWindowSettings::getJsonFromSetting()
{
    return {};
}
