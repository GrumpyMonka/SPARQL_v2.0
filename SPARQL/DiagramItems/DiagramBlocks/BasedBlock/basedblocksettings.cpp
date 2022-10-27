#include "basedblocksettings.h"

#include <QJsonObject>

BasedBlockSettings::BasedBlockSettings()
{
    block_name = "BasedBlock";
    label = true;
    label_text = "label";
    line_edit = true;
    line_edit_text = "";
    script = "";
    type_image = "self";
    pixmap = image();
}

void BasedBlockSettings::setSettingFromJson( const QJsonValue& value )
{
    if ( value.isObject() )
    {
        block_name = value["name"].toString();

        label = value["label"].toBool();
        label_text = value["label_text"].toString();

        line_edit = value["line_edit"].toBool();
        line_edit_text = value["line_edit_text"].toString();

        script = value["script"].toString();

        type_image = value["type_img"].toString();
        pixmap = pixmapFrom( value["image"] );
    }
}

QJsonObject BasedBlockSettings::getJsonFromSetting()
{
    QJsonObject obj;

    QJsonObject temp_obj;
    temp_obj.insert( "name", QJsonValue( block_name ) );

    temp_obj.insert( "label", QJsonValue( label ) );
    temp_obj.insert( "label_text", QJsonValue( label_text ) );

    temp_obj.insert( "line_edit", QJsonValue( line_edit ) );
    temp_obj.insert( "line_edit_text", QJsonValue( line_edit_text ) );

    temp_obj.insert( "script", QJsonValue( script ) );

    temp_obj.insert( "type_img", QJsonValue( type_image ) );
    temp_obj.insert( "image", jsonValFromPixmap( pixmap ) );

    obj.insert( "type", QJsonValue( "basic" ) );
    obj.insert( "data", QJsonValue( temp_obj ) );

    return obj;
}

QPixmap BasedBlockSettings::image() const
{
    return {};
}
