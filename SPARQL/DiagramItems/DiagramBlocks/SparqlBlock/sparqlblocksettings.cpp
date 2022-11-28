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

SparqlBlockSettings::~SparqlBlockSettings()
{
    clear();
}

void SparqlBlockSettings::setSettingFromJson( const QJsonValue& value )
{
    if ( value.isObject() )
    {
        areas.clear();
        lines.clear();

        QJsonValue header = value["Header"];
        QJsonValue body = value["Body"];

        block_name = header["Name"].toString();
        // type_image = value["type_img"].toString();
        pixmap = pixmapFrom( body["Image"] );
        limit = body["Limit"].toInt();
        start_area = body["Start_Area"].toInt();

        QJsonArray areas_array = body["Areas"].toArray();
        for ( const QJsonValue& area : areas_array )
        {
            auto blocks_array = area["Blocks"].toArray();
            auto lines_array = area["Lines"].toArray();
            auto settings = area["Settings"];

            AreaSaver area_saver;
            for ( const QJsonValue& block : blocks_array )
            {
                AtomBlockSettings* setting = new AtomBlockSettings();
                setting->setSettingFromJson( block );
                area_saver.blocks.push_back( setting );
            }

            for ( const QJsonValue& line : lines_array )
            {
                area_saver.lines.push_back( { line["Start"].toInt(), line["End"].toInt(), line["Text"].toString() } );
            }

            area_saver.settings = new AtomBlockSettings();
            area_saver.settings->setSettingFromJson( settings );
            areas.push_back( area_saver );
        }
    }
}

QJsonArray SparqlBlockSettings::getJsonArrayFromLineSaver( const QVector<LineSaver>& lines )
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

QJsonObject SparqlBlockSettings::getJsonFromSetting()
{
    QJsonObject json_object;

    QJsonObject header;
    QJsonObject body;

    header.insert( "Type", "Sparql" );
    header.insert( "Name", block_name );

    body.insert( "Limit", limit );
    body.insert( "Start_Area", start_area );

    body.insert( "Lines", getJsonArrayFromLineSaver( lines ) );

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

    QString body = "";
    QMap<int, QString> stack = { { settings->start_area, "" } };
    while ( !stack.empty() )
    {
        int index = stack.keys().at( 0 );
        QString name = stack[index];
        AreaSaver area = settings->areas.at( index );
        stack.remove( index );

        QString area_str = name + "\n{\n";
        for ( const auto& line : area.lines )
        {
            QString str = area.blocks.at( line.start_block )->text;
            str += " " + line.text;
            str += " " + area.blocks.at( line.end_block )->text;
            str += ".\n";
            area_str += str;
        }
        area_str += "}\n";
        body += area_str;

        for ( const auto& line : settings->lines )
        {
            if ( line.start_block == index )
            {
                stack.insert( line.end_block, line.text );
            }
        }
    }

    request += "\nWHERE\n"
               "{\n";
    request += "  " + body;
    request += "} LIMIT " + QString::number( settings->limit );

    BasedBlockSettings* setting = new BasedBlockSettings();
    setting->label = false;
    setting->line_edit = false;
    setting->block_name = settings->block_name;
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
    AreaSaver area_saver;
    area_saver.settings = AtomBlockSettings::GetAreaAtomBlock();
    area_saver.settings->pos = QPointF( 2500, 2500 );
    settings->areas.push_back( area_saver );
    return settings;
}

void SparqlBlockSettings::clear()
{
    /*for ( auto& area : areas )
    {
        delete areas.first().settings;
        for ( auto& block : area.blocks )
        {
            delete block;
        }
        area.blocks.clear();
    }
    areas.clear();*/
}
