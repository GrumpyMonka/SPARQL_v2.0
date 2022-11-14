#include "sparqlblocksettings.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QUrl>

SparqlBlockSettings::SparqlBlockSettings()
{
    block_name = "SparqlBlock";
    pixmap = image();
}

void SparqlBlockSettings::setSettingFromJson( const QJsonValue& value )
{
    if ( value.isObject() )
    {
        block_name = value["name"].toString();
        // type_image = value["type_img"].toString();
        pixmap = pixmapFrom( value["image"] );
        limit = value["limit"].toInt();

        blocks.clear();
        auto json_blocks = value["blocks"].toArray();
        for ( const QJsonValue& block : json_blocks )
        {
            blocks.push_back( { block["text"].toString(),
                QPointF( block["pos_x"].toDouble(), block["pos_y"].toDouble() ), "", block["type"].toString() } );
        }

        lines.clear();
        auto json_lines = value["lines"].toArray();
        for ( const QJsonValue& line : json_lines )
        {
            lines.push_back( { line["start_block"].toInt(), line["end_block"].toInt(), line["text"].toString() } );
        }

        areas.clear();
        auto json_areas = value["areas"].toArray();
        for ( const QJsonValue& area : json_areas )
        {
            areas.push_back( { polygonFromJsonArray( area["polygon"].toArray() ), QPointF( area["pos_x"].toDouble(), area["pox_y"].toDouble() ), area["name"].toString() } );
        }
    }
}

QJsonObject SparqlBlockSettings::getJsonFromSetting()
{
    QJsonObject json_object;

    QJsonObject data;
    data.insert( "name", QJsonValue( block_name ) );
    data.insert( "type_img", QJsonValue( "" ) );
    data.insert( "image", jsonValFromPixmap( pixmap ) );
    data.insert( "limit", QJsonValue( limit ) );

    QJsonArray array_blocks;
    for ( const auto& next_block : blocks )
    {
        QJsonObject temp_obj;
        temp_obj.insert( "text", QJsonValue( next_block.text ) );
        temp_obj.insert( "pos_x", QJsonValue( next_block.pos.x() ) );
        temp_obj.insert( "pos_y", QJsonValue( next_block.pos.y() ) );
        temp_obj.insert( "type", QJsonValue( next_block.type ) );
        array_blocks.push_back( temp_obj );
    }
    data.insert( "blocks", array_blocks );

    QJsonArray array_lines;
    for ( const auto& next_line : lines )
    {
        QJsonObject temp_obj;
        temp_obj.insert( "text", QJsonValue( next_line.text ) );
        temp_obj.insert( "start_block", QJsonValue( next_line.startBlock ) );
        temp_obj.insert( "end_block", QJsonValue( next_line.endBlock ) );
        array_lines.push_back( temp_obj );
    }
    data.insert( "lines", array_lines );

    QJsonArray array_areas;
    for ( const auto& next_area : areas )
    {
        QJsonObject temp_obj;
        temp_obj.insert( "name", QJsonValue( next_area.name ) );
        temp_obj.insert( "polygon", jsonArrayFromPolygon( next_area.polygon ) );
        temp_obj.insert( "pos_x", next_area.pos.x() );
        temp_obj.insert( "pos_y", next_area.pos.y() );
        array_areas.push_back( temp_obj );
    }
    data.insert( "areas", array_areas );

    json_object.insert( "type", QJsonValue( "sparql" ) );
    json_object.insert( "data", QJsonValue( data ) );
    return json_object;
}

BasedBlockSettings* SparqlBlockSettings::ConvertToBasedBlockSetting( SparqlBlockSettings* settings )
{
    QString script = "var xmlHttp = new XMLHttpRequest(network);\n"
                     "xmlHttp.setUrl(\"http://localhost:3030/nuclear/query\");\n"
                     "xmlHttp.open(\"POST\", \"/\");\n"
                     "xmlHttp.setRequestHeader(\"Connection\", \"keep-alive\");\n"
                     "xmlHttp.setRequestHeader(\"Accept\", \"application/sparql-results+json\");\n"
                     "var answer = xmlHttp.send(";

    QString request = "PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n"
                      "PREFIX owl: <http://www.w3.org/2002/07/owl#>\n"
                      "PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>\n"
                      "PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>\n"
                      "PREFIX : <http://www.semanticweb.org/SEARCH/ontologies/2021/4/OICA_project#>\n\n"
                      "SELECT *";

    QMap<QString, QString> area_body;
    for ( auto line : settings->lines )
    {
        QString path = settings->blocks[line.startBlock].path;
        QString str = settings->blocks[line.startBlock].text;
        str += " " + line.text;
        str += " " + settings->blocks[line.endBlock].text;
        str += ".\n";
        area_body[path] += str;
    }

    QString body;
    if ( area_body.end() != area_body.find( "ORIGIN" ) )
    {
        body += "{\n" + area_body["ORIGIN"] + "}";
    }
    area_body.remove( "ORIGIN" );

    auto keys = area_body.keys();
    for ( const auto& key : keys )
    {
        body += key + "{\n" + area_body[key] + "}";
    }

    request += "\nWHERE\n"
               "{\n";
    request += "  " + body;
    request += "} LIMIT " + QString::number( settings->limit );

    BasedBlockSettings* setting = new BasedBlockSettings();
    setting->label = false;
    setting->line_edit = false;
    setting->block_name = "SPARQL";
    setting->script = script + "\"query=" + QUrl::toPercentEncoding( request ) + "\");\ny.push(answer);";

    return setting;
}

QPixmap SparqlBlockSettings::image() const
{
    return {};
}

SparqlBlockSettings* SparqlBlockSettings::CreateTemplateSparqlSettings()
{
    auto settings = new SparqlBlockSettings();
    settings->areas.push_back( { AtomBlockSettings::GetDefaultAreaPolygon(), QPointF( 2500, 2500 ), "ORIGIN" } );
    return settings;
}
