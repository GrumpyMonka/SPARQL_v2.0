#include "projectwindowsettings.h"

#include <QJsonArray>
#include <QJsonObject>

#include <basedblocksettings.h>
#include <sparqlblocksettings.h>

ProjectWindowSettings::ProjectWindowSettings()
{
    block_name = "Project";
}

ProjectWindowSettings::~ProjectWindowSettings()
{
    clear();
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

void ProjectWindowSettings::setSettingFromJson( const QJsonObject& object )
{
    if ( object["Header"]["Type"].toString() == "Project" )
    {
        QJsonObject header = object["Header"].toObject();
        QJsonObject body = object["Body"].toObject();

        QJsonArray blocks_array = body["Blocks"].toArray();
        QJsonArray lines_array = body["Lines"].toArray();

        for ( const QJsonValue& line : lines_array )
        {
            lines_list.push_back( { line["Start"].toInt(), line["End"].toInt(), line["Text"].toString() } );
        }

        for ( const QJsonValue& block : blocks_array )
        {
            auto type_block = block["Header"]["Type"].toString();
            if ( "Based" == type_block )
            {
                BasedBlockSettings* settings = new BasedBlockSettings();
                settings->setSettingFromJson( block.toObject() );
                blocks_list.push_back( settings );
            }
            else if ( "Sparql" == type_block )
            {
                SparqlBlockSettings* settings = new SparqlBlockSettings();
                settings->setSettingFromJson( block.toObject() );
                blocks_list.push_back( settings );
            }
        }
    }
}

QJsonObject ProjectWindowSettings::getJsonFromSetting()
{
    QJsonObject json_object;

    QJsonObject header;
    QJsonObject body;

    header.insert( "Type", "Project" );
    header.insert( "Name", block_name );

    body.insert( "Lines", getJsonArrayFromLineSaver( lines_list ) );

    QJsonArray blocks_array;
    for ( const auto& block : blocks_list )
    {
        blocks_array.push_back( block->getJsonFromSetting() );
    }
    body.insert( "Blocks", blocks_array );

    json_object.insert( "Header", header );
    json_object.insert( "Body", body );

    return json_object;
}

void ProjectWindowSettings::clear()
{
    for ( auto& block : blocks_list )
    {
        if ( block != nullptr )
            delete block;
    }
    blocks_list.clear();
}
