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
    if ( object["Header"]["Type"] == "Based" )
    {

        QJsonObject header = object["Header"].toObject();
        QJsonObject body = object["Body"].toObject();

        block_name = header["Name"].toString();

        pixmap = pixmapFrom( body["Image"] );
        flag_custom_image = body["Flag_Custom_Image"].toBool();
        label = body["Flag_Label"].toBool();
        line_edit = body["Flag_Edit"].toBool();
        label_text = body["Text_Label"].toString();
        line_edit_text = body["Text_Edit"].toString();
        pos = pointFromJsonObject( body["Pos"] );
        script = body["Script"].toString();
    }
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
