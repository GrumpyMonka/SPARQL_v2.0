#include "compositeblocksettings.h"

#include <QJsonArray>
#include <QJsonObject>

CompositeBlockSettings::CompositeBlockSettings()
{
}

void CompositeBlockSettings::setSettingFromJson( const QJsonObject& object )
{
    if ( object["Header"]["Type"].toString() == "Composite" )
    {
        QJsonValue header = object["Header"];
        QJsonValue body = object["Body"];

        block_name = header["Name"].toString();

        pos = pointFromJsonObject( body["Pos"] );

        QJsonArray blocks_array = body["Blocks"].toArray();
        for ( const auto& block : blocks_array )
        {
            blocks.push_back( getDiagramSettingsFromJson( block.toObject() ) );
        }

        QJsonArray lines_array = body["Lines"].toArray();
        for ( const QJsonValue& line : lines_array )
        {
            lines.push_back( { line["Start"].toInt(), line["End"].toInt(), line["Text"].toString() } );
        }

        QJsonArray input_array = body["Input"].toArray();
        for ( const auto& input : input_array )
        {
            input_names.push_back( input.toString() );
        }

        QJsonArray output_array = body["Output"].toArray();
        for ( const auto& output : output_array )
        {
            output_names.push_back( output.toString() );
        }
    }
}

QJsonArray CompositeBlockSettings::getJsonArrayFromLineSaver( const QVector<LineSaver>& lines )
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

QJsonObject CompositeBlockSettings::getJsonFromSetting()
{
    QJsonObject json_object;

    QJsonObject header;
    QJsonObject body;

    header.insert( "Type", "Composite" );
    header.insert( "Name", block_name );

    body.insert( "Lines", getJsonArrayFromLineSaver( lines ) );

    QJsonArray blocks_array;
    for ( const auto& block : blocks )
    {
        blocks_array.push_back( block->getJsonFromSetting() );
    }
    body.insert( "Blocks", blocks_array );

    QJsonArray input_array;
    for ( const auto& input : input_names )
    {
        input_array.push_back( input );
    }
    body.insert( "Input", input_array );

    QJsonArray output_array;
    for ( const auto& output : output_names )
    {
        output_array.push_back( output );
    }
    body.insert( "Output", input_array );

    json_object.insert( "Header", header );
    json_object.insert( "Body", body );

    return json_object;
}

QPixmap CompositeBlockSettings::image() const
{
    return {};
}
