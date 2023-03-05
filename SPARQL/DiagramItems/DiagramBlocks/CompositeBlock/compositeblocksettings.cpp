#include "compositeblocksettings.h"

#include <QJsonArray>
#include <QJsonObject>

#include <ioblocksettings.h>

CompositeBlockSettings::CompositeBlockSettings()
{
    pixmap = image();
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
            lines.push_back( { line["Start"].toInt(), line["End"].toInt(),
                line["Text_Start"].toString(), line["Text_End"].toString() } );
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
        line_obj.insert( "Text_Start", line.text_start );
        line_obj.insert( "Text_End", line.text_end );
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
    body.insert( "Pos", jsonFromPointF( pos ) );

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
    body.insert( "Output", output_array );

    json_object.insert( "Header", header );
    json_object.insert( "Body", body );

    return json_object;
}

QPixmap CompositeBlockSettings::image() const
{
    return QPixmap( ":/Sources/images/chip.png" );
}

QString CompositeBlockSettings::getScript()
{
    return {};
    QString script = "blocks_list_size = blocks_list.length;\n";

    QVector<DiagramItemSettings*> block_list;
    for ( auto& block : blocks )
    {
        if ( DiagramItemSettings::IOItemSettingsType == block->typeSettings() )
            continue;
        QString temp = "blocks_list.push( new Block( \n";
        temp += "\tfunction( x, index ) {\n";
        temp += "\t\tvar y = [];";

        if ( DiagramItemSettings::BasedItemSettingsType == block->typeSettings() )
        {
            auto based_block = static_cast<BasedBlockSettings*>( block );
            if ( based_block->line_edit_text != "" )
            {
                temp += "\t\tvar input = " + based_block->line_edit_text + ";";
            }
        }

        temp += block->getScript();

        temp += "\t\treturn y;\n";
        temp += "\t},\n";
        temp += "\t[ ],\n";

        temp += "[ ]));\n\n";
        block_list.push_back( block );
        script += temp;
    }

    for ( const auto& line : lines )
    {
        if ( DiagramItemSettings::IOItemSettingsType != blocks[line.end_block]->typeSettings() )
        {
            int index_end = block_list.indexOf( blocks[line.end_block] );
            QVector<int> index_start;
            if ( DiagramItemSettings::IOItemSettingsType == blocks[line.start_block]->typeSettings() )
            {
                script += "addInputDependBlockForComposite( \""
                    + ( static_cast<IOBlockSettings*>( blocks[line.start_block] ) )->text
                    + "\", blocks_list_size + " + QString::number( index_end ) + ", index );\n";
            }
            else
            {
                index_start.push_back( block_list.indexOf( blocks[line.start_block] ) );
            }
            for ( auto index : index_start )
            {
                script += "blocks_list[blocks_list_size + " + QString::number( index_end ) + "].input_blocks.push( blocks_list_size + " + QString::number( index ) + " );\n";
            }
        }
        else
        {
            int index_start = block_list.indexOf( blocks[line.start_block] );
            script += "addOutputDependBlockForComposite( \""
                + ( static_cast<IOBlockSettings*>( blocks[line.end_block] ) )->text
                + "\", blocks_list_size + " + QString::number( index_start ) + ", index );\n";
        }
    }
    return script;
}
