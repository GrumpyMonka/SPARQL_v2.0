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

void BasedBlockSettings::setSettingFromJson( const QJsonObject& object )
{
    QJsonObject data = object;
    if ( object["type"].toString() == "basic" )
    {
        data = object["data"].toObject();
    }
    block_name = data["name"].toString();

    label = data["label"].toBool();
    label_text = data["label_text"].toString();

    line_edit = data["line_edit"].toBool();
    line_edit_text = data["line_edit_text"].toString();

    script = data["script"].toString();

    type_image = data["type_img"].toString();
    if ( type_image != "self" )
    {
        flag_custom_image = true;
    }
    else
    {
        flag_custom_image = false;
    }
    pixmap = pixmapFrom( data["image"] );
}

QJsonObject BasedBlockSettings::getJsonFromSetting()
{
    QJsonObject object;

    QJsonObject header;
    QJsonObject body;

    header.insert( "Name", block_name );
    header.insert( "Type", "Based" );

    body.insert( "Image", jsonValFromPixmap( pixmap ) );
    body.insert( "Flag_Custom_Image", flag_custom_image );
    body.insert( "Flag_Label", label );
    body.insert( "Flag_Edit", line_edit );
    body.insert( "Text_Label", label_text );
    body.insert( "Text_Edit", line_edit_text );
    body.insert( "Pos", jsonFromPointF( pos ) );
    body.insert( "Script", script );

    object.insert( "Header", header );
    object.insert( "Body", body );

    return object;
}

QPixmap BasedBlockSettings::image() const
{
    return {};
}
